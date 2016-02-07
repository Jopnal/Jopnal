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
    TextureSampler::TextureSampler()
        : m_sampler(0)
    {
        reset();
    }

    TextureSampler::~TextureSampler()
    {
        if (m_sampler)
            glCheck(gl::DeleteSamplers(1, &m_sampler));
    }

    //////////////////////////////////////////////

    bool TextureSampler::load(const Filter filterMode, const Repeat repeatMode, const float param)
    {
        setFilterMode(filterMode, param);
        setRepeatMode(repeatMode);

        return true;
    }

    //////////////////////////////////////////////

    void TextureSampler::bind(const unsigned int textureUnit) const
    {
        if (m_sampler)
            glCheck(gl::BindSampler(textureUnit, m_sampler));
    }

    //////////////////////////////////////////////

    void TextureSampler::reset()
    {
        if (m_sampler)
            glCheck(gl::DeleteSamplers(1, &m_sampler));

        glCheck(gl::GenSamplers(1, &m_sampler));
    }

    //////////////////////////////////////////////

    void TextureSampler::setFilterMode(const Filter mode, const float param)
    {
        if (m_sampler)
        {
            switch (mode)
            {
                case Filter::None:
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_MIN_FILTER, gl::NEAREST));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_MAG_FILTER, gl::NEAREST));
                    break;
                case Filter::Bilinear:
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_MIN_FILTER, gl::LINEAR));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_MAG_FILTER, gl::LINEAR));
                    break;
                case Filter::Trilinear:
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_MIN_FILTER, gl::LINEAR_MIPMAP_LINEAR));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_MAG_FILTER, gl::LINEAR));
                    break;
                case Filter::Anisotropic:
                    if (getMaxAnisotropy() > 0.f)
                    {
                        glCheck(gl::SamplerParameterf(m_sampler, gl::TEXTURE_MAX_ANISOTROPY_EXT, std::min(param, getMaxAnisotropy())));
                    }
                    else
                        // Should never happen but just to be sure.
                        JOP_DEBUG_WARNING("Anisotropic filtering is not supported on this system");
                    break;
                default:
                    JOP_DEBUG_WARNING("Incorrect sampler filtering mode specified");
                    break;
            }
        }
    }

    //////////////////////////////////////////////

    void TextureSampler::setRepeatMode(const Repeat repeat)
    {
        if (m_sampler)
        {
            switch (repeat)
            {
                case Repeat::Basic:
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_S, gl::REPEAT));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_T, gl::REPEAT));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_R, gl::REPEAT));
                    break;
                case Repeat::Mirrored:
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_S, gl::MIRRORED_REPEAT));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_T, gl::MIRRORED_REPEAT));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_R, gl::MIRRORED_REPEAT));
                    break;
                case Repeat::ClampEdge:
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_R, gl::CLAMP_TO_EDGE));
                    break;
                case Repeat::ClampBorder:
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_BORDER));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_BORDER));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_R, gl::CLAMP_TO_BORDER));
                    break;
                default:
                    JOP_DEBUG_WARNING("Invalid sampler repeat mode specified");
                    break;
            }
        }
    }

    //////////////////////////////////////////////

    void TextureSampler::setBorderColor(const Color& color)
    {
        if (m_sampler)
        {
            const glm::vec4 col = color.asFloatVector();
            glCheck(gl::SamplerParameterfv(m_sampler, gl::TEXTURE_BORDER_COLOR, &col[0]));
        }
    }

    //////////////////////////////////////////////

    unsigned int TextureSampler::getHandle() const
    {
        return m_sampler;
    }

    //////////////////////////////////////////////

    float TextureSampler::getMaxAnisotropy()
    {
        float level = 0.f;

        if (gl::exts::var_EXT_texture_filter_anisotropic)
            glCheck(gl::GetFloatv(gl::MAX_TEXTURE_MAX_ANISOTROPY_EXT, &level));

        return level;
    }

    //////////////////////////////////////////////

    const TextureSampler& TextureSampler::getDefault()
    {
        auto defSampler = ResourceManager::getNamedResource<TextureSampler>
        (
            "DefaultSampler",
            static_cast<Filter>(SettingManager::getUint("uDefaultTextureFilterMode", 1)),
            static_cast<Repeat>(SettingManager::getUint("uDefaultTextureRepeatMode", 2)),
            SettingManager::getFloat("fDefaultTextureAnisotropyLevel", 1.f)
        );

        JOP_ASSERT(!defSampler.expired(), "Couldn't create default sampler!");

        return *defSampler.lock();
    }
}