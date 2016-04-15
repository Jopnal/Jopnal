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

//////////////////////////////////////////////


namespace jop
{
    class Mouse
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
    };
}

#endif

/// \class Mouse
/// \ingroup window
///
/// #TODO Detailed description