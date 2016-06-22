// Jopnal Engine C++ Library
// Copyright (c) 2016 Team Jopnal
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgement in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

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


namespace jop
{
    namespace detail
    {
        struct Packer
        {
            stbrp_context               context;
            std::vector<stbrp_node>     nodes;
            glm::uvec2                  origin;
        };
        struct FontImpl
        {
            stbtt_fontinfo fontInfo;          ///< Font info  
            std::vector<Packer> packers;      ///< Texture packers
        };
    }

    //////////////////////////////////////////////

    Font::Font(const std::string& name)
        : Resource(name),
        m_texture(""),
        m_glyphs(),
        m_buffer(0),
        m_data(std::make_unique<detail::FontImpl>()),      
        m_fontSize(0),
        m_packerIndex(0)
    {}

    Font::~Font()
    {}

    //////////////////////////////////////////////

    const Texture2D& Font::getTexture() const
    {
        return m_texture;
    }

    //////////////////////////////////////////////

    int Font::getSize() const
    {
        return m_fontSize;
    }

    //////////////////////////////////////////////

    bool Font::load(const std::string& path, const int fontSize)
    {
        return FileLoader::readBinaryfile(path, m_buffer) && load(fontSize);
    }

    //////////////////////////////////////////////

    bool Font::load(const int id, const int fontSize)
    {
        return FileLoader::readResource(id, m_buffer) && load(fontSize);
    }

    //////////////////////////////////////////////

    bool Font::load(const int fontSize)
    {
        if (!m_buffer.empty() && stbtt_InitFont(&m_data->fontInfo, m_buffer.data(), 0))
        {
            m_fontSize = fontSize;
            static const unsigned int initialSize = std::max(64u, SettingManager::get<unsigned int>("engine@Graphics|Font|uTextureInitialSize", 256));

            // Create texture and context for glyph atlas;
            m_texture.load(glm::uvec2(initialSize, initialSize), 1, false);

            m_data->packers.emplace_back();
            m_data->packers.back().nodes.resize(initialSize);
            m_data->packers.back().origin = glm::uvec2(0);
            // Initialize rectangle packer - NOTE: Make sure 'num_nodes' >= 'width'
            stbrp_init_target(&m_data->packers.back().context, initialSize, initialSize, m_data->packers.back().nodes.data(), m_data->packers.back().nodes.size());

            // Create default glyph
            const unsigned char data[4] = { 255, 255, 255, 255 };

            m_texture.setPixels(glm::uvec2(0, 0), glm::uvec2(2, 2), data);

            Rect bounds{ 0, 0, 2, 2 };

            // Create rectangle
            stbrp_rect rectangle = { 0, static_cast<stbrp_coord>(bounds.right + 1), static_cast<stbrp_coord>(bounds.top + 1) };
            // Pack rectangle - was_packed is non-zero if packing succeeded
            stbrp_pack_rects(&m_data->packers.back().context, &rectangle, 1);

            if (rectangle.was_packed)
            {
                Glyph emptyGlyph;
                emptyGlyph.advance = m_fontSize;
                emptyGlyph.bounds = bounds;
                emptyGlyph.textCoord = bounds;
                m_glyphs[0] = emptyGlyph;
            }

            return true;
        }

        return false;
    }

    //////////////////////////////////////////////

    float Font::getKerning(const uint32 left, const uint32 right) const
    {
        return static_cast<float>(stbtt_GetCodepointKernAdvance(&m_data->fontInfo, left, right) * stbtt_ScaleForPixelHeight(&m_data->fontInfo, static_cast<float>(m_fontSize)));
    }

    //////////////////////////////////////////////

    const Glyph& Font::getGlyph(const uint32 codepoint) const
    {
        auto it = m_glyphs.find(codepoint);

        if (it != m_glyphs.end())
            return it->second;

        // If glyph was not found in the bitmap
        // Create new one and pack it
        if (packGlyph(codepoint))
            return m_glyphs[codepoint];

        // If everything else fails return empty glyph
        static const jop::Glyph emptyGlyph;
        return emptyGlyph;
    }

    //////////////////////////////////////////////

