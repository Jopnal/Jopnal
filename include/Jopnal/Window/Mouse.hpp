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
#include <utility>

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

        /// Mouse modes
        ///
        enum class Mode
        {
            Visible,    ///< Visible makes cursor visible and behave normally
            Hidden,     ///< Hidden makes cursor invisible when its not over the client area of the window, but does not restrict it from leaving
            Frozen      ///< Frozen hides and grabs the cursor, providing virtual and unlimited cursor movement
        };

    public:

        /// \brief Check if a mouse button is down
        ///
        /// \param button The mouse button
        ///
        /// \return True if button is down
        ///
        static bool isButtonDown(const Button button);

        /// \brief Get the mouse cursor position
        ///
        /// \return The mouse position
        ///
        static glm::vec2 getPosition();

        /// \brief Set the mouse mode
        ///
        /// \param mode The mouse mode
        ///
        static void setMouseMode(const Mode mode);

        /// \brief Restricts the mouse cursor inside the main window
        ///
        static void setClipping();

        /// \brief Restricts mouse into a rectangular area inside the main window
        ///
        /// The coordinates are relative to the main window.
        ///
        /// \param min The minimum coordinates of the restriction rectangle (left, top)
        /// \param max The maximum coordinates of the restriction rectangle (right, bottom)
        ///
        static void setClipping(const glm::ivec2& min, const glm::ivec2& max);

        /// \brief Get clipping coordinates of the mouse cursor
        ///
        /// \return The clipping coordinates (first = minimum, second = maximum)
        ///
        static const std::pair<glm::ivec2, glm::ivec2>& getClipping();

        /// \brief Check if the mouse cursor has been clipped
        ///
        /// \return True if the mouse cursor is clipped
        ///
        static bool isClipping();

        /// \brief Release the mouse cursor if it was previously clipped
        ///
        static void releaseClipping();
    };
}

/// \class jop::Mouse
/// \ingroup window

#endif