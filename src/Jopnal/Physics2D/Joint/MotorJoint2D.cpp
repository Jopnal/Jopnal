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

#include <Jopnal/Physics2D/Joint/MotorJoint2D.hpp>

#include <Jopnal/STL.hpp>
#include <Box2D/Dynamics/Joints/b2Joint.hpp>

#endif

//////////////////////////////////////////////

namespace jop
{

    MotorJoint2D::MotorJoint2D(World2D& worldRef, RigidBody2D& bodyA, RigidBody2D& bodyB) :
        Joint2D(worldRef, bodyA, bodyB),
        m_jointL(nullptr)
    {
        b2MotorJointDef jointDef;
        jointDef.Initialize(getBody(bodyA), getBody(bodyB));
        jointDef.userData = this;

        m_joint = static_cast<b2MotorJoint*>(getBody(bodyA)->GetWorld()->CreateJoint(&jointDef));
        m_jointL = static_cast<b2MotorJoint*>(m_joint);
    }

    glm::vec2 MotorJoint2D::getLinearOffset()
    {
        return glm::vec2(m_jointL->GetLinearOffset().x, m_jointL->GetLinearOffset().y);
    }

    float MotorJoint2D::getAngularOffset()
    {
        return m_jointL->GetAngularOffset();
    }

    MotorJoint2D& MotorJoint2D::setLinearOffset(const glm::vec2& offset)
    {
        m_jointL->SetLinearOffset(b2Vec2(offset.x, offset.y));
        return *this;
    }

    MotorJoint2D& MotorJoint2D::setAngularOffset(const float offset)
    {
        m_jointL->SetAngularOffset(offset);
        return *this;
    }

    MotorJoint2D& MotorJoint2D::setForces(const float force, const float torque)
    {
        m_jointL->SetMaxForce(force);
        m_jointL->SetMaxTorque(torque);
        return *this;
    }
}