    float Font::getLineSpacing() const
    {
        const float scale = stbtt_ScaleForPixelHeight(&m_data->fontInfo, static_cast<float>(m_fontSize));
        
        int ascent, descent, lineGap;
        stbtt_GetFontVMetrics(&m_data->fontInfo, &ascent, &descent, &lineGap);

        return ascent * scale - descent * scale + lineGap * scale;
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

    //////////////////////////////////////////////

    bool Font::packGlyph(const uint32 codepoint) const
    {
        // Scale according to font size (in pixels)
        const float scale = stbtt_ScaleForPixelHeight(&m_data->fontInfo, static_cast<float>(m_fontSize));
        int left = 0, right = 0, bottom = 0, top = 0, advance = 0;

        // Get bounding box
        stbtt_GetCodepointBox(&m_data->fontInfo, codepoint, &left, &top, &right, &bottom);
        // Get advance
        stbtt_GetCodepointHMetrics(&m_data->fontInfo, codepoint, &advance, 0);

        // Width & Height - scaled
        int width = static_cast<int>(right * scale - left * scale);
        int height = static_cast<int>(bottom * scale - top * scale);

        // Find an empty spot in the texture
        // Add padding (2 empty pixels) after each rect to avoid artifacts 
        stbrp_rect rectangle = {0, static_cast<stbrp_coord>(width + 2), static_cast<stbrp_coord>(height + 2)};
        stbrp_pack_rects(&m_data->packers[m_packerIndex].context, &rectangle, 1);
        rectangle.w -= 1;
        rectangle.h -= 1;

        if (rectangle.was_packed)
        {
            unsigned char* pixelData = stbtt_GetCodepointBitmap(&m_data->fontInfo, scale, scale, codepoint, &width, &height, 0, 0);
            const glm::uvec2& orig = m_data->packers[m_packerIndex].origin;

            // Pass pixel data to texture
            if (pixelData)
            {
                m_texture.setPixels(glm::uvec2(rectangle.x, rectangle.y) + orig, glm::uvec2(width, height), pixelData);
                stbtt_FreeBitmap(pixelData, nullptr);
            }

            // Create new glyph
            jop::Glyph glyph;
            glyph.advance = static_cast<int>(advance * scale);
            glyph.bounds = Rect{static_cast<int>(left * scale), static_cast<int>(right * scale), static_cast<int>(bottom * scale), static_cast<int>(top * scale)};
            glyph.textCoord = Rect{rectangle.x + orig.x, rectangle.x + width + orig.x, rectangle.y + orig.y, rectangle.y + height + orig.y};
            m_glyphs[codepoint] = glyph;

            return true;
        }

        // If texture is full - create new bigger one and copy and replace the old one
        return resizePacker(codepoint);
    }

    //////////////////////////////////////////////

    bool Font::resizePacker(const uint32 lastCodepoint) const
    {
        if (m_packerIndex > 0 || m_data->packers.size() < 2)
        {
            const unsigned int oldSize = m_texture.getSize().x;

            if (Texture::getMaximumSize() < oldSize * 2)
                return false;

            {
                // Clear old packers
                m_data->packers.clear();
                // Resize
                m_data->packers.resize(2);

                auto& first = m_data->packers[0];
                // Adjust nodes size
                first.nodes.resize(oldSize * 2);
                // Set init target
                stbrp_init_target(&first.context, oldSize * 2, oldSize, first.nodes.data(), first.nodes.size());
                // Adjust origin
                first.origin.y = oldSize;
                m_packerIndex = 0;
            }
            {
                auto& second = m_data->packers[1];
                // Adjust nodes size
                second.nodes.resize(oldSize);
                // Set init target
                stbrp_init_target(&second.context, oldSize, oldSize, second.nodes.data(), second.nodes.size());
                // Adjust origin
                second.origin.x = oldSize;
            }

            // Create image from old texture
            Image image = m_texture.getImage();
            // Get size and increase it
            glm::uvec2 size(image.getSize() * 2u);
            // Load texture with new size
            m_texture.load(size, 1, false);
            // Copy images pixels to new texture
            m_texture.setPixels(glm::uvec2(0, 0), size / 2u, image.getPixels());
        }

        ++m_packerIndex;

        // Pack the last glyph that did not fit into the old texture
        return packGlyph(lastCodepoint);
    }
}