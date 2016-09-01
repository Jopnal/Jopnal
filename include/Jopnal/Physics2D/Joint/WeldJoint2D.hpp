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

#ifndef JOP_WELDJOINT2D_HPP
#define JOP_WELDJOINT2D_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics2D/Joint/Joint2D.hpp>
#include <utility>

//////////////////////////////////////////////


class b2WeldJoint;

namespace jop
{
    class JOP_API WeldJoint2D : public Joint2D
    {
    public:

        /// \brief WeldJoint2D constructor.
        ///
        /// Welds two bodies together. Moving one body moves the other in similiar way.
        ///
        WeldJoint2D(World2D& worldRef, RigidBody2D& bodyA, RigidBody2D& bodyB, const bool collide);

        /// \brief Set new damping values
        ///
        /// \param frequency Damping frequency in Hz.
        /// \param damping Damping ratio. Please use values between 0.f - 1.f.
        ///
        WeldJoint2D& setDamping(const float frequency, const float damping);

        /// \return Returns current damping: first = frequency, second = damping.
        ///
        std::pair<float, float> getDamping() const;

    private:
        b2WeldJoint* m_jointL;
    };
}

/// \class jop::WeldJoint2D
/// \ingroup physics2d

#endif