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

//////////////////////////////////////////////


namespace jop
{
    class Controller
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
            };

            /// Axes
            enum Axis
            {
                LeftStickX,
                LeftStickY,
                Triggers,
                RightStickY,
                RightStickX
            };
        };

        /// Playstation controller enumerations
        ///
        struct Playstation
        {
            enum Button
            {
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
                PadLeft,
            };

            /// Axes
            enum Axis
            {
                LeftStickX,
                LeftStickY,
                Triggers,
                RightStickY,
                RightStickX
            };
        };
    };
}

#endif

/// \class Controller
/// \ingroup window
///
/// #TODO Detailed description