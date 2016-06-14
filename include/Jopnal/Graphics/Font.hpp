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

#ifndef JOP_Font_HPP
#define JOP_Font_HPP

// Headers

#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Resource.hpp>
#include <Jopnal/Graphics/Texture2D.hpp>
#include <Jopnal/MathInclude.hpp>
#include <memory>
#include <unordered_map>
#include <Jopnal/Graphics/Glyph.hpp>

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
        /// \param fontSize Glyph size in texture
        ///
        bool load(const std::string& path, const int fontSize);

        /// \brief Returns the necessary kerning correction between characters
        ///
        /// \param left Numerical value pointing to the desired character
        /// \param right Numerical value pointing to the next character
        ///
        float getKerning(const uint32 left,const uint32 right) const;

        /// \brief Returns a single glyph from given codepoint
        ///
        /// \param Unicode codepoint of a character
        ///
        const jop::Glyph& getGlyph(uint32 codepoint);

        /// \brief Returns the distance between two rows
        ///
        float getLineSpacing() const;

        /// \brief Returns the texture that contains all loaded glyphs
        ///
        const Texture2D& getTexture() const;

        /// \brief Returns pixel size 
        ///
        float getFontSize() const;


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

        std::unique_ptr<::detail::FontImpl> m_data;
        jop::Texture2D m_texture;  ///< Texture
        std::vector<unsigned char> m_buffer;        ///< File buffer
        std::unordered_map <int, jop::Glyph> m_bitmaps; ///< Texture coordinates
        int m_textureSize; ///< 256x256 by default
        int m_numNodes;
        int m_fontSize;

    };
}

#endif

/// \class Font
/// \ingroup graphics
///
/// Font manager class, which loads fonts from file.
/// 
///