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

    #include <Jopnal/Physics/World.hpp>

    #include <Jopnal/Core/SettingManager.hpp>
    #include <Jopnal/Core/DebugHandler.hpp>
    #include <Jopnal/Core/ResourceManager.hpp>
    #include <Jopnal/Utility/CommandHandler.hpp>
    #include <Jopnal/Graphics/Camera.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>
    #include <Jopnal/Graphics/OpenGL/GlState.hpp>
    #include <Jopnal/Graphics/ShaderProgram.hpp>
    #include <Jopnal/Graphics/VertexBuffer.hpp>
    #include <Jopnal/Physics/Collider.hpp>
    #include <Jopnal/Physics/Detail/WorldImpl.hpp>
    #include <Jopnal/Utility/Assert.hpp>
    #include <Jopnal/STL.hpp>
    #include <btBulletDynamicsCommon.h>
    #include <BulletCollision/CollisionDispatch/btGhostObject.h>

#endif

#include <Jopnal/Resources/Resources.hpp>

//////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_COMMAND_HANDLER(World)

        JOP_BIND_MEMBER_COMMAND(&World::setDebugMode, "setWorldDebugMode");

    JOP_END_COMMAND_HANDLER(World)
}

#ifdef JOP_DEBUG_MODE

namespace detail
{
    class DebugDrawer final : public btIDebugDraw
    {
    private:

        friend class jop::World;

        typedef std::vector<std::pair<btVector3, btVector3>> LineVec;

        jop::VertexBuffer m_buffer;
        LineVec m_lines;
        LineVec m_points;
        int m_mode;
        const jop::Camera* m_cam;

    public:

        DebugDrawer()
            : m_buffer  (jop::Buffer::Type::ArrayBuffer, jop::Buffer::DynamicDraw),
              m_lines   (),
              m_points  (),
              m_mode    (0),
              m_cam     (nullptr)
        {}

        void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override
        {
            m_lines.emplace_back(from, color);
            m_lines.emplace_back(to, color);
        }

        virtual void draw3dText(const btVector3&, const char*) override
        {}

        void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar, int, const btVector3& color) override
        {
            m_points.emplace_back(PointOnB, color);
            drawLine(PointOnB, PointOnB + normalOnB, color);
        }

        void reportErrorWarning(const char* warningString) override
        {
        #ifndef JOP_DEBUG_MODE
            warningString;
        #endif
            JOP_DEBUG_WARNING(warningString);
        }

        void setDebugMode(int debugMode) override
        {
            m_mode = debugMode;
        }

        int getDebugMode() const override
        {
            return m_mode;
        }

