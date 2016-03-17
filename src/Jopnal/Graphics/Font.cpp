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

//////////////////////////////////////////////


namespace jop
{
    Font::Font(const std::string& path) : Resource(path),
        m_texture("")
    {
        FT_Error err = FT_Init_FreeType(&m_library);
        if (err)
            JOP_DEBUG_ERROR("Could not initialize freetype: " << err);

        if (path != "")
            load(path);
    }

    Font::~Font()
    {
        FT_Done_FreeType(m_library);
        m_bitmaps.clear();
        delete[] m_nodes;
    }

    //////////////////////////////////////////////

    Texture& Font::getTexture()
    {
        return m_texture;
    }

    //////////////////////////////////////////////

    bool Font::load(const std::string& path)
    {
        // Create texture and context for glyph atlas
        auto context_ptr = std::make_unique<stbrp_context>();
        m_nodes = new stbrp_node[1024];
        m_numNodes = 1024;
        m_texture.load(1024, 1024, 1);
        stbrp_init_target(context_ptr.get(), 1024, 1024, m_nodes, m_numNodes);

        // Load font data from file
        FileLoader::read(path, m_buffer);

        if (!m_buffer.empty())
        {
            // Save loaded data
            FT_Error error = FT_New_Memory_Face(m_library, m_buffer.data(), m_buffer.size() * sizeof(unsigned char), 0, &m_face);
            JOP_ASSERT(!error, "Failure loading font!");

            FT_Select_Charmap(m_face, ft_encoding_unicode);

            // Set glyph size in pixels
            FT_Set_Pixel_Sizes(m_face, 64, 64);
            error = FT_Select_Charmap( m_face, FT_ENCODING_UNICODE);
            JOP_ASSERT(!error, "Failure selecting charmap!");

            m_context = std::move(context_ptr);
            
            // Create default glyph
            unsigned char data[4] = { 255, 255, 255, 255 };

            m_texture.setPixels(0, 0, 2, 2, data);

            std::pair<glm::ivec2, glm::ivec2> bounds;

            bounds.first.x = 0;
            bounds.first.y = 0;
            bounds.second.x = 2;
            bounds.second.y = 2;

            stbrp_rect rectangle = { 0, bounds.second.x, bounds.second.y };
            stbrp_pack_rects(m_context.get(), &rectangle, 1);

            if (rectangle.was_packed != 0)
            {
                GlState::setBlendFunc(true);
                m_texture.setPixels(rectangle.x, rectangle.y, rectangle.w, rectangle.h, data);
                m_bitmaps[0] = bounds;
            }
            else
                JOP_DEBUG_ERROR("Failure creating white rectangle!");

            return true;
        }
        return false;
    }

    //////////////////////////////////////////////
    
    std::pair<glm::ivec2, glm::ivec2> Font::getBounds(const int codepoint)
    {
        FT_Load_Glyph(m_face, FT_Get_Char_Index(m_face, codepoint), FT_LOAD_NO_BITMAP|FT_LOAD_NO_SCALE);

        // X and Y are offset from glyph origo
        int x, y, w, h;
        x = m_face->glyph->bitmap_left;
        y = m_face->glyph->bitmap_top;
        w = m_face->glyph->bitmap.width;
        h = m_face->glyph->bitmap.rows;

        return std::make_pair(glm::ivec2(x, y), glm::ivec2(w, h));
    }

    //////////////////////////////////////////////

    float Font::getKerning(const int codepoint1, const int codepoint2)
    {
        FT_Vector vector;
        FT_Get_Kerning(m_face, FT_Get_Char_Index(m_face, codepoint1), FT_Get_Char_Index(m_face, codepoint2), FT_KERNING_DEFAULT, &vector);
        return vector.x;
    }

    //////////////////////////////////////////////
    
    void Font::getTextureCoordinates(const int codepoint, int* width, int* height, int* x, int* y)
    {
        
        auto it = m_bitmaps.find(codepoint);
        if (it != m_bitmaps.end())
        {           
            // Return glyph texture coordinates in pixels
            *x = it->second.first.x;
            *y = it->second.first.y;
            *width = it->second.second.x;
            *height = it->second.second.y;
        }
        else
        {
            // Load glyph
            FT_UInt index = FT_Get_Char_Index(m_face, codepoint);

            FT_GlyphSlot slot = m_face->glyph;

            FT_Error err = FT_Load_Glyph(m_face, index, FT_LOAD_DEFAULT);
            if (err)
            {
                JOP_DEBUG_ERROR("Error while loading glyph: " << err);
            }

            FT_Glyph glyphDesc;
            err = FT_Get_Glyph(slot, &glyphDesc);
            if (err)
            {
                JOP_DEBUG_ERROR("Gylph not found: " << err);
            }
            FT_Glyph_To_Bitmap(&glyphDesc, FT_RENDER_MODE_NORMAL, 0, 1);
            FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyphDesc;

            FT_Bitmap& bitmap = bitmapGlyph->bitmap;

            // Get glyph rectangle size in pixels
            std::pair<glm::ivec2, glm::ivec2> bounds; 

            bounds.first.x = bitmapGlyph->left;
            bounds.first.y = bitmapGlyph->top;
            bounds.second.x = bitmapGlyph->bitmap.width;
            bounds.second.y = bitmapGlyph->bitmap.rows;
           
            unsigned char* pixelData = bitmap.buffer;
            
            // Find an empty spot in the texture
            stbrp_rect rectangle = { 0, bounds.second.x, bounds.second.y};
            stbrp_pack_rects(m_context.get(), &rectangle, 1);

            if (rectangle.was_packed != 0)
            {
                GlState::setBlendFunc(true);

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
                JOP_DEBUG_ERROR("Failure packing rectangle! Need bigger texture!");

            FT_Done_Glyph(glyphDesc);
        }
    }
}