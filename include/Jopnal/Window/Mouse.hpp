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

#ifndef JOP_MOUSE_HPP
#define JOP_MOUSE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Mouse
    {
    public:

        /// Mouse buttons
        ///
        enum Button
        {
            Unknown,
            Left,
            Right,
            Middle,
            X1,
            X2,
            X3,
            X4,
            X5
        };

        /// Mouse modes (Visible, Hidden, Frozen)
        ///
        enum class Mode
        {
            Visible,    ///< Visible makes cursor visible and behave normally.
            Hidden,     ///< Hidden makes cursor invisible when its not over the client area of the window, but does not restrict it from leaving.
            Frozen      ///< Frozen hides and grabs the cursor, providing virtual and unlimited cursor movement.
        };

    public:

        /// \brief Get boolean is button down
        ///
        /// \param Button's code
        ///
        /// \return True if down
        ///
        static bool isButtonDown(const Button button);

        /// \brief Get mouse position
        ///
        /// \return Glm vector of mouse position
        ///
        static glm::vec2 getPosition();

        /// \brief Sets mouse mode
        ///
        /// Mouse modes: Visible, Hidden, Frozen (Defined in mouse class)
        ///
        /// \param mode Enum mouse mode.
        ///
        static void setMouseMode(const Mode mode);

        /// \brief Restricts mouse into user defined retangle shaped area inside window
        ///
        /// \default Size of context window
        ///
        static void setClipping();

        /// \brief Restricts mouse into user defined retangle shaped area inside window
        ///
        /// \param Center position x,y and size of clipping area
        ///
        static void setClipping(const int x, const int y, const int size);

        /// \brief Restricts mouse into user defined retangle shaped area inside window
        ///
        /// \param Coordinates of clipping area (int minX, int maxX, int minY, int maxY)
        ///
        static void setClipping(const int minX, const int maxX, const int minY, const int maxY);

        /// \brief Restricts mouse into user defined retangle shaped area inside window
        ///
        /// \param Coordinates of clipping area as two min-max vector (glm::ivec2 x, glm::ivec2 y)
        ///
        static void setClipping(const glm::ivec2 x, const glm::ivec2 y);

        /// \brief Restricts mouse into user defined retangle shaped area inside window
        ///
        /// \param Coordinates of clipping area as vector (glm::ivec4(int minX, int maxX, int minY, int maxY))
        ///
        static void setClipping(const glm::ivec4 clipping);

        /// \brief Get clipping coordinates of mouse
        ///
        /// \param Coordinates of clipping area as vector (glm::ivec4(int minX, int maxX, int minY, int maxY))
        ///
        static const glm::ivec4& getClipping();

        /// \brief Does mouse have movement restrictions
        ///
        /// \return True if mouse has clipping area
        ///
        static bool isClipping();

        /// \brief Releases mouse if it's resrticted into clipping area
        ///
        static void releaseClipping();
    };
}

#endif

/// \class Mouse
/// \ingroup window
///
/// #TODO Detailed description