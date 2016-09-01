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

#ifndef JOP_WHEELJOINT2D_HPP
#define JOP_WHEELJOINT2D_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics2D/Joint/Joint2D.hpp>
#include <glm/vec2.hpp>
#include <utility>

//////////////////////////////////////////////


class b2WheelJoint;

namespace jop
{
    class JOP_API WheelJoint2D : public Joint2D
    {
    public:

        /// \brief WheelJoint2D constructor.
        ///
        /// Used to create rotating wheels.
        ///
        /// \param axis Single axis where movement is allowed.
        /// \param localAnchorA Given in world units in relation to the target body center.
        /// \param localAnchorB Given in local units. Defaults to the center of the wheel. Should be left empty unless user wants funny wheels.
        ///
        WheelJoint2D(World2D& worldRef, RigidBody2D& bodyA, RigidBody2D& bodyB,
            const bool collide, const glm::vec2& axis,
            const glm::vec2& localAnchorA,
            const glm::vec2& localAnchorB = glm::vec2(0.f, 0.f)
            );

        /// Set new damping values.
        ///
        /// \param frequency Damping frequency in Hz.
        /// \param damping Damping value. Please use values between 0.f - 1.f.
        ///
        /// \return Returns reference to self.
        ///
        WheelJoint2D& setDamping(const float frequency, const float damping);

        /// \return Returns current damping values: first = frequency, second = damping.
        ///
        std::pair<float, float> getDamping() const;

        //OPEN TO PUBLIC IF NEEDED
    private:
        /// \param enable Enables/disables motor.
        ///
        /// \return Returns reference to self.
        ///
        WheelJoint2D& enableMotor(const bool enable);

        /// \param speed Maximum rotation speed the motor tries to reach.
        /// \param torque Maximum torque the motor exerts to reachs the target speed.
        ///
        /// \return Returns reference to self.
        ///
        WheelJoint2D& setMotorForces(const float speed, const float torque);

        /// \return Returns motor forces: first = current motor speed, second = maximum torque.
        ///
        std::pair<float, float> getMotorForces() const;

    private:
        b2WheelJoint* m_jointL;

    };
}

/// \class jop::WheelJoint2D
/// \ingroup physics2d

#endif