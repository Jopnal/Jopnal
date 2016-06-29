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

    #include <Jopnal/Graphics/Image.hpp>
    #include <Jopnal/Core/FileLoader.hpp>

#endif

#define STB_IMAGE_IMPLEMENTATION
#include <Jopnal/Graphics/stb/stb_image_aug.h>

//////////////////////////////////////////////

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII


namespace jop
{
    Image::Image(const std::string& name) 
        : 
          m_pixels          (),
          m_bytesPerPixel   (0),
          m_size            (0),
          m_format          (),
          m_mipMapLevels    (0),
          m_isCubemap       (false),
          m_isCompressed    (false)
    {}

    //////////////////////////////////////////////

    bool Image::load(const std::string& path)
    {
        if (path.empty())
            return false;

        FileLoader f;

        if (!f.open(path))
            return false;

        // DDS Header
        unsigned char ddsheader[124];

        // Read in - ddsheader
        f.seek(4); // "DDS "
        f.read(ddsheader, sizeof(ddsheader));

        // DDS Header data       
        unsigned int height = *reinterpret_cast<unsigned int*>(&ddsheader[8]);
        unsigned int width = *reinterpret_cast<unsigned int*>(&ddsheader[12]);
        unsigned int linearSize = *reinterpret_cast<unsigned int*>(&ddsheader[16]);
        m_size = glm::uvec2(width, height);
        m_mipMapLevels = *reinterpret_cast<unsigned int*>(&ddsheader[24]);
        unsigned int fourCC = *reinterpret_cast<unsigned int*>(&ddsheader[80]);
        unsigned int dwCaps2 = *reinterpret_cast<unsigned int*>(&ddsheader[108]);

        // Check compressed image format (DXT1 / DXT3 / DXT5)
        switch (fourCC)
        {
        case FOURCC_DXT1:
            m_format = Format::DXT1RGBA;
            m_isCompressed = true;
            break;
        case FOURCC_DXT3:
            m_format = Format::DXT3RGBA;
            m_isCompressed = true;
            break;
        case FOURCC_DXT5:
            m_format = Format::DXT5RGBA;
            m_isCompressed = true;
            break;
        default:
            std::vector<uint8> buf;
            return FileLoader::readBinaryfile(path, buf) && load(buf.data(), buf.size());   
        }

        // Check if loaded image contains a cubemap - check if all in there DDS_CUBEMAP_ALLFACES ?
        if (dwCaps2 & 0x200)
            m_isCubemap = true;

        unsigned int pixelsSize = 0;
        pixelsSize = m_mipMapLevels > 1 ? linearSize * 2 : linearSize;

        if (m_isCubemap)
        {
            pixelsSize *= 6; // Need enough room for 6 images
            m_pixels.resize(pixelsSize * sizeof(unsigned char));
            // Read in - compressed pixels
            f.read(m_pixels.data(), pixelsSize);
        }
        else
        {
            m_pixels.resize(pixelsSize * sizeof(unsigned char));
            // Read in - compressed pixels
            f.read(m_pixels.data(), pixelsSize);
        }
        f.close();
      
        return true;      
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

    bool Image::load(const void* ptr, const uint32 size)
    {
        glm::ivec2 imageSize(0, 0);
        int bpp = 0;
        unsigned char* colorData = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(ptr), size, &imageSize.x, &imageSize.y, &bpp, 0);

        const bool success = colorData != nullptr && checkDepthValid(bpp) && load(imageSize, bpp, colorData);

        stbi_image_free(colorData);

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
        return (depth >= 1 && depth <= 4) || (depth <= 12 && depth % 3 == 0) || (depth <= 16 && depth % 4 == 0);
    }

    //////////////////////////////////////////////

    Image::Format Image::getFormat() const
    {
        return m_format;
    }

    //////////////////////////////////////////////

    unsigned int Image::getMipMapCount() const
    {
        return m_mipMapLevels;
    }

    //////////////////////////////////////////////

    bool Image::isCubemap() const
    {
        return m_isCubemap;
    }

    //////////////////////////////////////////////

    bool Image::isCompressed() const
    {
        return m_isCompressed;
    }

}