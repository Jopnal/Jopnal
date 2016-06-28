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



//////////////////////////////////////////////


namespace jop
{
    Texture2D::Texture2D(const std::string& name)
        : Texture           (name, GL_TEXTURE_2D),
          m_size            (0),
          m_bytesPerPixel   (0)
    {}

    //////////////////////////////////////////////

    bool Texture2D::load(const std::string& path, const bool srgb, const bool genMipmaps)
    {
        Image image;
        return image.load(path) && load(image, srgb, genMipmaps);
    }

    //////////////////////////////////////////////

    bool Texture2D::load(const glm::uvec2& size, const uint32 bytesPerPixel, const bool srgb, const bool genMipmaps)
    {
        return load(size, bytesPerPixel, nullptr, srgb, genMipmaps);
    }

    //////////////////////////////////////////////

    bool Texture2D::load(const glm::uvec2& size, const uint32 bytesPerPixel, const unsigned char* pixels, const bool srgb, const bool genMipmaps)
    {
        if (size.x > getMaximumSize() || size.y > getMaximumSize())
        {
            JOP_DEBUG_ERROR("Couldn't load texture. Maximum size is " << getMaximumSize());
            return false;
        }
        else if (!Image::checkDepthValid(bytesPerPixel))
        {
            JOP_DEBUG_ERROR("Couldn't load texture. Pixel depth (" << bytesPerPixel << ") is invalid. Must be between 1 and 4");
            return false;
        }

        destroy();
        bind();

        m_size = size;
        m_bytesPerPixel = bytesPerPixel;

        setPixelStore(bytesPerPixel);

        const GLenum depthEnum = getFormatEnum(bytesPerPixel);
        glCheck(glTexImage2D(GL_TEXTURE_2D, 0, getInternalFormatEnum(bytesPerPixel, srgb), size.x, size.y, 0, depthEnum, GL_UNSIGNED_BYTE, pixels));

        if (genMipmaps)
        {
            glCheck(glGenerateMipmap(GL_TEXTURE_2D));
        }

        return true;
    }

    //////////////////////////////////////////////

    bool Texture2D::load(const Image& image, const bool srgb, const bool genMipmaps)
    {
        return load(image.getSize(), image.getDepth(), image.getPixels(), srgb, genMipmaps);
    }

    //////////////////////////////////////////////

    bool Texture2D::load(const void* ptr, const uint32 size, const bool srgb, const bool genMipmaps)
    {
        Image image;
        return image.load(ptr, size) && load(image, srgb, genMipmaps);
    }

    //////////////////////////////////////////////

    void Texture2D::setPixels(const glm::uvec2& start, const glm::uvec2& size, const unsigned char* pixels)
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
        setPixelStore(m_bytesPerPixel);
        glCheck(glTexSubImage2D(GL_TEXTURE_2D, 0, start.x, start.y, size.x, size.y, getFormatEnum(m_bytesPerPixel), GL_UNSIGNED_BYTE, pixels));
    }

    //////////////////////////////////////////////

    glm::uvec2 Texture2D::getSize() const
    {
        return m_size;
    }

    //////////////////////////////////////////////

    unsigned int Texture2D::getDepth() const
    {
        return m_bytesPerPixel;
    }

    //////////////////////////////////////////////

    Image Texture2D::getImage() const
    {
        // If empty texture
        if (!getHandle())
            return Image();
        std::vector<uint8> pixels(m_size.x * m_size.y * m_bytesPerPixel);

    #ifdef JOP_OPENGL_ES   

        GLuint frameBuffer = 0;
        glCheck(glGenFramebuffers(1, &frameBuffer));

        if (frameBuffer)
        {
            GLint previousFrameBuffer;
            glCheck(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFrameBuffer));

            glCheck(glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer));
            glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, getHandle(), 0));
            glCheck(glReadPixels(0, 0, m_size.x, m_size.y, getFormatEnum(m_bytesPerPixel), GL_UNSIGNED_BYTE, &pixels[0]));
            glCheck(glDeleteFramebuffers(1, &frameBuffer));

            glCheck(glBindFramebuffer(GL_FRAMEBUFFER, previousFrameBuffer));
        }

    #else        

        bind();
        glCheck(glGetTexImage(GL_TEXTURE_2D, 0, getFormatEnum(m_bytesPerPixel), GL_UNSIGNED_BYTE, &pixels[0]));
        
    #endif

        Image image;
        image.load(m_size, m_bytesPerPixel, &pixels[0]);
        return image;
    }

    //////////////////////////////////////////////

    unsigned int Texture2D::getFormatEnum(const unsigned int bytesPerPixel)
    {
        switch (bytesPerPixel)
        {
            case 2:
                return GL_RG;

            case 3:
            case 6:
            case 12:
                return GL_RGB;

            case 4:
            case 8:
            case 16:
                return GL_RGBA;

            default:
                return GL_RED;
        }
    }

    //////////////////////////////////////////////

    unsigned int Texture2D::getInternalFormatEnum(const unsigned int bytesPerPixel, const bool srgb)
    {
        switch (bytesPerPixel)
        {
            case 2:
                return GL_RG8;

            case 3:
                return srgb ? GL_SRGB8 : GL_RGB8;
            case 6:
                return GL_RGB16F;
            case 12:
                return GL_RGB32F;

            case 4:
                return srgb ? GL_SRGB8_ALPHA8 : GL_RGBA8;
            case 8:
                return GL_RGBA16F;
            case 16:
                return GL_RGBA32F;


            default:
                return GL_R8;
        }
    }

    //////////////////////////////////////////////

        
    Texture2D& Texture2D::getError()
    {
        return CompressedTexture2D::getError();
    }

    //////////////////////////////////////////////

    Texture2D& Texture2D::getDefault()
    {
        return CompressedTexture2D::getDefault();
    }
}