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

#include <Jopnal/Physics/Joint/PulleyJoint.hpp>

#include <Jopnal/STL.hpp>
#include <BulletDynamics/ConstraintSolver/btSliderConstraint.h>

#endif

//////////////////////////////////////////////

namespace jop
{
    PulleyJoint::PulleyJoint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide, const float ratio, const glm::vec3& jPos, const glm::quat& jRot) :
        Joint(worldRef, bodyA, bodyB, collide),
        m_jointL(nullptr),
        m_jointL2(nullptr),
        m_jointP(nullptr),
        m_ratio(ratio)
    {
        btTransform ctwt = btTransform::getIdentity();
        glm::vec3 p = defaultCenter(jPos);
        ctwt.setOrigin(btVector3(p.x, p.y, p.z));
        ctwt.setRotation(btQuaternion(jRot.x, jRot.y, jRot.z, jRot.w));

        btTransform tInA = getBody(bodyA)->getCenterOfMassTransform().inverse() * ctwt;
        btTransform tInB = getBody(bodyB)->getCenterOfMassTransform().inverse() * ctwt;

        if (!collide)
        {
            //set rb-filters so that no collisions between them
        }

        m_joint = std::make_unique<btGeneric6DofConstraint>(*getBody(bodyA), tInA, true);
        getWorld(worldRef).addConstraint(m_joint.get(), !collide);
        m_jointL = static_cast<btGeneric6DofConstraint*>(m_joint.get());

        m_jointP = std::make_unique<btGeneric6DofConstraint>(*getBody(bodyB), tInB, true);
        getWorld(worldRef).addConstraint(m_jointP.get(), !collide);
        m_jointL2 = static_cast<btGeneric6DofConstraint*>(m_jointP.get());
    }

    PulleyJoint::~PulleyJoint()
    {
        getWorld(m_bodyA->getWorld()).removeConstraint(m_jointP.get());
    }

    glm::vec3 PulleyJoint::getLimits(const bool translation, const bool lower, const bool bodyA) const
    {
        btVector3 t_limits;

        if (translation) //Translation
            if (lower) //Lower limits
                if (bodyA)
                    m_jointL->getLinearLowerLimit(t_limits);
                else
                    m_jointL2->getLinearLowerLimit(t_limits);
            else //Upper limits
                if (bodyA)
                    m_jointL->getLinearUpperLimit(t_limits);
                else
                    m_jointL2->getLinearUpperLimit(t_limits);
        else //Rotation
            if (lower) //Lower limits
                if (bodyA)
                    m_jointL->getAngularLowerLimit(t_limits);
                else
                    m_jointL2->getAngularLowerLimit(t_limits);
            else //Upper limits
                if (bodyA)
                    m_jointL->getAngularUpperLimit(t_limits);
                else
                    m_jointL2->getAngularUpperLimit(t_limits);

        return glm::vec3(t_limits.m_floats[0], t_limits.m_floats[1], t_limits.m_floats[2]);
    }

    PulleyJoint& PulleyJoint::setLimits(const glm::vec3& limit, const bool translation, const bool lower, const bool bodyA)
    {
        btVector3 t_limits(limit.x, limit.y, limit.z);

        if (translation) //Translation
            if (lower) //Lower limits
                if (bodyA)
                    m_jointL->setLinearLowerLimit(t_limits);
                else
                    m_jointL2->setLinearLowerLimit(t_limits);
            else //Upper limits
                if (bodyA)
                    m_jointL->setLinearUpperLimit(t_limits);
                else
                    m_jointL2->setLinearUpperLimit(t_limits);
        else //Rotation
            if (lower) //Lower limits
                if (bodyA)
                    m_jointL->setAngularLowerLimit(t_limits);
                else
                    m_jointL2->setAngularLowerLimit(t_limits);
            else //Upper limits
                if (bodyA)
                    m_jointL->setAngularUpperLimit(t_limits);
                else
                    m_jointL2->setAngularUpperLimit(t_limits);

        return *this;
    }

    PulleyJoint& PulleyJoint::setLimits(const glm::vec3& limit, const bool translation, const bool lower)
    {
        btVector3 t_limits(limit.x, limit.y, limit.z);

        if (translation) //Translation
            if (lower) //Lower limits
            {
                m_jointL->setLinearLowerLimit(t_limits);
                m_jointL2->setLinearLowerLimit(t_limits);
            }
            else //Upper limits
            {
                m_jointL->setLinearUpperLimit(t_limits);
                m_jointL2->setLinearUpperLimit(t_limits);
            }
        else //Rotation
            if (lower) //Lower limits
            {
                m_jointL->setAngularLowerLimit(t_limits);
                m_jointL2->setAngularLowerLimit(t_limits);
            }
            else //Upper limits
            {
                m_jointL->setAngularUpperLimit(t_limits);
                m_jointL2->setAngularUpperLimit(t_limits);
            }

        return *this;
    }
}