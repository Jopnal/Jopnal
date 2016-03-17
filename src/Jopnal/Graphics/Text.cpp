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
    Text::Text(Object& object, const std::string& ID)
        :GenericDrawable(object, ID),
        Mesh(ID),
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
        std::vector<Vertex> vertices;

        float x = 0;

        for (auto i : string)
        {
            if (i == ' ')
            {
                x += 0.0275;
            }
            else
            {
                //init ints
                int bitmapX = 0;
                int bitmapY = 0;

                int bitmapWidth = 0;
                int bitmapHeight = 0;

                glm::vec2 glyphOrigin;

                float kerning = 0.01;

                //get bitmap location and size inside the texture in pixels
                m_font->getTextureCoordinates(i, &bitmapWidth, &bitmapHeight, &bitmapX, &bitmapY);

                Texture& tex = m_font->getTexture();

                glm::vec2 glyphPos;
                glyphPos.x = (float)bitmapX / (float)tex.getWidth();
                glyphPos.y = (float)bitmapY / (float)tex.getHeight();

                glm::vec2 glyphSize;
                glyphSize.x = (float)bitmapWidth / (float)tex.getWidth();
                glyphSize.y = (float)bitmapHeight / (float)tex.getHeight();

                std::pair<glm::ivec2, glm::ivec2> metrics = m_font->getBounds(i);

                Vertex v;
                v.position.x = (x + metrics.first.x) * m_size.x;
                v.position.y = ((float)metrics.first.y) * m_size.y;
                v.position.z = 0;
                v.texCoords.x = glyphPos.x;
                v.texCoords.y = glyphPos.y;
                vertices.push_back(v);

                v.position.y = ((float)metrics.first.y - glyphSize.y) * m_size.y;
                v.texCoords.y = glyphPos.y + glyphSize.y;
                vertices.push_back(v);

                v.position.x = (x + metrics.first.x + glyphSize.x) * m_size.x;
                v.texCoords.x = glyphPos.x + glyphSize.x;
                vertices.push_back(v);
                vertices.push_back(v);

                v.position.y = ((float)metrics.first.y) * m_size.y;
                v.texCoords.y = glyphPos.y;
                vertices.push_back(v);

                v.position.x = (x + metrics.first.x) * m_size.x;
                v.position.y = ((float)metrics.first.y) * m_size.y;
                v.texCoords.x = glyphPos.x;
                v.texCoords.y = glyphPos.y;
                vertices.push_back(v);

                //advance!
                x += (float)bitmapWidth / (float)tex.getWidth() + kerning;
            }
        }

        Mesh::load(vertices, std::vector<unsigned int>());
        m_material.setMap(Material::Map::Diffuse, m_font->getTexture());
        GenericDrawable::setModel(Model(*this, m_material));
    }

    void Text::setFont(Font& font)
    {
        m_font = static_ref_cast<Font>(font.getReference());
    }

    //void Text::draw(const Camera& cam, const LightContainer& light)
    //{
    //    Texture tex = m_font->getTexture();
    //    tex.bind();

    //}
}

