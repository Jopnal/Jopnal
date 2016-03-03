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

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H


//////////////////////////////////////////////


namespace jop
{
    Font::Font(const std::string& path) : Resource(path),
        m_texture("")
    {
        FT_Error err = FT_Init_FreeType(&m_library);
        if (err)
        {
            JOP_DEBUG_ERROR("Could not initialize freetype: " << err);
        }

        if (path != "")
        {
            load(path);
        }
    }

    Font::~Font()
    {
        FT_Done_FreeType(m_library);
        //for (auto pair : m_bitmaps)
        //{
        //    delete[] pair.second.data;
        //}
        //m_bitmaps.clear();
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
        // create texture and context for glyph atlas
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
            FT_Error error = FT_New_Memory_Face(m_library, buffer.data(), buffer.size() * sizeof(unsigned char), 0, &m_face);
            JOP_ASSERT(!error, "Failed to load font!");

            FT_Select_Charmap(m_face, ft_encoding_unicode);

            //char size
            FT_Set_Char_Size(m_face, 0, 16*64, 300, 300);

            //set pixel size, whatever it is...
            FT_Set_Pixel_Sizes(m_face, 16, 16);
            error = FT_Select_Charmap( m_face, FT_ENCODING_UNICODE);
            JOP_ASSERT(!error, "Failed to select charmap!");

            //m_face->available_sizes = 
            //new FT_Bitmap_Size {
            //    32,
            //    32,
            //    1,
            //    32,
            //    32
            //};

            m_context = std::move(context_ptr);
            return true;
        }
        return false;
    }

    //////////////////////////////////////////////
    
    std::pair<glm::ivec2, glm::ivec2> Font::getBounds(const int codepoint)
    {
        FT_Load_Glyph(m_face, FT_Get_Char_Index(m_face, codepoint), FT_LOAD_NO_BITMAP);

        //x and y are offset from glyph origo
        int x, y, w, h;
        x = m_face->glyph->bitmap_left;
        y = m_face->glyph->bitmap_top;
        w = m_face->glyph->bitmap.width;
        h = m_face->glyph->bitmap.rows;
        return std::make_pair(glm::ivec2(x, y), glm::ivec2(w, h)); // X, Y, width & height
    }

    //////////////////////////////////////////////

    float Font::getKerning(const int codepoint1, const int codepoint2)
    {
        FT_Vector vector;
        FT_Get_Kerning(m_face, FT_Get_Char_Index(m_face, codepoint1), FT_Get_Char_Index(m_face, codepoint2), FT_KERNING_DEFAULT, &vector);
        return vector.x;
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
            //load glyph
            FT_UInt index = 50;//FT_Get_Char_Index(m_face, codepoint);
            //FT_Load_Char(m_face, codepoint, FT_LOAD_DEFAULT);

            FT_GlyphSlot slot = m_face->glyph;

            FT_Error err = FT_Load_Glyph(m_face, index, FT_LOAD_DEFAULT);
            if (err)
            {
                JOP_DEBUG_ERROR("Error while loading glyph: " << err);
            }

            //err = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
            //if (err)
            //{
            //    JOP_DEBUG_ERROR("Error while rendering glyph: " << err);
            //}

            FT_Glyph glyphDesc;
            err = FT_Get_Glyph(slot, &glyphDesc);
            if (err)
            {
                JOP_DEBUG_ERROR("FAILED!: " << err);
            }
            FT_Glyph_To_Bitmap(&glyphDesc, FT_RENDER_MODE_NORMAL, 0, 1);
            FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyphDesc;

            FT_Bitmap& bitmap = bitmapGlyph->bitmap;

            FT_Done_Glyph(glyphDesc);

            // Get glyph rectangle size in pixels
            std::pair<glm::ivec2, glm::ivec2> bounds = getBounds(codepoint); // X, Y, width & height
           
            // Create a bitmap
            unsigned char* pixelData = bitmap.buffer;//slot->bitmap.buffer;
            
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

    //////////////////////////////////////////////

    int Font::getCharacters(uint8_t buffer[], unsigned long *id, size_t strlen, uint32_t *cp)
    {
        int remainUnits;
        uint8_t next, mask;

        if (*id >= strlen)
            return -1;

        next = buffer[(*id)++];
        if (next & 0x80)
        {
            mask = 0xe0;
            for (remainUnits = 1; (next & mask) != (mask << 1); remainUnits++)
            {
                mask = (mask >> 1) | 0x80;
            }
        }
        else
        {
            remainUnits = 0;
            mask = 0;
        }

        *cp = next ^ mask;

        while (remainUnits-- > 0)
        {
            *cp <<= 6;
            if (*id >= strlen)
                return -1;
            *cp |= buffer[(*id)++] & 0x3f;
        }

        return 0;
    }

    //////////////////////////////////////////////

    int Font::codePointCount(uint8_t chars[], size_t strlen, size_t *outSize)
    {
        unsigned long id = 0;

        for (*outSize = 0; *outSize < strlen; *outSize++)
        {
            uint32_t cp;
            getCharacters(chars, &id, strlen, &cp);
            if (!(cp < 0xd800 || cp > 0xdfff))
                return -1;
        }
        
        return 0;
    }

    //////////////////////////////////////////////

    int Font::convert(uint8_t input[], uint32_t output[], size_t count, size_t *outSize)
    {
        unsigned long id = 0;

        for (*outSize = 0; *outSize < count; *outSize++)
        {
            getCharacters(input, &id, count, &output[count]);
            if (!(id < 0xd800 || id > 0xdfff))
                return -1;
        }

        return 0;
    }
}