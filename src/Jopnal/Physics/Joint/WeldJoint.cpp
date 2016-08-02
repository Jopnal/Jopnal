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

#include <Jopnal/Physics/Joint/WeldJoint.hpp>

#include <Jopnal/STL.hpp>
#include <BulletDynamics/ConstraintSolver/btFixedConstraint.h>

#endif

//////////////////////////////////////////////


namespace jop
{
    WeldJoint::WeldJoint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide, const glm::vec3& jPos) :
        Joint(worldRef, bodyA, bodyB, collide),
        m_jointL(nullptr),
        m_damping(1.f)
    {
        btTransform ctwt = btTransform::getIdentity();
        
        glm::vec3 p = defaultCenter(jPos);
        ctwt.setOrigin(btVector3(p.x, p.y, p.z));
        
        ctwt.setRotation(btQuaternion(0.f, 0.f, 0.f, 1.f));
        btTransform tInA = getBody(bodyA)->getCenterOfMassTransform().inverse() * ctwt;
        btTransform tInB = getBody(bodyB)->getCenterOfMassTransform().inverse() * ctwt;

        m_joint = std::make_unique<btFixedConstraint>(*getBody(bodyA), *getBody(bodyB), tInA, tInB);
        getWorld(worldRef).addConstraint(m_joint.get(), !collide);
        m_jointL = static_cast<btFixedConstraint*>(m_joint.get());

        for (unsigned int i = 0; i < 6; ++i)
        {
            m_jointL->setDamping(i, m_damping, false);
        }
    }

    float WeldJoint::getDamping() const
    {
        return m_damping;
    }

    WeldJoint& WeldJoint::setDamping(const float damping)
    {
        for (unsigned int i = 0; i < 6; ++i)
        {
            m_jointL->setDamping(i, damping);
        }
        m_damping = damping;
        return *this;
    }
}