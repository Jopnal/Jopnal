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


namespace jop
{
    namespace detail
    {
        void setGLFilterMode(const GLenum target, const TextureSampler::Filter mode, const float param)
        {
            using Filter = TextureSampler::Filter;

            switch (mode)
            {
                case Filter::None:
                {
                    glCheck(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
                    glCheck(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
                    break;
                }
                case Filter::Bilinear:
                {
                    glCheck(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
                    glCheck(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
                    break;
                }
                case Filter::Trilinear:
                {
                    glCheck(glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
                    glCheck(glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
                    break;
                }
                case Filter::Anisotropic:
                {
                    if (TextureSampler::getMaxAnisotropy() > 0.f)
                    {
                        glCheck(glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, glm::clamp(param, 1.f, TextureSampler::getMaxAnisotropy())));
                    }
                    else
                        JOP_DEBUG_WARNING_ONCE("Anisotropic filtering is not supported on this system");
                }
            }
        }

        //////////////////////////////////////////////

        void setGLRepeatMode(const GLenum target, const TextureSampler::Repeat repeat)
        {
            using Repeat = TextureSampler::Repeat;

            switch (repeat)
            {
                case Repeat::Basic:
                {
                    glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT));
                    glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT));

                #if !defined(JOP_OPENGL_ES) || defined(JOP_OPENGL_ES3)
                    glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_REPEAT));
                #endif

                    break;
                }
                case Repeat::Mirrored:
                {
                    glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
                    glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));

