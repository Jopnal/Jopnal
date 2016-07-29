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

#ifndef JOP_ROPEJOINT_HPP
#define JOP_ROPEJOINT_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics/Joint.hpp>

//////////////////////////////////////////////

class btGeneric6DofConstraint;

namespace jop
{
    class JOP_API RopeJoint : public Joint
    {
    public:
        /// \brief RopeJoint constructor.
        ///
        /// Length of the rope is calculated from the difference in global coordinates between the two bodies.
        /// Longer ropes, where accurate physics computations are needed, should be created by creating separate bodíes and attaching RopeJoint to them one after another.
        ///
        /// \param collide Joined bodies collide with each other.
        ///
        RopeJoint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide);


        /// \param enable Enables/disables angular motor.
        ///
        /// \return Returns reference to self.
        ///
        RopeJoint& enableAngMotor(const bool enable);

        /*
        /// \param enable Enables/disables translation motor.
        ///
        /// \return Returns reference to self.
        ///
        RopeJoint& enableMoveMotor(const bool enable);

        /// \return Returns current rotational limits of the Rope: first = minimum, second = maximum.
        ///
        std::pair<float, float> getAngLimits() const;

        /// \return Returns current angular motor forces: first = speed, second = force
        ///
        std::pair<float, float> getAngMotorForces() const;

        /// \return Returns current angular target speed.
        ///
        float getAngTargetSpeed() const;

        /// \return Returns current translation limits of the Rope: first = minimum, second = maximum.
        ///
        std::pair<float, float> getMoveLimits() const;

        /// \return Returns current translational motor force.
        ///
        std::pair<float, float> getMoveMotorForces() const;

        /// \return Returns current translational target speed.
        ///
        float getMoveTargetSpeed() const;

        /// Set new rotation limits for the Rope in radians.
        ///
        /// \return Returns reference to self.
        ///
        RopeJoint& setAngLimits(const float min, const float max);

        /// Set new maximum forces for the angular motor.
        ///
        /// \param speed New angular target speed to reach.
        /// \param force New maximum angular motor force to exert to reach the target speed. Negative force causes the Rope to keep moving from one limit to the other.
        /// 
        /// \return Returns reference to self.
        ///
        RopeJoint& setAngMotorForces(const float speed, const float force);

        /// Set new limits where translation is allowed. 0 is the center of the bodyA.
        ///
        /// \return Returns reference to self.
        ///
        RopeJoint& setMoveLimits(const float min, const float max);

        /// Set new maximum forces for the translational motor to exert.
        ///
        /// \param speed New translational target speed.
        /// \param force New maximum translational force to exert to reach the target speed. Negative force causes the Rope to keep moving from one limit to the other.
        /// 
        /// \return Returns reference to self.
        ///
        RopeJoint& setMoveMotorForces(const float speed, const float force);
*/
    private:
        btGeneric6DofConstraint* m_jointL;
    };
}

#endif