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
    Text::Text(Object& object, Renderer& renderer)
        :GenericDrawable(object, renderer),
        Mesh(""),
        m_material("", Material::Attribute::Diffusemap),
        m_size(10, 10)
    {

    }

    void Text::setPosition(const glm::vec2 position)
    {
        m_position = position;
    }

    void Text::setString(const std::string &string)
    {
        m_string = string;

        std::vector<Vertex> vertices;

        float x = 0, y = 0;
        int previous = -1;

        for (auto i : string)
        {
            if (i == ' ')
            {
                x += (1.0 / 64.0);
                previous = -1;
                continue;
            }
            else if (i == '\n')
            {
                x = 1.0 / 256.0;
                y -= 1.0 / 16.0;
                continue;
            }
            //init ints
            int bitmapX = 0;
            int bitmapY = 0;

            int bitmapWidth = 0;
            int bitmapHeight = 0;

            glm::vec2 glyphOrigin;
            float kerning = 0.01;

            if (previous != -1)
            {
                kerning = 1.0 / 256.0;//m_font->getKerning(previous, i);
            }
            previous = i;

            //get bitmap location and size inside the texture in pixels
            m_font->getTextureCoordinates(i, &bitmapWidth, &bitmapHeight, &bitmapX, &bitmapY);

            Texture2D& tex = m_font->getTexture();

            std::pair<glm::vec2, glm::vec2> metrics = m_font->getBounds(i);

            metrics.first.x /= (32 * tex.getWidth());
            metrics.first.y /= (32 * tex.getHeight());
            metrics.second.x /= (64 * tex.getWidth());
            metrics.second.y /= (64 * tex.getHeight());


            glm::vec2 glyphPos;
            glyphPos.x = (float)bitmapX / (float)tex.getWidth();
            glyphPos.y = (float)bitmapY / (float)tex.getHeight();

            glm::vec2 glyphSize;
            glyphSize.x = (float)bitmapWidth / (float)tex.getWidth();
            glyphSize.y = (float)bitmapHeight / (float)tex.getHeight();

            x += kerning;

            Vertex v;
            v.position.x = (x + metrics.first.x) * m_size.x;
            v.position.y = (y + metrics.first.y) * m_size.y;
            v.position.z = 0;
            v.texCoords.x = glyphPos.x;
            v.texCoords.y = glyphPos.y;
            vertices.push_back(v);

            v.position.y = (y + metrics.first.y - glyphSize.y) * m_size.y;
            v.texCoords.y = glyphPos.y + glyphSize.y;
            vertices.push_back(v);

            v.position.x = (x + metrics.first.x + static_cast<float>(bitmapWidth) / tex.getWidth()) * m_size.x;
            v.texCoords.x = glyphPos.x + glyphSize.x;
            vertices.push_back(v);
            vertices.push_back(v);

            v.position.y = (y + metrics.first.y) * m_size.y;
            v.texCoords.y = glyphPos.y;
            vertices.push_back(v);

            v.position.x = (x + metrics.first.x) * m_size.x;
            v.position.y = (y + metrics.first.y) * m_size.y;
            v.texCoords.x = glyphPos.x;
            v.texCoords.y = glyphPos.y;
            vertices.push_back(v);

            //advance
            x += (float)bitmapWidth / (float)tex.getWidth();
        }

        Mesh::load(vertices, std::vector<unsigned int>());
        m_material.setMap(Material::Map::Opacity, m_font->getTexture());
        m_material.setAttributeField(Material::Attribute::OpacityMap);
        m_material.setReflection(Material::Reflection::Solid, Color::Orange);
        GenericDrawable::setModel(Model(*this, m_material));
    }

    void Text::setFont(Font& font)
    {
        m_font = static_ref_cast<Font>(font.getReference());
        setString(m_string);
    }

    void Text::setScale(const float x, const float y)
    {
        setScale(glm::vec2(x, y));
    }

    void Text::setScale(glm::vec2 scale)
    {
        m_size = scale;
        setString(m_string);
    }
}

