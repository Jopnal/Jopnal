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
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Physics2D/World2D.hpp>
    
    #include <Jopnal/Core/ResourceManager.hpp>
    #include <Jopnal/Core/SettingManager.hpp>
    #include <Jopnal/Graphics/Camera.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>
    #include <Jopnal/Graphics/OpenGL/GlState.hpp>
    #include <Jopnal/Graphics/ShaderProgram.hpp>
    #include <Jopnal/Graphics/VertexBuffer.hpp>
    #include <Jopnal/Physics2D/ContactListener2D.hpp>
    #include <Jopnal/Physics2D/ContactInfo2D.hpp>
    #include <Box2D/Collision/Shapes/b2PolygonShape.h>
    #include <Box2D/Common/b2Draw.h>
    #include <Box2D/Dynamics/b2World.h>
    #include <Box2D/Dynamics/b2Fixture.h>
    #include <Box2D/Dynamics/Contacts/b2Contact.h>
    #include <LinearMath/btVector3.h>
    #include <glm/gtc/constants.hpp>
    #include <set>

#endif

#include <Jopnal/Resources/Resources.hpp>

//////////////////////////////////////////////


namespace jop
{
    namespace detail
    {
        struct DebugDraw : b2Draw
        {
            WeakReference<ShaderProgram> shdr;

            typedef std::vector<std::pair<btVector3, btVector3>> LineVec;

            jop::VertexBuffer m_buffer;
            LineVec m_lines;
            LineVec m_points;

            const jop::Camera* m_cam;

        public:

            DebugDraw()
                : m_buffer(VertexBuffer::Type::ArrayBuffer, Buffer::Usage::DynamicDraw)
            {
                if (shdr.expired())
                {
                    shdr = static_ref_cast<ShaderProgram>(ResourceManager::getEmpty<ShaderProgram>("jop_physics_debug_shader").getReference());

                    if (!shdr->isValid())
                    {
                        Shader vertex("");
                        vertex.load(std::string(reinterpret_cast<const char*>(jopr::physicsDebugShaderVert), sizeof(jopr::physicsDebugShaderVert)), Shader::Type::Vertex, true);
                        Shader frag("");
                        frag.load(std::string(reinterpret_cast<const char*>(jopr::physicsDebugShaderFrag), sizeof(jopr::physicsDebugShaderFrag)), Shader::Type::Fragment, true);

                        JOP_ASSERT_EVAL(shdr->load("", vertex, frag), "Failed to compile physics debug shader!");
                    }
                }
            }

            void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
            {
                DrawSolidCircle(center, radius, b2Vec2(1.f, 0.f), color);
            }

            void DrawPoint(const b2Vec2& p1, float, const b2Color& color)
            {
                m_points.emplace_back(btVector3(p1.x, p1.y, 0.f), btVector3(color.r, color.g, color.b));
            }

            void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
            {
                for (int i = 1; i < vertexCount; ++i)
                {
                    DrawSegment(vertices[i - 1], vertices[i], color);
                }
                DrawSegment(vertices[vertexCount - 1], vertices[0], color);
            }

            void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
            {
                m_lines.emplace_back(btVector3(p1.x, p1.y, 0.f), btVector3(color.r, color.g, color.b));
                m_lines.emplace_back(btVector3(p2.x, p2.y, 0.f), btVector3(color.r, color.g, color.b));
            }

            void DrawTransform(const b2Transform& xf)
            {
                b2Vec2 up(-1.f * xf.q.c, 1.f * xf.q.s);
                b2Vec2 right(-1.f * xf.q.s, 1.f * xf.q.c);

                DrawSegment(xf.p, xf.p + up, b2Color(1.f, 0.f, 0.f, 1.f));
                DrawSegment(xf.p, xf.p + right, b2Color(1.f, 0.f, 0.f, 1.f));
            }

            void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
            {
                const int pointCount = 15;
                const float radians = glm::two_pi<float>() / pointCount;
                float totalRadians = std::atan2f(axis.y, axis.x);

                b2Vec2 lastPoint(axis.x * radius, axis.y * radius);

                DrawSegment(center, lastPoint + center, color);

                for (int i = 1; i <= pointCount; ++i)
                {
                    const b2Vec2 currentPoint(glm::cos(totalRadians) * radius, glm::sin(totalRadians) * radius);
                    DrawSegment(lastPoint + center, currentPoint + center, color);
                    lastPoint = currentPoint;
                    totalRadians += radians;
                }
                DrawSegment(lastPoint + center, b2Vec2(axis.x * radius, axis.y * radius) + center, color);
            }

            void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
            {
                DrawPolygon(vertices, vertexCount, color);
            }

            void flushLines()
            {
                using namespace jop;

                if ((m_lines.empty() && m_points.empty()) || !m_cam)
                    return;

                // Draw lines
                if (!m_lines.empty())
                {
                    GlState::setDepthTest(true, GlState::DepthFunc::LessEqual);

                    m_buffer.setData(m_lines.data(), m_lines.size() * sizeof(LineVec::value_type));

                    shdr->setUniform("u_PVMatrix", m_cam->getProjectionMatrix() * m_cam->getViewMatrix());

                    shdr->setAttribute("a_Positio", 0, GL_FLOAT, 3, sizeof(LineVec::value_type), reinterpret_cast<void*>(0));
                    shdr->setAttribute("a_Normal", 3, GL_FLOAT, 3, sizeof(LineVec::value_type), reinterpret_cast<void*>(sizeof(btVector3)));

                    glCheck(glDrawArrays(GL_LINES, 0, m_lines.size()));

                    m_lines.clear();
                }

                // Draw points
                if (m_points.empty())
                {
                #ifndef JOP_OPENGL_ES
                    glCheck(glPointSize(3));
                #endif

                    GlState::setDepthTest(true, GlState::DepthFunc::Always);

                    m_buffer.setData(m_points.data(), m_points.size() * sizeof(LineVec::value_type));

                    glCheck(glDrawArrays(GL_POINTS, 0, m_points.size()));

                    m_points.clear();
                }

                GlState::setDepthTest(true);
            }
        };

