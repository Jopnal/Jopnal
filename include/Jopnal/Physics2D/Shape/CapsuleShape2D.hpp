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

#ifndef JOP_CAPSULESHAPE2D_HPP
#define JOP_CAPSULESHAPE2D_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics2D/Shape/CircleShape2D.hpp>
#include <Jopnal/Physics2D/Shape/CompoundShape2D.hpp>
#include <Jopnal/Physics2D/Shape/RectangleShape2D.hpp>

//////////////////////////////////////////////

namespace jop
{
    class JOP_API CapsuleShape2D : public CompoundShape2D
    {
    public:

        /// \brief Constructor
        ///
        /// \param name Name of the resource
        ///
        CapsuleShape2D(const std::string& name);

        /// \brief Load this shape using different extents
        ///
        /// \param width Width of the capsule
        /// \param height Height of the capsule
        ///
        /// \return True if successful
        ///
        bool load(const float width, const float height);

    private:
        RectangleShape2D m_rect;
        CircleShape2D m_ball1;
        CircleShape2D m_ball2;
    };
}

/// \class jop::CapsuleShape2D
/// \ingroup physics2d

#endif