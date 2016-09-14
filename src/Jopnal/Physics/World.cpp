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

    #include <Jopnal/Core/Object.hpp>   
    #include <Jopnal/Core/SettingManager.hpp>
    #include <Jopnal/Core/DebugHandler.hpp>
    #include <Jopnal/Core/ResourceManager.hpp>
    #include <Jopnal/Utility/CommandHandler.hpp>
    #include <Jopnal/Graphics/Camera.hpp>
    #include <Jopnal/Graphics/Drawable.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>
    #include <Jopnal/Graphics/OpenGL/GlState.hpp>
    #include <Jopnal/Graphics/ShaderProgram.hpp>
    #include <Jopnal/Graphics/VertexBuffer.hpp>
    #include <Jopnal/Graphics/Mesh/Mesh.hpp>
    #include <Jopnal/Physics/Collider.hpp>
    #include <Jopnal/Physics/Detail/WorldImpl.hpp>
    #include <Jopnal/Utility/Assert.hpp>
    #include <Jopnal/STL.hpp>
    #include <Jopnal/Physics/ContactListener.hpp>

    #pragma warning(push)
    #pragma warning(disable: 4127)

    #include <btBulletDynamicsCommon.h>
    #include <BulletCollision/CollisionDispatch/btGhostObject.h>

    #pragma warning(pop)    

#endif

#include <Jopnal/Resources/Resources.hpp>

//////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_COMMAND_HANDLER(World)

        JOP_BIND_MEMBER_COMMAND(&World::setDebugMode, "setWorldDebugMode");

    JOP_END_COMMAND_HANDLER(World)
}

namespace jop
{
    namespace detail
    {
        class DebugDrawer final : public btIDebugDraw
        {
        private:

            friend class ::jop::World;

            typedef std::vector<std::pair<btVector3, btVector3>> LineVec;

            VertexBuffer m_buffer;
            LineVec m_lines;
            LineVec m_points;
            int m_mode;
            const Drawable::ProjectionInfo* m_proj;

        public:

            DebugDrawer()
                : m_buffer  (Buffer::Type::ArrayBuffer, Buffer::DynamicDraw),
                  m_lines   (),
                  m_points  (),
                  m_mode    (0),
                  m_proj    (nullptr)
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
                #if JOP_CONSOLE_VERBOSITY < 1
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
                if (m_lines.empty() && m_points.empty())
                    return;

                static WeakReference<ShaderProgram> shdr;

                if (shdr.expired())
                {
                    shdr = static_ref_cast<ShaderProgram>(ResourceManager::getEmpty<ShaderProgram>("jop_physics_debug_shader").getReference());

                    if (!shdr->isValid())
                    {
                        Shader vertex("");
                        vertex.load(std::string(reinterpret_cast<const char*>(jopr::physicsDebugShaderVert), sizeof(jopr::physicsDebugShaderVert)), Shader::Type::Vertex);
                        Shader frag("");
                        frag.load(std::string(reinterpret_cast<const char*>(jopr::physicsDebugShaderFrag), sizeof(jopr::physicsDebugShaderFrag)), Shader::Type::Fragment);

                        JOP_ASSERT_EVAL(shdr->load("", vertex, frag), "Failed to compile physics debug shader!");
                    }
                }

                // Draw lines
                if (!m_lines.empty())
                {
                    m_buffer.setData(m_lines.data(), m_lines.size() * sizeof(LineVec::value_type));

                    shdr->setUniform("u_PVMatrix", m_proj->projectionMatrix * m_proj->viewMatrix);

                    GlState::setVertexAttribute(true, Mesh::VertexIndex::Position);
                    GlState::setVertexAttribute(true, Mesh::VertexIndex::Color);
                    glCheck(glVertexAttribPointer(Mesh::VertexIndex::Position, 3, GL_FLOAT, GL_FALSE, sizeof(LineVec::value_type), 0));
                    glCheck(glVertexAttribPointer(Mesh::VertexIndex::Color, 3, GL_FLOAT, GL_FALSE, sizeof(LineVec::value_type), (void*)sizeof(btVector3)));

                    glCheck(glDrawArrays(GL_LINES, 0, m_lines.size()));

                    m_lines.clear();
                }

