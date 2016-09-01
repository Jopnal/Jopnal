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

#include <Jopnal/Physics/Joint/GearJoint.hpp>

#include <Jopnal/STL.hpp>
#include <Jopnal/Physics/Detail/WorldImpl.hpp>
#include <BulletDynamics/ConstraintSolver/btGearConstraint.h>

#endif

//////////////////////////////////////////////

namespace jop
{
    GearJoint::GearJoint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide, const glm::vec3& rotAxisA, const glm::vec3& rotAxisB, const float ratio) :
        Joint(worldRef, bodyA, bodyB, collide),
        m_jointL(nullptr)
    {
        m_joint = std::make_unique<btGearConstraint>(*getBody(bodyA), *getBody(bodyB), btVector3(rotAxisA.x, rotAxisA.y, rotAxisA.z), btVector3(rotAxisB.x, rotAxisB.y, rotAxisB.z), ratio);

        getWorld(worldRef).addConstraint(m_joint.get(), !collide);
        m_jointL = static_cast<btGearConstraint*>(m_joint.get());
    }

    //////////////////////////////////////////////

    glm::vec3 GearJoint::getAxis(const bool bodyA) const
    {
        return bodyA ?
            glm::vec3(m_jointL->getAxisA().m_floats[0], m_jointL->getAxisA().m_floats[1], m_jointL->getAxisA().m_floats[2]) :
            glm::vec3(m_jointL->getAxisB().m_floats[0], m_jointL->getAxisB().m_floats[1], m_jointL->getAxisB().m_floats[2]);
    }

    //////////////////////////////////////////////

    float GearJoint::getRatio() const
    {
        return m_jointL->getRatio();
    }

    //////////////////////////////////////////////

    GearJoint& GearJoint::setAxis(const glm::vec3& axis, const bool bodyA)
    {
        btVector3 _axis(axis.x, axis.y, axis.z);
        bodyA ?
            m_jointL->setAxisA(_axis) :
            m_jointL->setAxisB(_axis);

        return *this;
    }

    //////////////////////////////////////////////

    GearJoint& GearJoint::setRatio(const float ratio)
    {
        m_jointL->setRatio(ratio);
        return *this;
    }
}