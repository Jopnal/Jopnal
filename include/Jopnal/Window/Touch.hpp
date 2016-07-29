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

#ifndef JOP_TOUCH_HPP
#define JOP_TOUCH_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <glm/vec2.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Touch
    {
    public:

        /// Information
        ///
        enum Info
        {
            Pressure,   ///< Approximate pressure applied to the surface
            Size,       ///< Approximate size of the contact area
            TouchMajor, ///< Major axis of an ellipse that represents the touch area at the point of contact
            ToolMajor   ///< Major axis of an ellipse that represents the size of the approaching touch
        };

    public:

        /// \brief Is touch received
        ///
        /// \param Finger's id 0-9
        ///
        /// \return True if touch is received false if not
        ///
        static bool isDown(const int id = 0);

        /// \brief Get position of specific finger
        ///
        /// \param Finger's id 0-9
        ///
        /// \return Position as glm vector 2
        ///
        static glm::vec2 getPosition(const int id = 0);
    };
}

#endif

/// \class jop::Touch
/// \ingroup window
///
/// #TODO Detailed description