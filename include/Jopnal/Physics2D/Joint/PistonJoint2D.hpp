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

#ifndef JOP_PISTONJOINT2D_HPP
#define JOP_PISTONJOINT2D_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics2D/Joint/Joint2D.hpp>
#include <glm/vec2.hpp>
#include <utility>

//////////////////////////////////////////////


class b2PrismaticJoint;

namespace jop
{
    class JOP_API PistonJoint2D : public Joint2D
    {
    public:
        /// \brief PistonJoint2D constructor.
        ///
        /// \param collide Joined bodies collide with each other.
        /// \param axis A single axis where movement is permitted.
        ///
        PistonJoint2D(World2D& worldRef, RigidBody2D& bodyA, RigidBody2D& bodyB, const bool collide, const glm::vec2& axis);

        /// \param enable Enables/disables obeying of movement limits.
        ///
        /// \return Returns reference to self.
        ///
        PistonJoint2D& limit(const bool enable);

        /// Set new limits in world coordinates in relation to current position.
        /// Enables limiting when called.
        ///
        /// \return Returns reference to self.
        ///
        PistonJoint2D& setLimits(const float min, const float max);

        /// \return Returns current movement limits of the piston: first = minimum, second = maximum.
        ///
        std::pair<float, float> getLimits() const;

        //OPEN TO PUBLIC IF NEEDED
    private:
        /// \param enable Enables/disables motor.
        ///
        /// \return Returns reference to self.
        ///
        PistonJoint2D& enableMotor(const bool enable);

        /// Set new target speed and the maximum force for the motor to try to reach it.
        /// 
        /// \return Returns reference to self.
        ///
        PistonJoint2D& setMotorForces(const float speed, const float force);

        /// \return Returns current motor forces: first = speed, second = force.
        ///
        std::pair<float, float> getMotorForces() const;

    private:
        b2PrismaticJoint* m_jointL;

    };
}

#endif