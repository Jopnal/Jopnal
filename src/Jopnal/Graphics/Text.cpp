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

//////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_LOADABLE(jop, Text)[](Object& obj, const Scene& scene, const json::Value& val)
    {
        auto& text = obj.createComponent<Text>(scene.getRenderer());

        const char* const strField = "string";
        if (val.HasMember(strField) && val[strField].IsString())
            text.setString(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(val[strField].GetString()));

        const char* const colField = "color";
        if (val.HasMember(colField) && val[colField].IsUint())
            text.setColor(Color(val[colField].GetUint()));

        return Drawable::loadStateBase(text, scene, val);
    }
    JOP_END_LOADABLE_REGISTRATION(Text)

    JOP_REGISTER_SAVEABLE(jop, Text)[](const Component& comp, json::Value& val, json::Value::AllocatorType& alloc)
    {
        auto& text = static_cast<const Text&>(comp);

        val.AddMember(json::StringRef("string"), json::Value(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(text.getString()).c_str(), alloc), alloc);
        val.AddMember(json::StringRef("color"), text.getColor().asInteger(), alloc);

        return Drawable::saveStateBase(text, val, alloc);
    }
    JOP_END_SAVEABLE_REGISTRATION(Text)
}

namespace jop
{
    Text::Text(Object& object, Renderer& renderer)
        : GenericDrawable   (object, renderer),
          m_font            (static_ref_cast<Font>(Font::getDefault().getReference())),
          m_mesh            (""),
          m_material        ("", Material::Attribute::OpacityMap, false),
          m_string          (),
          m_bounds          ({ 0, 0, 0, 0 }),
          m_style           (jop::Text::Default),
          m_geometryNeedsUpdate(false)
    {
        GenericDrawable::setModel(Model(m_mesh, m_material));
    }

    Text::Text(const Text& other, Object& newObj)
        : GenericDrawable   (other, newObj),
          m_font            (other.m_font),
          m_mesh            (""),
          m_material        (other.m_material, ""),
          m_string          (),
          m_bounds          ({ 0, 0, 0, 0 }),
          m_style           (jop::Text::Default),
          m_geometryNeedsUpdate(false)
    {
        GenericDrawable::setModel(Model(m_mesh, m_material));
        setString(other.m_string);
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

    std::pair<glm::vec2, glm::vec2> Text::getBounds() const
    {
        return std::pair<glm::vec2, glm::vec2>(glm::vec2(m_bounds.left, m_bounds.right), glm::vec2(m_bounds.bottom, m_bounds.top));
    }

    //////////////////////////////////////////////

    glm::vec2 Text::getCharacterPosition(uint32 codepoint)
    {
        if (!m_font)
            return glm::vec2();

        float vspace = m_font->getLineSpacing();

        glm::vec2 position;
        uint32 prevChar = 0;

        for (auto i : m_string)
        {
            uint32 curChar = m_string[i];

            // Apply kerning offset
            position.x += m_font->getKerning(prevChar, curChar);
            prevChar = curChar;
            
            // Handle special characters
            if (i == L' ')
                position.x += m_font->getGlyph(i).advance;
            else if (i == L'\t')
                position.x += m_font->getGlyph(i).advance * 4;
            else if (i == L'\n')
                position.y += vspace; position.x = 0;
            
            // For regular characters, add the advance
            position.x += m_font->getGlyph(curChar).advance;
        }

        return position;     
    }

    //////////////////////////////////////////////

    Text& Text::setFont(const Font& font)
    {
        m_font = static_ref_cast<Font>(font.getReference());
        m_material.setMap(Material::Map::Opacity, m_font->getTexture());
        m_geometryNeedsUpdate = true;

        m_lastFontSize = font.getTexture().getSize().x;

        return setString(m_string);
    }

    //////////////////////////////////////////////

    const Font& Text::getFont() const
    {
        if (m_font.expired())
            return jop::Font::getDefault();

        return m_font;
    }

    //////////////////////////////////////////////

    Text& Text::setColor(const Color color)
    {
        m_material.setMap(Material::Map::Opacity, m_font->getTexture());
        m_material.setReflection(Material::Reflection::Solid, color);

        return *this;
    }

    //////////////////////////////////////////////

    Text& Text::setStyle(uint32 style)
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

    const Color Text::getColor() const
    {
        return m_material.getReflection(Material::Reflection::Solid);
    }

    //////////////////////////////////////////////

    void Text::updateGeometry() const
    {
        // If geometry has not changed - return
        if (!m_geometryNeedsUpdate)
            return;

        // Mark geometry as updated.
        m_geometryNeedsUpdate = false;

        // Clear previous geometry
        vertices.clear();
        m_bounds = { 0, 0, 0, 0 };
        
        // Initialize variables
        float x = 0, y = 0;
        float kerning = 0.f;
        float underlineOffset = 0;
        float strikethroughOffset = 0;
        float thickness = 0;
        int previous = -1;

        // Get font texture & calculate dimensions
        const Texture2D& tex = m_font->getTexture();
        float texWidth = static_cast<float>(tex.getSize().x);
        float texHeight = static_cast<float>(tex.getSize().y);

        for (auto i : m_string)
        {
            // Get glyph
            const jop::Glyph& glyph = m_font->getGlyph(i);

            // Compute values related to text style
            thickness = m_font->getSize() * 0.04;
            underlineOffset = -m_font->getSize() * 0.10f * ((m_style&Style::Underlined) != 0);
            strikethroughOffset = (glyph.bounds.bottom + glyph.bounds.top) / 2.f / ((m_style&Style::Strikethrough) != 0);
            float italic = 0;
            italic = 0.208*m_font->getSize() * ((m_style&Style::Italic) != 0); //(0.208 = 12 degrees)

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
                if ((m_style&Style::Underlined) != 0)
                    addLine(vertices, x, y, underlineOffset + m_font->getLineSpacing(), thickness);
                if ((m_style&Style::Strikethrough) != 0)
                    addLine(vertices, x, y, strikethroughOffset + m_font->getLineSpacing(), thickness);
                previous = -1;
                x = 0; // Move to start on x-axis
                continue;
            }

            if (previous != -1)
                kerning = m_font->getKerning(previous, i); // Calculate kerning if previous character was not special
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
        if ((m_style&Style::Underlined) != 0)
            addLine(vertices, x, y, underlineOffset, thickness);
        if ((m_style&Style::Strikethrough) != 0)
            addLine(vertices, x, y, strikethroughOffset, thickness);

        // Load vertices to mesh
        m_mesh.load(vertices, std::vector<unsigned int>());
    }

    //////////////////////////////////////////////

    void Text::addLine(std::vector<Vertex>& vertices, float lineLenght, float lineTop, float offset, float thickness) const
    {
        float top = std::floor(lineTop + offset - (thickness / 2) + 0.5);
        float bottom = top + std::floor(thickness + 0.5f);

        float left = 0;
        float right = lineLenght;

        Vertex v; 
        // Top left
        v.position.x = left;
        v.position.y = top;
        v.position.z = 0.1;
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

    void Text::draw(const Camera* camera, const LightContainer& lights, Shader& shader) const
    {
        while (m_font->getTexture().getSize().x != m_lastFontSize)
        {
            // If size has changed
            m_geometryNeedsUpdate = true;
            m_lastFontSize = m_font->getTexture().getSize().x;
            updateGeometry();
        }
        updateGeometry(); // Update geometry before drawing if necessary

        GlState::setFaceCull(false);
        GenericDrawable::draw(camera, lights, shader);
        GlState::setFaceCull(true);
    }
}