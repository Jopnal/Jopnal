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
#include <Jopnal/Physics/Joint.hpp>

//////////////////////////////////////////////

class btHingeConstraint;

namespace jop
{
    class JOP_API RotationJoint : public Joint
    {
    public:
        /// \brief RotationJoint constructor.
        ///
        /// \param collide Joined bodies collide with each other.
        /// \param jPos Position of the joint in world coordinates.
        /// \param jRot Rotation of the joint. Default axis-aligned.
        ///
        RotationJoint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide, const glm::vec3& jPos = glm::vec3(0.f, 0.f, FLT_MAX), const glm::quat& jRot = glm::quat(1.f, 0.f, 0.f, 0.f));

        /// \param enable Enables/disables the hinge motor.
        ///
        /// \return Returns reference to self.
        ///
        RotationJoint& enableMotor(const bool enable);

        /// \return Returns current rotational limits of the hinge: first = minimum, second = maximum.
        ///
        std::pair<float, float> getAngLimits() const;

        /// \return Returns current angular motor forces: first = speed, second = force
        ///
        std::pair<float, float> getAngForces() const;

        /// Set new rotational limits for the hinge in radians.
        ///
        /// \return Returns reference to self.
        ///
        RotationJoint& setAngLimits(const float min, const float max);

        /// Set new rotational forces for the hinge motor.
        ///
        /// \param speed New target speed to reach.
        /// \param force New maximum motor force to exert to reach the target speed. Negative force causes the hinge to keep moving from one limit to the other.
        /// 
        /// \return Returns reference to self.
        ///
        RotationJoint& setAngForces(const float speed, const float force);

    private:
        btHingeConstraint* m_jointL;
    };
}
#endif