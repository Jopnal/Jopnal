// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//////////////////////////////////////////////

// Headers
#include <Jopnal/Precompiled.hpp>

//STB
#define STB_IMAGE_IMPLEMENTATION
#pragma warning(push, 0)
#include <stb/stb_image.h>
#pragma warning(pop)

//////////////////////////////////////////////


namespace jop
{
    Texture::Texture()
        : m_width           (0),
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

            return loadFromPixels(x, y, bpp, colorData);
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
            glCheck(gl::BindTexture(gl::TEXTURE0 + texUnit, m_texture));

        return m_texture != 0;
    }

    //////////////////////////////////////////////

    void Texture::unbind(const unsigned int texUnit)
    {
        glCheck(gl::BindTexture(gl::TEXTURE0 + texUnit, 0));
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
        return depth == 3 || depth == 4;
    }
}