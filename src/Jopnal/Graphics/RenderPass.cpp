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
    const uint32 RenderPass::DefaultWeight = 0x88888888;

    //////////////////////////////////////////////

    RenderPass::RenderPass(Renderer& renderer, const RenderTarget& target, const Pass pass, const uint32 weight)
        : m_rendererRef (renderer),
          m_target      (target),
          m_weight      (weight),
          m_pass        (pass),
          m_active      (true)
    {}

    RenderPass::~RenderPass()
    {}

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

    RenderPass::Pass RenderPass::getPass() const
    {
        return m_pass;
    }

    //////////////////////////////////////////////

    RenderPass::Pass ns_defPass = RenderPass::Pass::BeforePost;
    uint32 ns_defWeight = RenderPass::DefaultWeight;

    void RenderPass::setDefaultType(const Pass pass)
    {
        ns_defPass = pass;
    }

    //////////////////////////////////////////////

    void RenderPass::setDefaultWeight(const uint32 weight)
    {
        ns_defWeight = weight;
    }

    //////////////////////////////////////////////

    RenderPass::Pass RenderPass::getDefaultType()
    {
        return ns_defPass;
    }

    //////////////////////////////////////////////

    uint32 RenderPass::getDefaultWeight()
    {
        return ns_defWeight;
    }

    //////////////////////////////////////////////


    SortedRenderPass::SortedRenderPass(Renderer& renderer, const RenderTarget& target, const Pass pass, const uint32 weight)
        : RenderPass(renderer, target, pass, weight)
    {}

    //////////////////////////////////////////////

    const LightContainer ns_dummyLightCont;

    void SortedRenderPass::draw()
    {
        if (!isActive() || m_drawables.empty())
            return;

        const auto& rend = getRenderer();

        const auto& target = rend.getRenderTarget();
        const auto& cameras = rend.getCameras();
        const auto& lights = rend.getLights();

        for (auto cam : cameras)
        {
            if (!cam->isActive() || !cam->getRenderMask())
                continue;

            const Drawable::ProjectionInfo projInfo(cam->getViewMatrix(), cam->getProjectionMatrix(), cam->getObject()->getGlobalPosition());

            if (!cam->getRenderTexture().bind())
                target.bind();

            cam->applyViewport(target);

            // 0 - Opaque
            // 1 - Translucent
            // 2 - Skybox & skysphere
            //
            std::vector<const Drawable*> sorted[3];
            sorted[0].reserve(m_drawables.size());

            for (auto d : m_drawables)
            {
                if (!d->isActive() || !cam->inView(*d))
                    continue;

                static const uint64 skyAttrib = Drawable::Attribute::__SkyBox | Drawable::Attribute::__SkySphere;

                sorted[std::min(2, d->hasAlpha() + ((d->getAttributes() & skyAttrib) != 0) * 2)].push_back(d);
            }

            std::sort(sorted[0].begin(), sorted[0].end(), [&projInfo](const Drawable* left, const Drawable* right) -> bool
            {
                return glm::distance2(left->getObject()->getGlobalPosition(), projInfo.cameraPosition) <
                       glm::distance2(right->getObject()->getGlobalPosition(), projInfo.cameraPosition);
            });

            std::sort(sorted[1].begin(), sorted[1].end(), [&projInfo](const Drawable* left, const Drawable* right) -> bool
            {
                return glm::distance2(left->getObject()->getGlobalPosition(), projInfo.cameraPosition) >
                       glm::distance2(right->getObject()->getGlobalPosition(), projInfo.cameraPosition);
            });

            auto drawSet = [&projInfo, &lights](const std::vector<const Drawable*>& set) -> void
            {
                for (auto d : set)
                {
                    if (d->hasFlag(Drawable::ReceiveLights))
                    {
                        LightContainer lightCont;

                        for (auto l : lights)
                        {
                            if (lightCont[l->getType()].size() < LightSource::getMaximumLights(l->getType()))
                                lightCont[l->getType()].push_back(l);
                        }

                        d->draw(projInfo, lightCont);
                    }
                    else
                        d->draw(projInfo, ns_dummyLightCont);
                }
            };

            GlState::setDepthTest(true);
            GlState::setDepthWrite(true);
            GlState::setBlendFunc(false);
            drawSet(sorted[0]);
            drawSet(sorted[2]);

            GlState::setBlendFunc(true);
            GlState::setDepthWrite(false);
            drawSet(sorted[1]);
            GlState::setDepthWrite(true);
        }
    }

    //////////////////////////////////////////////

    void SortedRenderPass::bind(const Drawable* drawable)
    {
        m_drawables.insert(drawable);
    }

    //////////////////////////////////////////////

    void SortedRenderPass::unbind(const Drawable* drawable)
    {
        m_drawables.erase(drawable);
    }

    //////////////////////////////////////////////

    OrderedRenderPass::OrderedRenderPass(Renderer& renderer, const RenderTarget&, const Pass pass, const uint32 weight)
        : RenderPass(renderer, Engine::getMainWindow(), pass, weight)
    {}

    //////////////////////////////////////////////

    void OrderedRenderPass::draw()
    {
        if (!isActive() || m_drawables.empty())
            return;

        const auto& rend = getRenderer();

        const auto& target = m_target;
        const auto& cameras = rend.getCameras();

        GlState::setDepthTest(true, GlState::DepthFunc::Always);
        GlState::setBlendFunc(true);
        GlState::setDepthWrite(false);

        for (auto cam : cameras)
        {
            if (!cam->isActive() || !cam->getRenderMask())
                continue;

            const Drawable::ProjectionInfo projInfo(cam->getViewMatrix(), cam->getProjectionMatrix(), cam->getObject()->getGlobalPosition());

            if (!cam->getRenderTexture().bind())
                target.bind();

            cam->applyViewport(target);

            for (auto d : m_drawables)
            {
                if (!d->isActive() || !cam->inView(*d))
                    continue;

                d->draw(projInfo, ns_dummyLightCont);
            }
        }

        GlState::setDepthTest(true);
        GlState::setDepthWrite(true);
    }

    //////////////////////////////////////////////

    void OrderedRenderPass::bind(const Drawable* drawable)
    {
        m_drawables.push_back(drawable);
    }

    //////////////////////////////////////////////

    void OrderedRenderPass::unbind(const Drawable* drawable)
    {
        for (auto itr = m_drawables.begin(); itr != m_drawables.end(); ++itr)
        {
            if (*itr == drawable)
            {
                m_drawables.erase(itr);
                return;
            }
        }
    }


    //////////////////////////////////////////////

    
    namespace detail
    {
        RenderPassProxy::RenderPassProxy(const RenderPass::Pass pass)
            : Subsystem (static_cast<uint32>(pass)),
              m_pass    (pass)
        {}

        //////////////////////////////////////////////

        void RenderPassProxy::draw()
        {
            if (Engine::getState() != Engine::State::Frozen)
            {
                if (Engine::hasCurrentScene() && Engine::getCurrentScene().isActive())
                    Engine::getCurrentScene().getRenderer().draw(m_pass);

                if (Engine::hasSharedScene() && Engine::getSharedScene().isActive())
                    Engine::getSharedScene().getRenderer().draw(m_pass);
            }
        }
    }
}