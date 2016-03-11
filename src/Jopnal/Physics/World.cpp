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


namespace detail
{
    class DebugDrawer final : public btIDebugDraw
    {
    private:

        friend class jop::World;

        typedef std::vector<std::pair<btVector3, btVector3>> LineVec;
        ::jop::VertexBuffer m_buffer;
        LineVec m_lines;
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

        void drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&) override
        {}

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

            if (m_lines.empty() || !m_cam)
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

            m_buffer.setData(m_lines.data(), m_lines.size() * sizeof(LineVec::value_type));

            shdr->setUniform("u_PVMatrix", m_cam->getProjectionMatrix() * m_cam->getViewMatrix());

            shdr->setAttribute(0, gl::FLOAT, 3, sizeof(LineVec::value_type), false, reinterpret_cast<void*>(0));
            shdr->setAttribute(3, gl::FLOAT, 3, sizeof(LineVec::value_type), false, reinterpret_cast<void*>(sizeof(btVector3)));

            glCheck(gl::DrawArrays(gl::LINES, 0, m_lines.size()));

            m_lines.clear();
        }
    };
}

namespace jop
{
    World::World(Renderer& renderer)
        : m_worldData(std::make_unique<detail::WorldImpl>(new ::detail::DebugDrawer))
    {
        renderer.m_physicsWorld = this;

        static const float gravity = SettingManager::getFloat("fDefaultWorldGravity", -9.81f);

        m_worldData->world->setGravity(btVector3(0.f, gravity, 0.f));
        setDebugMode(false);
    }

    World::~World()
    {
        // Need to define destructor because of forward declaration
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
        if (!m_worldData->world->getDebugDrawer()->getDebugMode())
            return;

        static_cast<::detail::DebugDrawer*>(m_worldData->world->getDebugDrawer())->m_cam = &camera;

        GlState::setDepthTest(true, GlState::DepthFunc::LessEqual);
        m_worldData->world->debugDrawWorld();
        GlState::setDepthTest(true);
    }

    //////////////////////////////////////////////

    void World::setDebugMode(const bool enable)
    {
        m_worldData->world->getDebugDrawer()->setDebugMode(enable ? btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE : btIDebugDraw::DBG_NoDebug);
    }

    //////////////////////////////////////////////

    bool World::debugMode() const
    {
        return m_worldData->world->getDebugDrawer()->getDebugMode() != btIDebugDraw::DBG_NoDebug;
    }
}