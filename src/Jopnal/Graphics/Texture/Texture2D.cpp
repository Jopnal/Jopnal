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

    #include <Jopnal/Graphics/Texture/Texture2D.hpp>

    #include <Jopnal/Core/FileLoader.hpp>
    #include <Jopnal/Core/DebugHandler.hpp>
    #include <Jopnal/Core/ResourceManager.hpp>
    #include <Jopnal/Graphics/Image.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>
    #include <Jopnal/Utility/Assert.hpp>

#endif

#include <Jopnal/Resources/Resources.hpp>

//////////////////////////////////////////////


namespace jop
{
    namespace detail
    {
        extern GLenum getFormatEnum(const Texture::Format format, const bool srgb);
        extern GLenum getInternalFormatEnum(const Texture::Format format, const bool srgb);
        extern GLenum getTypeEnum(const Texture::Format format);
        extern GLenum getCompressedInternalFormatEnum(const Image::Format format, const bool srgb);
    }

    //////////////////////////////////////////////

    Texture2D::Texture2D(const std::string& name)
        : Texture   (name, GL_TEXTURE_2D),
          m_size    (0),
          m_format  (Format::None)
    {}

    //////////////////////////////////////////////

    bool Texture2D::load(const std::string& path, const uint32 flags)
    {
        Image image;
        return image.load(path, (flags & Flag::DisallowCompression) == 0) && load(image, flags);
    }

    //////////////////////////////////////////////

    bool Texture2D::load(const void* ptr, const uint32 size, const uint32 flags)
    {
        Image image;
        return image.load(ptr, size) && load(image, flags);
    }

    //////////////////////////////////////////////

    bool Texture2D::load(const glm::uvec2& size, const Format format, const uint32 flags)
    {
        return load(size, format, nullptr, flags);
    }

    //////////////////////////////////////////////

