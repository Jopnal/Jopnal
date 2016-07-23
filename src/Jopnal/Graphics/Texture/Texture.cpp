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

    #include <Jopnal/Graphics/Texture/Texture.hpp>

    #include <Jopnal/Core/DebugHandler.hpp>
    #include <Jopnal/Core/SettingManager.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>

#endif

//////////////////////////////////////////////


namespace
{
    bool ns_allowSRGB = true;

    void setGLFilterMode(const GLenum target, const jop::TextureSampler::Filter mode, const float param)
    {
        using Filter = jop::TextureSampler::Filter;

        switch (mode)
        {
            case Filter::None:
                glCheck(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
                glCheck(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
                break;
            case Filter::Bilinear:
                glCheck(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
                glCheck(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
                break;
            case Filter::Trilinear:
                glCheck(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
                glCheck(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
                break;
            case Filter::Anisotropic:
                if (jop::TextureSampler::getMaxAnisotropy() > 0.f)
                {
                    glCheck(glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, glm::clamp(param, 1.f, jop::TextureSampler::getMaxAnisotropy())));
                }
                else
                    // Should never happen but just to be sure.
                    JOP_DEBUG_WARNING_ONCE("Anisotropic filtering is not supported on this system");
        }
    }

    void setGLRepeatMode(const GLenum target, const jop::TextureSampler::Repeat repeat)
    {
        using Repeat = jop::TextureSampler::Repeat;

        switch (repeat)
        {
            case Repeat::Basic:
                glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT));
                glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT));
                glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_REPEAT));
                break;
            case Repeat::Mirrored:
                glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
                glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
                glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT));
                break;
            case Repeat::ClampEdge:
                glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
                glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
                glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
                break;

        #ifndef JOP_OPENGL_ES

            case Repeat::ClampBorder:
                glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
                glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
                glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER));

        #endif
        }
    }

    void setGLBorderColor(const GLenum target, const jop::Color& color)
    {
    #ifndef JOP_OPENGL_ES
        glCheck(glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, &color.colors[0]));
    #endif
    }
}

namespace jop
{
    Texture::Texture(const std::string& name, const unsigned int glTarget)
        : Resource          (name),
          m_sampler         (),
          m_texture         (0),
          m_target          (glTarget),
          m_filter          (TextureSampler::Filter::None),
          m_repeat          (TextureSampler::Repeat::Basic),
          m_anisotropic     (1.f),
          m_borderColor     ()
    {
        static const TextureSampler::Filter defFilter = static_cast<TextureSampler::Filter>(SettingManager::get<unsigned int>("engine@Graphics|Texture|uDefaultFilterMode", 0));
        static const TextureSampler::Repeat defRepeat = static_cast<TextureSampler::Repeat>(SettingManager::get<unsigned int>("engine@Graphics|Texture|uDefaultRepeatMode", 0));
        static const float defAniso = SettingManager::get<float>("engine@Graphics|Texture|fDefaultAnisotropyLevel", 16.f);
        static const Color defColor = Color(SettingManager::get<std::string>("engine@Graphics|Texture|fDefaultBorderColor", "FFFFFFFF"));

        m_filter = defFilter;
        m_repeat = defRepeat;
        m_anisotropic = defAniso;
        m_borderColor = defColor;
    }

    Texture::~Texture()
    {
        destroy();
    }

    //////////////////////////////////////////////

    void Texture::destroy()
    {
        if (m_texture)
        {
            glCheck(glDeleteTextures(1, &m_texture));
            m_texture = 0;
        }
    }

    //////////////////////////////////////////////

    bool Texture::bind(const unsigned int texUnit) const
    {
        if (!m_texture)
        {
            glCheck(glGenTextures(1, &m_texture));
            glCheck(glActiveTexture(GL_TEXTURE0 + texUnit));
            glCheck(glBindTexture(m_target, m_texture));
            updateSampling();
        }

        glCheck(glActiveTexture(GL_TEXTURE0 + texUnit));
        glCheck(glBindTexture(m_target, m_texture));

        if (!m_sampler.expired())
            m_sampler->bind(texUnit);

        return isValid();
    }

