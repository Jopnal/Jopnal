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

// STB
#define STB_IMAGE_IMPLEMENTATION
#pragma warning(push, 0)
#include <Jopnal/Graphics/stb/stb_image.h>
#pragma warning(pop)

//////////////////////////////////////////////


namespace jop
{
    Texture::Texture()
        : m_sampler         (),
          m_defaultSampler  (TextureSampler::getDefaultSampler()),
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
        std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
        if (path.empty())
            return false;

        // If exclamation mark is detected creates new flat texture
        if (path[0] == '!')
              return loadEmpty(path);

        // If not new texture is loaded from file 
        else
        {
            std::vector<unsigned char> buf;
            FileLoader::read(path, buf);

            int x = 0, y = 0, bpp = 0;

            unsigned char* colorData = stbi_load_from_memory(buf.data(), buf.size(), &x, &y, &bpp, 4);

            const bool success = loadFromPixels(x, y, bpp, colorData);

            stbi_image_free(colorData);

            return success;
        }
    }

    //////////////////////////////////////////////
 
    bool Texture::loadEmpty(const std::string& xyBpp)
    {
        if (xyBpp.empty())
            return false;

        // Temporary string for memory usage
        std::string variable;
        int x = 0, y = 0, bpp = 0;

        // Loop Reads param one character at time
        for (std::size_t i = 1; i <= xyBpp.size(); i++)
        {
            // Reads character to string if exclamation mark is not detected
            if (xyBpp[i] != '!')
                variable.insert(variable.end(), xyBpp[i]);
            else
            {
                // If width x param is fully read it puts it in class's variable
                if (!m_width)
                    x = std::stoi(variable);

                // If width y param is fully read it puts it in class's variable
                else if (!m_height)
                    y = std::stoi(variable);
            }

            // If exclamation mark is detected but it is not end of string temporary variable is reset
            if (xyBpp[i] == '!' && xyBpp[i + 1] != NULL)
                variable = "";
        }

        // Last param bpp is taken to the class's variable after loop is ended
        m_bytesPerPixel = std::stoi(variable);

        return loadFromPixels(x, y, bpp, nullptr);
    }

    //////////////////////////////////////////////

    bool Texture::loadFromPixels(const int x, const int y, const int bytesPerPixel, const unsigned char* pixels)
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
            glCheck(gl::BindTexture(gl::TEXTURE0 + texUnit, m_texture));

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

    void Texture::setTextureSampler(const std::weak_ptr<const TextureSampler>& sampler)
    {
        m_sampler = sampler;
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

    unsigned int Texture::getHandle() const
    {
        return m_texture;
    }

    //////////////////////////////////////////////

    bool Texture::checkDepthValid(const int depth) const
    {
        return /*depth == 3 || */depth == 4;
    }
}