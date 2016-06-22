//Jopnal Engine C++ Library
//Copyright(c) 2016 Team Jopnal
//
//This software is provided 'as-is', without any express or implied
//warranty.In no event will the authors be held liable for any damages
//arising from the use of this software.
//
//Permission is granted to anyone to use this software for any purpose,
//including commercial applications, and to alter it and redistribute it
//freely, subject to the following restrictions :
//
//1. The origin of this software must not be misrepresented; you must not
//claim that you wrote the original software.If you use this software
//in a product, an acknowledgement in the product documentation would be
//appreciated but is not required.
//2. Altered source versions must be plainly marked as such, and must not be
//misrepresented as being the original software.
//3. This notice may not be removed or altered from any source distribution.

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

    Image::Image() :
        m_size(0,0),
        m_pixels()
    {
    }

    //////////////////////////////////////////////

    bool Image::load(const std::string& path)
    {
        if (path.empty())
            return false;

        std::vector<uint8> buf;
        FileLoader::readBinaryfile(path, buf);

        glm::ivec2 size(0,0);
        int bpp = 0;
        unsigned char* colorData = stbi_load_from_memory(buf.data(), buf.size(), &size.x, &size.y, &bpp, 0);

        bool success = false;
        if (colorData && checkDepthValid(bpp))
            success = load(size, bpp, colorData);

        stbi_image_free(colorData);

        return success;
    }

    //////////////////////////////////////////////

    bool Image::load(const glm::uvec2& size, const uint32 bytesPerPixel, const unsigned char* pixels)
    {
        if (pixels)
        {
            if (checkDepthValid(bytesPerPixel))
            {
                // Clear array
                m_pixels.clear();
                m_bytesPerPixel = bytesPerPixel;
                m_size = size;


                int s = size.x * size.y * bytesPerPixel;
                m_pixels.resize(s);
                // Copy new data into memory
                std::memcpy(&m_pixels[0], pixels, s);

                return true;
            }
        }
        else
        {
            JOP_DEBUG_ERROR("Failure loading image.");
            return false;
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

        bool success = false;
        if (pix && checkDepthValid(bpp))
            success = load(glm::uvec2(x, y), bpp, pix);

        stbi_image_free(pix);

        return success;
    }

    //////////////////////////////////////////////

    glm::uvec2 Image::getSize() const
    {
        return m_size;
    }

    //////////////////////////////////////////////

    jop::uint32 Image::getDepth() const
    {
        return m_bytesPerPixel;
    }

    //////////////////////////////////////////////

    const jop::uint8* Image::getPixels() const
    {
        return m_pixels.data();
    }

    //////////////////////////////////////////////

    bool Image::checkDepthValid(const uint32 depth)
    {
        return depth >= 1 && depth <= 4;
    }

}