    //////////////////////////////////////////////

    bool Texture::isValid() const
    {
        return m_texture != 0;
    }

    //////////////////////////////////////////////

    void Texture::setSampler(const TextureSampler& sampler)
    {
        m_sampler = static_ref_cast<const TextureSampler>(sampler.getReference());
    }

    //////////////////////////////////////////////

    const TextureSampler& Texture::getSampler() const
    {
        return *m_sampler;
    }

    //////////////////////////////////////////////

    Texture& Texture::setFilterMode(const TextureSampler::Filter mode, const float param)
    {
        if (bind())
        {
            setGLFilterMode(m_target, mode, param);

            m_filter = mode;
            m_anisotropic = param;
        }

        return *this;
    }

    //////////////////////////////////////////////

    Texture& Texture::setRepeatMode(const TextureSampler::Repeat repeat)
    {
        if (bind())
        {
            setGLRepeatMode(m_target, repeat);

            m_repeat = repeat;
        }

        return *this;
    }

    //////////////////////////////////////////////

    Texture& Texture::setBorderColor(const Color& color)
    {
        if (bind())
        {
            setGLBorderColor(m_target, color);
            m_borderColor = color;
        }

        return *this;
    }

    //////////////////////////////////////////////

    TextureSampler::Filter Texture::getFilteringMode() const
    {
        return m_filter;
    }

    //////////////////////////////////////////////

    TextureSampler::Repeat Texture::getRepeatMode() const
    {
        return m_repeat;
    }

    //////////////////////////////////////////////

    float Texture::getAnisotropyLevel() const
    {
        return m_anisotropic;
    }

    //////////////////////////////////////////////

    const Color& Texture::getBorderColor() const
    {
        return m_borderColor;
    }

    //////////////////////////////////////////////

    unsigned int Texture::getHandle() const
    {
        return m_texture;
    }

    //////////////////////////////////////////////

    unsigned int Texture::getMaximumSize()
    {
        static unsigned int size = 0;
        if (!size)
            glCheck(glGetIntegerv(GL_MAX_TEXTURE_SIZE, reinterpret_cast<GLint*>(&size)));

        return size;
    }

    //////////////////////////////////////////////

    unsigned int Texture::getMaxTextureUnits()
    {
        static unsigned int maxUnits = 0;
        if (!maxUnits)
            glCheck(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, reinterpret_cast<GLint*>(&maxUnits)));

        return maxUnits;
    }

    //////////////////////////////////////////////

    void Texture::setPixelStore(const unsigned int depth)
    {
        GLint param = 4;
        switch (depth)
        {
            case 1:
                param = 1;
                break;

            case 2:
                param = 2;
                break;

            case 8:
                param = 8;
        }

        glCheck(glPixelStorei(GL_UNPACK_ALIGNMENT, param));
    }

    //////////////////////////////////////////////

    void Texture::setAllowSRGB(const bool allow)
    {
        ns_allowSRGB = allow;
    }

    //////////////////////////////////////////////

    bool Texture::allowSRGB()
    {
    #ifdef JOP_OPENGL_ES

        #if __ANDROID_API__ < 18 && !defined(GL_EXT_sRGB)
            return false;

        #else

            if (jop::gl::getVersionMajor() < 3)
                return JOP_CHECK_GL_EXTENSION(GL_EXT_sRGB) && ns_allowSRGB;

        #endif

    #endif

        return ns_allowSRGB;
    }

    //////////////////////////////////////////////

    void Texture::updateSampling() const
    {
        setGLFilterMode(m_target, m_filter, m_anisotropic);
        setGLRepeatMode(m_target, m_repeat);
        setGLBorderColor(m_target, m_borderColor);
    }
}