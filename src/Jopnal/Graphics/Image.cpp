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
    Image::Image() 
        : m_size            (0),
          m_pixels          (),
          m_bytesPerPixel   (0)
    {}

    //////////////////////////////////////////////

    bool Image::load(const std::string& path)
    {
        if (path.empty())
            return false;

        std::vector<uint8> buf;
        if (!FileLoader::readBinaryfile(path, buf))
            return false;

        glm::ivec2 size(0,0);
        int bpp = 0;
        unsigned char* colorData = stbi_load_from_memory(buf.data(), buf.size(), &size.x, &size.y, &bpp, 0);

        const bool success = colorData != nullptr && checkDepthValid(bpp) && load(size, bpp, colorData);

        stbi_image_free(colorData);

        return success;
    }

    //////////////////////////////////////////////

    bool Image::load(const glm::uvec2& size, const uint32 bytesPerPixel, const unsigned char* pixels)
    {
        if (pixels && size.x > 0 && size.y > 0 && checkDepthValid(bytesPerPixel))
        {
            // Clear array
            m_pixels.clear();
            m_bytesPerPixel = bytesPerPixel;
            m_size = size;

            const unsigned int s = size.x * size.y * bytesPerPixel;
            m_pixels.resize(s);

            // Copy new data into memory
            std::memcpy(&m_pixels[0], pixels, s);

            return true;
        }

        return false;
    }

    //////////////////////////////////////////////

    bool Image::load(const int id)
    {
        std::vector<unsigned char> buf;
        if (!FileLoader::readResource(id, buf))
            return false;

        int x = 0;
        int y = 0;
        int bpp = 0;
        unsigned char* pix = stbi_load_from_memory(buf.data(), buf.size(), &x, &y, &bpp, 0);

        bool success = pix != nullptr && checkDepthValid(bpp) && load(glm::uvec2(x, y), bpp, pix);

        stbi_image_free(pix);

        return success;
    }

    //////////////////////////////////////////////

    glm::uvec2 Image::getSize() const
    {
        return m_size;
    }

    //////////////////////////////////////////////

    uint32 Image::getDepth() const
    {
        return m_bytesPerPixel;
    }

    //////////////////////////////////////////////

    const uint8* Image::getPixels() const
    {
        return m_pixels.data();
    }

    //////////////////////////////////////////////

    bool Image::checkDepthValid(const uint32 depth)
    {
        return depth >= 1 && depth <= 4;
    }
}