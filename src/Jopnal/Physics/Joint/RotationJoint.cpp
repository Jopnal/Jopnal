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
#include <Box/Dynamics/Joints/b2Joint.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    RotationJoint::RotationJoint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide, const glm::vec3& jPos, const glm::quat& jRot) :
        Joint(worldRef, bodyA, bodyB),
        m_jointL(nullptr)
    {
        btTransform ctwt = btTransform::getIdentity();
        ctwt.setOrigin(btVector3(jPos.x, jPos.y, jPos.z));
        ctwt.setRotation(btQuaternion(jRot.x, jRot.y, jRot.z, jRot.w));
        btTransform tInA = getBody(bodyA)->getCenterOfMassTransform().inverse() * ctwt;
        btTransform tInB = getBody(bodyB)->getCenterOfMassTransform().inverse() * ctwt;

        m_joint = std::make_unique<btHingeConstraint>(*getBody(bodyA), *getBody(bodyB), tInA, tInB, true);
        getWorld(worldRef).addConstraint(m_joint.get(), !collide);
        m_jointL = static_cast<btHingeConstraint*>(m_joint.get());
    }

    RotationJoint& RotationJoint::enableMotor(const bool enable)
    {
        m_jointL->enableMotor(enable);
        return *this;
    }

    std::pair<float, float> RotationJoint::getAngLimits() const
    {
        return std::make_pair<float, float>(m_jointL->getLowerLimit(), m_jointL->getUpperLimit());
    }

    std::pair<float, float> RotationJoint::getAngForces() const
    {
        return std::make_pair<float, float>(m_jointL->getMotorTargetVelosity(), m_jointL->getMaxMotorImpulse());
    }

    RotationJoint& RotationJoint::setAngLimits(const float min, const float max)
    {
        m_jointL->setLimit(min, max);
        return *this;
    }

    RotationJoint& RotationJoint::setAngForces(const float speed, const float force)
    {
        m_jointL->setMotorTargetVelocity(speed);
        m_jointL->setMaxMotorImpulse(force);
        return *this;
    }
}