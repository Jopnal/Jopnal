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
        :Drawable(object, ID),
        Mesh(ID)
    {

    }
    void Text::setString(const std::string &string)
    {
        std::vector<Vertex> vertices;

        float x;

        for (auto i : string)
        {
            //init ints
            int bitmapX = 0;
            int bitmapY = 0;

            int bitmapWidth = 0;
            int bitmapHeight = 0;

            glm::vec2 glyphOrigin;
            float kerning;

            //get bitmap location and size inside the texture in pixels
            m_font->getCodepointBitmap(i, &bitmapWidth, &bitmapHeight, &bitmapX, &bitmapY);

            //pixels to texture coordinates
            Texture tex = m_font->getTexture();

            glm::vec2 glyphPos;
            glyphPos.x = (float)bitmapX / (float)tex.getWidth();
            glyphPos.y = (float)bitmapY / (float)tex.getHeight();

            //advance!
            x += (float)bitmapWidth / (float)tex.getWidth() + kerning;
        }

        Mesh::load(vertices, std::vector<unsigned int>());
    }

}

