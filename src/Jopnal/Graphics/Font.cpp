//Jopnal Engine C++ Library
//Copyright(c) 2016 Team Jopnal
//
//This software is provided 'as-is', without any express or implied
//warranty.In no event will the authors be held liable for any damages
//arising from the use of this software.
//
//Permission is granted to anyone to use this software for any purpose,
//including commercial applications, and to alter it and redistribute it
//freely, subject to the following restrictions :
//
//1. The origin of this software must not be misrepresented; you must not
//claim that you wrote the original software.If you use this software
//in a product, an acknowledgement in the product documentation would be
//appreciated but is not required.
//2. Altered source versions must be plainly marked as such, and must not be
//misrepresented as being the original software.
//3. This notice may not be removed or altered from any source distribution.

//////////////////////////////////////////////

#define STB_TRUETYPE_IMPLEMENTATION

// Headers
#include <Jopnal/Precompiled.hpp>

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

        // Load font data from file
        std::vector<unsigned char> buffer;

        FileLoader::read(path, buffer);

        if (!buffer.empty())
        {
            // Save loaded data
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
        return std::make_pair(glm::ivec2(x0, y0), glm::ivec2(x1-x0, y1-y0)); // X, Y, width & height
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
            // Return data of the found glyph bitmap
            *width = it->second.width;
            *height = it->second.width;
            return it->second.data;
        }
        else
        {
            // Get glyph rectangle size in pixels
            std::pair<glm::ivec2, glm::ivec2> bounds = getBounds(codepoint);
            
            // Create a bitmap
            unsigned char* data = stbtt_GetCodepointBitmap(m_info.get(), scaleX, scaleY, codepoint, 
                &bounds.second.x, &bounds.second.y,
                &bounds.first.x,
                &bounds.first.y);
            
            // Add the new bitmap to the map
            Bitmap bm;
            bm.height = bounds.second.x;
            bm.width = bounds.second.y;
            bm.data = data;
            m_bitmaps[codepoint] = bm;

            // Return necessary data
            *width = bounds.second.x;
            *height = bounds.second.y;
            return data;
        }
    }
}