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

#ifndef JOP_GLYPH_HPP
#define JOP_GLYPH_HPP

// Headers
#include <Jopnal/Header.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Glyph
    {
    public:

        Glyph(float advance, float left, float right, float bottom, float top);
            
        Glyph();

        ~Glyph();

        const float getAdvance() const;
        std::pair<glm::vec2, glm::vec2> getBounds();

    private:

        float m_advance; ///< Advance to next glyph
        float m_left; ///< Glyphs left border
        float m_right; ///< Glyphs right border
        float m_bottom; ///< Glyphs bottom border
        float m_top; ///< Glyphs top border
    };
}

#endif

/// \class Glyph
/// \ingroup graphics
///
/// Glyph holds data for text rendering
/// 
///