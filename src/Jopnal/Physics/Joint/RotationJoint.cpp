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

    RotationJoint::RotationJoint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide) :
        Joint(worldRef, bodyA, bodyB),
        m_jointL(nullptr)
    {
       // bistanceJointDef jd;
       //
       // b2RevoluteJointDef jointDef;
       // jointDef.Initialize(getBody(bodyA), getBody(bodyB), getBody(bodyA)->GetWorldCenter());
       // jointDef.collideConnected = collide;
       //
       // m_joint = static_cast<b2RevoluteJoint*>(getBody(bodyA)->GetWorld()->CreateJoint(&jointDef));
       // m_jointL = static_cast<b2RevoluteJoint*>(m_joint);
    }
/*
    RotationJoint& RotationJoint::limit(const bool enable)
    {
        m_jointL->EnableLimit(enable);
        return *this;
    }

    RotationJoint& RotationJoint::setLimits(const float minAngle, const float maxAngle)
    {
        m_jointL->EnableLimit(true);
        m_jointL->SetLimits(minAngle, maxAngle);
        return *this;
    }

    std::pair<float, float> RotationJoint::getLimits()
    {
        return std::make_pair<float, float>(m_jointL->GetLowerLimit(), m_jointL->GetUpperLimit());
    }

    RotationJoint& RotationJoint::enableMotor(const bool enable)
    {
        m_jointL->EnableMotor(enable);
        return *this;
    }

    RotationJoint& RotationJoint::setMotorForces(const float speed, const float torque)
    {
        m_jointL->SetMotorSpeed(speed);
        m_jointL->SetMaxMotorTorque(torque);
        return *this;
    }

    std::pair<float, float> RotationJoint::getMotorForces()
    {
        return std::make_pair<float, float>(m_jointL->GetMotorSpeed(), m_jointL->GetMaxMotorTorque());
    }*/

}