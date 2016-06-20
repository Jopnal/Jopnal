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

    void Image::create(uint32 width, uint32 height, const Color& color)
    {
        if (width && height)
        {
            // Assign the new size
            m_size.x = width;
            m_size.y = height;

            // Resize pixel buffer
            m_pixels.resize(width*height*4);

            // Fill it with given color
            uint32* ptr = &m_pixels[0];
            uint32* end = ptr + m_pixels.size();
            // Could be done with Auto ?
            while (ptr < end)
            {
                *ptr++ = color.r;
                *ptr++ = color.g;
                *ptr++ = color.b;
                *ptr++ = color.a;
            }
        }
        else
        {
            // Create empty image
            m_size.x = 0;
            m_size.y = 0;
            m_pixels.clear();
        }
    }

    void Image::create(uint32 width, uint32 height, const unsigned char* pixels)
    {
        if (pixels && width && height)
        {
            // Assign the new size
            m_size.x = width;
            m_size.y = height;

            // Copy the pixels
            m_pixels.resize(width*height * 4);
            std::memcpy(&m_pixels[0], pixels, m_pixels.size());
        }
        else
        {
            // Create empty image
            m_size.x = 0;
            m_size.y = 0;
            m_pixels.clear();
        }
    }

    bool Image::load(const std::string& path, const bool srgb)
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
            success = load(size, bpp, colorData, srgb);

        stbi_image_free(colorData);

        return success;
    }

    bool Image::load(const void* data, uint32 size)
    {

    }

    glm::vec2 Image::getSize() const
    {

    }

    void Image::copy(const Image& sourceImg, uint32 x, uint32 y, const Rect sourceRect, bool applyAlhpa)
    {

    }

    void Image::setPixel(uint32 x, uint32 y, const Color& color)
    {

    }

    const jop::Color Image::getPixelColor(uint32 x, uint32 y) const
    {

    }

    void Image::flipHorizontally()
    {

    }

    void Image::flipVertically()
    {

    }

}