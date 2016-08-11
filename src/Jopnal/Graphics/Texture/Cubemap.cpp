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
    namespace detail
    {
        extern GLenum getFormatEnum(const Texture::Format format, const bool srgb);
        extern GLenum getInternalFormatEnum(const Texture::Format format, const bool srgb);
        extern GLenum getTypeEnum(const Texture::Format format);
        extern GLenum getCompressedInternalFormatEnum(const Image::Format format, const bool srgb);
    }

    //////////////////////////////////////////////

    Cubemap::Cubemap(const std::string& name)
        : Texture           (name, GL_TEXTURE_CUBE_MAP),
          m_size            (),
          m_format          (Format::None)
    {}

    //////////////////////////////////////////////

    bool Cubemap::load(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, const std::string& back, const std::string& front, const uint32 flags)
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
        const bool srgb = (flags & Flag::DisallowSRGB) == 0;

        for (std::size_t i = 0; i < 6; ++i)
        {
            std::vector<uint8> buf;
            if (!FileLoader::readBinaryfile(*paths[i], buf))
            {
                JOP_DEBUG_ERROR("Couldn't read cube map texture, face " << i << ", path " << paths[i]);
                return false;
            }

            unsigned char* pix = stbi_load_from_memory(buf.data(), buf.size(), &size.x, &size.y, &bytes, 0);

            if (!pix)
            {
                JOP_DEBUG_ERROR("Couldn't load cube map texture, face " << i << ", path " << paths[i]);
                return false;
            }

            m_format = getFormatFromDepth(bytes);

            glCheck(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, detail::getInternalFormatEnum(m_format, srgb),
                                 size.x, size.y, 0, detail::getFormatEnum(m_format, srgb), detail::getTypeEnum(m_format), pix));

            stbi_image_free(pix);
        }

        if (allowGenMipmaps(size, srgb) && !(flags & Flag::DisallowMipmapGeneration))
        {
            glCheck(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
        }

        m_size = glm::uvec2(size);

        return true;
    }

    //////////////////////////////////////////////

    bool Cubemap::load(const glm::uvec2& size, const Format format, const uint32 flags)
    {
        bind();

        const bool srgb = (flags & Flag::DisallowSRGB) == 0;

        for (std::size_t i = 0; i < 6; ++i)
        {
            glCheck(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, detail::getInternalFormatEnum(format, srgb),
                                 size.x, size.y, 0, detail::getFormatEnum(format, srgb), detail::getTypeEnum(format), NULL));
        }

        if (allowGenMipmaps(size, srgb) && !(flags & Flag::DisallowMipmapGeneration))
        {
            glCheck(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
        }

        m_size = size;

        return true;
    }

    //////////////////////////////////////////////

    bool Cubemap::load(const std::string& path, const uint32 flags)
    {
        Image image;
        return image.load(path, (flags & Flag::DisallowCompression) == 0) && load(image, flags);
    }

    //////////////////////////////////////////////

    bool Cubemap::load(const Image& image, const uint32 flags)
    {
        // Check if image is cubemap and that extensions are valid
        if (!image.isCubemap() || !JOP_CHECK_GL_EXTENSION(EXT_texture_compression_s3tc))
            return false;

        destroy();
        bind();

        setUnpackAlignment(Format::Alpha_UB_8);

        const unsigned int mipMapCount = image.getMipMapCount();
        const bool srgb = (flags & Flag::DisallowSRGB) == 0;

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

                glCheck(glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, level, detail::getCompressedInternalFormatEnum(image.getFormat(), srgb),
                                               width, height, 0, imageSize, image.getPixels() + offset));

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

    unsigned int Cubemap::getPixelDepth() const
    {
        return getDepthFromFormat(m_format);
    }

    //////////////////////////////////////////////

    unsigned int Cubemap::getMaximumSize()
    {
        static unsigned int size = 0;

        if (!size)
        {
            glCheck(glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, reinterpret_cast<GLint*>(&size)));
        }

        return size;
    }

    //////////////////////////////////////////////

    Cubemap& Cubemap::getError()
    {
        static WeakReference<Cubemap> errTex;

        if (errTex.expired())
        {
            errTex = static_ref_cast<Cubemap>(ResourceManager::getEmpty<Cubemap>("jop_error_cubemap").getReference());

            int x, y, bpp;
            unsigned char* pix = stbi_load_from_memory(jopr::errorTexture, sizeof(jopr::errorTexture), &x, &y, &bpp, 3);

            errTex->load(glm::uvec2(x, y), Format::RGB_UB_8);

            const Format format = getFormatFromDepth(bpp);

            for (std::size_t i = 0; i < 6; ++i)
            {
                glCheck(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, detail::getInternalFormatEnum(format, true), x, y, 0, detail::getFormatEnum(format, true), detail::getTypeEnum(format), pix));
            }

            if (allowGenMipmaps(glm::uvec2(x, y), true))
            {
                glCheck(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
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
            unsigned char* pix = stbi_load_from_memory(jopr::defaultTexture, sizeof(jopr::defaultTexture), &x, &y, &bpp, 3);

            defTex->load(glm::uvec2(x, y), Format::RGB_UB_8);

            const Format format = getFormatFromDepth(bpp);

            for (std::size_t i = 0; i < 6; ++i)
            {
                glCheck(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, detail::getInternalFormatEnum(format, true), x, y, 0, detail::getFormatEnum(format, true), detail::getTypeEnum(format), pix));
            }

            if (allowGenMipmaps(glm::uvec2(x, y), true))
            {
                glCheck(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
            }

            stbi_image_free(pix);
        }

        return *defTex;
    }
}