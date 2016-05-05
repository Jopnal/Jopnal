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
          m_font            (static_ref_cast<const Font>(Font::getDefault().getReference())),
          m_mesh            (""),
          m_material        ("", Material::Attribute::OpacityMap, false),
          m_string          ()
    {
        GenericDrawable::setModel(Model(m_mesh, m_material));
    }

    Text::Text(const Text& other, Object& newObj)
        : GenericDrawable   (other, newObj),
          m_font            (other.m_font),
          m_mesh            (""),
          m_material        (other.m_material, ""),
          m_string          ()
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

        std::vector<Vertex> vertices;

        float x = 0, y = 0;
        int previous = -1;

        for (auto i : m_string)
        {
            if (i == L' ')
            {
                x += (1.0 / 128.0);
                previous = -1;
                continue;
            }
            else if (i == L'\n')
            {
                x = 1.0f / 256.0f;
                y -= 1.0f / 24.0f;
                continue;
            }

            // Init variables
            int bitmapX = 0;
            int bitmapY = 0;

            int bitmapWidth = 0;
            int bitmapHeight = 0;

            float kerning = 0.01f;

            if (previous != -1)
            {
                kerning = 1.0f / 256.0f;

                // TODO: Kerning not working... fix later if there's time
                //m_font->getKerning(previous, i);
            }
            previous = i;

            // Get bitmap location and size inside the texture in pixels
            m_font->getTextureCoordinates(i, &bitmapWidth, &bitmapHeight, &bitmapX, &bitmapY);

            // Get font texture
            const Texture2D& tex = m_font->getTexture();

            // Get font origin
            std::pair<glm::vec2, glm::vec2> metrics = m_font->getBounds(i);

            float texWidth = static_cast<float>(tex.getSize().x);
            float texHeight = static_cast<float>(tex.getSize().y);

            metrics.first.x /= (32.f * (64.f / m_font->getPixelSize()) * texWidth);
            metrics.first.y /= (32.f * (64.f / m_font->getPixelSize()) * texHeight);

            // Calculate coordinates
            glm::vec2 glyphPos;
            glyphPos.x = static_cast<float>(bitmapX) / texWidth;
            glyphPos.y = static_cast<float>(bitmapY) / texHeight;

            glm::vec2 glyphSize;
            glyphSize.x = static_cast<float>(bitmapWidth) / texWidth;
            glyphSize.y = static_cast<float>(bitmapHeight) / texHeight;

            x += kerning;

            // Calculate vertex positions
            // Top left
            Vertex v;
            v.position.x = (x + metrics.first.x);
            v.position.y = (y + metrics.first.y);
            v.position.z = 0;
            v.texCoords.x = glyphPos.x;
            v.texCoords.y = glyphPos.y;
            vertices.push_back(v);

            // Bottom left
            v.position.y = (y + metrics.first.y - glyphSize.y);
            v.texCoords.y = glyphPos.y + glyphSize.y;
            vertices.push_back(v);

            // Bottom right
            v.position.x = (x + metrics.first.x + static_cast<float>(bitmapWidth) / texWidth);
            v.texCoords.x = glyphPos.x + glyphSize.x;
            vertices.push_back(v);
            vertices.push_back(v);

            // Top right
            v.position.y = (y + metrics.first.y);
            v.texCoords.y = glyphPos.y;
            vertices.push_back(v);

            // Top left
            v.position.x = (x + metrics.first.x);
            v.position.y = (y + metrics.first.y);
            v.texCoords.x = glyphPos.x;
            v.texCoords.y = glyphPos.y;
            vertices.push_back(v);

            // Advance
            x += static_cast<float>(bitmapWidth) / texWidth;
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

    Text& Text::setFont(const Font& font)
    {
        m_font = static_ref_cast<const Font>(font.getReference());
        m_material.setMap(Material::Map::Opacity, m_font->getTexture());

        return setString(m_string);
    }

    //////////////////////////////////////////////

    Text& Text::setColor(const Color& color)
    {
        m_material.setMap(Material::Map::Opacity, m_font->getTexture());
        m_material.setReflection(Material::Reflection::Solid, color);

        return *this;
    }

    //////////////////////////////////////////////

    const Color& Text::getColor() const
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