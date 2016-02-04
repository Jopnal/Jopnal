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

#define STB_TRUETYPE_IMPLEMENTATION

#include <Jopnal/Graphics/stb/stb_truetype.h>

//////////////////////////////////////////////

namespace jop
{
    Font::~Font()
    {
        for (auto pair : m_bitmaps)
        {
            delete[] pair.second.data;
        }
        m_bitmaps.clear();
    }

    //////////////////////////////////////////////

    bool Font::load(const std::string& path)
    {
        auto info_ptr = std::make_unique<stbtt_fontinfo>();

        //load font data from file
        std::vector<unsigned char> buffer;

        FileLoader::read(path, buffer);

        if (!buffer.empty())
        {
            //save data
            bool success = stbtt_InitFont(info_ptr.get(), buffer.data(), 0);
            JOP_ASSERT(success, "Font failed!");

            m_info = std::move(info_ptr);
            return true;
        }
        return false;
    }

    //////////////////////////////////////////////
    
    std::pair<glm::ivec2, glm::ivec2> Font::getBounds(const int codepoint)
    {
        int x0, y0, x1, y1;
        stbtt_GetCodepointBox(m_info.get(), codepoint, &x0, &y0, &x1, &y1);
        return std::make_pair(glm::ivec2(x0, y0), glm::ivec2(x1-x0, y1-y0)); //x, y, w, h
    }

    //////////////////////////////////////////////

    int Font::getKerning(const int codepoint1, const int codepoint2)
    {
        return stbtt_GetCodepointKernAdvance(m_info.get(), codepoint1, codepoint2);
    }

    //////////////////////////////////////////////
    
    unsigned char* Font::getCodepointBitmap(const float scaleX, const float scaleY, const int codepoint, int* width, int* height)
    {
        auto it = m_bitmaps.find(codepoint);
        if (it != m_bitmaps.end())
        {
            //return data of the found glyph bitmap
            *width = it->second.width;
            *height = it->second.width;
            return it->second.data;
        }
        else
        {
            //get glyph rectangle size in pixels
            std::pair<glm::ivec2, glm::ivec2> bounds = getBounds(codepoint);
            //x, y, width, height
            
            //create the bitmap
            unsigned char* data = stbtt_GetCodepointBitmap(m_info.get(), scaleX, scaleY, codepoint, 
                &bounds.second.x, &bounds.second.y,
                &bounds.first.x,
                &bounds.first.y);
            
            //add the new bitmap to the map
            Bitmap bm;
            bm.height = bounds.second.x;
            bm.width = bounds.second.y;
            bm.data = data;
            m_bitmaps[codepoint] = bm;

            //return necessary stuff
            *width = bounds.second.x;
            *height = bounds.second.y;
            return data;
        }
    }
}