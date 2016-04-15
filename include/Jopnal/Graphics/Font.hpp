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
#include <Jopnal/Graphics/Texture2D.hpp>
#include <Jopnal/MathInclude.hpp>
#include <memory>
#include <unordered_map>

//////////////////////////////////////////////


namespace detail
{
    struct FontImpl;
}

namespace jop
{
    class JOP_API Font : public Resource
    {
    private:

        JOP_DISALLOW_COPY_MOVE(Font);

    public:

        /// \brief Constructor
        ///
        /// \param name Name of the resource
        ///
        Font(const std::string& name);

        /// \brief Destructor
        ///
        ~Font() override;


        /// \brief Loads a font from targeted path
        ///
        /// \param path Path to desired .ttf font file
        /// \param pixelSize Glyph size in texture
        ///
        bool load(const std::string& path, const int pixelSize);

        /// \brief Calculates font boundaries
        ///
        /// \param codepoint Numerical value pointing to a single character
        ///
        std::pair<glm::vec2, glm::vec2> getBounds(const int codepoint) const;

        /// \brief Returns the necessary kerning between characters
        ///
        /// \param codepoint1 Numerical value pointing to the desired character
        /// \param codepoint2 Ditto
        ///
        float getKerning(const int codepoint1, const int codepoint2) const;

        /// \brief Checks if glyph is in bitmap
        ///
        /// \param codepoint Pointer to a single character
        /// \param width Width of a single character
        /// \param height Height of a single character
        /// \param x The X coordinate
        /// \param y The Y coordinate
        ///
        void getTextureCoordinates(const int codepoint, int* width, int* height, int* x, int* y) const;

        /// \brief Returns the texture that contains all loaded glyphs
        ///
        const Texture2D& getTexture() const;

        /// \brief Returns pixel size 
        ///
        float getPixelSize() const;


        /// \brief Get the error font
        ///
        /// \return Reference to the font
        ///
        static Font& getError();

        /// \brief Get the default font
        ///
        /// \return Reference to the font
        ///
        static Font& getDefault();

    private:

        /// \brief Loads a font from DLL file
        ///
        /// \param id ID
        /// \param pixelSize Glyph size in texture
        ///
        bool load(const int id, const int pixelSize);

        /// \brief Loads a font from internal buffer
        ///
        /// \param pixelSize Glyph size in texture
        ///
        bool load(const int pixelSize);


        std::unique_ptr<::detail::FontImpl> m_data;
        int m_pixelSize;
    };
}

#endif

/// \class Font
/// \ingroup graphics
///
/// Font manager class, which loads fonts from file.
/// 
///