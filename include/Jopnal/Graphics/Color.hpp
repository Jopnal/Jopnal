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

#ifndef JOP_COLOR_HPP
#define JOP_COLOR_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <glm/glm.hpp>
#include <string>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Color
    {
    public:

        /// \brief Default constructor
        ///
        /// Initializes this color as white.
        ///
        Color();

        /// \brief Overloaded constructor
        ///
        /// \param _r The red value
        /// \param _g The green value
        /// \param _b The blue value
        /// \param _a The alpha value
        ///
        Color(const uint8 _r, const uint8 _g, const uint8 _b, const uint8 _a = 255);

        /// \brief Overloaded constructor
        ///
        /// The values need to be clamped between 0 and 1.
        ///
        Color(const float _r, const float _g, const float _b, const float _a = 1.f);

        /// \brief Overloaded constructor
        ///
        /// \param colors A 32-bit unsigned integer to represent the colors,
        ///               as in 0xFFFFFFFF
        ///
        Color(const uint32 colors);

        /// \brief Overloaded constructor
        ///
        /// \param hexString A string in hex form to describe the color, as
        ///                  in "FFFFFFFF"
        ///
        Color(const std::string& hexString);


        /// \brief Get this color as a RGBA float vector
        ///
        /// The precision of the values is 32 bits each.
        ///
        /// \return A float vector describing the color
        ///
        glm::vec4 asRGBAFloatVector() const;

        /// \brief Get this color as a RGB float vector
        ///
        /// The precision of the values is 32 bits each.
        ///
        /// \return A float vector describing the color
        ///
        glm::vec3 asRGBFloatVector() const;

        /// \brief Get this color as an 32-bit unsigned integer
        ///
        /// \return 32-bit unsigned integer
        ///
        unsigned int asInteger() const;


    public:

        uint8   r, ///< The red component
                g, ///< The green component
                b, ///< The blue component
                a; ///< The alpha component

        static const Color Black;       ///< Black color
        static const Color White;       ///< White color
        static const Color Red;         ///< Red color
        static const Color Green;       ///< Green color
        static const Color Blue;        ///< Blue color
        static const Color Cyan;        ///< Cyan color
        static const Color Yellow;      ///< Yellow color
        static const Color Magenta;     ///< Magenta color
        static const Color Purple;      ///< Purple color
        static const Color Orange;      ///< Orange color
        static const Color Gray;        ///< Gray color
        static const Color Brown;       ///< Brown color
        static const Color Transparent; ///< Transparent color


    public:

        /// \brief Equality operator
        ///
        bool operator ==(const Color& other) const;

        /// \brief Inequality operator
        ///
        bool operator !=(const Color& other) const;
    };
}

#endif

/// \class Color
/// \ingroup graphics
///
/// #TODO Detailed description