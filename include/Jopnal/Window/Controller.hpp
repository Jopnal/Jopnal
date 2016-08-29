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

#ifndef JOP_CONTROLLER_HPP
#define JOP_CONTROLLER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <glm/vec2.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Controller
    {
    public:

        /// X-Box controller enumerations
        ///
        struct XBox
        {
            /// Buttons
            ///
            enum Button
            {
                Unknown,
                A,
                B,
                X,
                Y,
                LShoulder,
                RShoulder,
                Back,
                Start,
                LStick,
                RStick,
                PadUp,
                PadRight,
                PadDown,
                PadLeft,

                __Count ///< For internal functionality, do not use
            };

            /// Axes
            enum Axis
            {
                LeftStickX,
                LeftStickY,
                RightStickX,
                RightStickY,
                LTrigger,
                RTrigger
            };
        };

        /// Playstation controller enumerations
        ///
        struct Playstation
        {
            enum Button
            {
                Unknown,
                X,
                O,
                Square,
                Triangle,
                LShoulder,
                RShoulder,
                Select,
                Start,
                LStick,
                RStick,
                PadUp,
                PadRight,
                PadDown,
                PadLeft
            };

            /// Axes
            enum Axis
            {
                LeftStickX,
                LeftStickY,
                RightStickX,
                RightStickY,
                LTrigger,
                RTrigger
            };
        };

    public:

        /// \brief Check how many controllers are connected
        ///
        /// \return Number of controllers
        ///
        static int controllersPresent();

        /// \brief Check if specific controller is connected
        ///
        /// \param index The index of the controller to check
        ///
        /// \return True if connected
        ///
        static bool isControllerPresent(const int index);

        /// \brief Check if a controller button is currently pressed
        ///
        /// \param index The controller index
        /// \param button The controller button
        ///
        /// \return True if currently pressed
        ///
        static bool isButtonDown(const int index, const int button);

        /// \brief Get an axis offset
        ///
        /// \param index The index if the controller
        /// \param axis The axis
        ///
        /// \return The axis offsed, between -1 and 1
        ///
        static float getAxisOffset(const int index, const int axis);
    };
}

/// \class jop::Controller
/// \ingroup window

#endif