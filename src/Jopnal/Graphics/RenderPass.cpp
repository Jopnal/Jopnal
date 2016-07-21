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
            : Subsystem (0),
              m_pass    (pass)
        {}

        //////////////////////////////////////////////

        void RenderPassProxy::draw()
        {
            if (Engine::hasCurrentScene())
                Engine::getCurrentScene().getRenderer().draw(m_pass);

            if (Engine::hasSharedScene())
                Engine::getSharedScene().getRenderer().draw(m_pass);
        }
    }

    //////////////////////////////////////////////

    RenderPass::RenderPass(Renderer& renderer, const RenderTarget& target)
        : m_drawables   (),
          m_rendererRef (renderer),
          m_target      (target)
    {}

    //////////////////////////////////////////////

    void RenderPass::draw(const unsigned int group)
    {
        // Dummy LightContainer to pass into drawables when no lights were selected
        static const LightContainer dummyLightCont;

        auto& rend = m_rendererRef;

        m_target.bind();

        for (auto cam : rend.m_cameras)
        {
            const uint32 camMask = cam->getRenderMask();
            if (!cam->isActive() || (camMask & group) == 0)
                continue;

            cam->getRenderTexture().bind();
            cam->applyViewport(m_target);

            // 0 - Opaque
            // 1 - Translucent
            // 2 - Sky box/sphere
            std::array<std::vector<const Drawable*>, 3> sorted;
            sorted[0].reserve(m_drawables.size());
            sorted[1].reserve(m_drawables.size() / 4);

            for (auto drawable : m_drawables)
            {
                const uint32 groupBit = 1 << drawable->getRenderGroup();
                if (drawable->isActive() && drawable->getModel().isValid() && (group & groupBit) > 0 && (camMask & groupBit) > 0)
                {
                    using M = Material::Attribute;
                    auto& mat = *drawable->getModel().getMaterial();

                    sorted[std::min(2, mat.hasAttributes(M::Alpha | M::DiffuseAlpha) + (mat.hasAttribute(M::__SkyBox | M::__SkySphere) * 2))].push_back(drawable);
                }
            }

            std::sort(sorted[0].begin(), sorted[0].end(), detail::DrawableSorterOpaque(*cam));
            std::sort(sorted[1].begin(), sorted[1].end(), detail::DrawableSorterTranslucent(*cam));

            auto drawSet = [this](const std::vector<const Drawable*>& drawables, const Camera& cam)
            {
                for (auto drawable : drawables)
                {
                    if (drawable->receiveLights())
                    {
                        // Select lights
                        LightContainer lights;
                        chooseLights(*drawable, lights);
                        drawable->draw(cam, lights);
                    }
                    else
                        drawable->draw(cam, dummyLightCont);
                }
            };

            // Opaque
            drawSet(sorted[0], *cam);

            // Sky boxes/spheres
            drawSet(sorted[2], *cam);

            // Translucent
            GlState::setDepthWrite(false);
            drawSet(sorted[1], *cam);
            GlState::setDepthWrite(true);
        }
    }

    //////////////////////////////////////////////

    void RenderPass::bind(const Drawable& drawable)
    {
        m_drawables.insert(&drawable);
    }

    //////////////////////////////////////////////

    void RenderPass::unbind(const Drawable& drawable)
    {
        m_drawables.erase(&drawable);
    }

    //////////////////////////////////////////////

    void RenderPass::chooseLights(const Drawable& drawable, LightContainer& lights) const
    {
        for (auto l : m_rendererRef.m_lights)
        {
            auto& container = lights[l->getType()];

            if (!l->isActive() || LightSource::getMaximumLights(l->getType()) <= container.size())
                continue;

            const uint32 lightMask = l->getRenderMask();
            if ((m_rendererRef.m_mask & lightMask) == 0 || (lightMask & (1 << drawable.getRenderGroup())) == 0)
                continue;

            if (drawable.lightTouches(*l))
                container.push_back(l);
        }
    }
}