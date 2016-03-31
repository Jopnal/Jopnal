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
            : m_buffer(jop::Buffer::Type::ArrayBuffer, jop::Buffer::DynamicDraw)
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

            static WeakReference<Shader> shdr;

            if (shdr.expired())
            {
                std::vector<unsigned char> vert, frag;
                JOP_ASSERT_EVAL(FileLoader::readFromDll(IDR_PHYSICSDEBUGVERT, vert) && FileLoader::readFromDll(IDR_PHYSICSDEBUGFRAG, frag), "Failed to read physics debug shader source!");

                shdr = static_ref_cast<Shader>(ResourceManager::getEmptyResource<Shader>("jop_physics_debug_shader").getReference());

                JOP_ASSERT_EVAL(shdr->load(std::string(reinterpret_cast<const char*>(vert.data()), vert.size()),
                    "",
                    std::string(reinterpret_cast<const char*>(frag.data()), frag.size())),
                    "Failed to compile physics debug shader!");
            }

            // Draw lines
            if (!m_lines.empty())
            {
                GlState::setDepthTest(true, GlState::DepthFunc::LessEqual);

                m_buffer.setData(m_lines.data(), m_lines.size() * sizeof(LineVec::value_type));

                shdr->setUniform("u_PVMatrix", m_cam->getProjectionMatrix() * m_cam->getViewMatrix());

                shdr->setAttribute(0, gl::FLOAT, 3, sizeof(LineVec::value_type), false, reinterpret_cast<void*>(0));
                shdr->setAttribute(3, gl::FLOAT, 3, sizeof(LineVec::value_type), false, reinterpret_cast<void*>(sizeof(btVector3)));

                glCheck(gl::DrawArrays(gl::LINES, 0, m_lines.size()));

                m_lines.clear();
            }

            // Draw points
            if (m_points.empty())
            {
                glCheck(gl::PointSize(3));
                GlState::setDepthTest(true, GlState::DepthFunc::Always);

                m_buffer.setData(m_points.data(), m_points.size() * sizeof(LineVec::value_type));

                glCheck(gl::DrawArrays(gl::POINTS, 0, m_points.size()));

                m_points.clear();
            }

            GlState::setDepthTest(true);
        }
    };

#endif

    struct GhostCallback : btGhostPairCallback
    {
        btBroadphasePair* addOverlappingPair(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) override
        {
            auto p0 = static_cast<jop::Collider*>(static_cast<btCollisionObject*>(proxy0->m_clientObject)->getUserPointer());
            auto p1 = static_cast<jop::Collider*>(static_cast<btCollisionObject*>(proxy1->m_clientObject)->getUserPointer());

            if (p0 && p1)
            {
            #ifdef JOP_DEBUG_MODE
                JOP_DEBUG_INFO("Objects \"" << p0->getObject()->getID() << "\" and \"" << p1->getObject()->getID() << "\" began overlapping");
            #endif

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
            #ifdef JOP_DEBUG_MODE
                JOP_DEBUG_INFO("Objects \"" << p0->getObject()->getID() << "\" and \"" << p1->getObject()->getID() << "\" ended overlap");
            #endif

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
        : Component         (obj, "world"),
          m_worldData       (std::make_unique<detail::WorldImpl>(CREATE_DRAWER)),
          m_ghostCallback   (std::make_unique<::detail::GhostCallback>())
    {
    #ifdef JOP_DEBUG_MODE
        renderer.m_physicsWorld = this;
    #else
        renderer;
    #endif
        
        static const float gravity = SettingManager::getFloat("fDefaultWorldGravity", -9.81f);

        m_worldData->world->setGravity(btVector3(0.f, gravity, 0.f));
        m_worldData->world->getPairCache()->setInternalGhostPairCallback(m_ghostCallback.get());
        m_worldData->world->setWorldUserInfo(this);
        
        setDebugMode(false);

        /*static bool contactCallbacksSet = false;
        if (!contactCallbacksSet)
        {
            auto processedCallback = [](btManifoldPoint& mp, void* body0, void* body1) -> bool
            {
                auto c0 = static_cast<Collider*>(static_cast<btCollisionObject*>(body0)->getUserPointer());
                auto c1 = static_cast<Collider*>(static_cast<btCollisionObject*>(body1)->getUserPointer());

                if (c0 && c1)
                {
                    auto& aPos = mp.getPositionWorldOnA();
                    auto& bPos = mp.getPositionWorldOnB();

                    c0->handleContact(*c1, ContactInfo(glm::vec3(aPos.x(), aPos.y(), aPos.z())));
                    c1->handleContact(*c0, ContactInfo(glm::vec3(bPos.x(), bPos.y(), bPos.z())));
                }

                return true;
            };

            gContactProcessedCallback = processedCallback;

            contactCallbacksSet = true;
        }*/
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
        static const float timeStep = 1.f / static_cast<float>(SettingManager::getUint("uPhysicsUpdateFrequency", 50));

        m_worldData->world->stepSimulation(deltaTime, 10, timeStep);
    }

    //////////////////////////////////////////////

    void World::draw(const Camera& camera)
    {
    #ifdef JOP_DEBUG_MODE
        if (!m_worldData->world->getDebugDrawer()->getDebugMode())
            return;

        static_cast<::detail::DebugDrawer*>(m_worldData->world->getDebugDrawer())->m_cam = &camera;

        m_worldData->world->debugDrawWorld();
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
}