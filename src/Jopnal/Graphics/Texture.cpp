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
    {
        m_textureWidth=NULL;
        m_textureHeight=NULL;
        m_bitsPerPixel=NULL;
    }

    //////////////////////////////////////////////

	Texture::~Texture()
	{   }

    //////////////////////////////////////////////
    //////////////////////////////////////////////

    bool Texture::load(const std::string& path)
    {
        /// \If exclamation mark is detected creates new flat texture
        if (path[0] == '!')
              return loadEmpty(path);
        /// \If not new texture is loaded from file 
        else
        {
            std::vector<unsigned char> buf;
            FileLoader::read(path, buf);

            unsigned char* colorData = stbi_load_from_memory(buf.data(), buf.size(), &m_textureWidth, &m_textureHeight, &m_bitsPerPixel, 4);
            return true;
        }
    }

    //////////////////////////////////////////////
 
    bool Texture::loadEmpty(const std::string& xyBpp)
    {
        //// \Temporary string for memory usage
        std::string variable="";
        /// \Loop Reads param one character at time
        for (int i = 1; i <= xyBpp.size(); i++)
        {
            /// \Reads character to string if exclamation mark	is not detected
            if (xyBpp[i] != '!')
                variable.insert(variable.end(), xyBpp[i]);
            else
            {
                /// \If width x param is fully readed it puts it in class's variable
                if (m_textureWidth == NULL)
                    m_textureWidth = std::stoi(variable);
                /// \If width y param is fully readed it puts it in class's variable
                else if (m_textureHeight == NULL)
                    m_textureHeight = std::stoi(variable);
            }
            /// \If exclamation mark is detected but it is not end of string temporary variable is reseted
            if (xyBpp[i] == '!'&&xyBpp[i + 1] != NULL)
                variable = "";
        }
        /// \Last param Bpp is taken to the class's variable after loop is ended
        m_bitsPerPixel = std::stoi(variable);
        /// \Check for legible class's variables and for moving it in gpu memory
        if (m_textureWidth != NULL || m_textureHeight != NULL || m_bitsPerPixel != NULL/*glTexture2D*/)
            return true;
        else
            JOP_DEBUG_ERROR("Couldn't create resource: " << xyBpp); 
            return false;
    }

    //////////////////////////////////////////////

    int Texture::getWidth()
    {
        return m_textureWidth;
    }

    //////////////////////////////////////////////

    int Texture::getHeight()
    {
        return  m_textureHeight;
    }

    //////////////////////////////////////////////

    int Texture::getBpp()
    {
        return m_bitsPerPixel;
    }
    
}