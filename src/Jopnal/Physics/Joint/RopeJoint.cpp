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

#include <Jopnal/Physics/Joint/RopeJoint.hpp>

#include <Jopnal/STL.hpp>
#include <BulletDynamics/ConstraintSolver/btFixedConstraint.h>
#include <BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h>

#endif

//////////////////////////////////////////////

namespace jop
{
    RopeJoint::RopeJoint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide) :
        Joint(worldRef, bodyA, bodyB, collide),
        m_jointL(nullptr)
    {
        btTransform ctwt = btTransform::getIdentity();
        ctwt.setOrigin(getBody(bodyA)->getWorldTransform().getOrigin());
        btTransform tInA = getBody(bodyA)->getCenterOfMassTransform().inverse() * ctwt;
        btTransform tInB = getBody(bodyB)->getCenterOfMassTransform().inverse() * ctwt;

        m_joint = std::make_unique<btGeneric6DofConstraint>(*getBody(bodyA), *getBody(bodyB), tInA, tInB, false);
        getWorld(worldRef).addConstraint(m_joint.get(), !collide);
        m_jointL = static_cast<btGeneric6DofConstraint*>(m_joint.get());
    }

    RopeJoint& RopeJoint::lockAxis(const bool lock, const unsigned int axis)
    {
        JOP_ASSERT(axis < 3u, "RopeJoint::lockAxis: Invalid axis value.");

        if (lock)
            m_jointL->setLimit(axis + 3u, 0.f, 0.f);
        else
            m_jointL->setLimit(axis + 3u, 1.f, 0.f);
        return *this;
    }


}