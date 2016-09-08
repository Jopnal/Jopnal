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

    #include <Jopnal/Graphics/Renderer.hpp>

    #include <Jopnal/Core/Engine.hpp>
    #include <Jopnal/Core/Object.hpp>
    #include <Jopnal/Core/Scene.hpp>
    #include <Jopnal/Graphics/Camera.hpp>
    #include <Jopnal/Graphics/LightSource.hpp>
    #include <Jopnal/Graphics/EnvironmentRecorder.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    Renderer::Renderer(const RenderTarget& mainTarget, Scene& sceneRef)
        : m_lights          (),
          m_cameras         (),
          m_passes          (),
          m_envRecorders    (),
          m_target          (mainTarget),
          m_sceneRef        (sceneRef)
    {
        createRenderPass<SortedRenderPass>(RenderPass::Pass::BeforePost, RenderPass::DefaultWeight);
        createRenderPass<OrderedRenderPass>(RenderPass::Pass::AfterPost, RenderPass::DefaultWeight);
    }

    //////////////////////////////////////////////

    const RenderTarget& Renderer::getRenderTarget() const
    {
        return m_target;
    }

    //////////////////////////////////////////////

    const Renderer::CameraSet& Renderer::getCameras() const
    {
        return m_cameras;
    }

    //////////////////////////////////////////////

    const Renderer::LightSet& Renderer::getLights() const
    {
        return m_lights;
    }

    //////////////////////////////////////////////

    void Renderer::bind(const LightSource* light)
    {
        m_lights.insert(light);
    }

    //////////////////////////////////////////////

    void Renderer::bind(const Camera* camera)
    {
        m_cameras.insert(camera);
    }

    //////////////////////////////////////////////

    void Renderer::bind(const Drawable* drawable, const RenderPass::Pass pass)
    {
        m_passes[static_cast<int>(pass)][RenderPass::DefaultWeight]->bind(drawable);
    }

    //////////////////////////////////////////////

    void Renderer::bind(const EnvironmentRecorder* envRecorder)
    {
        m_envRecorders.insert(envRecorder);
    }

    //////////////////////////////////////////////

    void Renderer::unbind(const LightSource* light)
    {
        m_lights.erase(light);
    }

    //////////////////////////////////////////////

    void Renderer::unbind(const Camera* camera)
    {
        m_cameras.erase(camera);
    }

    //////////////////////////////////////////////

    void Renderer::unbind(const Drawable* drawable, const RenderPass::Pass pass)
    {
        m_passes[static_cast<int>(pass)][RenderPass::DefaultWeight]->unbind(drawable);
    }

    //////////////////////////////////////////////

    void Renderer::unbind(const EnvironmentRecorder* envRecorder)
    {
        m_envRecorders.erase(envRecorder);
    }

    //////////////////////////////////////////////

    void Renderer::removeRenderPass(const RenderPass::Pass pass, const uint32 weight)
    {
        m_passes[static_cast<int>(pass)].erase(weight);
    }

    //////////////////////////////////////////////

    void Renderer::draw(const RenderPass::Pass pass)
    {
        //if (pass == RenderPass::Pass::BeforePost)
        //{
            // Render shadow maps
            for (auto light : m_lights)
                light->drawShadowMap();

            // Render environment maps
            /*for (auto envmap : m_envRecorders)
            {
            if (envmap->isActive() && (m_mask & envmap->getRenderMask()) != 0)
            envmap->record();
            }*/
        //}

        // Render objects
        for (auto& i : m_passes[static_cast<int>(pass)])
            i.second->draw();
    }

    //////////////////////////////////////////////

    World& Renderer::getCullingWorld()
    {
        return m_sceneRef.m_cullingWorld;
    }
}