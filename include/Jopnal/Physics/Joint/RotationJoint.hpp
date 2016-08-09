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

#ifndef JOP_ROTATIONJOINT_HPP
#define JOP_ROTATIONJOINT_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics/Joint/Joint.hpp>

//////////////////////////////////////////////

class btGeneric6DofConstraint;

namespace jop
{
    class JOP_API RotationJoint : public Joint
    {
    public:
        /// \brief RotationJoint constructor.
        ///
        /// \param collide Joined bodies collide with each other.
        /// \param jPos Position of the joint in world coordinates.
        ///
        RotationJoint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide, const glm::vec3& jPos = glm::vec3(0.f, 0.f, FLT_MAX));

        /// \param lower True = return lower limits. False = return upper limits.
        ///
        /// \return Returns desired limits of the joint.
        ///
        glm::vec3 getAngLimits(const bool lower) const;

        /// Set new rotational limits for the joint in radians.
        ///
        /// \param lower True = set lower limits. False = set upper limits.
        /// \param limits New limits to set to the joint.
        ///
        /// \return Returns reference to self.
        ///
        RotationJoint& setAngLimits(const bool lower, const glm::vec3& limits);

    private:
        btGeneric6DofConstraint* m_jointL;
    };
}
#endif