        struct ContactListener2DImpl : b2ContactListener
        {
            virtual void BeginContact(b2Contact* contact) override
            {
                ContactInfo2D ci(glm::vec2(contact->GetManifold()->localPoint.x, contact->GetManifold()->localPoint.y), glm::vec2(contact->GetManifold()->localNormal.x, contact->GetManifold()->localNormal.y));

                auto a = static_cast<Collider2D*>(contact->GetFixtureA()->GetBody()->GetUserData());
                auto b = static_cast<Collider2D*>(contact->GetFixtureB()->GetBody()->GetUserData());

                for (auto& i : a->m_listeners)
                {
                    i->beginContact(*b, ci);
                }
            }

            virtual void EndContact(b2Contact* contact) override
            {
                ContactInfo2D ci(glm::vec2(contact->GetManifold()->localPoint.x, contact->GetManifold()->localPoint.y), glm::vec2(contact->GetManifold()->localNormal.x, contact->GetManifold()->localNormal.y));
                auto a = static_cast<Collider2D*>(contact->GetFixtureA()->GetBody()->GetUserData());
                auto b = static_cast<Collider2D*>(contact->GetFixtureB()->GetBody()->GetUserData());

                for (auto& i : a->m_listeners)
                {
                    i->endContact(*b);
                }
            }
        };
    }


    World2D::World2D(Object& obj, Renderer& renderer)
        : Drawable      (obj, renderer, RenderPass::Pass::Forward, 0),
          m_contactListener(std::make_unique<detail::ContactListener2DImpl>()),
          m_worldData2D (std::make_unique<b2World>(b2Vec2(0.f, 0.0f))),
          m_step        (0.f),
          m_dd          (std::make_unique<detail::DebugDraw>())
    {
        static const float gravity = SettingManager::get<float>("engine@Physics2D|DefaultWorld|fGravity", -9.81f);

        m_worldData2D->SetGravity(b2Vec2(0.f, gravity));
        m_worldData2D->SetAllowSleeping(false);
        m_worldData2D->SetDebugDraw(m_dd.get());
        m_worldData2D->SetContactListener(m_contactListener.get());

        setDebugMode(false);
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
            m_worldData2D->Step(timeStep, 8, 3); // 8 velocity and 3 position check done for each timeStep
            m_step -= timeStep;
            m_worldData2D->ClearForces();
        }
    }

    //////////////////////////////////////////////

    void World2D::draw(const Camera* camera, const LightContainer&, ShaderProgram&) const
    {
    #ifdef JOP_DEBUG_MODE

        if (camera && debugMode())
        {
            m_dd->m_cam = camera;
            m_worldData2D->DrawDebugData();
            m_dd->flushLines();
        }

    #else

        camera;

    #endif
    }

    //////////////////////////////////////////////

    void World2D::setDebugMode(const bool enable)
    {
    #ifdef JOP_DEBUG_MODE

        m_dd->SetFlags(enable * (b2Draw::e_shapeBit | b2Draw::e_aabbBit));

    #else

        enable;

    #endif
    }

    //////////////////////////////////////////////

    bool World2D::debugMode() const
    {
    #ifdef JOP_DEBUG_MODE
        return m_dd->GetFlags() != 0;
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

            float ReportFixture(b2Fixture* fix, const b2Vec2& point, const b2Vec2& normal, float)
            {
                if ((fix->GetFilterData().maskBits & group) && (mask & fix->GetFilterData().groupIndex))
                    hits.emplace_back(RayInfo2D(static_cast<Collider2D*>(fix->GetBody()->GetUserData()), glm::vec2(normal.x, normal.y), glm::vec2(point.x, point.y)));

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
            std::set<Collider2D*> overlaps;
            int group;
            unsigned int mask;

            bool ReportFixture(b2Fixture* fix) override
            {
                if ((fix->GetFilterData().maskBits & group) && (mask & fix->GetFilterData().groupIndex))
                    overlaps.emplace(static_cast<Collider2D*>(fix->GetBody()->GetUserData()));

                return true;
            }
        } cb;

        cb.group = group;
        cb.mask = mask;

        b2AABB totalaabb;
        totalaabb.lowerBound = b2Vec2(aabbStart.x, aabbStart.y);
        totalaabb.upperBound = b2Vec2(aabbEnd.x, aabbEnd.y);

        m_worldData2D->QueryAABB(&cb, totalaabb);

        return std::vector<Collider2D*>(cb.overlaps.begin(), cb.overlaps.end());
    }

    //////////////////////////////////////////////

    Message::Result World2D::receiveMessage(const Message& message)
    {
        // if (JOP_EXECUTE_COMMAND(World2D, message.getString(), this) == Message::Result::Escape)
        //    return Message::Result::Escape;

        return Component::receiveMessage(message);
    }
}