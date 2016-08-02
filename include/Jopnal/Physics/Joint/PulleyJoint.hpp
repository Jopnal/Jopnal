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

#ifndef JOP_PULLEYJOINT_HPP
#define JOP_PULLEYJOINT_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics/Joint.hpp>

//////////////////////////////////////////////

class btQuaternion;
class btGeneric6DofConstraint;
class btVector3;

namespace jop
{
    class JOP_API PulleyJoint : public Joint
    {
    public:

        /// \brief PulleyJoint constructor.
        ///
        /// \param collide Joined bodies collide with each other.
        /// \param jPos Position of the joint in world coordinates.
        /// \param jRot Rotation of the joint. Default axis-aligned with X.
        ///
        PulleyJoint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide, const float ratio,
            const glm::vec3& jPos = glm::vec3(0.f, 0.f, FLT_MAX), const glm::quat& jRot = glm::quat(1.f, 0.f, 0.f, 0.f));

        ~PulleyJoint();

        /// \return Returns current desired limits of the pulley in desired axis.
        ///
        /// \param translation True for translation axes. False for rotation axes.
        /// \param lower True for lower limits. False for upper limits.
        /// \param bodyA True for bodyA. False for bodyB.
        ///
        glm::vec3 getLimits(const bool translation, const bool lower, const bool bodyA) const;

        /// Set new limits to the desired axis.
        /// lower = upper -> axis is locked.
        /// lower > upper -> axis is free.
        /// lower < upper -> axis is restricted to the range.
        ///
        /// \param translation True for translation axes. False for rotation axes.
        /// \param lower True for lower limits. False for upper limits.
        /// \param bodyA True for bodyA. False for bodyB.
        ///
        /// \return Returns reference to self.
        ///
        PulleyJoint& setLimits(const glm::vec3& limit, const bool translation, const bool lower, const bool bodyA);

        /// Set the same new limits to the desired axis for both bodies at once.
        ///
        /// \return Returns reference to self.
        ///
        PulleyJoint& setLimits(const glm::vec3& limit, const bool translation, const bool lower);

    private:

        btGeneric6DofConstraint* m_jointL;
        btGeneric6DofConstraint* m_jointL2;

        std::unique_ptr<btTypedConstraint> m_jointP;

        float m_ratio;
    };
}
#endif