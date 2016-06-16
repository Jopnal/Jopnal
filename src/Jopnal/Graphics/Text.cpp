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
          m_bounds          ({ 0, 0, 0, 0 })
    {
        GenericDrawable::setModel(Model(m_mesh, m_material));
    }

    Text::Text(const Text& other, Object& newObj)
        : GenericDrawable   (other, newObj),
          m_font            (other.m_font),
          m_mesh            (""),
          m_material        (other.m_material, ""),
          m_string          (),
          m_bounds          ({ 0, 0, 0, 0 })
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
        std::vector<Vertex> vertices;

        for (auto i : m_string)
        {
            if (i == L' ')
            {

                x += m_font->getFontSize() / 2;
                previous = -1;
                continue;
            }
            else if (i == L'\n')
            {
                
                x = 0; // Move to start of the bitmap
          /*      if (m_font->getLineSpacing() <= 0)
                    y -= m_font->getFontSize();
                else
                    y -= m_font->getLineSpacing();*/

                y -= m_font->getFontSize();
                continue;
            }

            float kerning = 0.01f;

            if (previous != -1)
            {
                // Kerning
                kerning = m_font->getKerning(previous, i);
            }
            previous = i;
          
            // Get font texture
            const Texture2D& tex = m_font->getTexture();
            float texWidth = static_cast<float>(tex.getSize().x);
            float texHeight = static_cast<float>(tex.getSize().y);

            // Kerning advancement
            x += (kerning / texWidth) * 8; // Multiply for greater effect
            //x += kerning;

            // Get glyph
            const jop::Glyph& glyph = m_font->getGlyph(i);

            // Calculate vertex positions
            
            // Top left
            Vertex v;
            v.position.x = (x + glyph.bounds.left);
            v.position.y = (y + glyph.bounds.top);
            v.position.z = 0;
            v.texCoords.x = glyph.textCoord.left / texWidth;
            v.texCoords.y = glyph.textCoord.top / texHeight;
            vertices.push_back(v);

            // Bottom left
            v.position.y = (y + glyph.bounds.bottom);
            v.texCoords.y = glyph.textCoord.bottom / texHeight;
            vertices.push_back(v);

            // Bottom right
            v.position.x = (x + glyph.bounds.right);
            v.texCoords.x = glyph.textCoord.right / texWidth;
            // NOTE: push_back twice since the 2 drawn triangles share this point
            vertices.push_back(v);
            vertices.push_back(v);

            // Top right
            v.position.y = (y + glyph.bounds.top);
            v.texCoords.y = glyph.textCoord.top / texHeight;
            vertices.push_back(v);

            // Top left
            v.position.x = (x + glyph.bounds.left);
            v.texCoords.x = glyph.textCoord.left / texWidth;
            vertices.push_back(v);
            
            // Advance
            x += glyph.advance;
        }

        // Load vertices to mesh and set material
        m_mesh.load(vertices, std::vector<unsigned int>());

        return *this;
    }

    //////////////////////////////////////////////

    const std::wstring& Text::getString() const
    {
        return m_string;
    }

    //////////////////////////////////////////////

    const std::pair<glm::vec2, glm::vec2> Text::getBounds() const
    {
        return std::pair<glm::vec2, glm::vec2>(glm::vec2(m_bounds.left, m_bounds.right), glm::vec2(m_bounds.bottom, m_bounds.top));
    }

    //////////////////////////////////////////////

    glm::vec2 Text::getCharacterPosition(const int codepoint)
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

    Text& Text::setColor(const Color color)
    {
        m_material.setMap(Material::Map::Opacity, m_font->getTexture());
        m_material.setReflection(Material::Reflection::Solid, color);

        return *this;
    }

    //////////////////////////////////////////////

    jop::Text& Text::setStyle()
    {
        // TODO:

        //     stbtt_FindMatchingFont() will use *case-sensitive* comparisons on
        //             unicode-encoded names to try to find the font you want;
        //             you can run this before calling stbtt_InitFont()
        //
        //     stbtt_GetFontNameString() lets you get any of the various strings
        //             from the file yourself and do your own comparisons on them.
        //             You have to have called stbtt_InitFont() first.

        /*
        stbtt_FindMatchingFont(const unsigned char *fontdata, const char *name, int flags);
        // returns the offset (not index) of the font that matches, or -1 if none
        //   if you use STBTT_MACSTYLE_DONTCARE, use a font name like "Arial Bold".
        //   if you use any other flag, use a font name like "Arial"; this checks
        //     the 'macStyle' header field; i don't know if fonts set this consistently
        #define STBTT_MACSTYLE_DONTCARE     0
        #define STBTT_MACSTYLE_BOLD         1
        #define STBTT_MACSTYLE_ITALIC       2
        #define STBTT_MACSTYLE_UNDERSCORE   4
        #define STBTT_MACSTYLE_NONE         8   // <= not same as 0, this makes us check the bitfield is 0
        */
        return *this;
    }

    //////////////////////////////////////////////

    Color Text::getColor() const
    {
        return m_material.getReflection(Material::Reflection::Solid);
    }

    //////////////////////////////////////////////

    void Text::draw(const Camera* camera, const LightContainer& lights, Shader& shader)const
    {
        GlState::setFaceCull(false);
        GenericDrawable::draw(camera, lights, shader);
        GlState::setFaceCull(true);
    }
}