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
#include <Jopnal/Precompiled/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
    Text::Text(Object& object, Renderer& renderer, const RenderPass::Pass pass)
        : Drawable              (object, renderer, pass),
          m_material            ("", false),
          m_font                (),
          m_string              (),
          m_style               (jop::Text::Default),
          m_lastFontSize        (0),
          m_geometryNeedsUpdate (false),
          m_bounds              ({ 0, 0, 0, 0 }),
          m_mesh                ("")
    {
        m_material.setAttributes(Material::Attribute::OpacityMap);

        setFont(Font::getDefault());
        setModel(Model(m_mesh, m_material));
    }

    Text::Text(Object& object, RenderPass& pass)
        : Drawable              (object, pass),
          m_material            ("", false),
          m_font                (),
          m_string              (),
          m_style               (jop::Text::Default),
          m_lastFontSize        (0),
          m_geometryNeedsUpdate (false),
          m_bounds              ({ 0, 0, 0, 0 }),
          m_mesh                ("")
    {
        m_material.setAttributes(Material::Attribute::OpacityMap);

        setFont(Font::getDefault());
        setModel(Model(m_mesh, m_material));
    }

    Text::Text(const Text& other, Object& newObj)
        : Drawable              (other, newObj),
          m_material            (other.m_material, ""),
          m_font                (other.m_font),
          m_string              (other.m_string),
          m_style               (other.m_style),
          m_lastFontSize        (other.m_lastFontSize),
          m_geometryNeedsUpdate (true),
          m_bounds              (other.m_bounds),
          m_mesh                ("")
    {
        setModel(Model(m_mesh, m_material));
    }

    //////////////////////////////////////////////

    Text& Text::setString(const std::string& string)
    {
        return setString(std::wstring(string.begin(), string.end()));
    }

    //////////////////////////////////////////////

    Text& Text::setString(const std::wstring& string)
    {
        if (m_string != string)
        {
            m_string = string;
            m_geometryNeedsUpdate = true;
        }

        return *this;
    }

    //////////////////////////////////////////////

    const std::wstring& Text::getString() const
    {
        return m_string;
    }

    //////////////////////////////////////////////

    const Rect& Text::getBounds() const
    {
        return m_bounds;
    }

    //////////////////////////////////////////////

    glm::vec2 Text::getCharacterPosition(const uint32 index)
    {
        if (m_font.expired())
            return glm::vec2();

        const float vspace = m_font->getLineSpacing();

        glm::vec2 position(0);
        uint32 prevChar = 0;

        for (size_t i = 0; i < index; ++i)
        {
            uint32 curChar = m_string[i];

            // Apply kerning offset
            position.x += m_font->getKerning(prevChar, curChar);
            prevChar = curChar;
            
            // Handle special characters
            if (curChar == L' ')
                position.x += m_font->getGlyph(curChar).advance;
            else if (curChar == L'\t')
            {
                curChar = L' ';
                position.x += m_font->getGlyph(curChar).advance * 4;
            }
            else if (curChar == L'\n')
                position.y += vspace; position.x = 0;
            
            // For regular characters, add the advance
            position.x += m_font->getGlyph(curChar).advance;
        }

        return position;     
    }

    //////////////////////////////////////////////

    Text& Text::setFont(const Font& font)
    {
        if (m_font.get() != &font)
        {
            m_font = static_ref_cast<const Font>(font.getReference());
            m_material.setMap(Material::Map::Opacity, m_font->getTexture());

            m_geometryNeedsUpdate = true;

            m_lastFontSize = font.getTexture().getSize().x;
        }

        return *this;
    }

    //////////////////////////////////////////////

    const Font& Text::getFont() const
    {
        return m_font.expired() ? Font::getDefault() : *m_font;
    }

    //////////////////////////////////////////////

    Text& Text::setStyle(const uint32 style)
    {
        if (m_style != style)
        {
            m_style = style;
            m_geometryNeedsUpdate = true;
        }

        return *this;
    }

    //////////////////////////////////////////////

    uint32 Text::getStyle() const
    {
        return m_style;
    }

    //////////////////////////////////////////////

    void Text::updateGeometry() const
    {
        // If geometry has not changed - return
        if (!m_geometryNeedsUpdate)
            return;

        // Mark geometry as updated.
        m_geometryNeedsUpdate = false;

        m_bounds = { 0, 0, 0, 0 };

        std::vector<Vertex> vertices;
        vertices.reserve(m_string.size() * 6);

        // Initialize variables
        float x = 0, y = 0, strikethroughOffset = 0;
        int previous = -1;

        // Compute values related to text style
        const float underlineOffset = -m_font->getSize() * 0.1f * ((m_style & Style::Underlined) != 0);
        const float thickness = m_font->getSize() * 0.04f;
        const float italic = 0.208f * m_font->getSize() * ((m_style & Style::Italic) != 0); //(0.208 = 12 degrees)
        
        // Get font texture & calculate dimensions
        const Texture2D& tex = m_font->getTexture();
        const float texWidth = static_cast<float>(tex.getSize().x);
        const float texHeight = static_cast<float>(tex.getSize().y);

        for (auto i : m_string)
        {
            // Get glyph
            const jop::Glyph& glyph = m_font->getGlyph(i);

            strikethroughOffset = (glyph.bounds.bottom + glyph.bounds.top) * 0.5f * ((m_style&Style::Strikethrough) != 0);

            // Handle special characters
            if (i == L' ')
            {
                x += m_font->getGlyph(i).advance;
                previous = -1;
                continue;
            }
            else if (i == L'\t')
            {
                i = L' ';
                x += m_font->getGlyph(i).advance * 4; // Add tabulator
                previous = -1;
                continue;
            }
            else if (i == L'\n')
            {
                y -= m_font->getLineSpacing(); // Advance to next row on y-axis

                // Create new underline/strikethrough line
                if ((m_style & Style::Underlined) != 0)
                    addLine(vertices, x, y, underlineOffset + m_font->getLineSpacing(), thickness);
                if ((m_style & Style::Strikethrough) != 0)
                    addLine(vertices, x, y, strikethroughOffset + m_font->getLineSpacing(), thickness);

                previous = -1;
                x = 0; // Move to start on x-axis

                continue;
            }

            const float kerning = previous == -1 ? 0.f : m_font->getKerning(previous, i);
            
            previous = i;

            // Kerning advancement
            x += kerning;

            // Calculate vertex positions:
            // Top left
            Vertex v;
            v.position.x = (x + glyph.bounds.left);
            v.position.y = (y + glyph.bounds.top);
            v.position.z = 0;
            v.texCoords.x = glyph.textCoord.left / texWidth;
            v.texCoords.y = glyph.textCoord.top / texHeight;
            vertices.push_back(v);

            // Bottom left
            v.position.x = (x + glyph.bounds.left + italic);
            v.position.y = (y + glyph.bounds.bottom);
            v.texCoords.y = glyph.textCoord.bottom / texHeight;
            vertices.push_back(v);

            // Bottom right
            v.position.x = (x + glyph.bounds.right + italic);
            v.texCoords.x = glyph.textCoord.right / texWidth;
            // NOTE: push_back twice since the 2 drawn triangles share this vertex
            vertices.push_back(v);
            vertices.push_back(v);

            // Top right
            v.position.x = (x + glyph.bounds.right);
            v.position.y = (y + glyph.bounds.top);
            v.texCoords.y = glyph.textCoord.top / texHeight;
            vertices.push_back(v);

            // Top left
            v.position.x = (x + glyph.bounds.left);
            v.texCoords.x = glyph.textCoord.left / texWidth;
            vertices.push_back(v);

            // Update text bounds
            m_bounds.left = std::min(m_bounds.left, static_cast<int>(x)+glyph.bounds.left);
            m_bounds.top = std::min(m_bounds.top, static_cast<int>(y)+glyph.bounds.top);
            m_bounds.right = std::max(m_bounds.right, static_cast<int>(x)+glyph.bounds.right);
            m_bounds.bottom = std::max(m_bounds.bottom, static_cast<int>(y)+glyph.bounds.bottom);

            // Advance
            x += glyph.advance;
        }

        // Add underline / strikethrough
        if ((m_style & Style::Underlined) != 0)
            addLine(vertices, x, y, underlineOffset, thickness);
        if ((m_style & Style::Strikethrough) != 0)
            addLine(vertices, x, y, strikethroughOffset, thickness);

        // Load vertices to mesh
        m_mesh.load(vertices, std::vector<unsigned int>());
    }

    //////////////////////////////////////////////

    void Text::addLine(std::vector<Vertex>& vertices, const float lineLenght, const float lineTop, const float offset, const float thickness) const
    {
        const float top = std::floor(lineTop + offset - (thickness / 2) + 0.5f);
        const float bottom = top + std::floor(thickness + 0.5f);

        const float left = 0;
        const float right = lineLenght;

        Vertex v; 
        // Top left
        v.position.x = left;
        v.position.y = top;
        v.position.z = 0.1f;
        vertices.push_back(v);

        // Bottom left
        v.position.y = bottom;
        vertices.push_back(v);

        // Bottom right - push_back x2
        v.position.x = right;
        vertices.push_back(v);
        vertices.push_back(v);

        // Top right
        v.position.y = top;
        vertices.push_back(v);

        // Top left
        v.position.x = left;
        vertices.push_back(v);
    }

    //////////////////////////////////////////////

    void Text::draw(const ProjectionInfo& proj, const LightContainer& lights) const
    {
        if (m_font.expired())
            return;
        
        while (m_font->getTexture().getSize().x != m_lastFontSize)
        {
            // If size has changed
            m_geometryNeedsUpdate = true;
            m_lastFontSize = m_font->getTexture().getSize().x;
            updateGeometry();
        }
        updateGeometry(); // Update geometry before drawing if necessary

        GlState::setFaceCull(false);
        Drawable::draw(proj, lights);
        GlState::setFaceCull(true);
    }
}