                #if !defined(JOP_OPENGL_ES) || defined(JOP_OPENGL_ES3)
                    glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT));
                #endif

                    break;
                }
                case Repeat::ClampEdge:
                {
                    glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
                    glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

                #if !defined(JOP_OPENGL_ES) || defined(JOP_OPENGL_ES3)
                    glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
                #endif

                    break;
                }

            #ifndef JOP_OPENGL_ES

                case Repeat::ClampBorder:
                {
                    glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
                    glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
                    glCheck(glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER));
                }

            #endif
            }
        }

        //////////////////////////////////////////////

        void setGLBorderColor(const GLenum target, const Color& color)
        {
        #ifndef JOP_OPENGL_ES
            glCheck(glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, &color.colors[0]));
        #endif
        }

        //////////////////////////////////////////////

        GLenum getFormatEnum(const Texture::Format format, const bool srgb)
        {
        #ifdef JOP_OPENGL_ES



        #else

        #endif
        };

        //////////////////////////////////////////////

        GLenum getInternalFormatEnum(const Texture::Format format, const bool srgb)
        {
            using F = Texture::Format;

        #if defined(JOP_OPENGL_ES) && JOP_MIN_OPENGL_ES_VERSION < 300
            static const bool sizedSupport = gl::getVersionMajor() >= 3 || JOP_CHECK_GL_EXTENSION(OES_required_internalformat);
            static const bool hdrSupport = gl::getVersionMajor() >= 3 || JOP_CHECK_GL_EXTENSION(OES_texture_half_float);
        #endif

            const bool allowSRGB = srgb && Texture::allowSRGB();

            switch (format)
            {
                // 8 bit alpha texture, internal formats differ
                case F::Alpha_UB_8:
                {
                    return

                #if defined(JOP_OPENGL_ES) && JOP_MIN_OPENGL_ES_VERSION < 300

                    #ifdef GL_ES_VERSION_3_0

                        // If version >= 3.0
                        gl::getVersionMajor() >= 3 ? GL_R8 :

                    #endif

                    // If version < 3.0
                    (sizedSupport ? GL_ALPHA8_OES : GL_ALPHA);

                #else
                    GL_R8;

                #endif
                }

                // 24 bit RGB texture
                // Sized internal format & srgb only with extensions on GLES 2.0
                case F::RGB_UB_8:
                {
                    return

                #if defined(JOP_OPENGL_ES) && JOP_MIN_OPENGL_ES_VERSION < 300

                    #ifdef GL_ES_VERSION_3_0

                        // If version >= 3.0
                        gl::getVersionMajor() >= 3 ? (allowSRGB ? GL_SRGB8 : GL_RGB8) :

                    #endif

                    // If version < 3.0
                    (allowSRGB ? GL_SRGB_EXT : (sizedSupport ? GL_RGB8_OES : GL_RGB));

                #else
                    allowSRGB ? GL_SRGB8 : GL_RGB8;

                #endif
                }

                // 32 bit RGBA texture
                // Sized internal format & srgb only with extensions in GLES 2.0
                case F::RGBA_UB_8:
                {
                    return

                #if defined(JOP_OPENGL_ES) && JOP_MIN_OPENGL_ES_VERSION < 300

                    #ifdef GL_ES_VERSION_3_0

                        // If version >= 3.0
                        gl::getVersionMajor() >= 3 ? (allowSRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8) :

                    #endif

                    // If version < 3.0
                    (allowSRGB ? GL_SRGB_ALPHA_EXT : (sizedSupport ? GL_RGBA8_OES : GL_RGBA));

                #else
                    allowSRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;

                #endif
                }

                // 48 bit RGB floating point texture
                // Only with extensions in GLES 2.0
                case F::RGB_F_16:
                {
                    return

                #if defined(JOP_OPENGL_ES) && JOP_MIN_OPENGL_ES_VERSION < 300

                    #ifdef GL_ES_VERSION_3_0

                        // If version >= 3.0
                        gl::getVersionMajor() >= 3 ? GL_RGB16F :

                    #endif

                    // If version < 3.0
                    // "Floatness" or the texture will be driven
                    // by the <type> parameter
                    GL_RGB;

                #else
                    GL_RGB16F;

                #endif
                }

                // 64 bit RGBA floating point texture
                // Only with extensions in GLES 2.0
                case F::RGBA_F_16:
                {
                    return

                #if defined(JOP_OPENGL_ES) && JOP_MIN_OPENGL_ES_VERSION < 300

                    #ifdef GL_ES_VERSION_3_0

                        // If version >= 3.0
                        gl::getVersionMajor() >= 3 ? GL_RGBA16F :

                    #endif

                    // If version < 3.0
                    // See comment for RGB_F_16
                    GL_RGB;

                #else
                    GL_RGBA16F;

                #endif
                }

                // 16 bit depth texture
                case F::Depth_US_16:
                {

                }
            }
        };

        //////////////////////////////////////////////

        GLenum getTypeEnum(const Texture::Format format)
        {

        }

        //////////////////////////////////////////////

        GLenum getCompressedInternalFormatEnum(const Image::Format format, const bool srgb)
        {

        }
    }

    //////////////////////////////////////////////

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
        glCheck(glDeleteTextures(1, &m_texture));
        m_texture = 0;
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
        else
            TextureSampler::unbind(texUnit);

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

    const TextureSampler* Texture::getSampler() const
    {
        return m_sampler.get();
    }

    //////////////////////////////////////////////

    Texture& Texture::setFilterMode(const TextureSampler::Filter mode, const float param)
    {
        if (bind())
        {
            detail::setGLFilterMode(m_target, mode, param);

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
            detail::setGLRepeatMode(m_target, repeat);
            m_repeat = repeat;
        }

        return *this;
    }

    //////////////////////////////////////////////

    Texture& Texture::setBorderColor(const Color& color)
    {
        if (bind())
        {
            detail::setGLBorderColor(m_target, color);
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

    unsigned int Texture::getMaxTextureUnits()
    {
        static unsigned int maxUnits = 0;

        if (!maxUnits)
        {
            glCheck(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, reinterpret_cast<GLint*>(&maxUnits)));
        }

        return maxUnits;
    }

    //////////////////////////////////////////////

    void Texture::setUnpackAlignment(const Format format)
    {
        GLint param = 4;
        
        switch (format)
        {
            case Format::Alpha_UB_8:
                param = 1;
                break;

            case Format::Depth_US_16:
                param = 2;
                break;

            case Format::RGB_F_16:
            case Format::RGBA_F_16:
                param = 8;
        }

        glCheck(glPixelStorei(GL_UNPACK_ALIGNMENT, param));
    }

    //////////////////////////////////////////////

    bool Texture::allowSRGB()
    {
        static const bool allow = SettingManager::get<bool>("engine@Graphics|Texture|bAllowSRGB", true);

    #ifdef JOP_OPENGL_ES

        return allow
        
        #if JOP_MIN_OPENGL_ES_VERSION < 300
            && (gl::getVersionMajor() >= 3 || JOP_CHECK_GL_EXTENSION(GL_EXT_sRGB))
        #endif;

    #endif

        return allow;
    }

    //////////////////////////////////////////////

    bool Texture::allowGenMipmaps(const glm::uvec2& size, const bool srgb)
    {
        static const bool allow = SettingManager::get<bool>("engine@Graphics|Texture|bAllowMipmapGeneration", true);

    #ifdef JOP_OPENGL_ES

        static const bool srgbMip = JOP_CHECK_GL_EXTENSION(GL_NV_generate_mipmap_sRGB);

        return allow && (!srgb || srgbMip)

        #if JOP_MIN_OPENGL_ES_VERSION < 300

            // Check NPOT
            && (gl::getVersionMajor() >= 3 || JOP_CHECK_GL_EXTENSION(GL_OES_texture_npot) || ((size.x & (size.x - 1)) == 0 && (size.y & (size.y - 1)) == 0))

        #endif;

    #else

        size;
        srgb;

        return allow;

    #endif
    }

    //////////////////////////////////////////////

    Texture::Format Texture::getFormatFromDepth(const uint32 depth)
    {
        switch (depth)
        {
            case 1:
                return Format::Alpha_UB_8;

            case 3:
                return Format::RGB_UB_8;
            case 4:
                return Format::RGBA_UB_8;

            case 6:
                return Format::RGB_F_16;
            case 8:
                return Format::RGBA_F_16;
        }

        return Format::None;
    }

    //////////////////////////////////////////////

    unsigned int Texture::getDepthFromFormat(const Format format)
    {
        switch (format)
        {
            case Format::Alpha_UB_8:
                return 1;

            case Format::Depth_US_16:
                return 2;

            case Format::RGB_UB_8:
            case Format::Depth_UI_24:
                return 3;
            case Format::RGBA_UB_8:
                return 4;

            case Format::RGB_F_16:
                return 6;
            case Format::RGBA_F_16:
                return 8;
        }

        return 0;
    }

    //////////////////////////////////////////////

    void Texture::updateSampling() const
    {
        detail::setGLFilterMode(m_target, m_filter, m_anisotropic);
        detail::setGLRepeatMode(m_target, m_repeat);
        detail::setGLBorderColor(m_target, m_borderColor);
    }
}