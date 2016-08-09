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

#ifndef JOP_ROTATIONJOINT2D_HPP
#define JOP_ROTATIONJOINT2D_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics2D/Joint/Joint2D.hpp>
#include <glm/vec2.hpp>
#include <utility>

//////////////////////////////////////////////


class b2RevoluteJoint;

namespace jop
{
    class JOP_API RotationJoint2D : public Joint2D
    {
    public:
        /// \brief RotationJoint2D constructor
        ///
        /// \param localAnchorA Anchor point in the bodyA to start the connection from. Local coordinates: -1.f - +1.f. Default is center.
        /// Anchor point for bodyB can not changed. It is at the center of bodyB.
        ///
        RotationJoint2D(World2D& worldRef, RigidBody2D& bodyA, RigidBody2D& bodyB, const bool collide, const glm::vec2& localAnchorA = glm::vec2(0.f, 0.f));

        /// \param enable Enables/disables obeying rotation limits.
        ///
        /// \return Returns reference to self.
        ///
        RotationJoint2D& limit(const bool enable);

        /// \param minAngle Minimum angle of rotation in radians.
        /// \param maxAngle Maximum angle of rotation in radians.
        ///
        /// Enables limiting when called.
        ///
        /// \return Returns reference to self.
        ///
        RotationJoint2D& setLimits(const float minAngle, const float maxAngle);

        /// \return Returns current rotation limits in radians: first = minimum angle, second = maximum angle.
        ///
        std::pair<float, float> getLimits() const;

        //OPEN TO PUBLIC IF NEEDED
    private:
        /// \param enable Enables/disables motor.
        ///
        /// \return Returns reference to self.
        ///
        RotationJoint2D& enableMotor(const bool enable);

        /// \param speed Maximum speed the motor tries to rotate.
        /// \param torque Maximum torque the engine exerts to achieve the target speed.
        ///
        /// \return Returns reference to self.
        ///
        RotationJoint2D& setMotorForces(const float speed, const float torque);

        /// \return Returns motor forces: first = current rotation speed, second = maximum torque.
        ///
        std::pair<float, float> getMotorForces() const;

    private:
        b2RevoluteJoint* m_jointL;
    };
}
#endif