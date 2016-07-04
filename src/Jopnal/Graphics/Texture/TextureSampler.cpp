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

    #include <Jopnal/Graphics/Texture/TextureSampler.hpp>

    #include <Jopnal/Core/DebugHandler.hpp>
    #include <Jopnal/Core/SettingManager.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>

#endif

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
        static const float defAniso = SettingManager::get<float>("engine@Graphics|Texture|fDefaultAnisotropyLevel", 16.f);
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
            glCheck(glDeleteSamplers(1, &m_sampler));
    }

    //////////////////////////////////////////////

    void TextureSampler::bind(const unsigned int textureUnit) const
    {
        if (m_sampler)
            glCheck(glBindSampler(textureUnit, m_sampler));
    }

    //////////////////////////////////////////////

    TextureSampler& TextureSampler::reset()
    {
        if (m_sampler)
            glCheck(glDeleteSamplers(1, &m_sampler));

        glCheck(glGenSamplers(1, &m_sampler));

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
                    glCheck(glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
                    glCheck(glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
                    break;                                                        
                case Filter::Bilinear:                                            
                    glCheck(glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
                    glCheck(glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
                    break;                                                        
                case Filter::Trilinear:                                           
                    glCheck(glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
                    glCheck(glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
                    break;
                case Filter::Anisotropic:
                    if (getMaxAnisotropy() > 0.f)
                    {
                        glCheck(glSamplerParameterf(m_sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, glm::clamp(param, 1.f, getMaxAnisotropy())));
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
                    glCheck(glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, GL_REPEAT));
                    glCheck(glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, GL_REPEAT));
                    glCheck(glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_R, GL_REPEAT));
                    break;
                case Repeat::Mirrored:
                    glCheck(glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
                    glCheck(glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
                    glCheck(glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT));
                    break;
                case Repeat::ClampEdge:
                    glCheck(glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
                    glCheck(glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
                    glCheck(glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
                    break;

            #ifndef JOP_OPENGL_ES

                case Repeat::ClampBorder:
                    glCheck(glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
                    glCheck(glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
                    glCheck(glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER));

            #endif
            }

            m_repeat = repeat;
        }

        return *this;
    }

    //////////////////////////////////////////////

    TextureSampler& TextureSampler::setBorderColor(const Color& color)
    {
    #ifndef JOP_OPENGL_ES

        if (m_sampler && m_borderColor != color)
        {
            glCheck(glSamplerParameterfv(m_sampler, GL_TEXTURE_BORDER_COLOR, &m_borderColor.colors[0]));

            m_borderColor = color;
        }

    #else

        color;

    #endif

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

        if (JOP_CHECK_GL_EXTENSION(EXT_texture_filter_anisotropic))
            glCheck(glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &level));

        return level;
    }
}