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
          m_style           (jop::Text::Default)
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
          m_style           (jop::Text::Default)
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
        m_string = string;

        if (m_font.expired())
        {
            setFont(Font::getDefault());
            return *this;
        }

        float x = 0, y = 0;
        int previous = -1;
        

        // Get font texture
        const Texture2D& tex = m_font->getTexture();
        float texWidth = static_cast<float>(tex.getSize().x);
        float texHeight = static_cast<float>(tex.getSize().y);

        for (auto i : m_string)
        {
            if (i == L' ')
            {
                x += m_font->getFontSize() / 2; // Add empty space
                previous = -1;
                continue;
            }
            else if (i == L'\n')
            {
                
                x = 0; // Move to start on x-axis
                y -= m_font->getLineSpacing(); // Advance to next row on y-axis
                continue;
            }

            float kerning = 0.01f;

            if (previous != -1)
            {
                // Kerning
                kerning = m_font->getKerning(previous, i);
            }
            previous = i;
          
            // Kerning advancement
            x += kerning;

            // Get glyph
            const jop::Glyph& glyph = m_font->getGlyph(i);
        
            // Check if italic
            float italic = 0;
            italic = 0.208*m_font->getFontSize() * ((m_style&Style::Italic)!= 0);

            // Calculate vertex positions
            // Top left
            Vertex v;
            v.position.x = (x + glyph.bounds.left);
            v.position.y = (y + glyph.bounds.top);
            v.position.z = 0;
            v.texCoords.x = glyph.textCoord.left / texWidth;
            v.texCoords.y = glyph.textCoord.top / texHeight;
            m_vertices.push_back(v);

            // Bottom left
            v.position.x = (x + glyph.bounds.left + italic);
            v.position.y = (y + glyph.bounds.bottom);
            v.texCoords.y = glyph.textCoord.bottom / texHeight;
            m_vertices.push_back(v);

            // Bottom right
            v.position.x = (x + glyph.bounds.right + italic);
            v.texCoords.x = glyph.textCoord.right / texWidth;
            // NOTE: push_back twice since the 2 drawn triangles share this point
            m_vertices.push_back(v);
            m_vertices.push_back(v);

            // Top right
            v.position.x = (x + glyph.bounds.right);
            v.position.y = (y + glyph.bounds.top);
            v.texCoords.y = glyph.textCoord.top / texHeight;
            m_vertices.push_back(v);

            // Top left
            v.position.x = (x + glyph.bounds.left);
            v.texCoords.x = glyph.textCoord.left / texWidth;
            m_vertices.push_back(v);
            
            // Update text bounds
            m_bounds.left = std::min(m_bounds.left, static_cast<int>(x) + glyph.bounds.left);
            m_bounds.top = std::min(m_bounds.top, static_cast<int>(y)+glyph.bounds.top);
            m_bounds.right = std::max(m_bounds.right, static_cast<int>(x)+glyph.bounds.right);
            m_bounds.bottom = std::max(m_bounds.bottom, static_cast<int>(y) + glyph.bounds.bottom);

            addLine(m_vertices, 1, 50);
            // Advance
            x += glyph.advance;
        }

        
        // Load m_vertices to mesh and set material
        m_mesh.load(m_vertices, std::vector<unsigned int>());

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

    glm::vec2 Text::getCharacterPosition(const int codepoint) const
    {
        // TODO: Implement      
        return glm::vec2(0, 0);     
    }

    //////////////////////////////////////////////

    Text& Text::setFont(const Font& font)
    {
        m_font = static_ref_cast<Font>(font.getReference());
        m_material.setMap(Material::Map::Opacity, m_font->getTexture());

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
            //geometryneedsupdate = true
        }

        return *this;
    }

    //////////////////////////////////////////////

    uint32 Text::getStyle() const
    {
        return m_style;
    }

    //////////////////////////////////////////////

    Color Text::getColor() const
    {
        return m_material.getReflection(Material::Reflection::Solid);
    }

    void Text::addLine(std::vector<Vertex>& m_vertices, float offset, float thickness)
    {
        
        float top = m_bounds.bottom + offset;
        float bottom = m_bounds.bottom + offset + thickness;

        Vertex v; 
        // Top left
        v.position.x = m_bounds.left;
        v.position.y = top;
        v.position.z = 0;
        v.texCoords.x = 1;
        v.texCoords.y = 1;
        m_vertices.push_back(v);

        // Bottom left
        v.position.y = bottom;
        //texcoords
        m_vertices.push_back(v);

        // Bottom right - push_back x2
        v.position.x = m_bounds.left;
        //texcoords
        m_vertices.push_back(v);
        m_vertices.push_back(v);

        // Top right
        v.position.y = top;
        //texcoords
        m_vertices.push_back(v);

        // Top left
        v.position.x = m_bounds.left;
        //v.texCoords.x
        //v.texCoords.y
        m_vertices.push_back(v);



    }

    //////////////////////////////////////////////

    void Text::draw(const Camera* camera, const LightContainer& lights, Shader& shader)const
    {
        GlState::setFaceCull(false);
        GenericDrawable::draw(camera, lights, shader);
        GlState::setFaceCull(true);
    }
}