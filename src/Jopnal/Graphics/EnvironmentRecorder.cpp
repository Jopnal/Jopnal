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
    EnvironmentRecorder::EnvironmentRecorder(Object& obj, Renderer& renderer)
        : Component (obj, "environmentrecorder"),
          m_matrices(6),
          m_fbo     (),
          m_rendererRef(renderer)
    {
        static const int mapResolution = SettingManager::getUint("uEnvironmentMapSize", 256);

        m_fbo.create(RenderTexture::ColorAttachment::RGBACube, glm::ivec2(mapResolution)/*, RenderTexture::DepthAttachment::Renderbuffer24, RenderTexture::StencilAttachment::Int8*/);

        m_rendererRef.bind(*this);
    }

    EnvironmentRecorder::EnvironmentRecorder(const EnvironmentRecorder& other, Object& newObj)
        : Component (other, newObj),
          m_fbo     (),
          m_rendererRef(other.m_rendererRef)
    {
        m_rendererRef.bind(*this);
    }

    EnvironmentRecorder::~EnvironmentRecorder()
    {
        m_rendererRef.unbind(*this);
    }

    //////////////////////////////////////////////

    void EnvironmentRecorder::record()
    {
        static const float farPlane = SettingManager::getFloat("fEnvirinmentMapFarPlane", 1000.f);
        static const glm::mat4 proj = glm::perspective(glm::half_pi<float>(), 1.f, 1.f, farPlane);

        LightSource::makeCubemapMatrices(proj, getObject()->getGlobalPosition(), m_matrices);

        auto& rend = m_rendererRef;

        m_fbo.clear();

        Shader* lastShader = nullptr;

        for (uint32 i = 1; i != 0; i <<= 1)
        {
            if ((rend.m_mask & i) == 0)
                continue;

            for (auto drawable : rend.m_drawables)
            {
                uint32 drawableBit = 1 << drawable->getRenderGroup();
                if (!drawable->isActive() || (i & drawableBit) == 0)
                    continue;

                auto shdr = &ShaderManager::getShader(drawable->getModel().getMaterial()->getAttributeField() | Material::Attribute::RecordEnv);

                if (shdr == &Shader::getDefault())
                {
                    JOP_DEBUG_ERROR("Couldn't compile environment record shader. Trying to draw the rest...");
                    continue;
                }

                if (lastShader != shdr)
                {
                    shdr->setUniform("u_PVMatrices", glm::value_ptr(m_matrices[0]), 6);

                    lastShader = shdr;
                }

                LightContainer lights;
                rend.chooseLights(*drawable, lights);

                drawable->draw(nullptr, lights, *shdr);
            }
        }
    }

    //////////////////////////////////////////////

    void EnvironmentRecorder::setRenderMask(const uint32 mask)
    {
        m_mask = mask;
    }

    //////////////////////////////////////////////

    uint32 EnvironmentRecorder::getRenderMask() const
    {
        return m_mask;
    }

    //////////////////////////////////////////////

    const Texture* EnvironmentRecorder::getTexture() const
    {
        return m_fbo.getTexture();
    }
}