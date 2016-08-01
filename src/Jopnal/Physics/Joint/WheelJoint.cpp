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
#include <BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.h>

#endif

//////////////////////////////////////////////

namespace jop
{
    WheelJoint::WheelJoint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide, const float maxSteering, const glm::quat& jRot, const glm::vec3& jPos) :
        Joint(worldRef, bodyA, bodyB),
        m_jointL(nullptr),
        m_maxAngle(maxSteering)
    {
        btTransform ctwt = btTransform::getIdentity();

        if (jRot == glm::quat(0.f, 0.f, 0.f, 0.f))
        {
            getBody(bodyB)->getCenterOfMassPosition().x() >
                getBody(bodyA)->getCenterOfMassPosition().x() ?
                ctwt.setRotation(btQuaternion(jRot.x, jRot.y, jRot.z, 1.f)) :
                ctwt.setRotation(btQuaternion(jRot.x, 1.f, jRot.z, jRot.w));
        }
        else
            ctwt.setRotation(btQuaternion(jRot.x, jRot.y, jRot.z, jRot.w));

        if (jPos == glm::vec3(0.f, 0.f, FLT_MAX))
            ctwt.setOrigin(getBody(bodyB)->getWorldTransform().getOrigin());
        else
        {
            glm::vec3 p = defaultCenter(jPos);
            ctwt.setOrigin(btVector3(p.x, p.y, p.z));
        }

        btTransform tInA = getBody(bodyA)->getCenterOfMassTransform().inverse() * ctwt;
        btTransform tInB = getBody(bodyB)->getCenterOfMassTransform().inverse() * ctwt;

        m_joint = std::make_unique<btGeneric6DofSpringConstraint>(*getBody(bodyA), *getBody(bodyB), tInA, tInB, false);
        getWorld(worldRef).addConstraint(m_joint.get(), !collide);
        m_jointL = static_cast<btGeneric6DofSpringConstraint*>(m_joint.get());

        //Axis locks
        {
            for (int i = 0; i < 6; ++i)
                m_jointL->setLimit(i, 0.f, 0.f);

            m_jointL->setLimit(static_cast<unsigned int>(Axis::R_X), 1.f, 0.f); //X-rotation free
        }
    }

    WheelJoint& WheelJoint::applyTorque(const float torque, const Axis axis)
    {
        glm::vec3 tq(0.f, 0.f, 0.f);
        tq[static_cast<unsigned int>(axis)] = torque;

        return applyTorque(tq);
    }

    WheelJoint& WheelJoint::applyTorque(glm::vec3 torque)
    {
        m_jointL->getRigidBodyB().activate();
        m_jointL->getRigidBodyB().applyTorque(m_jointL->getRigidBodyB().getWorldTransform().getBasis() * btVector3(torque.x, torque.y, torque.z));
        return *this;
    }

    float WheelJoint::getTorque(const Axis axis) const
    {
        return m_jointL->getRigidBodyB().getTotalTorque().m_floats[static_cast<unsigned int>(axis)];
    }

    glm::vec3 WheelJoint::getTorque() const
    {
        btVector3 tq = m_jointL->getRigidBodyB().getTotalTorque();
        return glm::vec3(tq.m_floats[0], tq.m_floats[1], tq.m_floats[2]);
    }

    float WheelJoint::getAngle(const Axis axis) const
    {
        btVector3 angles(0.f, 0.f, 0.f);
        m_jointL->getAngularLowerLimit(angles);

        return angles[static_cast<unsigned int>(axis)];
    }

    float WheelJoint::getStiffness(const Axis axis)
    {
        return m_jointL->getStiffness(static_cast<unsigned int>(axis));
    }

    WheelJoint& WheelJoint::setAngle(const float steeringAngle, const Axis axis)
    {
        unsigned int t_axis = static_cast<unsigned int>(axis);
        t_axis = axis > Axis::T_Z ? t_axis : t_axis + 3u;

        m_jointL->getRigidBodyB().activate();
        float newAngle = std::min(std::max(steeringAngle, -m_maxAngle), m_maxAngle);
        m_jointL->setLimit(t_axis, newAngle, newAngle);
        return *this;
    }

    WheelJoint& WheelJoint::setStiffness(const float stiffness, const Axis axis)
    {
        m_jointL->enableSpring(static_cast<unsigned int>(axis), true);
        m_jointL->setStiffness(static_cast<unsigned int>(axis), stiffness);
        return *this;
    }
}