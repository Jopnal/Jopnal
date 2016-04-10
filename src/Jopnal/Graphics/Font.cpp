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
#pragma warning(push)
#pragma warning(disable: 4100)
#include <Jopnal/Graphics/stb/stb_rect_pack.h>
#pragma warning(pop)
#pragma warning(disable: 4505)

//////////////////////////////////////////////


namespace detail
{
    struct FontImpl
    {
        FT_Library m_library;                       ///< Freetype library
        FT_Face m_face;                             ///< Font info
        std::vector<unsigned char> m_buffer;        ///< File buffer
        std::unique_ptr<stbrp_context> m_context;   ///< Rectangle packing context
        ::jop::Texture2D m_texture;                 ///< Texture
        std::vector<stbrp_node> m_nodes;
        int m_numNodes;
        std::unordered_map <int, std::pair<glm::ivec2, glm::ivec2>> m_bitmaps; ///< Texture coordinates

        FontImpl()
            : m_texture     (""),
              m_nodes       (1024),
              m_numNodes    (1024)
        {
            FT_Error err = FT_Init_FreeType(&m_library);
            if (err)
                JOP_DEBUG_ERROR("Could not initialize freetype: " << err);
        }

        ~FontImpl()
        {
            FT_Done_Face(m_face);
            FT_Done_FreeType(m_library);
        }
    };
}

namespace jop
{
    Font::Font(const std::string& name)
        : Resource      (name),
          m_data        (std::make_unique<::detail::FontImpl>()),
          m_pixelSize   (64)
    {}

    Font::~Font()
    {}

    //////////////////////////////////////////////

    const Texture2D& Font::getTexture()const
    {
        return m_data.get()->m_texture;
    }

    //////////////////////////////////////////////

    float Font::getPixelSize() const
    {
        return static_cast<float>(m_pixelSize);
    }

    //////////////////////////////////////////////

    bool Font::load(const std::string& path, const int pixelSize)
    {
        // Load font data from file
        FileLoader::readBinaryfile(path, m_data.get()->m_buffer);
        return load(pixelSize);
    }

    //////////////////////////////////////////////

    bool Font::load(const int id, const int pixelSize)
    {
        if (!FileLoader::readResource(id, m_data.get()->m_buffer))
            return false;

        return load(pixelSize);
    }

    //////////////////////////////////////////////

    bool Font::load(const int pixelSize)
    {
        m_pixelSize = pixelSize;

        // Create texture and context for glyph atlas
        auto context_ptr = std::make_unique<stbrp_context>();

        m_data->m_texture.load(glm::uvec2(m_pixelSize * 32, m_pixelSize * 32), 1);
        stbrp_init_target(context_ptr.get(), 1024, 1024, m_data->m_nodes.data(), m_data->m_numNodes);

        if (!m_data.get()->m_buffer.empty())
        {
            // Save loaded data
            FT_Error error = FT_New_Memory_Face(m_data.get()->m_library, m_data.get()->m_buffer.data(), m_data.get()->m_buffer.size() * sizeof(unsigned char), 0, &m_data.get()->m_face);
            JOP_ASSERT(!error, "Failure loading font!");
            
            FT_Select_Charmap(m_data.get()->m_face, ft_encoding_unicode);

            // Set glyph size in pixels
            FT_Set_Pixel_Sizes(m_data.get()->m_face, m_pixelSize, m_pixelSize);
            error = FT_Select_Charmap(m_data.get()->m_face, FT_ENCODING_UNICODE);
            JOP_ASSERT(!error, "Failure selecting charmap!");

            m_data.get()->m_context = std::move(context_ptr);

            // Create default glyph
            const unsigned char data[4] = { 255, 255, 255, 255 };

            gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
            m_data->m_texture.setPixels(glm::uvec2(0, 0), glm::uvec2(2, 2), data);

            std::pair<glm::ivec2, glm::ivec2> bounds;

            bounds.first.x = 0;
            bounds.first.y = 0;
            bounds.second.x = 2;
            bounds.second.y = 2;
            
            stbrp_rect rectangle = { 0, static_cast<stbrp_coord>(bounds.second.x + 1), static_cast<stbrp_coord>(bounds.second.y + 1) };
            stbrp_pack_rects(m_data.get()->m_context.get(), &rectangle, 1);

            if (rectangle.was_packed != 0)
            {
                GlState::setBlendFunc(true);
                m_data->m_bitmaps[0] = bounds;
            }
            else
                JOP_DEBUG_ERROR("Failure creating white rectangle!");

            return true;
        }

        return false;
    }
    