        void flushLines() override
        {
            using namespace jop;

            if ((m_lines.empty() && m_points.empty()) || !m_cam)
                return;

            static WeakReference<ShaderProgram> shdr;

            if (shdr.expired())
            {
                shdr = static_ref_cast<ShaderProgram>(ResourceManager::getEmptyResource<ShaderProgram>("jop_physics_debug_shader").getReference());

                Shader vertex("");
                vertex.load(std::string(reinterpret_cast<const char*>(jopr::physicsDebugShaderVert), sizeof(jopr::physicsDebugShaderVert)), Shader::Type::Vertex, true);
                Shader frag("");
                frag.load(std::string(reinterpret_cast<const char*>(jopr::physicsDebugShaderFrag), sizeof(jopr::physicsDebugShaderFrag)), Shader::Type::Fragment, true);

                JOP_ASSERT_EVAL(shdr->load("",vertex, frag),"Failed to compile physics debug shader!");
            }

            // Draw lines
            if (!m_lines.empty())
            {
                GlState::setDepthTest(true, GlState::DepthFunc::LessEqual);

                m_buffer.setData(m_lines.data(), m_lines.size() * sizeof(LineVec::value_type));

                shdr->setUniform("u_PVMatrix", m_cam->getProjectionMatrix() * m_cam->getViewMatrix());

                shdr->setAttribute(0, GL_FLOAT, 3, sizeof(LineVec::value_type), reinterpret_cast<void*>(0));
                shdr->setAttribute(3, GL_FLOAT, 3, sizeof(LineVec::value_type), reinterpret_cast<void*>(sizeof(btVector3)));

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
}

#endif

namespace detail
{
    struct GhostCallback : btGhostPairCallback
    {
        btBroadphasePair* addOverlappingPair(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) override
        {
            auto p0 = static_cast<jop::Collider*>(static_cast<btCollisionObject*>(proxy0->m_clientObject)->getUserPointer());
            auto p1 = static_cast<jop::Collider*>(static_cast<btCollisionObject*>(proxy1->m_clientObject)->getUserPointer());

            if (p0 && p1)
            {
                JOP_DEBUG_DIAG("Objects \"" << p0->getObject()->getID() << "\" and \"" << p1->getObject()->getID() << "\" began overlapping");

                p0->beginOverlap(*p1);
                p1->beginOverlap(*p0);
            }

            return btGhostPairCallback::addOverlappingPair(proxy0, proxy1);
        }

        void* removeOverlappingPair(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1, btDispatcher* dispatcher) override
        {
            auto p0 = static_cast<jop::Collider*>(static_cast<btCollisionObject*>(proxy0->m_clientObject)->getUserPointer());
            auto p1 = static_cast<jop::Collider*>(static_cast<btCollisionObject*>(proxy1->m_clientObject)->getUserPointer());

            if (p0 && p1)
            {
                JOP_DEBUG_DIAG("Objects \"" << p0->getObject()->getID() << "\" and \"" << p1->getObject()->getID() << "\" ended overlap");

                p0->endOverlap(*p1);
                p1->endOverlap(*p0);
            }

            return btGhostPairCallback::removeOverlappingPair(proxy0, proxy1, dispatcher);
        }
    };
}

#ifdef JOP_DEBUG_MODE
    #define CREATE_DRAWER new ::detail::DebugDrawer
#else
    #define CREATE_DRAWER nullptr
#endif

namespace jop
{
    World::World(Object& obj, Renderer& renderer)
        : Drawable          (obj, renderer, 0),
          m_worldData       (std::make_unique<detail::WorldImpl>(CREATE_DRAWER)),
          m_ghostCallback   (std::make_unique<::detail::GhostCallback>())
    {
        static const float gravity = SettingManager::get<float>("engine@Physics|DefaultWorld|fGravity", -9.81f);

        m_worldData->world->setGravity(btVector3(0.f, gravity, 0.f));
        m_worldData->world->getPairCache()->setInternalGhostPairCallback(m_ghostCallback.get());
        m_worldData->world->setWorldUserInfo(this);
        
        setDebugMode(false);

        setCastShadows(false).setReceiveLights(false).setReceiveShadows(false).setReflected(false);
    }

    World::~World()
    {
        // Need to define destructor because of forward declarations
    }

    //////////////////////////////////////////////

    World* World::clone(Object&) const
    {
        JOP_ASSERT(false, "Copying jop::World is forbidden!");
        return nullptr;
    }

    //////////////////////////////////////////////

    void World::update(const float deltaTime)
    {
        static const char* const str = "engine@Physics|uUpdateFrequency";
        static float timeStep = 1.f / SettingManager::get<unsigned int>(str, 50);

        static struct Callback : SettingCallback<unsigned int>
        {
            float* val;
            Callback(float* _val, const char* str) : val(_val)
            {SettingManager::registerCallback(str, *this);}
            void valueChanged(const unsigned int& value) override
            {*val = 1.f / value;}

        } cb(&timeStep, str);

        m_worldData->world->stepSimulation(deltaTime, 10, timeStep);
    }

    //////////////////////////////////////////////

    void World::draw(const Camera* camera, const LightContainer&, ShaderProgram&) const
    {
    #ifdef JOP_DEBUG_MODE

        if (camera && m_worldData->world->getDebugDrawer()->getDebugMode())
        {
            static_cast<::detail::DebugDrawer*>(m_worldData->world->getDebugDrawer())->m_cam = camera;
            m_worldData->world->debugDrawWorld();
        }

    #else
        camera;
    #endif
    }

    //////////////////////////////////////////////

    void World::setDebugMode(const bool enable)
    {
    #ifdef JOP_DEBUG_MODE

        static const int debugField = btIDebugDraw::DBG_DrawAabb
                                    | btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE;

        m_worldData->world->getDebugDrawer()->setDebugMode(enable * debugField);

    #else
        enable;
    #endif
    }

    //////////////////////////////////////////////

    bool World::debugMode() const
    {
    #ifdef JOP_DEBUG_MODE
        return m_worldData->world->getDebugDrawer()->getDebugMode() != btIDebugDraw::DBG_NoDebug;
    #else
        return false;
    #endif
    }

    //////////////////////////////////////////////

    RayInfo World::checkRayClosest(const glm::vec3& start, const glm::vec3& ray, const short group, const short mask) const
    {
        const glm::vec3 fromTo(start + ray);

        const btVector3 rayFromWorld(start.x, start.y, start.z);
        const btVector3 rayToWorld(fromTo.x, fromTo.y, fromTo.z);

        btCollisionWorld::ClosestRayResultCallback cb(rayFromWorld, rayToWorld);

        m_worldData->world->rayTest(rayFromWorld, rayToWorld, cb);
        cb.m_collisionFilterGroup = group;
        cb.m_collisionFilterMask = mask;

        if (cb.hasHit() && cb.m_collisionObject != nullptr)
            return RayInfo(static_cast<Collider*>(cb.m_collisionObject->getUserPointer()),
                           glm::vec3(cb.m_hitPointWorld.x(), cb.m_hitPointWorld.y(), cb.m_hitPointWorld.z()),
                           glm::vec3(cb.m_hitNormalWorld.x(), cb.m_hitNormalWorld.y(), cb.m_hitNormalWorld.z()));
        
        return RayInfo();
    }

    //////////////////////////////////////////////

    std::vector<RayInfo> jop::World::checkRayAllHits(const glm::vec3& start, const glm::vec3& ray, const short group, const short mask) const
    {
        const glm::vec3 fromTo(start + ray);

        const btVector3 rayFromWorld(start.x, start.y, start.z);
        const btVector3 rayToWorld(fromTo.x, fromTo.y, fromTo.z);

        btCollisionWorld::AllHitsRayResultCallback cb(rayFromWorld, rayToWorld);
        cb.m_collisionFilterGroup = group;
        cb.m_collisionFilterMask = mask;
        
        std::vector<RayInfo> objContainer;
        m_worldData->world->rayTest(rayFromWorld, rayToWorld, cb);

        for (size_t i = 0; cb.m_collisionObjects.size(); ++i)
        {
            const auto& point = cb.m_hitPointWorld[i];
            const auto& normal = cb.m_hitNormalWorld[i];

            objContainer.push_back(RayInfo(static_cast<Collider*>(cb.m_collisionObjects[i]->getUserPointer()),
                                           glm::vec3(point.x(), point.y(), point.z()),
                                           glm::vec3(normal.x(), normal.y(), normal.z())));
        }
        
        return objContainer;
    }

    //////////////////////////////////////////////

    std::vector<Collider*> World::checkOverlapAll(const glm::vec3& aabbStart, const glm::vec3& aabbEnd, const short group, const short mask) const
    {
        struct Callback : btBroadphaseAabbCallback
        {
            std::vector<Collider*> vec;
            short group;
            short mask;

            bool process(const btBroadphaseProxy* proxy) override
            {
                if (proxy->m_clientObject && (proxy->m_collisionFilterMask & group) != 0 && (mask & proxy->m_collisionFilterGroup) != 0)
                    vec.push_back(static_cast<Collider*>(static_cast<btCollisionObject*>(proxy->m_clientObject)->getUserPointer()));

                return false;
            }

        } cb;
        cb.group = group;
        cb.mask = mask;
        
        m_worldData->world->getBroadphase()->aabbTest(btVector3(aabbStart.x, aabbStart.y, aabbStart.z), btVector3(aabbEnd.x, aabbEnd.y, aabbEnd.z), cb);

        return cb.vec;
    }

    //////////////////////////////////////////////

    Message::Result World::receiveMessage(const Message& message)
    {
        if (JOP_EXECUTE_COMMAND(World, message.getString(), this) == Message::Result::Escape)
            return Message::Result::Escape;

        return Component::receiveMessage(message);
    }
}