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


namespace
{
    void flip(const unsigned int width, const unsigned int height, const unsigned int bpp, unsigned char* pixels)
    {
        unsigned int rowSize = width * bpp;

        for (std::size_t y = 0; y < height; ++y)
        {
            unsigned char* left = pixels + y * rowSize;
            unsigned char* right = pixels + (y + 1) * rowSize - bpp;

            for (std::size_t x = 0; x < width / 2; ++x)
            {
                std::swap_ranges(left, left + bpp, right);

                left += bpp;
                right -= bpp;
            }
        }
    }
}

namespace jop
{
    Texture2D::Texture2D(const std::string& name)
        : Texture(name, gl::TEXTURE_2D)
    {}

    //////////////////////////////////////////////

    bool Texture2D::load(const std::string& path)
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
        {
            //flip(size.x, size.y, bpp, colorData);
            success = load(size, bpp, colorData);
        }

        stbi_image_free(colorData);

        return success;
    }

    //////////////////////////////////////////////

    bool Texture2D::load(const glm::uvec2& size, const unsigned int bytesPerPixel)
    {
        return load(size, bytesPerPixel, nullptr);
    }

    //////////////////////////////////////////////

    bool Texture2D::load(const glm::uvec2& size, const unsigned int bytesPerPixel, const unsigned char* pixels)
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
        glCheck(gl::TexImage2D(gl::TEXTURE_2D, 0, getInternalFormatEnum(depthEnum), size.x, size.y, 0, depthEnum, gl::UNSIGNED_BYTE, pixels));

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
        glCheck(gl::TexSubImage2D(gl::TEXTURE_2D, 0, start.x, start.y, size.x, size.y, getFormatEnum(m_bytesPerPixel), gl::UNSIGNED_BYTE, pixels));
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

    bool Texture2D::load(const int id)
    {
        std::vector<unsigned char> buf;
        if (!FileLoader::readResource(id, buf))
            return false;

        int x, y, bpp;
        unsigned char* pix = stbi_load_from_memory(buf.data(), buf.size(), &x, &y, &bpp, 0);

        bool success = false;
        if (pix && checkDepthValid(bpp))
        {
            flip(x, y, bpp, pix);
            success = load(glm::uvec2(x, y), bpp, pix);
        }

        stbi_image_free(pix);

        return success;
    }

    //////////////////////////////////////////////

    unsigned int Texture2D::getFormatEnum(const unsigned int bytesPerPixel)
    {
        switch (bytesPerPixel)
        {
            case 1:
                return gl::RED;
            case 2:
                return gl::RG;
            case 3:
                return gl::RGB;
            case 4:
            default:
                return gl::RGBA;
        }
    }

    //////////////////////////////////////////////

    unsigned int Texture2D::getInternalFormatEnum(const unsigned int formatEnum)
    {
        switch (formatEnum)
        {
            case gl::RED:
                return gl::R8;
            case gl::RG:
                return gl::RG8;
            case gl::RGB:
                return gl::RGB8;
            case gl::RGBA:
            default:
                return gl::RGBA8;
        }
    }

    //////////////////////////////////////////////

    bool Texture2D::checkDepthValid(const unsigned int depth)
    {
        return depth >= 1 && depth <= 4;
    }

    //////////////////////////////////////////////

    Texture2D& Texture2D::getError()
    {
        static WeakReference<Texture2D> errTex;

        if (errTex.expired())
        {
            errTex = static_ref_cast<Texture2D>(ResourceManager::getEmptyResource<Texture2D>("jop_error_texture").getReference());

            JOP_ASSERT_EVAL(errTex->load(JOP_RES_ERROR_TEXTURE), "Failed to load error texture!");

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

            JOP_ASSERT_EVAL(defTex->load(JOP_RES_DEFAULT_TEXTURE), "Failed to load default texture!");

            defTex->setPersistence(0);
        }

        return *defTex;
    }
}