    bool Texture2D::load(const glm::uvec2& size, const Format format, const void* pixels, const uint32 flags)
    {
        if (size.x > getMaximumSize() || size.y > getMaximumSize())
        {
            JOP_DEBUG_ERROR("Couldn't load texture. Maximum size is " << getMaximumSize());
            return false;
        }

        destroy();
        bind();

        m_size = size;
        m_format = format;
        const bool srgb = (flags & Flag::DisallowSRGB) == 0;

        setUnpackAlignment(format);

        glCheck(glTexImage2D(GL_TEXTURE_2D, 0, detail::getInternalFormatEnum(format, srgb), size.x, size.y, 0, detail::getFormatEnum(format, srgb), detail::getTypeEnum(format), pixels));

        if (allowGenMipmaps(m_size, srgb) && !(flags & Flag::DisallowMipmapGeneration))
        {
            glCheck(glGenerateMipmap(GL_TEXTURE_2D));
        }

        // Swizzle R to A in GLES >=3.0 and GL >=3.3
        if ((!gl::isES() || gl::getVersionMajor() >= 3) && Texture2D::getPixelDepth() == 1)
        {
            glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED));
        }

        unbind();

        return true;
    }

    //////////////////////////////////////////////

    bool Texture2D::load(const Image& image, const uint32 flags)
    {
        if (!image.isCompressed())
            return load(image.getSize(), getFormatFromDepth(image.getDepth()), image.getPixels(), flags);

        else if (JOP_CHECK_GL_EXTENSION(EXT_texture_compression_s3tc))
        {
            destroy();
            bind();

            m_size = image.getSize();
            setUnpackAlignment(Format::Alpha_UB_8);

            // 8 bytes for DXT1 and 16 bytes for DXT3/5
            const unsigned int blockSize = (image.getFormat() <= Image::Format::DXT1RGBA) ? 8 : 16;

            const unsigned int mipMapCount = image.getMipMapCount();
            const bool srgb = (flags & Flag::DisallowSRGB) == 0;

            unsigned int offset = 0;
            unsigned int width = m_size.x;
            unsigned int height = m_size.y;

            for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
            {
                unsigned int imageSize = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;

                glCheck(glCompressedTexImage2D(GL_TEXTURE_2D, level, detail::getCompressedInternalFormatEnum(image.getFormat(), srgb), width, height, 0, imageSize, image.getPixels() + offset));

                offset += imageSize;
                width /= 2;
                height /= 2;

                // For non-power-of-two sized textures
                width  = std::max(width, 1u);
                height = std::max(height, 1u);
            }

            if (allowGenMipmaps(m_size, srgb) && !(flags & Flag::DisallowMipmapGeneration) && image.getMipMapCount() <= 1)
            {
                glCheck(glGenerateMipmap(GL_TEXTURE_2D));
            }

            unbind();

            return true;
        }

        return false;
    }

    //////////////////////////////////////////////

    void Texture2D::setPixels(const glm::uvec2& start, const glm::uvec2& size, const void* pixels)
    {
        if ((start.x + size.x > m_size.x) || (start.y + size.y > m_size.y))
        {
            JOP_DEBUG_ERROR("Couldn't set texture pixels. Would cause overflow");
            return;
        }
        else if (!pixels)
        {
            JOP_DEBUG_ERROR("Couldn't set texture pixels. Pixel pointer is null");
            return;
        }

        bind();
        setUnpackAlignment(m_format);
        glCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, start.x, start.y, size.x, size.y, detail::getFormatEnum(m_format, false), detail::getTypeEnum(m_format), pixels));

        unbind();
    }

    //////////////////////////////////////////////

    void Texture2D::setPixels(const glm::uvec2& start, const Image& image)
    {
        setPixels(start, image.getSize(), image.getPixels());
    }

    //////////////////////////////////////////////

    glm::uvec2 Texture2D::getSize() const
    {
        return m_size;
    }

    //////////////////////////////////////////////

    unsigned int Texture2D::getPixelDepth() const
    {
        return getDepthFromFormat(m_format);
    }

    //////////////////////////////////////////////

    Image Texture2D::getImage() const
    {
        // If empty texture
        if (!getHandle())
            return Image();

        std::vector<uint8> pixels(m_size.x * m_size.y * Texture2D::getPixelDepth());

    #ifdef JOP_OPENGL_ES   

        GLuint frameBuffer = 0;
        glCheck(glGenFramebuffers(1, &frameBuffer));

        if (frameBuffer)
        {
            GLint previousFrameBuffer;
            glCheck(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBuffer));

            glCheck(glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer));
            glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, getHandle(), 0));
            glCheck(glReadPixels(0, 0, m_size.x, m_size.y, getFormatEnum(m_format, false), GL_UNSIGNED_BYTE, &pixels[0]));
            glCheck(glDeleteFramebuffers(1, &frameBuffer));

            glCheck(glBindFramebuffer(GL_FRAMEBUFFER, previousFrameBuffer));
        }

    #else        

        bind();
        glCheck(glGetTexImage(GL_TEXTURE_2D, 0, detail::getFormatEnum(m_format, false), GL_UNSIGNED_BYTE, &pixels[0]));
        
    #endif

        Image image;
        image.load(m_size, Texture2D::getPixelDepth(), &pixels[0]);

        unbind();

        return image;
    }

    //////////////////////////////////////////////

    unsigned int Texture2D::getMaximumSize()
    {
        static unsigned int size = 0;

        if (!size)
        {
            glCheck(glGetIntegerv(GL_MAX_TEXTURE_SIZE, reinterpret_cast<GLint*>(&size)));
        }

        return size;
    }

    //////////////////////////////////////////////

    Texture2D& Texture2D::getError()
    {
        static WeakReference<Texture2D> errTex;

        if (errTex.expired())
        {
            errTex = static_ref_cast<Texture2D>(ResourceManager::getEmpty<Texture2D>("jop_error_texture").getReference());

            JOP_ASSERT_EVAL(errTex->load(jopr::errorTexture, sizeof(jopr::errorTexture)), "Failed to load error 2D texture!");

            errTex->setPersistence(0);
        }

        return *errTex;
    }

    //////////////////////////////////////////////

    Texture2D& Texture2D::getDefault()
    {
        static WeakReference<Texture2D> defTex;

        if (defTex.expired())
        {
            defTex = static_ref_cast<Texture2D>(ResourceManager::getEmpty<Texture2D>("jop_default_texture").getReference());

            JOP_ASSERT_EVAL(defTex->load(jopr::defaultTexture, sizeof(jopr::defaultTexture)), "Failed to load default texture!");

            defTex->setPersistence(0);
        }

        return *defTex;
    }
}