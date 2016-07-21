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

#include <Jopnal/Physics2D/Joint/RotationJoint2D.hpp>

#include <Jopnal/STL.hpp>
#include <Box2D/Dynamics/Joints/b2Joint.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{

    RotationJoint2D::RotationJoint2D(World2D& worldRef, RigidBody2D& bodyA, RigidBody2D& bodyB, const bool collide) : 
        Joint2D(worldRef, bodyA, bodyB),
        m_jointL(nullptr)
    {
        b2DistanceJointDef jd;

        b2RevoluteJointDef jointDef;
        jointDef.Initialize(getBody(bodyA), getBody(bodyB), getBody(bodyA)->GetWorldCenter());
        jointDef.collideConnected = collide;

        m_joint = static_cast<b2RevoluteJoint*>(getBody(bodyA)->GetWorld()->CreateJoint(&jointDef));
        m_jointL = static_cast<b2RevoluteJoint*>(m_joint);
    }

    RotationJoint2D& RotationJoint2D::limit(const bool limit)
    {
        m_jointL->EnableLimit(limit);
        return *this;
    }

    RotationJoint2D& RotationJoint2D::setLimits(const float minAngle, const float maxAngle)
    {
        m_jointL->EnableLimit(true);
        m_jointL->SetLimits(minAngle, maxAngle);
        return *this;
    }

    RotationJoint2D& RotationJoint2D::enableMotor(const bool set)
    {
        m_jointL->EnableMotor(set);
        return *this;
    }

    RotationJoint2D& RotationJoint2D::setMotor(const float speed, const float torque)
    {
        m_jointL->EnableMotor(true);
        m_jointL->SetMotorSpeed(speed);
        m_jointL->SetMaxMotorTorque(torque);
        return *this;
    }

}