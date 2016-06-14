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

#define STB_IMAGE_IMPLEMENTATION
#pragma warning(push)
#pragma warning(disable: 4189)
#pragma warning(disable: 4244)
#include <Jopnal/Graphics/stb/stb_image.h>
#pragma warning(pop)

//////////////////////////////////////////////


namespace jop
{
    Texture2D::Texture2D(const std::string& name)
        : Texture(name, GL_TEXTURE_2D)
    {}

    //////////////////////////////////////////////

    bool Texture2D::load(const std::string& path, const bool srgb, const bool genMipmap)
    {
        if (path.empty())
            return false;

        std::vector<uint8> buf;
        FileLoader::readBinaryfile(path, buf);

        glm::ivec2 size;
        int bpp;
        unsigned char* colorData = stbi_load_from_memory(buf.data(), buf.size(), &size.x, &size.y, &bpp, 0);

        bool success = false;
        if (colorData && checkDepthValid(bpp))
            success = load(size, bpp, colorData, srgb, genMipmap);

        stbi_image_free(colorData);

        return success;
    }

    //////////////////////////////////////////////

    bool Texture2D::load(const glm::uvec2& size, const unsigned int bytesPerPixel, const bool srgb, const bool genMipmap)
    {
        return load(size, bytesPerPixel, nullptr, srgb, genMipmap);
    }

    //////////////////////////////////////////////

    bool Texture2D::load(const glm::uvec2& size, const unsigned int bytesPerPixel, const unsigned char* pixels, const bool srgb, const bool genMipmap)
    {
        if (size.x > getMaximumSize() || size.y > getMaximumSize())
        {
            JOP_DEBUG_ERROR("Couldn't load texture. Maximum size is " << getMaximumSize());
            return false;
        }
        else if (!checkDepthValid(bytesPerPixel))
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

        if (genMipmap)
        {
            glCheck(glGenerateMipmap(GL_TEXTURE_2D));
        }

        return true;
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

    bool Texture2D::load(const unsigned char* data, const unsigned int dataSize, const bool srgb, const bool genMipmap)
    {
        int x, y, bpp;
        unsigned char* pix = stbi_load_from_memory(data, dataSize, &x, &y, &bpp, 0);

        bool success = false;
        if (pix && checkDepthValid(bpp))
            success = load(glm::uvec2(x, y), bpp, pix, srgb, genMipmap);

        stbi_image_free(pix);

        return success;
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

    bool Texture2D::checkDepthValid(const unsigned int depth)
    {
        return (depth >= 1 && depth <= 4) || (depth <= 12 && depth % 3 == 0) || (depth <= 16 && depth % 4 == 0);
    }

    //////////////////////////////////////////////

    Texture2D& Texture2D::getError()
    {
        static WeakReference<Texture2D> errTex;

        if (errTex.expired())
        {
            errTex = static_ref_cast<Texture2D>(ResourceManager::getEmptyResource<Texture2D>("jop_error_texture").getReference());

            JOP_ASSERT_EVAL(errTex->load(jopr::errorTexture, sizeof(jopr::errorTexture), true, true), "Failed to load error 2D texture!");

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
            defTex = static_ref_cast<Texture2D>(ResourceManager::getEmptyResource<Texture2D>("jop_default_texture").getReference());

            JOP_ASSERT_EVAL(defTex->load(jopr::defaultTexture, sizeof(jopr::defaultTexture), true, true), "Failed to load default 2D texture!");

            defTex->setPersistence(0);
        }

        return *defTex;
    }
}