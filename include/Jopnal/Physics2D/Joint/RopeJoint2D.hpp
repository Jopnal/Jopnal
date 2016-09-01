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

#ifndef JOP_ROPEJOINT2D_HPP
#define JOP_ROPEJOINT2D_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics2D/Joint/Joint2D.hpp>
#include <glm/vec2.hpp>
#include <utility>

//////////////////////////////////////////////


class b2DistanceJoint;

namespace jop
{
    class JOP_API RopeJoint2D : public Joint2D
    {
    public:

        /// \brief RopeJoint2D constructor.
        ///
        /// Creates a link / rope between the bodies. For longer ropes, ropes made from several rigidbodies and attached one after another should be used.
        ///
        /// \param anchorA Anchor point in local coordinates. Please use values between -1.f - +1.f. Defaults to the center of the body.
        /// \param anchorB Anchor point in local coordinates. Please use values between -1.f - +1.f. Defaults to the center of the body.
        ///
        RopeJoint2D(World2D& worldRef, RigidBody2D& bodyA, RigidBody2D& bodyB, const bool collide,
            const glm::vec2& anchorA = glm::vec2(0.f, 0.f), const glm::vec2& anchorB = glm::vec2(0.f, 0.f));

        /// \return Returns damping values: first = frequency, second = damping.
        ///
        std::pair<float, float>getDamping() const;

        /// \param frequency Damping frequency in Hz.
        /// \param damping Damping ratio. Please use values between 0.f - 1.0f.
        ///
        /// \return Returns reference to self.
        ///
        RopeJoint2D& setDamping(const float frequency, const float damping);

    private:
        b2DistanceJoint* m_jointL;
    };
}

/// \class jop::RopeJoint2D
/// \ingroup physics2d

#endif