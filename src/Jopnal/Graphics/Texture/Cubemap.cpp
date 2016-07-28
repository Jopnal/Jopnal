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

    #include <Jopnal/Graphics/Texture/Cubemap.hpp>

    #include <Jopnal/Core/FileLoader.hpp>
    #include <Jopnal/Core/ResourceManager.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>
    #include <Jopnal/Graphics/Texture/Texture2D.hpp>
    #include <vector>

#endif

#include <Jopnal/Resources/Resources.hpp>
#include <STB/stb_image_aug.h>

//////////////////////////////////////////////


namespace jop
{
    Cubemap::Cubemap(const std::string& name)
        : Texture           (name, GL_TEXTURE_CUBE_MAP),
          m_size            (),
          m_bytesPerPixel   (0)
    {
        setFilterMode(TextureSampler::Filter::Bilinear).setRepeatMode(TextureSampler::Repeat::ClampEdge);
    }

    //////////////////////////////////////////////

    bool Cubemap::load(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, const std::string& back, const std::string& front, const bool srgb, const bool genMipmaps)
    {
        const std::string* const paths[] =
        {
            &right, &left,
            &top, &bottom,
            &back, &front
        };

        bind();

        glm::ivec2 size;
        int bytes;
        for (std::size_t i = 0; i < 6; ++i)
        {
            std::vector<uint8> buf;
            if (!FileLoader::readBinaryfile(*paths[i], buf))
            {
                JOP_DEBUG_ERROR("Couldn't read cube map texture, face " << i);
                return false;
            }

            unsigned char* pix = stbi_load_from_memory(buf.data(), buf.size(), &size.x, &size.y, &bytes, 0);

            if (!pix)
            {
                JOP_DEBUG_ERROR("Couldn't load cube map texture, face " << i);
                stbi_image_free(pix);
                return false;
            }

            GLenum depthEnum = Texture2D::getFormatEnum(bytes, srgb);
            glCheck(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Texture2D::getInternalFormatEnum(bytes, srgb), size.x, size.y, 0, depthEnum, GL_UNSIGNED_BYTE, pix));

            if (allowGenMipmaps(size, srgb) && genMipmaps)
            {
                glCheck(glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i));
            }

            stbi_image_free(pix);
        }

        m_size = size;
        m_bytesPerPixel = bytes;

        return true;
    }

    //////////////////////////////////////////////

    bool Cubemap::load(const glm::uvec2& size, const unsigned int bpp, const bool srgb, const bool genMipmaps)
    {
        bind();

        GLenum depthEnum = Texture2D::getFormatEnum(bpp, srgb);
        for (std::size_t i = 0; i < 6; ++i)
        {
            glCheck(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Texture2D::getInternalFormatEnum(bpp, srgb), size.x, size.y, 0, depthEnum, GL_UNSIGNED_BYTE, NULL));

            if (allowGenMipmaps(size, srgb) && genMipmaps)
            {
                glCheck(glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i));
            }
        }

        m_size = size;
        m_bytesPerPixel = bpp;

        return true;
    }

    //////////////////////////////////////////////

    bool Cubemap::load(const std::string& path, bool srgb)
    {
        Image image;
        return image.load(path) && load(image, srgb);
    }

    //////////////////////////////////////////////

    bool Cubemap::load(const Image& image, bool srgb)
    {
        // Check if image is cubemap and that extensions are valid
        if (!image.isCubemap() || !JOP_CHECK_GL_EXTENSION(EXT_texture_compression_s3tc))
            return false;

        destroy();
        bind();

        setPixelStore(1);

        const unsigned int mipMapCount = image.getMipMapCount();
        unsigned int offset = 0;
        glm::uvec2 size = image.getSize();

        // 8 bytes for DXT1 and 16 bytes for DXT3/5
        const unsigned int blockSize = (image.getFormat() <= Image::Format::DXT1RGBA) ? 8 : 16;

        // Go through 6 faces of the cube map and their mipmaps
        for (size_t i = 0; i < 6; ++i)
        {
            unsigned int width = size.x;
            unsigned int height = size.y;

            for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
            {
                // For non-power-of-two sized textures
                width  = std::max(width, 1u);
                height = std::max(height, 1u);

                unsigned int imageSize = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;

                glCheck(glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, level, Texture2D::getCompressedInternalFormatEnum(image.getFormat(), srgb), width, height, 0, imageSize, image.getPixels() + offset));

                // Calculate values for next mipmap level
                offset += imageSize;
                width /= 2;
                height /= 2;
            }
        }

        return true;
    }

    //////////////////////////////////////////////

    glm::uvec2 Cubemap::getSize() const
    {
        return m_size;
    }

    //////////////////////////////////////////////

    Cubemap& Cubemap::getError()
    {
        static WeakReference<Cubemap> errTex;

        if (errTex.expired())
        {
            errTex = static_ref_cast<Cubemap>(ResourceManager::getEmpty<Cubemap>("jop_error_cubemap").getReference());

            int x, y, bpp;
            unsigned char* pix = stbi_load_from_memory(jopr::errorTexture, sizeof(jopr::errorTexture), &x, &y, &bpp, 0);

            errTex->load(glm::uvec2(x, y), bpp, true, false);

            GLenum depthEnum = Texture2D::getFormatEnum(bpp, true);
            for (std::size_t i = 0; i < 6; ++i)
            {
                glCheck(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Texture2D::getInternalFormatEnum(bpp, true), x, y, 0, depthEnum, GL_UNSIGNED_BYTE, pix));

                if (allowGenMipmaps(glm::uvec2(x, y), true))
                {
                    glCheck(glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i));
                }
            }

            stbi_image_free(pix);
        }

        return *errTex;
    }

    //////////////////////////////////////////////

    Cubemap& Cubemap::getDefault()
    {
        static WeakReference<Cubemap> defTex;

        if (defTex.expired())
        {
            defTex = static_ref_cast<Cubemap>(ResourceManager::getEmpty<Cubemap>("jop_error_cubemap").getReference());

            int x, y, bpp;
            unsigned char* pix = stbi_load_from_memory(jopr::defaultTexture, sizeof(jopr::defaultTexture), &x, &y, &bpp, 0);

            defTex->load(glm::uvec2(x, y), bpp, true, false);

            GLenum depthEnum = Texture2D::getFormatEnum(bpp, true);
            for (std::size_t i = 0; i < 6; ++i)
            {
                glCheck(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Texture2D::getInternalFormatEnum(bpp, true), x, y, 0, depthEnum, GL_UNSIGNED_BYTE, pix));

                if (allowGenMipmaps(glm::uvec2(x, y), true))
                {
                    glCheck(glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i));
                }
            }

            stbi_image_free(pix);
        }

        return *defTex;
    }
}