                // Draw points
                if (!m_points.empty())
                {
                #ifndef JOP_OPENGL_ES
                    glCheck(glPointSize(3));
                #endif

                    m_buffer.setData(m_points.data(), m_points.size() * sizeof(LineVec::value_type));

                    glCheck(glDrawArrays(GL_POINTS, 0, m_points.size()));

                    m_points.clear();
                }
            }
        };

        struct GhostCallback : btGhostPairCallback
        {
            btBroadphasePair* addOverlappingPair(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) override
            {
                auto p0 = static_cast<jop::Collider*>(static_cast<btCollisionObject*>(proxy0->m_clientObject)->getUserPointer());
                auto p1 = static_cast<jop::Collider*>(static_cast<btCollisionObject*>(proxy1->m_clientObject)->getUserPointer());

                if (p0 && p1)
                {
                    for (auto& i : p0->m_listeners)
                        i->beginOverlap(*p1);

                    for (auto& i : p1->m_listeners)
                        i->beginOverlap(*p0);
                }

                return btGhostPairCallback::addOverlappingPair(proxy0, proxy1);
            }

            void* removeOverlappingPair(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1, btDispatcher* dispatcher) override
            {
                auto p0 = static_cast<jop::Collider*>(static_cast<btCollisionObject*>(proxy0->m_clientObject)->getUserPointer());
                auto p1 = static_cast<jop::Collider*>(static_cast<btCollisionObject*>(proxy1->m_clientObject)->getUserPointer());

                if (p0 && p1)
                {
                    for (auto& i : p0->m_listeners)
                        i->endOverlap(*p1);

                    for (auto& i : p1->m_listeners)
                        i->endOverlap(*p0);
                }

                return btGhostPairCallback::removeOverlappingPair(proxy0, proxy1, dispatcher);
            }
        };

        struct ContactListenerImpl
        {
        private:

            struct ContactData
            {
                Collider* A;
                Collider* B;

                ContactData(Collider& a, Collider& b)
                    : A(&a),
                      B(&b)
                {}
            };

        public:

            static bool contactProcessedCallback(btManifoldPoint& cp, void* body0, void* body1)
            {
                if (!body0 || !body1)
                    return false;

                auto a = static_cast<Collider*>(static_cast<btCollisionObject*>(body0)->getUserPointer());
                auto b = static_cast<Collider*>(static_cast<btCollisionObject*>(body1)->getUserPointer());

                const auto& pos = cp.m_positionWorldOnB;
                const auto& norm = cp.m_normalWorldOnB;

                ContactInfo ci(glm::vec3(pos.x(), pos.y(), pos.z()), glm::vec3(norm.x(), norm.y(), norm.z()));

                cp.m_userPersistentData = new ContactData(*a, *b);

                for (auto& i : a->m_listeners)
                    i->beginContact(*b, ci);

                return true;
            }

            static bool contactDestroyedCallback(void* userPersistentData)
            {
                ContactData* cd = static_cast<ContactData*>(userPersistentData);

                for (auto& i : cd->A->m_listeners)
                    i->endContact(*cd->B);

                delete cd;

                return true;
            }
        };

        class BroadPhaseCallback : public btOverlapFilterCallback
        {
			JOP_DISALLOW_COPY_MOVE(BroadPhaseCallback);

            const World::BroadphaseCallback& m_cb;

        public:

            BroadPhaseCallback(const World::BroadphaseCallback& cb)
                : m_cb(cb)
            {}

            bool needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const override
            {
                auto btColl0 = static_cast<btCollisionObject*>(proxy0->m_clientObject);
                auto btColl1 = static_cast<btCollisionObject*>(proxy1->m_clientObject);

                return !(btColl0->isStaticObject() && btColl1->isStaticObject())                    &&
                         m_cb.collide(*static_cast<const Collider*>(btColl0->getUserPointer()),
                                      *static_cast<const Collider*>(btColl1->getUserPointer()))     &&
                        (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0       &&
                        (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask) != 0;
            }
        };
    }

