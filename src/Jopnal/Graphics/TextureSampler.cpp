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
        : m_sampler     (0),
          m_filter      (Filter::None),
          m_repeat      (Repeat::Basic),
          m_anisotropic (1.f),
          m_borderColor ()
    {
        static const Filter defFilter = static_cast<Filter>(SettingManager::get<unsigned int>("engine@Graphics|Texture|uDefaultFilterMode", 0));
        static const Repeat defRepeat = static_cast<Repeat>(SettingManager::get<unsigned int>("engine@Graphics|Texture|uDefaultRepeatMode", 0));
        static const float defAniso = SettingManager::get<float>("engine@Graphics|Texture|fDefaultAnisotropyLevel", 1.f);
        static const Color defColor = Color(SettingManager::get<std::string>("engine@Graphics|Texture|fDefaultBorderColor", "FFFFFFFF"));

        reset().setFilterMode(defFilter, defAniso).setRepeatMode(defRepeat).setBorderColor(defColor);
    }

    TextureSampler::TextureSampler(const Filter filter, const Repeat repeat, const float param)
        : m_sampler     (0),
          m_filter      (Filter::None),
          m_repeat      (Repeat::Basic),
          m_anisotropic (1.f),
          m_borderColor ()
    {
        reset().setFilterMode(filter, param).setRepeatMode(repeat);
    }

    TextureSampler::TextureSampler(const TextureSampler& other)
        : m_sampler     (0),
          m_filter      (Filter::None),
          m_repeat      (Repeat::Basic),
          m_anisotropic (1.f),
          m_borderColor ()
    {
        *this = other;
    }

    TextureSampler& TextureSampler::operator =(const TextureSampler& other)
    {
        return reset()
              .setFilterMode(other.getFilteringMode(), other.getAnisotropyLevel())
              .setRepeatMode(other.getRepeatMode()).setBorderColor(other.getBorderColor());
    }

    TextureSampler::TextureSampler(TextureSampler&& other)
        : m_sampler     (other.m_sampler),
          m_filter      (other.m_filter),
          m_repeat      (other.m_repeat),
          m_anisotropic (other.m_anisotropic),
          m_borderColor (other.m_borderColor)
    {
        other.m_sampler = 0;
    }

    TextureSampler& TextureSampler::operator=(TextureSampler&& other)
    {
        m_sampler = other.m_sampler;
        other.m_sampler = 0;

        m_filter = other.m_filter;
        m_repeat = other.m_repeat;
        m_anisotropic = other.m_anisotropic;
        m_borderColor = other.m_borderColor;

        return *this;
    }

    TextureSampler::~TextureSampler()
    {
        if (m_sampler)
            glCheck(gl::DeleteSamplers(1, &m_sampler));
    }

    //////////////////////////////////////////////

    void TextureSampler::bind(const unsigned int textureUnit) const
    {
        if (m_sampler)
            glCheck(gl::BindSampler(textureUnit, m_sampler));
    }

    //////////////////////////////////////////////

    TextureSampler& TextureSampler::reset()
    {
        if (m_sampler)
            glCheck(gl::DeleteSamplers(1, &m_sampler));

        glCheck(gl::GenSamplers(1, &m_sampler));

        return *this;
    }

    //////////////////////////////////////////////

    TextureSampler& TextureSampler::setFilterMode(const Filter mode, const float param)
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
                        glCheck(gl::SamplerParameterf(m_sampler, gl::TEXTURE_MAX_ANISOTROPY_EXT, glm::clamp(param, 1.f, getMaxAnisotropy())));
                    }
                    else
                        // Should never happen but just to be sure.
                        JOP_DEBUG_WARNING("Anisotropic filtering is not supported on this system");
            }

            m_filter = mode;
            m_anisotropic = param;
        }

        return *this;
    }

    //////////////////////////////////////////////

    TextureSampler& TextureSampler::setRepeatMode(const Repeat repeat)
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
            }

            m_repeat = repeat;
        }

        return *this;
    }

    //////////////////////////////////////////////

    TextureSampler& TextureSampler::setBorderColor(const Color& color)
    {
        if (m_sampler && m_borderColor != color)
        {
            glCheck(gl::SamplerParameterfv(m_sampler, gl::TEXTURE_BORDER_COLOR, &m_borderColor.colors[0]));

            m_borderColor = color;
        }

        return *this;
    }

    //////////////////////////////////////////////

    unsigned int TextureSampler::getHandle() const
    {
        return m_sampler;
    }

    //////////////////////////////////////////////

    TextureSampler::Filter TextureSampler::getFilteringMode() const
    {
        return m_filter;
    }

    //////////////////////////////////////////////

    TextureSampler::Repeat TextureSampler::getRepeatMode() const
    {
        return m_repeat;
    }

    //////////////////////////////////////////////

    float TextureSampler::getAnisotropyLevel() const
    {
        return m_anisotropic;
    }

    //////////////////////////////////////////////

    const Color& TextureSampler::getBorderColor() const
    {
        return m_borderColor;
    }

    //////////////////////////////////////////////

    float TextureSampler::getMaxAnisotropy()
    {
        float level = 0.f;

        if (gl::exts::var_EXT_texture_filter_anisotropic)
            glCheck(gl::GetFloatv(gl::MAX_TEXTURE_MAX_ANISOTROPY_EXT, &level));

        return level;
    }
}