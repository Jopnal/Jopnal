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

#ifndef JOP_PISTONJOINT_HPP
#define JOP_PISTONJOINT_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics/Joint/Joint.hpp>
#include <glm/gtx/quaternion.hpp>
#include <utility>

//////////////////////////////////////////////


class btSliderConstraint;

namespace jop
{
    class JOP_API PistonJoint : public Joint
    {
    public:
        /// \brief PistonJoint constructor.
        ///
        /// \param anchor Position of the joint in world coordinates. Default in the middle between the bodies.
        /// \param jRot Rotation of the joint. Default axis-aligned.
        ///
        PistonJoint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide,
            const glm::vec3& anchor = glm::vec3(0.f, 0.f, FLT_MAX), const glm::quat& jRot = glm::quat(1.f, 0.f, 0.f, 0.f));

        /// \return Returns current rotational limits of the piston: first = minimum, second = maximum.
        ///
        std::pair<float, float> getAngLimits() const;

        /// \return Returns current translation limits of the piston: first = minimum, second = maximum.
        ///
        std::pair<float, float> getMoveLimits() const;

        /// Set new rotation limits for the piston in radians.
        ///
        /// \return Returns reference to self.
        ///
        PistonJoint& setAngLimits(const float min, const float max);

        /// Set new limits where translation is allowed. 0 is the center of the bodyA.
        ///
        /// \return Returns reference to self.
        ///
        PistonJoint& setMoveLimits(const float min, const float max);


        //OPEN TO PUBLIC IF NEEDED
    private:
        /// \param enable Enables/disables angular motor.
        ///
        /// \return Returns reference to self.
        ///
        PistonJoint& enableAngMotor(const bool enable);

        /// \param enable Enables/disables translation motor.
        ///
        /// \return Returns reference to self.
        ///
        PistonJoint& enableMoveMotor(const bool enable);

        /// \return Returns current angular motor forces: first = speed, second = force
        ///
        std::pair<float, float> getAngMotorForces() const;

        /// \return Returns current angular target speed.
        ///
        float getAngTargetSpeed() const;

        /// \return Returns current translational motor force.
        ///
        std::pair<float, float> getMoveMotorForces() const;

        /// \return Returns current translational target speed.
        ///
        float getMoveTargetSpeed() const;

        /// Set new maximum forces for the angular motor.
        ///
        /// \param speed New angular target speed to reach.
        /// \param force New maximum angular motor force to exert to reach the target speed. Negative force causes the piston to keep moving from one limit to the other.
        /// 
        /// \return Returns reference to self.
        ///
        PistonJoint& setAngMotorForces(const float speed, const float force);

        /// Set new maximum forces for the translational motor to exert.
        ///
        /// \param speed New translational target speed.
        /// \param force New maximum translational force to exert to reach the target speed. Negative force causes the piston to keep moving from one limit to the other.
        /// 
        /// \return Returns reference to self.
        ///
        PistonJoint& setMoveMotorForces(const float speed, const float force);

    private:
        btSliderConstraint* m_jointL;

    };
}

/// \class jop::PistonJoint
/// \ingroup physics

#endif