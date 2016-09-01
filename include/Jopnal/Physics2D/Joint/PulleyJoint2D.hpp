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

#ifndef JOP_PULLEYJOINT2D_HPP
#define JOP_PULLEYJOINT2D_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics2D/Joint/Joint2D.hpp>
#include <glm/vec2.hpp>
#include <utility>

//////////////////////////////////////////////


class b2PulleyJoint;

namespace jop
{
    class JOP_API PulleyJoint2D : public Joint2D
    {
    public:

        /// \brief PulleyJoint2D constructor.
        ///
        /// \note These pulleys have the other end as a static attachment to the world. If the user wishes to create a pulley that also moves sideways, the user can create one in the following way:
        /// Create four RigidBody2Ds.
        /// Attach two of them with WeldJoint2D.
        /// Attach a free body to either welded body via PulleyJoint2D.
        /// Attach final body to the other welded body via PulleyJoint2D.
        /// Create logic between the PulleyJoint2Ds and apply forces to a welded body.
        ///
        /// \param ratio Ratio for which the ropes move in relation to each other.
        /// \param groundAnchorA World coordinates for attachment point of the second end of the rope for body A.
        /// \param groundAnchorB World coordinates for attachment point of the second end of the rope for body B.
        /// \param localAnchorA Local coordinates of body A to attach the rope to. Uses local center of the object if left empty. Please use values between -1.f and 1.f.
        /// \param localAnchorB Local coordinates of body B to attach the rope to. Uses local center of the object if left empty. Please use values between -1.f and 1.f.
        ///
        /// \return PulleyJoint2D
        ///
        PulleyJoint2D(World2D& worldRef, RigidBody2D& bodyA, RigidBody2D& bodyB,
            const bool collide, const float ratio,
            const glm::vec2& groundAnchorA, const glm::vec2& groundAnchorB,
            const glm::vec2& localAnchorA = glm::vec2(0.f, 0.f),
            const glm::vec2& localAnchorB = glm::vec2(0.f, 0.f)
            );

        /// \return Returns current rope lengths: first = bodyA, second = bodyB
        ///
        std::pair<float, float> getRopeLengths() const;

    private:
        b2PulleyJoint* m_jointL;
    };
}

/// \class jop::PulleyJoint2D
/// \ingroup physics2d

#endif