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

//////////////////////////////////////////////

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

namespace jop
{
    CompressedImage::CompressedImage() 
        : m_pixels  (),
          m_size    (0),
          m_format  (0),
          m_mipMapLevels (0)
    {}

    //////////////////////////////////////////////

    bool CompressedImage::load(const std::string& path)
    {
        if (path.empty())
            return false;

        FileLoader f;

        if (!f.open(path))
            return false;

        // DDS Header
        unsigned char ddsheader[124];

        // Read in ddsheader
        f.read(ddsheader, sizeof(ddsheader));

        // DDS Header data       
        unsigned int height = *reinterpret_cast<unsigned int*>(&ddsheader[8]);
        unsigned int width = *reinterpret_cast<unsigned int*>(&ddsheader[12]);
        const unsigned int linearSize = *reinterpret_cast<unsigned int*>(&ddsheader[16]);
        m_size = glm::uvec2(width, height);
        m_mipMapLevels = *reinterpret_cast<unsigned int*>(&ddsheader[24]);
        const unsigned int fourCC = *reinterpret_cast<unsigned int*>(&ddsheader[80]);

        // Total size of the image including all mipmaps
        unsigned int pixelsSize;
        pixelsSize = m_mipMapLevels > 1 ? linearSize * 2 : linearSize;
        m_pixels = (unsigned char*)malloc(pixelsSize * sizeof(unsigned char));
        // Read in compressed pixels
        f.read(m_pixels, pixelsSize);
        f.close();
        
        //unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;

        // Get compressed image format (DXT1 / DXT3 / DXT5)
        switch (fourCC)
        {
        case FOURCC_DXT1:
            m_format = gl::COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
        case FOURCC_DXT3:
            m_format = gl::COMPRESSED_RGBA_S3TC_DXT3_EXT;
            break;
        case FOURCC_DXT5:
            m_format = gl::COMPRESSED_RGBA_S3TC_DXT5_EXT;
            break;
        default:
            JOP_DEBUG_ERROR("Unidentified DDS compression format.");
            return 0;
        }       

        return true;
    }

    //////////////////////////////////////////////

    const unsigned char* CompressedImage::getPixels() const
    {
        return m_pixels;
    }

    //////////////////////////////////////////////

    glm::uvec2 CompressedImage::getSize() const
    {
        return m_size;
    }

    //////////////////////////////////////////////

    unsigned int CompressedImage::getFormat() const
    {
        return m_format;
    }

    //////////////////////////////////////////////

    unsigned int CompressedImage::getMipMapCount() const
    {
        return m_mipMapLevels;
    }

}