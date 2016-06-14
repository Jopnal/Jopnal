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
#define STB_TRUETYPE_IMPLEMENTATION
#include <Jopnal/Graphics/STB/stb_truetype.h>
#define STB_RECT_PACK_IMPLEMENTATION
#include <Jopnal/Graphics/stb/stb_rect_pack.h>
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
            m_nodes(),
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
        m_textureSize(256),
        m_fontSize(64)
    {}

    Font::~Font()
    {}

    //////////////////////////////////////////////

    const Texture2D& Font::getTexture() const
    {
        return m_texture;
    }

    //////////////////////////////////////////////

    float Font::getFontSize() const
    {
        return static_cast<float>(m_fontSize);
    }

    //////////////////////////////////////////////

    bool Font::load(const std::string& path, const int fontSize)
    {
        FileLoader::readBinaryfile(path, m_buffer);

        m_fontSize = fontSize;

        // Create texture and context for glyph atlas
        auto context_ptr = std::make_unique<stbrp_context>();
        m_texture.load(glm::uvec2(m_fontSize * 32, m_fontSize * 32), 1, false);

        // Initialize rectangle packer
        stbrp_init_target(context_ptr.get(), m_textureSize, m_textureSize, m_data->m_nodes.data(), m_numNodes);

        if (!m_buffer.empty())
        {
            // Load & init
            stbtt_InitFont(&m_data->m_fontInfo, reinterpret_cast<const unsigned char*>(m_buffer.data()), 0/*stbtt_GetFontOffsetForIndex(buffer,0)*/); 
            // Calculate scale
            stbtt_ScaleForPixelHeight(&m_data->m_fontInfo, 15);

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
            // Pack rectangle - was_packed is non zero if packing succeeded
            stbrp_pack_rects(&m_data->m_context, &rectangle, 1);

            if (rectangle.was_packed != 0)
            {
                Glyph emptyGlyph(2, 0, 0, 2, 2);
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

    float Font::getKerning(const uint32 left, const uint32 right) const
    {
        return stbtt_GetCodepointKernAdvance(&m_data->m_fontInfo, left, right);
    }

    //////////////////////////////////////////////

    const jop::Glyph& Font::getGlyph(uint32 codepoint)
    {
        static const jop::Glyph emptyGlyph;

        auto it = m_bitmaps.find(codepoint);

        if (it != m_bitmaps.end())
            return it->second;

        int left, right, bottom, top, advance;
        // Get glyph id and bounding boxes
        int glyphID = stbtt_FindGlyphIndex(&m_data->m_fontInfo, codepoint);
        stbtt_GetGlyphBox(&m_data->m_fontInfo, glyphID, &left, &right, &bottom, &top);
        // Get advance
        stbtt_GetCodepointHMetrics(&m_data->m_fontInfo, codepoint, &advance, 0);
        
        // Find an empty spot in the texture
        stbrp_rect rectangle = 
        { 0, static_cast<stbrp_coord>(right + 1), static_cast<stbrp_coord>(top + 1) };
        stbrp_pack_rects(&m_data->m_context, &rectangle, 1);
        rectangle.w -= 1;
        rectangle.h -= 1;

        if (rectangle.was_packed != 0)
        {
            //GlState::setBlendFunc(true);
            // Was packed successfully, pass pixel data to texture
           // m_texture.setPixels(glm::uvec2(rectangle.x, rectangle.y), glm::uvec2(rectangle.w, rectangle.h), pixelData);

            // Glyph data to map
            //bounds.first.x = rectangle.x;
            //bounds.first.y = rectangle.y;

            // Create new glyph & return it
            jop::Glyph glyph(advance, left, right, bottom, top);
            return glyph;
        }
        else
            JOP_DEBUG_ERROR("Failure packing glyph!");
    }

    //////////////////////////////////////////////

    float Font::getLineSpacing() const
    {
        int lineGap;
        stbtt_GetFontVMetrics(&m_data->m_fontInfo, 0, 0, &lineGap);
        return static_cast<float>(lineGap);
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