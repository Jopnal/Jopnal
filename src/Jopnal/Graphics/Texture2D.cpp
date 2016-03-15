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
    void flip(const int width, const int height, const int bpp, unsigned char* pixels)
    {
        int rowSize = width * bpp;

        for (int y = 0; y < height; ++y)
        {
            unsigned char* left = pixels + y * rowSize;
            unsigned char* right = pixels + (y + 1) * rowSize - bpp;

            for (int x = 0; x < width / 2; ++x)
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

        std::vector<unsigned char> buf;
        FileLoader::read(path, buf);

        int x = 0, y = 0, bpp = 0;
        unsigned char* colorData = stbi_load_from_memory(buf.data(), buf.size(), &x, &y, &bpp, 0);

        bool success = false;
        if (colorData && checkDepthValid(bpp))
        {
            flip(x, y, bpp, colorData);
            success = load(x, y, bpp, colorData);
        }

        stbi_image_free(colorData);

        return success;
    }

    //////////////////////////////////////////////

    bool Texture2D::load(const int x, const int y, const int bytesPerPixel)
    {
        return load(x, y, bytesPerPixel, nullptr);
    }

    //////////////////////////////////////////////

    bool Texture2D::load(const int x, const int y, const int bytesPerPixel, const unsigned char* pixels)
    {
        if (!x || !y)
        {
            JOP_DEBUG_ERROR("Couldn't load texture. One or both dimensions are less than 1");
            return false;
        }
        if (x > getMaximumSize() || y > getMaximumSize())
        {
            JOP_DEBUG_ERROR("Couldn't load texture. Maximum size is " << getMaximumSize());
            return false;
        }
        else if (!checkDepthValid(bytesPerPixel))
        {
            JOP_DEBUG_ERROR("Couldn't load texture. Pixel depth (" << bytesPerPixel << ") is invalid. Must be either 1, 3 or 4");
            return false;
        }

        destroy();
        bind();

        m_width = x; m_height = y;
        m_bytesPerPixel = bytesPerPixel;

        const GLenum depthEnum = getFormatEnum(bytesPerPixel);
        glCheck(gl::TexImage2D(gl::TEXTURE_2D, 0, getInternalFormatEnum(depthEnum), x, y, 0, depthEnum, gl::UNSIGNED_BYTE, pixels));

        return true;
    }

    //////////////////////////////////////////////

    void Texture2D::setPixels(const int x, const int y, const int width, const int height, const unsigned char* pixels)
    {
        if ((x + width > m_width) || (y + height > m_height))
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
        glCheck(gl::TexSubImage2D(gl::TEXTURE_2D, 0, x, y, width, height, getFormatEnum(m_bytesPerPixel), gl::UNSIGNED_BYTE, pixels));
    }

    //////////////////////////////////////////////

    int Texture2D::getWidth() const
    {
        return m_width;
    }

    //////////////////////////////////////////////

    int Texture2D::getHeight() const
    {
        return m_height;
    }

    //////////////////////////////////////////////

    int Texture2D::getDepth() const
    {
        return m_bytesPerPixel;
    }

    //////////////////////////////////////////////

    bool Texture2D::load(const int id)
    {
        std::vector<unsigned char> buf;
        if (!FileLoader::readFromDll(id, buf))
            return false;

        int x, y, bpp;
        unsigned char* pix = stbi_load_from_memory(buf.data(), buf.size(), &x, &y, &bpp, 0);

        bool success = false;
        if (pix && checkDepthValid(bpp))
        {
            flip(x, y, bpp, pix);
            success = load(x, y, bpp, pix);
        }

        stbi_image_free(pix);

        return success;
    }

    //////////////////////////////////////////////

    unsigned int Texture2D::getFormatEnum(const int bytesPerPixel)
    {
        switch (bytesPerPixel)
        {
            case 1:
                return gl::RED;
            case 3:
                return gl::RGB;
            case 4:
                return gl::RGBA;
        }

        return gl::RED;
    }

    //////////////////////////////////////////////

    unsigned int Texture2D::getInternalFormatEnum(const int formatEnum)
    {
        switch (formatEnum)
        {
            case gl::RED:
                return gl::R8;
            case gl::RGB:
                return gl::RGB8;
            case gl::RGBA:
                return gl::RGBA8;
        }

        return gl::R8;
    }

    //////////////////////////////////////////////

    bool Texture2D::checkDepthValid(const int depth)
    {
        return depth == 1 || depth == 3 || depth == 4;
    }
}