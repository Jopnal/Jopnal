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


namespace jop
{
    CompressedTexture2D::CompressedTexture2D(const std::string& name)
        : Texture(name, gl::TEXTURE_2D)
    {}

    //////////////////////////////////////////////

    bool CompressedTexture2D::load(const std::string& path)
    {
        CompressedImage compImage;
        return compImage.load(path) && load(path);
    }

    //////////////////////////////////////////////

    bool CompressedTexture2D::load(const CompressedImage& compImage)
    {
        return load(compImage.getSize(), compImage.getFormat(), compImage.getPixels(), compImage.getMipMapCount());
    }

    //////////////////////////////////////////////

    bool CompressedTexture2D::load(const glm::uvec2 size, const unsigned int format, const unsigned char* pixels, const unsigned int mipMapCount)
    {      
        if (size.x > getMaximumSize() || size.y > getMaximumSize())
        {
            return false;
        }

        destroy();
        bind();

        m_size = size;
        setPixelStore(1);

        const unsigned int blockSize = (format == gl::COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
        unsigned int offset = 0;
        unsigned int width = size.x;
        unsigned int height = size.y;
        
        // Go through mipmap levels
        for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
        {
            unsigned int imageSize = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;

            gl::CompressedTexImage2D(gl::TEXTURE_2D, level, format, width, height, 0, imageSize, pixels + offset);

            offset += imageSize;
            width /= 2;
            height /= 2;

            // For non-power-of-two sized textures
            if (width < 1)
                width = 1;
            if (height < 1)
                height = 1;
        }

        return true;       
    }
}