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
#include <Jopnal/Physics/Joint/Joint.hpp>
#include <glm/gtx/quaternion.hpp>

//////////////////////////////////////////////


class btGeneric6DofSpringConstraint;

namespace jop
{
    class JOP_API WheelJoint : public Joint
    {
    public:

        /// T_ : Translation
        /// R_ : Rotation
        ///
        enum class Axis
        {
            T_X,
            T_Y,
            T_Z,
            R_X,
            R_Y,
            R_Z
        };

        /// \brief WheelJoint constructor.
        ///
        /// <b>When creating WheelJoints, always call the link from the chassis and pass the wheel in as the first argument.</b>
        /// \note When creating a vehicle, it is best to create it in the following manner:
        /// The vehicle is parallel to global Z-axis (by length).
        /// The vehicle wheels are then +/- of the chassis in X-axis.
        ///
        /// \param maxSteering Maximum steering angle of the wheel in radians. Function setAngle can not override this. Same value is set for both +/- rotations. Default is 0.7f ~ 40 degrees.
        /// \param jRot Rotation of the joint in a quaternion. By default aligns the wheel by X- or Z-axis, depending on the global positions of the wheel and chassis.
        /// \param anchor Position of the joint in world coordinates. Default is in the middle of the wheel.
        ///
        WheelJoint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide, const float maxSteering = 0.7f,
            const glm::quat& jRot = glm::quat(0.f, 0.f, 0.f, 0.f), const glm::vec3& anchor = glm::vec3(0.f, 0.f, FLT_MAX));

        /// \return Returns angle of the joint in an axis in radians. Default Y-axis.
        ///
        float getAngle(const Axis axis = Axis::T_Y) const;

        /// \return Returns currently used stiffness in desired axis. Default Y-axis.
        ///
        float getStiffness(const Axis axis = Axis::T_Y);

        /// \brief Set new angle for the joint in an axis.
        ///
        /// This can not override the maxSteering of the joint.
        ///
        /// \param steeringAngle New steering angle to be set to the wheel. Negative value turns the wheel in the other direction.
        /// \param axis Axis to change. Default Y-axis.
        ///
        /// \return Returns reference to self.
        ///
        WheelJoint& setAngle(const float steeringAngle, const Axis axis = Axis::T_Y);

        /// Set spring stiffness.
        ///
        /// \param stiffness New stiffness to apply. Please use values between 0.f - 1.f.
        /// \param axis Axis to apply the stiffness to. Default Y.
        ///
        /// \return Returns reference to self.
        ///
        WheelJoint& setStiffness(const float stiffness, const Axis axis = Axis::T_Y);

    private:

        btGeneric6DofSpringConstraint* m_jointL;
        float m_maxAngle;
    };
}

/// \class jop::WheelJoint
/// \ingroup physics

#endif