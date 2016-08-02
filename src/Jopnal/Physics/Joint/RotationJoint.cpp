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

// Headers
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

#include <Jopnal/Physics/Joint/RotationJoint.hpp>

#include <Jopnal/STL.hpp>
#include <BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h>

#endif

//////////////////////////////////////////////

namespace jop
{
    RotationJoint::RotationJoint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide, const glm::vec3& jPos) :
        Joint(worldRef, bodyA, bodyB, collide),
        m_jointL(nullptr)
    {
        btTransform ctwt = btTransform::getIdentity();
        glm::vec3 p = defaultCenter(jPos);
        ctwt.setOrigin(btVector3(p.x, p.y, p.z));

        btTransform tInA = getBody(bodyA)->getCenterOfMassTransform().inverse() * ctwt;
        btTransform tInB = getBody(bodyB)->getCenterOfMassTransform().inverse() * ctwt;

        m_joint = std::make_unique<btGeneric6DofConstraint>(*getBody(bodyA), *getBody(bodyB), tInA, tInB, true);
        getWorld(worldRef).addConstraint(m_joint.get(), !collide);
        m_jointL = static_cast<btGeneric6DofConstraint*>(m_joint.get());

        for (unsigned int i = 3; i <= 5; ++i)
            m_jointL->setLimit(i, 1.f, 0.f);
    }

    glm::vec3 RotationJoint::getAngLimits(const bool lower) const
    {
        btVector3 temp;
        if (lower)
            m_jointL->getAngularLowerLimit(temp);
        else
            m_jointL->getAngularUpperLimit(temp);

        return glm::vec3(temp.m_floats[0], temp.m_floats[1], temp.m_floats[2]);
    }

    RotationJoint& RotationJoint::setAngLimits(const bool lower, const glm::vec3& limits)
    {
        btVector3 temp(limits.x, limits.y, limits.z);
        if (lower)
            m_jointL->setAngularLowerLimit(temp);
        else
            m_jointL->setAngularUpperLimit(temp);
        return *this;
    }
}