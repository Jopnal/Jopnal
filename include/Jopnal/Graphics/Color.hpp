// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//////////////////////////////////////////////

#ifndef JOP_COLOR_HPP
#define JOP_COLOR_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <glm/vec4.hpp>
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
        /// Values must be between 0 and 255.
        ///
        /// \param _r The red value
        /// \param _g The green value
        /// \param _b The blue value
        /// \param _a The alpha value
        ///
        Color(const jop::uint8 _r, const jop::uint8 _g, const jop::uint8 _b, const jop::uint8 _a = 255);

        /// \brief Overloaded constructor
        ///
        /// \param colors A 32-bit unsigned integer to represent the colors,
        ///               as in 0xFFFFFFFF
        ///
        Color(const jop::uint32 colors);

        /// \brief Overloaded constructor
        ///
        /// \param hexString A string in hex form to describe the color, as
        ///                  in "FFFFFFFF"
        ///
        Color(const std::string& hexString);


        /// \brief Get this color as a float vector
        ///
        /// The precision of the values is 32 bits each.
        ///
        /// \return A float vector describing the color
        ///
        glm::fvec4 asFloatVector() const;


    public:

        jop::uint8   r, ///< The red component
                     g, ///< The green component
                     b, ///< The blue component
                     a; ///< The alpha component

        static const Color Black;
        static const Color White;
        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Cyan;
        static const Color Yellow;
        static const Color Magenta;
        static const Color Purple;
        static const Color Orange;
        static const Color Gray;
        static const Color Brown;
        static const Color Transparent;


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