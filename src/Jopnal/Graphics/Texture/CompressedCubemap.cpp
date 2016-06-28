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
#include <Jopnal/Precompiled/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
    CompressedCubemap::CompressedCubemap(const std::string& name)
        : Texture(name, GL_TEXTURE_CUBE_MAP),
        m_size(0)
    {
        getSampler().setFilterMode(TextureSampler::Filter::Bilinear).setRepeatMode(TextureSampler::Repeat::ClampEdge);
    }

    bool CompressedCubemap::load(const CompressedImage& image, bool srgb)
    {
        if (!image.isCubemap())
            return false;

        destroy();
        bind();

        setPixelStore(1);

        const unsigned int mipMapCount = image.getMipMapCount();
        unsigned int offset = 0;
        glm::uvec2 size = image.getSize();
        
        const unsigned int blockSize = (image.getFormat() == CompressedImage::Format::DXT1RGBA) ? 8 : 16;

        static const GLenum formatEnum[] =
        {
            GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
            GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
            GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
        };

        for (size_t i = 0; i < 6; ++i)
        {
            unsigned int width = size.x;
            unsigned int height = size.y;

            for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
            {
                // For non-power-of-two sized textures
                if (width < 1)
                    width = 1;
                if (height < 1)
                    height = 1;
                
                unsigned int imageSize = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;

                glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, level, formatEnum[static_cast<int>(image.getFormat())], width, height, 0, imageSize, image.getPixels() + offset);

                // Calculate values for next mipmap level
                offset += imageSize;
                width /= 2;
                height /= 2;                 
            }

        }

        return true;
    }

    //////////////////////////////////////////////

    bool CompressedCubemap::load(const std::string& path, bool srgb)
    {
        CompressedImage compImage("");
        return compImage.load(path) && load(compImage, srgb);
    }

    //////////////////////////////////////////////

    glm::uvec2 CompressedCubemap::getSize() const
    {
        return m_size;
    }

}