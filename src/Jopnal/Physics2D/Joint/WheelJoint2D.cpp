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

#include <Jopnal/Physics2D/Joint/WheelJoint2D.hpp>

#include <Jopnal/STL.hpp>
#include <Box2D/Dynamics/Joints/b2Joint.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{

    WheelJoint2D::WheelJoint2D(World2D& worldRef, RigidBody2D& bodyA, RigidBody2D& bodyB,
        const bool collide, const glm::vec2& axis,
        const glm::vec2& localAnchorA, const glm::vec2& localAnchorB
        ) :
        Joint2D(worldRef, bodyA, bodyB),
        m_jointL(nullptr)
    {
        b2WheelJointDef jointDef;

        jointDef.bodyA = getBody(bodyA);
        jointDef.bodyB = getBody(bodyB);
        jointDef.collideConnected = collide;
        jointDef.localAxisA = b2Vec2(axis.x, axis.y);
        jointDef.localAnchorA = b2Vec2(localAnchorA.x, localAnchorA.y);
        jointDef.localAnchorB = b2Vec2(localAnchorB.x, localAnchorB.y);
        jointDef.userData = this;

        m_joint = static_cast<b2WheelJoint*>(getBody(bodyA)->GetWorld()->CreateJoint(&jointDef));
        m_jointL = static_cast<b2WheelJoint*>(m_joint);
    }

    WheelJoint2D& WheelJoint2D::enableMotor(const bool enable)
    {
        m_jointL->EnableMotor(enable);
        return *this;
    }

    WheelJoint2D& WheelJoint2D::setMotorForces(const float speed, const float torque)
    {
        m_jointL->SetMotorSpeed(speed);
        m_jointL->SetMaxMotorTorque(torque);
        return *this;
    }

    std::pair<float, float> WheelJoint2D::getMotorForces() const
    {
        return std::make_pair<float, float>(m_jointL->GetMotorSpeed(), m_jointL->GetMaxMotorTorque());
    }

    WheelJoint2D& WheelJoint2D::setDamping(const float frequency, const float damping)
    {
        m_jointL->SetSpringFrequencyHz(frequency);
        m_jointL->SetSpringDampingRatio(damping);
        return *this;
    }

    std::pair<float, float> WheelJoint2D::getDamping() const
    {
        return std::make_pair(m_jointL->GetSpringFrequencyHz(), m_jointL->GetSpringDampingRatio());
    }
}