    //////////////////////////////////////////////


    World::BroadphaseCallback::BroadphaseCallback(World& world)
        : m_worldRef(world)
    {}

    World::BroadphaseCallback::~BroadphaseCallback()
    {
        m_worldRef.setDefaultBroadphaseCallback();
    }

    bool World::BroadphaseCallback::collide(const Collider& c0, const Collider& c1) const
    {
        return c0.isActive() && c1.isActive();
    }

    //////////////////////////////////////////////

    World::World(Object& obj, Renderer& renderer)
        : Drawable              (obj, renderer, RenderPass::Pass::AfterPost, RenderPass::DefaultWeight, false),
          m_worldData           (std::make_unique<detail::WorldImpl>(new detail::DebugDrawer)),
          m_ghostCallback       (std::make_unique<detail::GhostCallback>()),
          m_contactListener     (std::make_unique<detail::ContactListenerImpl>()),
          m_bpCallback          (),
          m_defaultBpCallback   (*this)
    {
        static const float gravity = SettingManager::get<float>("engine@Physics|DefaultWorld|fGravity", -9.81f);

        setGravity(glm::vec3(0.f, gravity, 0.f));
        m_worldData->world->getPairCache()->setInternalGhostPairCallback(m_ghostCallback.get());
        setDefaultBroadphaseCallback();
        m_worldData->world->setWorldUserInfo(this);
        gContactProcessedCallback = m_contactListener->contactProcessedCallback;
        gContactDestroyedCallback = m_contactListener->contactDestroyedCallback;
        
        setDebugMode(false);
        setFlags(0);
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

    void World::draw(const ProjectionInfo& proj, const LightContainer&) const
    {
    #ifdef JOP_DEBUG_MODE

        if (m_worldData->world->getDebugDrawer()->getDebugMode())
        {
            static_cast<detail::DebugDrawer*>(m_worldData->world->getDebugDrawer())->m_proj = &proj;
            m_worldData->world->debugDrawWorld();
        }

    #else
        camera;
    #endif
    }

    //////////////////////////////////////////////

    void World::setGravity(const glm::vec3& gravity)
    {
        m_worldData->world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
    }

    //////////////////////////////////////////////

    glm::vec3 World::getGravity() const
    {
        const btVector3 gravity = m_worldData->world->getGravity();
        return glm::vec3(gravity.x(), gravity.y(), gravity.z());
    }

    //////////////////////////////////////////////

    void World::setBroadphaseBallback(const BroadphaseCallback& callback)
    {
        m_bpCallback = std::make_unique<detail::BroadPhaseCallback>(callback);
        m_worldData->world->getPairCache()->setOverlapFilterCallback(m_bpCallback.get());
    }

    //////////////////////////////////////////////

    void World::setDefaultBroadphaseCallback()
    {
        setBroadphaseBallback(m_defaultBpCallback);
    }

    //////////////////////////////////////////////

    void World::setDebugMode(const bool enable)
    {
    #ifdef JOP_DEBUG_MODE

        static const int debugField = btIDebugDraw::DBG_DrawAabb
                                    | btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE
                                    | btIDebugDraw::DBG_DrawConstraints
                                    | btIDebugDraw::DBG_DrawConstraintLimits
                                    ;

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

    std::vector<RayInfo> World::checkRayAllHits(const glm::vec3& start, const glm::vec3& ray, const short group, const short mask) const
    {
        const glm::vec3 fromTo(start + ray);

        const btVector3 rayFromWorld(start.x, start.y, start.z);
        const btVector3 rayToWorld(fromTo.x, fromTo.y, fromTo.z);

        btCollisionWorld::AllHitsRayResultCallback cb(rayFromWorld, rayToWorld);
        cb.m_collisionFilterGroup = group;
        cb.m_collisionFilterMask = mask;
        
        std::vector<RayInfo> objContainer;
        m_worldData->world->rayTest(rayFromWorld, rayToWorld, cb);

        for (int i = 0; i < cb.m_collisionObjects.size(); ++i)
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