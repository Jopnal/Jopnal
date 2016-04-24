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
    JOP_DERIVED_COMMAND_HANDLER(Component, EnvironmentRecorder)

        JOP_BIND_MEMBER_COMMAND(&EnvironmentRecorder::setRenderMask, "setEnvRecorderRenderMask");

    JOP_END_COMMAND_HANDLER(EnvironmentRecorder)

    JOP_REGISTER_LOADABLE(jop, EnvironmentRecorder)[](Object& obj, const Scene& scene, const json::Value& val) -> bool
    {
        auto& rec = obj.createComponent<EnvironmentRecorder>(scene.getRenderer());

        const char* const idField = "id";
        if (val.HasMember(idField) && val[idField].IsString())
            rec.setID(val[idField].GetString());

        const char* const maskField = "mask";
        if (val.HasMember(maskField) && val[maskField].IsUint())
            rec.setRenderMask(val[maskField].GetUint());

        return true;
    }
    JOP_END_LOADABLE_REGISTRATION(EnvironmentRecorder)

    JOP_REGISTER_SAVEABLE(jop, EnvironmentRecorder)[](const Component& comp, json::Value& val, json::Value::AllocatorType& alloc) -> bool
    {
        auto& rec = static_cast<const EnvironmentRecorder&>(comp);

        val.AddMember(json::StringRef("id"), json::StringRef(rec.getID().c_str()), alloc);
        val.AddMember(json::StringRef("mask"), rec.getRenderMask(), alloc);

        return true;
    }
    JOP_END_SAVEABLE_REGISTRATION(EnvironmentRecorder)
}

namespace jop
{
    EnvironmentRecorder::EnvironmentRecorder(Object& obj, Renderer& renderer)
        : Component     (obj, "environmentrecorder"),
          m_matrices    (6),
          m_fbo         (),
          m_mask        (1),
          m_rendererRef (renderer)
    {
        static const int mapResolution = SettingManager::get<unsigned int>("engine/Graphics|Shading|uEnvironmentMapSize", 128);

        using ca = RenderTexture::ColorAttachment;
        using da = RenderTexture::DepthAttachment;

        m_fbo.create(glm::uvec2(mapResolution), ca::RGBACube, da::Texture16);

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
        static const float farPlane = SettingManager::get<float>("engine/Graphics|Shading|fEnvironmentMapFarPlane", 1000.f);
        static const glm::mat4 proj = glm::perspective(glm::half_pi<float>(), 1.f, 0.5f, farPlane);

        LightSource::makeCubemapMatrices(proj, getObject()->getGlobalPosition(), m_matrices);

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
                shdr->setUniform("u_PVMatrices", glm::value_ptr(m_matrices[0]), 6);

                lastShader = shdr;
            }

            LightContainer lights;
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
        return m_fbo.getTexture();
    }
}