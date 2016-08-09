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

#ifndef JOP_GEARJOINT_HPP
#define JOP_GEARJOINT_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics/Joint/Joint.hpp>

//////////////////////////////////////////////


class btGearConstraint;

namespace jop
{
    class JOP_API GearJoint : public Joint
    {
    public:
        /// \brief GearJoint constructor.
        ///
        /// \param collide Joined bodies collide with each other.
        /// \param rotAxisX Local axis of body X in which to apply and observe rotation.
        /// \param ratio Ratio in which to affect bodies.
        ///
        GearJoint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide, const glm::vec3& rotAxisA, const glm::vec3& rotAxisB, const float ratio = 1.f);

        /// \param bodyA Body which axis to get. True for bodyA, false for bodyB. Default true.
        ///
        /// \return Returns current target vector
        ///
        glm::vec3 getAxis(const bool bodyA = true) const;

        /// \return Returns current ratio of the joint.
        ///
        float getRatio() const;

        /// \param axis Axis to use 
        /// \param bodyA Body which axis to get. True for bodyA, false for bodyB. Default true.
        ///
        /// \return Returns reference to self.
        ///
        GearJoint& setAxis(const glm::vec3& axis, const bool bodyA = true);

        /// \param ratio New ratio to use in the joint.
        ///
        /// \return Returns reference to self.
        ///
        GearJoint& setRatio(const float ratio);

    private:
        btGearConstraint* m_jointL;

    };
}
#endif