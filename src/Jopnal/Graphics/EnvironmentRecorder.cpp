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

#ifndef JOP_PRECOMPILED_HEADER

	#include <Jopnal/Graphics/EnvironmentRecorder.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_COMMAND_HANDLER(EnvironmentRecorder)

        JOP_BIND_MEMBER_COMMAND(&EnvironmentRecorder::setRenderMask, "setEnvRecorderRenderMask");

    JOP_END_COMMAND_HANDLER(EnvironmentRecorder)
}

namespace jop
{
    EnvironmentRecorder::EnvironmentRecorder(Object& obj, Renderer& renderer)
        : Component     (obj, 0),
          m_matrices    (6),
          m_fbo         (),
          m_mask        (1),
          m_rendererRef (renderer)
    {
        static const int mapResolution = SettingManager::get<unsigned int>("engine@Graphics|Shading|uEnvironmentMapSize", 128);

        using CA = RenderTexture::ColorAttachment;
        using DA = RenderTexture::DepthAttachment;

        m_fbo.addColorAttachment(RenderTexture::ColorAttachmentSlot::_1, CA::RGBACubeFloat16, glm::uvec2(mapResolution));
        m_fbo.addDepthAttachment(DA::TextureCube16, glm::uvec2(mapResolution));

        m_fbo.getColorTexture(RenderTexture::ColorAttachmentSlot::_1)->getSampler().setFilterMode(TextureSampler::Filter::Bilinear);

        m_rendererRef.bind(*this);
    }

    EnvironmentRecorder::EnvironmentRecorder(const EnvironmentRecorder& other, Object& newObj)
        : Component     (other, newObj),
          m_matrices    (6),
          m_fbo         (),
          m_mask        (other.m_mask),
          m_rendererRef (other.m_rendererRef)
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
        static const struct Callback : SettingCallback<float>
        {
            const char* const str;
            float farPlane;
            glm::mat4 proj;
            void updateProj()
            {
                proj = glm::perspective(glm::half_pi<float>(), 1.f, 0.1f, farPlane);
            }
            Callback()
                : str("engine@Graphics|Shading|fEnvironmentRecordFarPlane"),
                  farPlane(SettingManager::get<float>(str, 1000.f)),
                  proj()
            {
                updateProj();
                SettingManager::registerCallback(str, *this);
            }
            void valueChanged(const float& value) override
            {
                farPlane = value;
                updateProj();
            }
        } cb;

        LightSource::makeCubemapMatrices(cb.proj, getObject()->getGlobalPosition(), m_matrices);

        auto& rend = m_rendererRef;

        m_fbo.clear(RenderTarget::ColorBit | RenderTarget::DepthBit);

        Shader* lastShader = nullptr;

        for (auto drawable : rend.m_drawables)
        {
            uint32 drawableBit = 1 << drawable->getRenderGroup();
            if (!drawable->isActive() || !drawable->getModel().isValid() || !drawable->isReflected() || (m_mask & drawableBit) == 0)
                continue;

            auto shdr = &ShaderAssembler::getShader(drawable->getModel().getMaterial()->getAttributeField() | Material::Attribute::__RecordEnv);

            if (shdr == &Shader::getDefault())
            {
                JOP_DEBUG_ERROR("Couldn't compile environment record shader. Trying to draw the rest...");
                continue;
            }

            if (lastShader != shdr)
            {
                if (drawable->getModel().getMaterial()->hasAttribute(Material::Attribute::__SkyBox | Material::Attribute::__SkySphere))
                {
                    std::vector<glm::mat4> tempMat(6);

                    LightSource::makeCubemapMatrices(cb.proj, glm::vec3(0.f), tempMat);

                    shdr->setUniform("u_PVMatrices", glm::value_ptr(tempMat[0]), 6);
                }
                else
                    shdr->setUniform("u_PVMatrices", glm::value_ptr(m_matrices[0]), 6);

                lastShader = shdr;
            }

            LightContainer lights;
            if (drawable->receiveLights())
                rend.chooseLights(*drawable, lights);

            drawable->draw(nullptr, lights, *shdr);
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
        return m_fbo.getColorTexture(RenderTexture::ColorAttachmentSlot::_1);
    }

    //////////////////////////////////////////////

    Message::Result EnvironmentRecorder::receiveMessage(const Message& message)
    {
        if (JOP_EXECUTE_COMMAND(EnvironmentRecorder, message.getString(), this) == Message::Result::Escape)
            return Message::Result::Escape;

        return Component::receiveMessage(message);
    }
}