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

    #include <Jopnal/Physics/Joint/PistonJoint.hpp>

    #include <Jopnal/STL.hpp>
    #include <Jopnal/Physics/Detail/WorldImpl.hpp>
    #include <BulletDynamics/ConstraintSolver/btSliderConstraint.h>

#endif

//////////////////////////////////////////////

namespace jop
{
    PistonJoint::PistonJoint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide, const glm::vec3& anchor, const glm::quat& jRot) :
        Joint(worldRef, bodyA, bodyB, collide),
        m_jointL(nullptr)
    {
        btTransform ctwt = btTransform::getIdentity();
        glm::vec3 p = defaultCenter(anchor);
        ctwt.setOrigin(btVector3(p.x, p.y, p.z));
        ctwt.setRotation(btQuaternion(jRot.x, jRot.y, jRot.z, jRot.w));

        btTransform tInA = getBody(bodyA)->getCenterOfMassTransform().inverse() * ctwt;
        btTransform tInB = getBody(bodyB)->getCenterOfMassTransform().inverse() * ctwt;

        m_joint = std::make_unique<btSliderConstraint>(*getBody(bodyA), *getBody(bodyB), tInA, tInB, true);
        getWorld(worldRef).addConstraint(m_joint.get(), !collide);
        m_jointL = static_cast<btSliderConstraint*>(m_joint.get());
    }

    //////////////////////////////////////////////

    std::pair<float, float> PistonJoint::getAngLimits() const
    {
        return std::make_pair(m_jointL->getLowerAngLimit(), m_jointL->getUpperAngLimit());
    }

    //////////////////////////////////////////////

    std::pair<float, float> PistonJoint::getMoveLimits() const
    {
        return std::make_pair(m_jointL->getLowerLinLimit(), m_jointL->getUpperLinLimit());
    }

    //////////////////////////////////////////////

    PistonJoint& PistonJoint::setAngLimits(const float min, const float max)
    {
        m_jointL->setLowerAngLimit(min);
        m_jointL->setUpperAngLimit(max);
        return *this;
    }

    //////////////////////////////////////////////

    PistonJoint& PistonJoint::setMoveLimits(const float min, const float max)
    {
        m_jointL->setLowerLinLimit(min);
        m_jointL->setUpperLinLimit(max);
        return *this;
    }

    //////////////////////////////////////////////

    PistonJoint& PistonJoint::enableAngMotor(const bool enable)
    {
        m_jointL->setPoweredAngMotor(enable);
        return *this;
    }

    //////////////////////////////////////////////

    PistonJoint& PistonJoint::enableMoveMotor(const bool enable)
    {
        m_jointL->setPoweredLinMotor(enable);
        return *this;
    }

    //////////////////////////////////////////////

    std::pair<float, float> PistonJoint::getAngMotorForces() const
    {
        return std::make_pair(m_jointL->getTargetAngMotorVelocity(), m_jointL->getMaxAngMotorForce());
    }

    //////////////////////////////////////////////

    std::pair<float, float> PistonJoint::getMoveMotorForces() const
    {
        return std::make_pair(m_jointL->getTargetLinMotorVelocity(), m_jointL->getMaxLinMotorForce());
    }

    //////////////////////////////////////////////

    PistonJoint& PistonJoint::setAngMotorForces(const float speed, const float force)
    {
        m_jointL->setTargetAngMotorVelocity(speed);
        m_jointL->setMaxAngMotorForce(force);
        return *this;
    }

    //////////////////////////////////////////////

    PistonJoint& PistonJoint::setMoveMotorForces(const float speed, const float force)
    {
        m_jointL->setTargetLinMotorVelocity(speed);
        m_jointL->setMaxLinMotorForce(force);
        return *this;
    }
}