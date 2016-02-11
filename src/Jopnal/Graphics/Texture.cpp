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

#define STB_IMAGE_IMPLEMENTATION
#pragma warning(push)
#pragma warning(disable: 4189)
#pragma warning(disable: 4244)
#include <Jopnal/Graphics/stb/stb_image.h>
#pragma warning(pop)

//////////////////////////////////////////////


namespace jop
{
    Texture::Texture(const std::string& name)
        : Resource          (name),
          m_sampler         (),
          m_defaultSampler  (TextureSampler::getDefault()),
          m_width           (0),
          m_height          (0),
          m_bytesPerPixel   (0),
          m_texture         (0)
    {}

    Texture::~Texture()
    {
        destroy();
    }

    //////////////////////////////////////////////

    bool Texture::load(const std::string& path)
    {
        if (path.empty())
            return false;

        std::vector<unsigned char> buf;
        FileLoader::read(path, buf);

        int x = 0, y = 0, bpp = 0;
        unsigned char* colorData = stbi_load_from_memory(buf.data(), buf.size(), &x, &y, &bpp, 4);

        bool success = false;
        if (colorData)
            success = load(x, y, bpp, colorData);

        stbi_image_free(colorData);

        return success;
    }

    //////////////////////////////////////////////
 
    bool Texture::load(const int x, const int y, const int bytesPerPixel)
    {
        return load(x, y, bytesPerPixel, nullptr);
    }

    //////////////////////////////////////////////

    bool Texture::load(const int x, const int y, const int bytesPerPixel, const unsigned char* pixels)
    {
        if (!x || !y)
        {
            JOP_DEBUG_ERROR("Couldn't load texture. One or both dimensions are less than 1");
            return false;
        }
        if (x > getMaximumSize() || y > getMaximumSize())
        {
            JOP_DEBUG_ERROR("Couldn't load texture. Maximum size is " << getMaximumSize());
            return false;
        }
        else if (!checkDepthValid(bytesPerPixel))
        {
            JOP_DEBUG_ERROR("Couldn't load texture. Pixel depth (" << bytesPerPixel << ") is invalid. Must be either 3 or 4");
            return false;
        }

        destroy();
        glCheck(gl::GenTextures(1, &m_texture));
        bind();

        m_width = x; m_height = y;
        m_bytesPerPixel = bytesPerPixel;

        glCheck(gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGBA8, x, y, 0, gl::RGBA, gl::UNSIGNED_BYTE, pixels));

        return true;
    }

    //////////////////////////////////////////////

    void Texture::destroy()
    {
        if (m_texture)
        {
            glCheck(gl::DeleteTextures(1, &m_texture));
            m_texture = 0;
            m_width = 0;
            m_height = 0;
            m_bytesPerPixel = 0;
        }
    }

    //////////////////////////////////////////////

    bool Texture::bind(const unsigned int texUnit) const
    {
        if (m_texture)
        {
            glCheck(gl::ActiveTexture(gl::TEXTURE0 + texUnit));
            glCheck(gl::BindTexture(gl::TEXTURE_2D, m_texture));

            if (!m_sampler.expired())
                m_sampler.lock()->bind(texUnit);
            else
                m_defaultSampler->bind(texUnit);
        }

        return m_texture != 0;
    }

    //////////////////////////////////////////////

    void Texture::unbind(const unsigned int texUnit)
    {
        glCheck(gl::BindTexture(gl::TEXTURE0 + texUnit, 0));
    }

    //////////////////////////////////////////////

    void Texture::setTextureSampler(std::weak_ptr<const TextureSampler> sampler)
    {
        m_sampler = sampler;
    }

    //////////////////////////////////////////////

    void Texture::setPixels(const int x, const int y, const int width, const int height, const unsigned char* pixels)
    {
        if ((x + width > m_width) || (y + height > m_height))
        {
            JOP_DEBUG_ERROR("Couldn't set texture pixels. Would cause overflow");
            return;
        }
        else if (!pixels)
        {
            JOP_DEBUG_ERROR("Couldn't set texture pixels. Pixel pointer is null");
            return;
        }

        bind(0);
        glCheck(gl::TexSubImage2D(gl::TEXTURE_2D, 0, x, y, width, height, gl::RGBA, gl::UNSIGNED_BYTE, pixels));
    }

    //////////////////////////////////////////////

    int Texture::getWidth() const
    {
        return m_width;
    }

    //////////////////////////////////////////////

    int Texture::getHeight() const
    {
        return m_height;
    }

    //////////////////////////////////////////////

    int Texture::getDepth() const
    {
        return m_bytesPerPixel;
    }

    //////////////////////////////////////////////

    int Texture::getMaximumSize()
    {
        static int size = 0;
        if (!size)
            glCheck(gl::GetIntegerv(gl::MAX_TEXTURE_SIZE, &size));

        return size;
    }

    //////////////////////////////////////////////

    std::weak_ptr<Texture> Texture::getError()
    {
        static std::weak_ptr<Texture> errTex;

        if (errTex.expired())
        {
            errTex = ResourceManager::getEmptyResource<Texture>("Error Texture");

            JOP_ASSERT_EVAL(errTex.lock()->load(IDB_PNG2), "Failed to load error texture!");
        }

        return errTex;
    }

    //////////////////////////////////////////////

    std::weak_ptr<Texture> Texture::getDefault()
    {
        static std::weak_ptr<Texture> defTex;

        if (defTex.expired())
        {
            defTex = ResourceManager::getEmptyResource<Texture>("Default Texture");
            
            JOP_ASSERT_EVAL(defTex.lock()->load(IDB_PNG1), "Failed to load default texture!");
        }

        return defTex;
    }

    //////////////////////////////////////////////

    unsigned int Texture::getHandle() const
    {
        return m_texture;
    }

    //////////////////////////////////////////////

    bool Texture::load(const int id)
    {
        std::vector<unsigned char> buf;
        if (!FileLoader::readFromDll(id, buf))
            return false;

        int x, y, bpp;
        unsigned char* pix = stbi_load_from_memory(buf.data(), buf.size(), &x, &y, &bpp, 4);

        if (!pix)
            return false;

        if (!load(x, y, bpp, pix))
            return false;

        stbi_image_free(pix);

        return true;
    }

    //////////////////////////////////////////////

    bool Texture::checkDepthValid(const int depth) const
    {
        return depth == 3 || depth == 4;
    }
}