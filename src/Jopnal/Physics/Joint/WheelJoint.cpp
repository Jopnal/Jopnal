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

#include <Jopnal/Physics/Joint/WheelJoint.hpp>

#include <Jopnal/STL.hpp>
#include <Box/Dynamics/Joints/b2Joint.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    WheelJoint::WheelJoint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide, const glm::vec3& jPos, const bool enableSteering,
        const glm::quat& jRot, const float steeringAngle) :
        Joint(worldRef, bodyA, bodyB),
        m_jointL(nullptr)
    {
        btTransform ctwt = btTransform::getIdentity();
        ctwt.setOrigin(btVector3(jPos.x, jPos.y, jPos.z));

        if (jRot == glm::quat(0.f, 0.f, 0.f, 0.f))
        {
            getBody(bodyB)->getCenterOfMassPosition().m_floats[0] >
                getBody(bodyA)->getCenterOfMassPosition().m_floats[0] ?
                ctwt.setRotation(btQuaternion(jRot.x, jRot.y, jRot.z, 1.f)) :
                ctwt.setRotation(btQuaternion(jRot.x, 1.f, jRot.z, jRot.w));

            JOP_DEBUG_ERROR("WHEELJOINT VECTOR Y U NO WORK WITH X BUT M_ ?!");
        }

        btTransform tInA = getBody(bodyA)->getCenterOfMassTransform().inverse() * ctwt; //mounting point
        btTransform tInB = getBody(bodyB)->getCenterOfMassTransform().inverse() * ctwt; //tyre center

        m_joint = std::make_unique<btGeneric6DofConstraint>(*getBody(bodyA), *getBody(bodyB), tInA, tInB, false);
        getWorld(worldRef).addConstraint(m_joint.get(), !collide);
        m_jointL = static_cast<btGeneric6DofConstraint*>(m_joint.get());

        //Axis locks
        {
            for (int i = 0; i < 3; ++i)
                m_jointL->setLimit(i, 0.f, 0.f);

            m_jointL->setLimit(3, 1.f, 0.f); //X-rotation free

            enableSteering ?
                m_jointL->setLimit(4, -steeringAngle, steeringAngle) : m_jointL->setLimit(4, 0.f, 0.f);

            m_jointL->setLimit(5, 0.f, 0.f); //camber, disabled
        }
    }

    /*WheelJoint& WheelJoint::applyForce(const float force)
    {

    return *this;
    }

    std::pair<float, float> WheelJoint::getAngLimits() const
    {
    return std::make_pair<float, float>(m_jointL->getLowerLimit(), m_jointL->getUpperLimit());
    }

    std::pair<float, float> WheelJoint::getAngForces() const
    {
    return std::make_pair<float, float>(m_jointL->getMotorTargetVelosity(), m_jointL->getMaxMotorImpulse());
    }

    WheelJoint& WheelJoint::setAngLimits(const float min, const float max)
    {
    m_jointL->setLimit(min, max);
    return *this;
    }

    WheelJoint& WheelJoint::setAngForces(const float speed, const float force)
    {
    m_jointL->setMotorTargetVelocity(speed);
    m_jointL->setMaxMotorImpulse(force);
    return *this;
    }*/
}