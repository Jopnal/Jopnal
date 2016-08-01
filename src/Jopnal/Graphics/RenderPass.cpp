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

    #include <Jopnal/Graphics/RenderPass.hpp>

    #include <Jopnal/Core/Engine.hpp>
    #include <Jopnal/Core/Object.hpp>
    #include <Jopnal/Core/Scene.hpp>
    #include <Jopnal/Graphics/Camera.hpp>
    #include <Jopnal/Graphics/Drawable.hpp>
    #include <Jopnal/Graphics/LightSource.hpp>
    #include <Jopnal/Graphics/Material.hpp>
    #include <Jopnal/Graphics/Renderer.hpp>
    #include <Jopnal/Graphics/RenderTarget.hpp>
    #include <Jopnal/Graphics/OpenGL/GlState.hpp>
    #include <glm/gtx/norm.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    namespace detail
    {
        class DrawableSorterOpaque
        {
            void operator =(const DrawableSorterOpaque&) = delete;

            const glm::vec3& m_camPos;

        public:

            DrawableSorterOpaque(const Camera& cam)
                : m_camPos(cam.getObject()->getGlobalPosition())
            {}

            bool operator ()(const Drawable* first, const Drawable* second) const
            {
                return glm::distance2(m_camPos, first->getObject()->getGlobalPosition()) <
                       glm::distance2(m_camPos, second->getObject()->getGlobalPosition());
            }
        };

        class DrawableSorterTranslucent
        {
            void operator =(const DrawableSorterTranslucent&) = delete;

            const glm::vec3& m_camPos;

        public:

            DrawableSorterTranslucent(const Camera& cam)
                : m_camPos(cam.getObject()->getGlobalPosition())
            {}

            bool operator ()(const Drawable* first, const Drawable* second) const
            {
                return glm::distance2(m_camPos, first->getObject()->getGlobalPosition()) >
                       glm::distance2(m_camPos, second->getObject()->getGlobalPosition());
            }
        };

        //////////////////////////////////////////////

        RenderPassProxy::RenderPassProxy(const RenderPass::Pass pass)
            : Subsystem (static_cast<uint32>(pass)),
              m_pass    (pass)
        {}

        //////////////////////////////////////////////

        void RenderPassProxy::draw()
        {
            if (Engine::getState() != Engine::State::Frozen)
            {
                if (Engine::hasCurrentScene())
                    Engine::getCurrentScene().getRenderer().draw(m_pass);

                if (Engine::hasSharedScene())
                    Engine::getSharedScene().getRenderer().draw(m_pass);
            }
        }
    }

    //////////////////////////////////////////////

    const uint32 RenderPass::DefaultWeight = 0x88888888;

    //////////////////////////////////////////////

    RenderPass::RenderPass(Renderer& renderer, const RenderTarget& target, const Pass pass, const uint32 weight)
        : m_drawables   (),
          m_rendererRef (renderer),
          m_target      (target),
          m_weight      (weight),
          m_pass        (pass),
          m_active      (true)
    {}

    //////////////////////////////////////////////

    void RenderPass::draw()
    {
        if (!isActive())
            return;
    }

    //////////////////////////////////////////////

    void RenderPass::bind(const Drawable* drawable)
    {
        m_drawables.insert(drawable);
    }

    //////////////////////////////////////////////

    void RenderPass::unbind(const Drawable* drawable)
    {
        m_drawables.erase(drawable);
    }

    //////////////////////////////////////////////

    void RenderPass::setActive(const bool active)
    {
        m_active = active;
    }

    //////////////////////////////////////////////

    bool RenderPass::isActive() const
    {
        return m_active;
    }

    //////////////////////////////////////////////

    Renderer& RenderPass::getRenderer()
    {
        return m_rendererRef;
    }

    //////////////////////////////////////////////

    RenderPass::Pass RenderPass::whichPass() const
    {
        return m_pass;
    }


    //////////////////////////////////////////////


    DefaultPrePass::DefaultPrePass(Renderer& renderer, const RenderTarget& target, const Pass pass, const uint32 weight)
        : RenderPass(renderer, target, pass, weight)
    {}

    void DefaultPrePass::draw()
    {
        
    }

    //////////////////////////////////////////////

    DefaultPostPass::DefaultPostPass(Renderer& renderer, const RenderTarget& target, const Pass pass, const uint32 weight)
        : RenderPass(renderer, target, pass, weight)
    {}

    void DefaultPostPass::draw()
    {

    }
}