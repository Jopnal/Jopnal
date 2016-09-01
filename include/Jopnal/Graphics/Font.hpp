// Jopnal Engine C++ Library
// Copyright (c) 2016 Team Jopnal
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgement in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

//////////////////////////////////////////////

#ifndef JOP_FONT_HPP
#define JOP_FONT_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Glyph.hpp>
#include <Jopnal/Core/Resource.hpp>
#include <Jopnal/Graphics/Texture/Texture2D.hpp>
#include <memory>
#include <unordered_map>

//////////////////////////////////////////////


namespace jop
{
    namespace detail
    {
        struct FontImpl;
    }

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


        /// \brief Load from file
        ///
        /// \param path Path to desired .ttf font file
        /// \param fontSize Glyph size in texture
        ///
        /// \return True if successful
        ///
        bool load(const std::string& path, const int fontSize);

        /// \brief Loads a font from memory
        ///
        /// The memory will be copied.
        ///
        /// \param ptr Pointer to memory
        /// \param size Amount of bytes to read
        /// \param fontSize Glyph size in texture
        ///
        /// \return True if successful
        ///
        bool load(const void* ptr, const uint32 size, const int fontSize);

        /// \brief Returns the necessary kerning advancement between two characters
        ///
        /// \param left Code point value pointing to the desired character
        /// \param right Code point value pointing to the next character
        ///
        /// \warning Every font does NOT have support for this!
        ///
        /// \return True if successful
        ///
        float getKerning(const uint32 left, const uint32 right) const;

        /// \brief Returns a single glyph from given codepoint
        ///
        /// If glyph with the given codepoint does not exist yet calls packGlyph() to create and pack it.
        ///
        /// \param codepoint Unicode codepoint of a character
        ///
        /// \return Reference to the glyph
        ///
        const Glyph& getGlyph(const uint32 codepoint) const;

        /// \brief Return the spacing between two rows in the font
        ///
        /// \return The line spacing
        ///
        float getLineSpacing() const;

        /// \brief Return the texture that contains all loaded glyphs
        ///
        /// \return Reference to the font texture
        ///
        const Texture2D& getTexture() const;

        /// \brief Returns pixel size of the font
        ///
        /// \return The font pixel size
        ///
        int getSize() const;

        /// \brief Get the default font
        ///
        /// \return Reference to the font
        ///
        static Font& getDefault();

    private:

        /// \brief Loads a font from internal buffer
        ///
        /// \param pixelSize Glyph size in texture
        ///
        /// \return True if successful
        ///
        bool load(const int fontSize);

        /// \brief Pack and create a glyph
        ///
        /// \param codepoint Unicode codepoint
        ///
        /// Packs a glyph in to a texture and checks if there is room in the texture
        /// if there is no room resizePacker() is called
        ///
        /// \return True if successful
        ///
        bool packGlyph(const uint32 codepoint) const;

        /// \brief Resizes and remakes the packed texture
        ///
        /// \param lastCodepoint The last glyphs codepoint that did not fit into the old packer
        ///
        /// When a packer runs out of space this function gets called
        /// Creates two new packers and creates new bigger texture and copy the old texture on it
        ///
        bool resizePacker(const uint32 lastCodepoint) const;


        mutable Texture2D m_texture;                        ///< Texture
        mutable std::unordered_map<int, Glyph> m_glyphs;    ///< Texture coordinates
        std::vector<uint8> m_buffer;                        ///< File buffer
        std::unique_ptr<detail::FontImpl> m_data;           ///< Font data
        int m_fontSize;                                     ///< Font size
        mutable unsigned int m_packerIndex;                 ///< Current packer index
    };
}

/// \class jop::Font
/// \ingroup graphics
///
/// Font manager class, which loads fonts from file and packs them to textures.
/// Supports .ttf format.
///

#endif