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
#include <Jopnal/Graphics/stb/stb_image.h>

//////////////////////////////////////////////


namespace jop
{
    Cubemap::Cubemap(const std::string& name)
        : Texture           (name, gl::TEXTURE_CUBE_MAP),
          m_size            (),
          m_bytesPerPixel   (0)
    {
        static WeakReference<TextureSampler> sampler;

        if (sampler.expired())
        {
            if (ResourceManager::resourceExists<TextureSampler>("jop_cube_sampler"))
                sampler = static_ref_cast<TextureSampler>(ResourceManager::getExistingResource<TextureSampler>("jop_cube_sampler").getReference());
            else
            {
                sampler = static_ref_cast<TextureSampler>(ResourceManager::getEmptyResource<TextureSampler>("jop_cube_sampler").getReference());
                sampler->setPersistence(0);

                sampler->setFilterMode(TextureSampler::Filter::Bilinear);
                sampler->setRepeatMode(TextureSampler::Repeat::ClampEdge);
            }
        }

        setSampler(*sampler);
    }

    //////////////////////////////////////////////

    bool Cubemap::load(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, const std::string& back, const std::string& front)
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

            GLenum depthEnum = Texture2D::getFormatEnum(m_bytesPerPixel);
            glCheck(gl::TexImage2D(gl::TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Texture2D::getInternalFormatEnum(depthEnum), size.x, size.y, 0, depthEnum, gl::UNSIGNED_BYTE, pix));

            stbi_image_free(pix);
        }

        m_size = size;
        m_bytesPerPixel = bytes;

        return true;
    }

    //////////////////////////////////////////////

    bool Cubemap::load(const glm::uvec2& size, const unsigned int bpp)
    {
        bind();

        GLenum depthEnum = Texture2D::getFormatEnum(bpp);
        for (std::size_t i = 0; i < 6; ++i)
        {
            glCheck(gl::TexImage2D(gl::TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Texture2D::getInternalFormatEnum(depthEnum), size.x, size.y, 0, depthEnum, gl::UNSIGNED_BYTE, NULL));
        }

        m_size = size;
        m_bytesPerPixel = bpp;

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
            errTex = static_ref_cast<Cubemap>(ResourceManager::getEmptyResource<Cubemap>("jop_error_cubemap").getReference());

            std::vector<unsigned char> buf;
            FileLoader::readResource(IDB_PNG2, buf);

            int x, y, bpp;
            unsigned char* pix = stbi_load_from_memory(buf.data(), buf.size(), &x, &y, &bpp, 0);

            errTex->load(glm::uvec2(x, y), bpp);

            GLenum depthEnum = Texture2D::getFormatEnum(bpp);
            for (std::size_t i = 0; i < 6; ++i)
            {
                glCheck(gl::TexImage2D(gl::TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Texture2D::getInternalFormatEnum(depthEnum), x, y, 0, depthEnum, gl::UNSIGNED_BYTE, pix));
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
            defTex = static_ref_cast<Cubemap>(ResourceManager::getEmptyResource<Cubemap>("jop_error_cubemap").getReference());

            std::vector<unsigned char> buf;
            FileLoader::readResource(IDB_PNG1, buf);

            int x, y, bpp;
            unsigned char* pix = stbi_load_from_memory(buf.data(), buf.size(), &x, &y, &bpp, 0);

            defTex->load(glm::uvec2(x, y), bpp);

            GLenum depthEnum = Texture2D::getFormatEnum(bpp);
            for (std::size_t i = 0; i < 6; ++i)
            {
                glCheck(gl::TexImage2D(gl::TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Texture2D::getInternalFormatEnum(depthEnum), x, y, 0, depthEnum, gl::UNSIGNED_BYTE, pix));
            }

            stbi_image_free(pix);
        }

        return *defTex;
    }
}