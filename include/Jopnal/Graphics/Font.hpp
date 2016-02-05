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

#ifndef JOP_FONT_HPP
#define JOP_FONT_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Resource.hpp>
#include <../src/Jopnal/Graphics/stb/stb_truetype.h>
#include <Jopnal/MathInclude.hpp>
#include <memory>
#include <unordered_map>

//////////////////////////////////////////////


struct stbtt_fontinfo;

namespace jop
{
    struct Bitmap
    {
        int width, height;
        unsigned char* data;
    };

	class Font
        :public Resource
	{
	public:

        /// \brief Loads a font from targeted path
        ///
        /// \param path Path to desired .ttf font file
        ///
        bool load(const std::string& path);

        /// \brief Calculates font boundaries
        ///
        /// \param codepoint Numerical value pointing to a single character
        ///
        std::pair<glm::ivec2, glm::ivec2> getBounds(const int codepoint);

        /// \brief Returns the necessary kerning between characters
        ///
        /// \param codepoint1 Numerical value pointing to the desired character
        /// \param codepoint2 Ditto
        ///
        int getKerning(const int codepoint1, const int codepoint2);

        /// \brief Checks if glyph is in bitmap
        ///
        /// \param scaleX Horizontal scale of a single character
        /// \param scaleY Vertical scale of a single character
        /// \param codepoint Pointer to a single character
        /// \param width Width of a single character
        /// \param height Height of a single character
        ///
        unsigned char* getCodepointBitmap(const float scaleX, const float scaleY, const int codepoint, int* width, int* height);
        
        /// \brief Deconstructor
        ///
        ///
        ~Font();

	private:

        std::unique_ptr<stbtt_fontinfo> m_info; ///< Font info
        std::unordered_map <int, Bitmap> m_bitmaps; ///< Bitmaps

	};
}

#endif

/// \class Font
/// \ingroup graphics
///
/// Font manager class which loads any desired TrueType font file (.ttf)
/// 
///