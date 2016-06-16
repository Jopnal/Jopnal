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
#pragma warning(push)
#pragma warning(disable: 4100)
// NOTE: stb_rect_pack HAS to be included before stb_truetype to avoid errors
#define STB_RECT_PACK_IMPLEMENTATION
#include <Jopnal/Graphics/stb/stb_rect_pack.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include <Jopnal/Graphics/STB/stb_truetype.h>
#pragma warning(pop)
#pragma warning(disable: 4505)

//////////////////////////////////////////////

namespace detail
{
    struct FontImpl
    {
        stbtt_fontinfo m_fontInfo; ///< Font info
        stbrp_context m_context;   ///< Rectangle packing context
        std::vector<stbrp_node> m_nodes;
        
        FontImpl():
            m_nodes(256),
            m_fontInfo(),
            m_context()
        {}
    };
}

namespace jop
{
    Font::Font(const std::string& name)
        : Resource(name),
        m_data(std::make_unique<::detail::FontImpl>()),
        m_texture(""),
        m_buffer(0),
        m_packerSize(256),
        m_fontSize(64)
    {}

    Font::~Font()
    {}

    //////////////////////////////////////////////

    const Texture2D& Font::getTexture()
    {
        return m_texture;
    }

    //////////////////////////////////////////////

    float Font::getFontSize()
    {
        return static_cast<float>(m_fontSize);
    }

    //////////////////////////////////////////////

    bool Font::load(const std::string& path, const int fontSize)
    {
        // Load font data from file
        FileLoader::readBinaryfile(path, m_buffer);
        return load(fontSize);
    }

    //////////////////////////////////////////////

    bool Font::load(const int id, const int fontSize)
    {
        if (!FileLoader::readResource(id, m_buffer))
            return false;

        return load(fontSize);
    }

    //////////////////////////////////////////////

    bool Font::load(const int fontSize)
    {
        m_fontSize = fontSize;

        // Create texture and context for glyph atlas
        //auto context_ptr = std::make_unique<stbrp_context>();
        m_texture.load(glm::uvec2(m_packerSize, m_packerSize), 1, false);

        // Initialize rectangle packer
        stbrp_init_target(&m_data->m_context, m_packerSize, m_packerSize, m_data->m_nodes.data(), m_data->m_nodes.size());

        if (!m_buffer.empty())
        {
            // Load & init
            stbtt_InitFont(&m_data->m_fontInfo, m_buffer.data(), 0); 
  

            // Create default glyph
            const unsigned char data[4] = { 255, 255, 255, 255 };

            // Set pixel storage modes
            gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
            m_texture.setPixels(glm::uvec2(0, 0), glm::uvec2(2, 2), data);

            std::pair<glm::ivec2, glm::ivec2> bounds;
            bounds.first.x = 0;
            bounds.first.y = 0;
            bounds.second.x = 2;
            bounds.second.y = 2;

            // Create stb_rectangle
            stbrp_rect rectangle = { 0, static_cast<stbrp_coord>(bounds.second.x + 1), static_cast<stbrp_coord>(bounds.second.y + 1) };
            // Pack rectangle - was_packed is non-zero if packing succeeded
            stbrp_pack_rects(&m_data->m_context, &rectangle, 1);

            if (rectangle.was_packed != 0)
            {
                Glyph emptyGlyph;
                emptyGlyph.advance = m_fontSize;
                emptyGlyph.bounds = Rect{ 0, 0, 2, 2 };
                emptyGlyph.textCoord = Rect{ 0, 0, 2, 2 };
                GlState::setBlendFunc(true);
                m_bitmaps[0] = emptyGlyph;
            }
            else
                JOP_DEBUG_ERROR("Failure creating empty glyph!");

            return true;
        }

        return false;
    }

    //////////////////////////////////////////////

    const float Font::getKerning(const uint32 left, const uint32 right)
    {
        float scale = stbtt_ScaleForPixelHeight(&m_data->m_fontInfo, m_fontSize);
        return (float)stbtt_GetCodepointKernAdvance(&m_data->m_fontInfo, left, right) * scale;
    }

    //////////////////////////////////////////////

    const jop::Glyph& Font::getGlyph(uint32 codepoint)
    {
        auto it = m_bitmaps.find(codepoint);

        if (it != m_bitmaps.end())
            return it->second;

        // Scale according to font size
        float scale = stbtt_ScaleForPixelHeight(&m_data->m_fontInfo, m_fontSize);

        int left = 0, right = 0, bottom = 0, top = 0, advance = 0;

        // Get bounding box
        stbtt_GetCodepointBox(&m_data->m_fontInfo, codepoint, &left, &top, &right, &bottom);
        // Get advance
        stbtt_GetCodepointHMetrics(&m_data->m_fontInfo, codepoint, &advance, 0);
        
        int width = right * scale - left * scale;
        int height = bottom * scale - top * scale;
        // Find an empty spot in the texture - Add empty pixels (2) after each rect to avoid artifacts
        stbrp_rect rectangle = 
        { 0, static_cast<stbrp_coord>(width + 2), static_cast<stbrp_coord>(height + 2) };
        stbrp_pack_rects(&m_data->m_context, &rectangle, 1);
        rectangle.w -= 1;
        rectangle.h -= 1;

        if (rectangle.was_packed != 0)
        {
            GlState::setBlendFunc(true);
            
            unsigned char* pixelData = stbtt_GetCodepointBitmap(&m_data->m_fontInfo, scale, scale, codepoint, &width, &height, 0, 0);
            // Pass pixel data to texture
            m_texture.setPixels(glm::uvec2(rectangle.x, rectangle.y), glm::uvec2(width, height), pixelData);

            // Create new glyph & return it (left-to-right, top-to-bottom)
            jop::Glyph glyph; 
            glyph.advance = advance * scale;
            glyph.bounds = Rect{ left * scale, right * scale, bottom * scale, top * scale };
            glyph.textCoord = Rect{ rectangle.x, rectangle.x + width, rectangle.y, rectangle.y + height };
            m_bitmaps[codepoint] = glyph;
            return m_bitmaps[codepoint];
        }
        else        
            JOP_DEBUG_ERROR("Failure packing glyph!");

        // If everything else fails return dummy glyph
        static const jop::Glyph emptyGlyph;
        return emptyGlyph;
    }

    //////////////////////////////////////////////

    float Font::getLineSpacing()
    {
        float scale = stbtt_ScaleForPixelHeight(&m_data->m_fontInfo, m_fontSize);
        float lineSpace = 0;
        int ascent, descent, lineGap;
        stbtt_GetFontVMetrics(&m_data->m_fontInfo, &ascent, &descent, &lineGap);
        lineSpace = ascent * scale - descent * scale + lineGap * scale;
        return lineSpace;
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