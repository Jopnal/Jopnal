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

#ifndef JOP_MOTORJOINT2D_HPP
#define JOP_MOTORJOINT2D_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics2D/Joint2D.hpp>

//////////////////////////////////////////////

class b2MotorJoint;

namespace jop
{
    class JOP_API MotorJoint2D : public Joint2D
    {
    public:

        /// \brief MotorJoint2D constructor
        ///
        /// \param collide Joined bodies collide with each other.
        ///
        /// The MotorJoint2D tries to keep the distance and angle between the two bodies constant.
        /// This is automatically calculated upon creation. To change it later, call setLinearOffset or setAngularOffset.
        ///
        MotorJoint2D(World2D& worldRef, RigidBody2D& bodyA, RigidBody2D& bodyB, const bool collide);

        /// \return glm::vec2 Returns target linear offsets as a glm::vec2 with x and y components.
        ///
        glm::vec2 getLinearOffset();

        /// \return float Returns target angular offset as radians.
        ///
        float getAngularOffset();

        /// Set new target linear offsets for x and y components in relation to the other body.
        ///
        /// \return Reference to self
        ///
        MotorJoint2D& setLinearOffset(const glm::vec2& offset);

        /// Set new target angular offset as radians in relation to the other body.
        ///
        /// \return Reference to self
        ///
        MotorJoint2D& setAngularOffset(const float offset);

        /// Set new force with which the body tries to reach the target offsets.
        ///
        /// \return Reference to self
        ///
        MotorJoint2D& setForce(const float force);

        /// \return float Returns the current force with which the body tries to reach the target offsets.
        ///
        float getForce();

        /// Set new torque with which the body tries to reach the target offsets.
        ///
        /// \return Reference to self
        ///
        MotorJoint2D& setTorque(const float torque);

        /// \return float Returns the current torque with which the body tries to reach the target offsets.
        ///
        float getTorque();

    private:
        b2MotorJoint* m_jointL;
    };
}

#endif