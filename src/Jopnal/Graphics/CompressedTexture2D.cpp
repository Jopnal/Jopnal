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

#include <Jopnal/Graphics/STB/stb_image_aug.h>


//////////////////////////////////////////////


namespace jop
{
    CompressedTexture2D::CompressedTexture2D(const std::string& name)
        : Texture2D   (name),
          m_size      (0)
    {}

    //////////////////////////////////////////////

    bool CompressedTexture2D::load(const std::string& path, bool srgb)
    {
        CompressedImage compImage("");

        if (!gl::exts::var_EXT_texture_compression_s3tc || !compImage.load(path))
        {
            Image image;
            return image.load(path) && Texture2D::load(image, srgb);
        }
        return load(compImage, srgb);
    }

    //////////////////////////////////////////////

    bool CompressedTexture2D::load(const CompressedImage& compImage, bool srgb)
    {   
        glm::uvec2 size = compImage.getSize();

        if (size.x > getMaximumSize() || size.y > getMaximumSize())
        {
            return false;
        }

        destroy();
        bind();

        m_size = size;
        setPixelStore(1);

        const unsigned int blockSize = (compImage.getFormat() == CompressedImage::Format::DXT1RGBA) ? 8 : 16;

        const unsigned int mipMapCount = compImage.getMipMapCount();
        unsigned int offset = 0;
        unsigned int width = size.x;
        unsigned int height = size.y;
        
        
            for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
            {
                unsigned int imageSize = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;

                static const GLenum formatEnum[] =
                {
                    gl::COMPRESSED_RGBA_S3TC_DXT1_EXT,
                    gl::COMPRESSED_RGBA_S3TC_DXT3_EXT,
                    gl::COMPRESSED_RGBA_S3TC_DXT5_EXT
                };

                gl::CompressedTexImage2D(gl::TEXTURE_2D, level, formatEnum[static_cast<int>(compImage.getFormat())], width, height, 0, imageSize, compImage.getPixels() + offset);

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

    //////////////////////////////////////////////

    glm::uvec2 CompressedTexture2D::getSize() const
    {
        return m_size;
    }

    //////////////////////////////////////////////

    //CompressedTexture2D& CompressedTexture2D::getError()
    //{
    //    static WeakReference<CompressedTexture2D> errTex;

    //    if (errTex.expired())
    //    {
    //        errTex = static_ref_cast<CompressedTexture2D>(ResourceManager::getEmptyResource<CompressedTexture2D>("jop_error_compressed_texture").getReference());

    //        JOP_ASSERT_EVAL(errTex->load(JOP_RES_ERROR_COMPRESSED_TEXTURE, true), "Failed to load error texture!");

    //        errTex->setPersistence(0);
    //    }

    //    return *errTex;
    //}

    //////////////////////////////////////////////////

    //CompressedTexture2D& CompressedTexture2D::getDefault()
    //{
    //    static WeakReference<CompressedTexture2D> defTex;

    //    if (defTex.expired())
    //    {
    //        defTex = static_ref_cast<CompressedTexture2D>(ResourceManager::getEmptyResource<CompressedTexture2D>("jop_default_compressed_texture").getReference());

    //        JOP_ASSERT_EVAL(defTex->load(JOP_RES_DEFAULT_COMPRESSED_TEXTURE, true), "Failed to load default texture!");

    //        defTex->setPersistence(0);
    //    }

    //    return *defTex;
    //}

}