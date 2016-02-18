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


// Headers
#include <Jopnal/Precompiled.hpp>

#define STB_RECT_PACK_IMPLEMENTATION
#include <Jopnal/Graphics/stb/stb_rect_pack.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include <Jopnal/Graphics/stb/stb_truetype.h>

//////////////////////////////////////////////


namespace jop
{
    Font::~Font()
    {
        //for (auto pair : m_bitmaps)
        //{
        //    delete[] pair.second.data;
        //}
        //m_bitmaps.clear();
        delete[] m_nodes;
    }

    Font::Font(const std::string& path) : Resource(path),
        m_texture("")
    {
        if (path != "")
        {
            load(path);
        }
    }

    //////////////////////////////////////////////

    Texture& Font::getTexture()
    {
        return m_texture;
    }

    //////////////////////////////////////////////

    bool Font::load(const std::string& path)
    {
        auto info_ptr = std::make_unique<stbtt_fontinfo>();
        auto context_ptr = std::make_unique<stbrp_context>();

        m_nodes = new stbrp_node[1024];
        m_numNodes = 1024;

        m_texture.load(1024, 1024, 4);

        stbrp_init_target(context_ptr.get(), 1024, 1024, m_nodes, m_numNodes);

        // Load font data from file
        std::vector<unsigned char> buffer;

        FileLoader::read(path, buffer);

        if (!buffer.empty())
        {
            // Save loaded data
            bool success = stbtt_InitFont(info_ptr.get(), buffer.data(), 0);
            JOP_ASSERT(success, "Failed to load font!");

            m_info = std::move(info_ptr);
            m_context = std::move(context_ptr);
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
    
    void Font::getCodepointBitmap(const float scaleX, const float scaleY, const int codepoint, int* width, int* height, int* x, int* y)
    {
        auto it = m_bitmaps.find(codepoint);
        if (it != m_bitmaps.end())
        {
            
            //return glyph texture coordinates
            *x = it->second.first.x;
            *y = it->second.first.y;
            *width = it->second.second.x;
            *height = it->second.second.y;

        }
        else
        {
            // Get glyph rectangle size in pixels
            std::pair<glm::ivec2, glm::ivec2> bounds = getBounds(codepoint); // X, Y, width & height
           
            // Create a bitmap
            unsigned char* pixelData = stbtt_GetCodepointBitmap(m_info.get(), scaleX, scaleY, codepoint, 
                &bounds.second.x, &bounds.second.y,
                &bounds.first.x, &bounds.first.y);
            
            // Find an empty spot in the texture
            stbrp_rect rectangle = { 0, bounds.second.x, bounds.second.y};
            stbrp_pack_rects(m_context.get(), &rectangle, 1);

            if (rectangle.was_packed != 0)
            {
                // Was packed successfully, pass pixel data to texture
                m_texture.setPixels(rectangle.x, rectangle.y, rectangle.w, rectangle.h, pixelData);
                
                // Glyph data to map
                bounds.first.x = rectangle.x;
                bounds.first.y = rectangle.y;

                m_bitmaps[codepoint] = bounds;

                *x = rectangle.x;
                *y = rectangle.y;
                *width = rectangle.w;
                *height = rectangle.h;
            }
            else
            {
                //we need a bigger texture!
            }
        }
    }
}