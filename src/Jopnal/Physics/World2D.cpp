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
    World2D::World2D(Object& obj, Renderer& renderer)
        : Drawable          (obj, renderer, 0),
        m_worldData2D       (std::make_unique<b2World>(b2Vec2(0.f, 0.0f)))
    {
        static const float gravity = SettingManager::get<float>("engine@Physics2D|DefaultWorld|fGravity", -9.81f);

        m_worldData2D->SetGravity(b2Vec2(0.f, gravity));

        
        //m_worldData->world->getPairCache()->setInternalGhostPairCallback(m_ghostCallback.get());
        //m_worldData->world->setWorldUserInfo(this);
        
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
        static const char* const str = "engine@Physics|uUpdateFrequency";
        static float timeStep = 1.f / 60.f;     //static_cast<float>(SettingManager::get<unsigned int>(str, 50));

        static struct Callback : SettingCallback<unsigned int>
        {
            float* val;
            Callback(float* _val, const char* str) : val(_val)
            {SettingManager::registerCallback(str, *this);}
            void valueChanged(const unsigned int& value) override
            {*val = 1.f / static_cast<float>(value);}
        } cb(&timeStep, str);

        m_worldData2D->world2D->Step(timeStep, 1, 1); // 1 velocity and 1 position check done for each timeStep      // stepSimulation(deltaTime, 10, timeStep);
    }

    //////////////////////////////////////////////

    void World2D::draw(const Camera* camera, const LightContainer&, Shader&) const
    {
    #ifdef JOP_DEBUG_MODE
      // if (camera && m_worldData->world->getDebugDrawer()->getDebugMode())
      // {
      //     static_cast<::detail::DebugDrawer*>(m_worldData->world->getDebugDrawer())->m_cam = camera;
      //     m_worldData->world->debugDrawWorld2D();
      // }
    #else
        camera;
    #endif
    }

    //////////////////////////////////////////////

    void World2D::setDebugMode(const bool enable)
    {
    #ifdef JOP_DEBUG_MODE
        static const int debugField = btIDebugDraw::DBG_DrawAabb
                                    | btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE;

        //m_worldData->world->getDebugDrawer()->setDebugMode(enable * debugField);
    #else
        enable;
    #endif
    }

    //////////////////////////////////////////////

    bool World2D::debugMode() const
    {
    #ifdef JOP_DEBUG_MODE
        //return m_worldData->world->getDebugDrawer()->getDebugMode() != btIDebugDraw::DBG_NoDebug;
    #else
        return false;
    #endif
    }

    //////////////////////////////////////////////

    RayInfo World2D::checkRayClosest(const b2Vec2 start, const b2Vec2 ray, const short group, const short mask) const
    {
        const b2Vec2 fromTo(start + ray);

        const b2Vec2 rayFromWorld2D(start.x, start.y);
        const b2Vec2 rayToWorld2D(fromTo.x, fromTo.y);

        //b2RayCastCallback::ReportFixture()

        //btCollisionWorld2D::ClosestRayResultCallback2D cb(rayFromWorld2D, rayToWorld2D);
        
        b2RayCastCallback::
        auto cb = b2RayCastCallback::ReportFixture;


        m_worldData2D->world2D->RayCast(cb, rayFromWorld2D, rayToWorld2D);
        cb.m_collisionFilterGroup = group;
        cb.m_collisionFilterMask = mask;

        if (cb.hasHit() && cb.m_collisionObject != nullptr)
            return RayInfo(static_cast<Collider*>(cb.m_collisionObject->getUserPointer()),
                           glm::vec3(cb.m_hitPointWorld2D.x(), cb.m_hitPointWorld2D.y(), cb.m_hitPointWorld2D.z()),
                           glm::vec3(cb.m_hitNormalWorld2D.x(), cb.m_hitNormalWorld2D.y(), cb.m_hitNormalWorld2D.z()));
        
        return RayInfo();
    }

    //////////////////////////////////////////////

    std::vector<RayInfo> jop::World2D::checkRayAllHits(const b2Vec2& start, const b2Vec2& ray, const short group, const short mask) const
    {
        const glm::vec3 fromTo(start + ray);

        const btVector3 rayFromWorld2D(start.x, start.y, start.z);
        const btVector3 rayToWorld2D(fromTo.x, fromTo.y, fromTo.z);

        btCollisionWorld2D::AllHitsRayResultCallback cb(rayFromWorld2D, rayToWorld2D);
        cb.m_collisionFilterGroup = group;
        cb.m_collisionFilterMask = mask;
        
        std::vector<RayInfo> objContainer;
        m_worldData->world->rayTest(rayFromWorld2D, rayToWorld2D, cb);

        for (size_t i = 0; cb.m_collisionObjects.size(); ++i)
        {
            const auto& point = cb.m_hitPointWorld2D[i];
            const auto& normal = cb.m_hitNormalWorld2D[i];

            objContainer.push_back(RayInfo(static_cast<Collider*>(cb.m_collisionObjects[i]->getUserPointer()),
                                           glm::vec3(point.x(), point.y(), point.z()),
                                           glm::vec3(normal.x(), normal.y(), normal.z())));
        }
        
        return objContainer;
    }

    //////////////////////////////////////////////

    std::vector<Collider2D*> World2D::checkOverlapAll(const b2Vec2& aabbStart, const b2Vec2& aabbEnd, const short group, const short mask) const
    {
        struct Callback : b2QueryCallback //btBroadphaseAabbCallback
        {
            std::vector<Collider2D*> vec;
            short group;
            short mask;

            bool process(const btBroadphaseProxy* proxy) override
            {
                if (proxy->m_clientObject && (proxy->m_collisionFilterMask & group) != 0 && (mask & proxy->m_collisionFilterGroup) != 0)
                    vec.push_back(static_cast<Collider2D*>(static_cast<btCollisionObject*>(proxy->m_clientObject)->getUserPointer()));

                return false;
            }

        } cb;
        cb.group = group;
        cb.mask = mask;
        
        m_worldData->world->getBroadphase()->aabbTest(btVector3(aabbStart.x, aabbStart.y, aabbStart.z), btVector3(aabbEnd.x, aabbEnd.y, aabbEnd.z), cb);

        return cb.vec;
    }

    //////////////////////////////////////////////

    Message::Result World2D::receiveMessage(const Message& message)
    {
        if (JOP_EXECUTE_COMMAND(World2D, message.getString(), this) == Message::Result::Escape)
            return Message::Result::Escape;

        return Component::receiveMessage(message);
    }
}