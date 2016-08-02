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

#ifndef JOP_TEXT_HPP
#define JOP_TEXT_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Glyph.hpp>
#include <Jopnal/Graphics/Drawable.hpp>
#include <Jopnal/Graphics/Mesh/Mesh.hpp>
#include <Jopnal/Graphics/Material.hpp>
#include <Jopnal/Graphics/Color.hpp>

//////////////////////////////////////////////


namespace jop
{
    class Font;

    class JOP_API Text : public Drawable
    {
    private:

        JOP_GENERIC_COMPONENT_CLONE(Text)

    public:

        enum Style : uint32
        {
            Default         = 0,
            Italic          = 1,
            Underlined      = 1 << 1,
            Strikethrough   = 1 << 2
        };

    public:

        /// \brief Constructor
        ///
        /// \param object Parent
        /// \param renderer Renderer
        ///
        Text(Object& object, Renderer& renderer);

        /// \brief Set string that is displayed
        ///
        /// \param string text
        ///
        Text& setString(const std::wstring& string);

        /// \brief Set string that is displayed and calculate vertex positions
        ///
        /// \param string text
        ///
        Text& setString(const std::string& string);

        /// \brief Get the string
        ///
        /// \return Reference to the internal string
        ///
        const std::wstring& getString() const;
        
        /// \brief Get bounds for whole text
        ///
        const Rect& getBounds() const;

        /// \brief Get the position of a single character
        ///
        /// \return Single characters position in local coordinates.
        ///
        glm::vec2 getCharacterPosition(const uint32 index);

        /// \brief Set font
        ///
        Text& setFont(const Font& font);

        /// \brief Get font
        ///
        const Font& getFont() const;

        /// \brief Set color
        ///
        Text& setColor(const Color& color);

        /// \brief Get the color
        ///
        /// \return The color
        ///
        const Color& getColor() const;

        /// \brief Set text style
        ///
        /// \param style jop::Text::Style (Default, Italic, Underlined, Strikethrough)
        ///
        /// Can use multiple styles simultaneously
        ///
        Text& setStyle(const uint32 style);

        /// \brief Get text style
        ///
        uint32 getStyle() const;

    private:

        /// \brief Updates geometry of the text when necessary
        ///
        /// This has to be called before drawing.
        ///
        void updateGeometry() const;

        /// \brief Adds a line to the text
        ///
        /// Adds extra vertices to be drawn, depending on text style (Strikethrough/Underline)
        ///
        /// \param m_vertices Reference to vertex vector
        /// \param lineLength Length of the drawn line
        /// \param lineTop Starting height of the line
        /// \param offset Offset distance from lineTop
        /// \param thickness Thickness of the drawn line
        ///
        void addLine(std::vector<Vertex>& m_vertices, const float lineLenght, const float lineTop, const float offset, const float thickness) const;

        /// \brief Draws text from given string
        ///
        void draw(const ProjectionInfo& proj, const LightContainer& lights) const override;


        Material m_material;                    ///< Material to be used
        WeakReference<const Font> m_font;       ///< Reference to the current font
        std::wstring m_string;                  ///< String to display
        uint32 m_style;                         ///< Text style
        mutable unsigned int m_lastFontSize;    ///< Most recent font size
        mutable bool m_geometryNeedsUpdate;     ///< Does geometry need to be recomputed
        mutable Rect m_bounds;                  ///< Bounding rectangle around text
        mutable Mesh m_mesh;                    ///< Mesh for holding vertices and drawing
    };
}

#endif

/// \class Text
/// \ingroup graphics
///
/// Handles text rendering
/// Supports multiple styles
///