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

#ifndef JOP_WHEELJOINT_HPP
#define JOP_WHEELJOINT_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics/Joint.hpp>

//////////////////////////////////////////////

class btGeneric6DofConstraint;

namespace jop
{
    class JOP_API WheelJoint : public Joint
    {
    public:

        enum class Axis
        {
            X,
            Y,
            Z
        };

        /// \brief WheelJoint constructor.
        ///
        /// <b>When creating WheelJoints, always call the link from the chassis and pass the wheel in as the first argument.</b>
        /// \note When creating a vehicle, it is best to create it in the following manner:
        /// The vehicle is parallel to global Z-axis (by length).
        /// The vehicle wheels are then +/- of the chassis in X-axis.
        ///
        /// \param collide Joined bodies collide with each other.
        /// \param jPos Position of the joint in world coordinates.
        /// \param maxSteering Maximum steering angle of the wheel in radians. setAngle can not override this. Default is ~40 degrees.
        /// \param jRot Rotation of the joint in a quaternion. Defaults the orientation of the joint based on global difference between the chassis and the wheel X-positions.
        ///
        WheelJoint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide, const glm::vec3& jPos, const float maxSteering = 0.7f, const glm::quat& jRot = glm::fquat(0.f, 0.f, 0.f, 0.f));

        /// \brief Apply rotational force to the wheel's rigidbody.
        ///
        /// \param torque Torque to be applied to the wheel. Negative value rotates the wheel in opposite direction.
        /// \param axis Axis to apply the force to. Default is Y-axis.
        ///
        /// \return Returns reference to self
        ///
        WheelJoint& applyTorque(const float torque, const Axis axis = Axis::Y);

        /// \brief Apply rotational force to the wheel.
        ///
        /// \param torque Torque to be applied to the wheel. Negative value rotates the wheel in opposite direction.
        ///
        /// \return Returns reference to self.
        ///
        WheelJoint& applyTorque(glm::vec3 torque);

        /// \return Returns current torque affecting the joint in an axis.
        ///
        float getTorque(const Axis axis = Axis::Y) const;

        /// \return Returns vector of torques affecting the joint.
        ///
        glm::vec3 getTorque() const;

        /// \return Returns angle of the joint in an axis in radians. Default Y-axis.
        ///
        float getAngle(const Axis axis = Axis::Y) const;
        
        /// \brief Set new angle for the joint in an axis.
        ///
        /// This can not override the maxSteering of the joint.
        ///
        /// \param steeringAngle New steering angle to be set to the wheel. Negative value turns the wheel in the other direction.
        /// \param axis Axis to change. Default Y-axis.
        ///
        /// \return Returns reference to self.
        ///
        WheelJoint& setAngle(const float steeringAngle, const Axis axis = Axis::Y);

    private:

        btGeneric6DofConstraint* m_jointL;
        float m_maxAngle;
    };
}
#endif