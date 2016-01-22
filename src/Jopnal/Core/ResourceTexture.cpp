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
    ResourceTexture::ResourceTexture()
    {
        m_textureWidth=NULL;
        m_textureHeight=NULL;
        m_bitsPerPixel=NULL;
    }

	ResourceTexture::~ResourceTexture()
	{   }


    bool ResourceTexture::load(const std::string& path)
    {
 
            long long size = FileLoader::getSize(path.c_str());
            std::vector<unsigned char> buf(size);
            FileLoader::read(buf.data(), path.c_str(), size);
            //textureBuffer[size] = '\0';

            //stbi_uc stbBuffer = textureBuffer[size];
            
            //auto pixBuf = stbi_info_from_memory(&stbBuffer, size, &m_textureWidth, &m_textureHeight, &m_bitsPerPixel);
            unsigned char* colorData = stbi_load_from_memory(buf.data(), size, &m_textureWidth, &m_textureHeight, &m_bitsPerPixel, 4);

            return true;
    }

    int ResourceTexture::getTextureSizeAndBpp()
    {
        return (m_textureWidth, m_textureHeight, m_bitsPerPixel);
    }
    
}