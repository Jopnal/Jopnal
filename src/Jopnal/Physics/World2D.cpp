// Jopnal Engine C++ Library
// Copyright (c) 2016 Team Jopnal
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgement in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

//////////////////////////////////////////////

// Headers
#include <Jopnal/Precompiled.hpp>


//////////////////////////////////////////////



namespace jop
{

    namespace detail
    {
        class DebugDraw : public b2Draw
        {
        public:
            void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {}
            void DrawPoint(const b2Vec2& p1, float size, const b2Color& color){}
            void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {}
            void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {}
            void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {}
            void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {}
            void DrawTransform(const b2Transform& xf) {}
            
        };
    }


    World2D::World2D(Object& obj, Renderer& renderer)
        : Drawable(obj, renderer, 0),
        m_worldData2D(std::make_unique<b2World>(b2Vec2(0.f, 0.0f))),
        m_step(0.f)
    {
        static const float gravity = SettingManager::get<float>("engine@Physics2D|DefaultWorld|fGravity", -9.81f);

        m_worldData2D->SetGravity(b2Vec2(0.f, gravity));

        setDebugMode(true);
        setCastShadows(false).setReceiveLights(false).setReceiveShadows(false).setReflected(false);
    }

    World2D::~World2D()
    {
        // Need to define destructor because of forward declarations
    }

    //////////////////////////////////////////////

    World2D* World2D::clone(Object&) const
    {
        JOP_ASSERT(false, "Copying jop::World2D is forbidden!");
        return nullptr;
    }

    //////////////////////////////////////////////

    void World2D::update(const float deltaTime)
    {

        static const char* const str = "engine@Physics2D|uUpdateFrequency";
        static float timeStep = 1.f / static_cast<float>(SettingManager::get<unsigned int>(str, 50));

        static struct Callback : SettingCallback<unsigned int>
        {
            float* val;
            Callback(float* _val, const char* str) : val(_val)
            {
                SettingManager::registerCallback(str, *this);
            }
            void valueChanged(const unsigned int& value) override
            {
                *val = 1.f / static_cast<float>(value);
            }
        } cb(&timeStep, str);


        m_step = std::min(0.1f, m_step + deltaTime);
        while (m_step >= timeStep)
        {
            m_worldData2D->Step(timeStep, 1, 1); // 1 velocity and 1 position check done for each timeStep
            m_step -= timeStep;
            m_worldData2D->ClearForces(); //to clear or not to clear?
        }



    }

    //////////////////////////////////////////////

    void World2D::draw(const Camera* camera, const LightContainer&, Shader&) const
    {
#ifdef JOP_DEBUG_MODE
       
        m_worldData2D->DrawDebugData();

#else
        camera;
#endif
    }

    //////////////////////////////////////////////

    void World2D::setDebugMode(const bool enable)
    {
#ifdef JOP_DEBUG_MODE

        detail::DebugDraw dd;

        m_worldData2D->SetDebugDraw(&dd);

        dd.SetFlags(b2Draw::e_shapeBit & b2Draw::e_aabbBit);

#else
        enable;
#endif
    }

    //////////////////////////////////////////////

    bool World2D::debugMode() const
    {
#ifdef JOP_DEBUG_MODE

        return true;
#else
        return false;
#endif
    }

    //////////////////////////////////////////////

    RayInfo2D World2D::checkRayClosest(const glm::vec2 start, const glm::vec2 ray, const short group, const short mask) const
    {
        struct Callback : b2RayCastCallback
        {
            RayInfo2D rayData;
            int group;
            unsigned int mask;
            float currentFraction;

            float ReportFixture(b2Fixture* fix, const b2Vec2& point, const b2Vec2& normal, float fraction)
            {
                if (fraction < currentFraction && (fix->GetFilterData().maskBits & group) && (mask & fix->GetFilterData().groupIndex))
                {
                    currentFraction = fraction;
                    rayData.collider = static_cast<Collider2D*>(fix->GetBody()->GetUserData());
                    rayData.normal = glm::vec2(normal.x, normal.y);
                    rayData.point = glm::vec2(point.x, point.y);
                }
                return 1.f;
            }
        } cb;

        cb.currentFraction = glm::length(ray);
        cb.group = group;
        cb.mask = mask;

        m_worldData2D->RayCast(&cb, b2Vec2(start.x, start.y), b2Vec2(start.x + ray.x, start.y + ray.y));

        return cb.rayData;
    }


    //////////////////////////////////////////////

    std::vector<RayInfo2D> jop::World2D::checkRayAllHits(const glm::vec2& start, const glm::vec2& ray, const short group, const short mask) const
    {
        struct Callback : b2RayCastCallback
        {
            std::vector<RayInfo2D> hits;
            int group;
            unsigned int mask;


            float ReportFixture(b2Fixture* fix, const b2Vec2& point, const b2Vec2& normal, float fraction)
            {
                if ((fix->GetFilterData().maskBits & group) && (mask & fix->GetFilterData().groupIndex))
                {
                    hits.emplace_back(RayInfo2D(static_cast<Collider2D*>(fix->GetBody()->GetUserData()), glm::vec2(normal.x, normal.y), glm::vec2(point.x, point.y)));
                }
                return 1.f;
            }
        } cb;

        cb.group = group;
        cb.mask = mask;

        m_worldData2D->RayCast(&cb, b2Vec2(start.x, start.y), b2Vec2(start.x + ray.x, start.y + ray.y));

        return cb.hits;
    }

    //////////////////////////////////////////////

    std::vector<Collider2D*> World2D::checkOverlapAll(const glm::vec2& aabbStart, const glm::vec2& aabbEnd, const short group, const short mask) const
    {
        struct Callback : b2QueryCallback
        {
            std::vector<Collider2D*> overlaps;
            bool overlapping = false;
            int group;
            unsigned int mask;

            bool ReportFixture(b2Fixture* fix) override
            {
                if ((fix->GetFilterData().maskBits & group) && (mask & fix->GetFilterData().groupIndex))
                {
                    overlaps.push_back(static_cast<Collider2D*>(fix->GetUserData()));
                }
                return !overlapping;
            }
        } cb;

        cb.group = group;
        cb.mask = mask;

        auto bodList = this->m_worldData2D->GetBodyList();
        auto fixList = bodList->GetFixtureList();
        b2AABB totalaabb(fixList->GetAABB(0));

        while (bodList)
        {
            while (fixList)
            {
                totalaabb.Combine(fixList->GetAABB(0));
                fixList->GetNext();
            }
            bodList->GetNext();
        }

        this->m_worldData2D->QueryAABB(&cb, totalaabb);

        return cb.overlaps;
    }

    //////////////////////////////////////////////

    Message::Result World2D::receiveMessage(const Message& message)
    {
        // if (JOP_EXECUTE_COMMAND(World2D, message.getString(), this) == Message::Result::Escape)
        //    return Message::Result::Escape;

        return Component::receiveMessage(message);
    }
}