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
    Glyph::Glyph() : 
        m_advance(0),
        m_left(0),
        m_right(0),
        m_bottom(0),
        m_top(0)
    {}

    Glyph::Glyph(float advance, float left, float right, float bottom, float top)
        : m_advance(advance),
        m_left(left),
        m_right(right),
        m_bottom(bottom),
        m_top(top)
    {}

    Glyph::~Glyph()
    {}

    //////////////////////////////////////////////

    const float Glyph::getAdvance() const
    {
        return m_advance;
    }

    //////////////////////////////////////////////

    std::pair<glm::vec2, glm::vec2> Glyph::getBounds()
    {
        return std::make_pair(glm::vec2(m_left, m_right), glm::vec2(m_bottom, m_top));
    }
}