    std::pair<glm::vec2, glm::vec2> Font::getBounds(const int codepoint) const
    {
        FT_Load_Glyph(m_data.get()->m_face, FT_Get_Char_Index(m_data.get()->m_face, codepoint), FT_LOAD_NO_BITMAP | FT_LOAD_NO_SCALE);

        // X and Y are offset from glyph origin
        int x, y, w, h;
        x = m_data.get()->m_face->glyph->metrics.horiBearingX;  // bitmap_left;
        y = m_data.get()->m_face->glyph->metrics.horiBearingY;  //bitmap_top;
        w = m_data.get()->m_face->glyph->metrics.width;         //bitmap.width;
        h = m_data.get()->m_face->glyph->metrics.height;        //bitmap.rows;

        return std::make_pair(glm::vec2(x, y), glm::vec2(w, h));
    }

    //////////////////////////////////////////////

    float Font::getKerning(const int codepoint1, const int codepoint2) const
    {
        FT_Vector vector;
        FT_Get_Kerning(m_data.get()->m_face, FT_Get_Char_Index(m_data.get()->m_face, codepoint1), FT_Get_Char_Index(m_data.get()->m_face, codepoint2), FT_KERNING_DEFAULT, &vector);

        return static_cast<float>(vector.x);
    }

    //////////////////////////////////////////////
    
    void Font::getTextureCoordinates(const int codepoint, int* width, int* height, int* x, int* y) const
    {
        auto it = m_data->m_bitmaps.find(codepoint);

        if (it != m_data->m_bitmaps.end())
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
            FT_UInt index = FT_Get_Char_Index(m_data.get()->m_face, codepoint);

            FT_GlyphSlot slot = m_data.get()->m_face->glyph;

            FT_Error err = FT_Load_Glyph(m_data.get()->m_face, index, FT_LOAD_DEFAULT);
            if (err)
                JOP_DEBUG_ERROR("Error while loading glyph: " << err);

            FT_Glyph glyphDesc;
            err = FT_Get_Glyph(slot, &glyphDesc);

            if (err)
                JOP_DEBUG_ERROR("Gylph not found: " << err);

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
            stbrp_rect rectangle = { 0, static_cast<stbrp_coord>(bounds.second.x + 1), static_cast<stbrp_coord>(bounds.second.y + 1) };
            stbrp_pack_rects(m_data.get()->m_context.get(), &rectangle, 1);
            rectangle.w -= 1;
            rectangle.h -= 1;

            if (rectangle.was_packed != 0)
            {
                GlState::setBlendFunc(true);

                // Was packed successfully, pass pixel data to texture
                m_data->m_texture.setPixels(glm::uvec2(rectangle.x, rectangle.y), glm::uvec2(rectangle.w, rectangle.h), pixelData);
                
                // Glyph data to map
                bounds.first.x = rectangle.x;
                bounds.first.y = rectangle.y;

                m_data->m_bitmaps[codepoint] = bounds;

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

    //////////////////////////////////////////////

    Font& Font::getError()
    {
        static WeakReference<Font> errFont;

        if (errFont.expired())
        {
            errFont = static_ref_cast<Font>(ResourceManager::getEmptyResource<Font>("jop_error_font").getReference());

            JOP_ASSERT_EVAL(errFont->load(JOP_RES_ERROR_FONT, 32), "Failed to load error font!");

            errFont->setPersistence(0);
        }

        return *errFont;
    }

    //////////////////////////////////////////////

    Font& Font::getDefault()
    {
        static WeakReference<Font> defFont;

        if (defFont.expired())
        {
            defFont = static_ref_cast<Font>(ResourceManager::getEmptyResource<Font>("jop_default_font").getReference());

            JOP_ASSERT_EVAL(defFont->load(JOP_RES_DEFAULT_FONT, 32), "Failed to load default font!");

            defFont->setPersistence(0);
        }

        return *defFont;
    }
}