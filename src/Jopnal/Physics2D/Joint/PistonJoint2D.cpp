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

    #include <Jopnal/Physics2D/Joint/PistonJoint2D.hpp>

    #include <Jopnal/Utility/Assert.hpp>
    #include <Jopnal/STL.hpp>
    #include <Box2D/Dynamics/Joints/b2PrismaticJoint.h>
    #include <Box2D/Dynamics/b2Body.h>
    #include <Box2D/Dynamics/b2World.h>

#endif

//////////////////////////////////////////////


namespace jop
{
    PistonJoint2D::PistonJoint2D(World2D& worldRef, RigidBody2D& bodyA, RigidBody2D& bodyB, const bool collide, const glm::vec2& axis) :
        Joint2D(worldRef, bodyA, bodyB),
        m_jointL(nullptr)
    {
        b2PrismaticJointDef jointDef;
        jointDef.Initialize(getBody(bodyA), getBody(bodyB), getBody(bodyA)->GetWorldCenter(), b2Vec2(axis.x, axis.y));
        jointDef.collideConnected = collide;
        jointDef.userData = this;

        m_joint = static_cast<b2PrismaticJoint*>(getBody(bodyA)->GetWorld()->CreateJoint(&jointDef));
        m_jointL = static_cast<b2PrismaticJoint*>(m_joint);
    }

    PistonJoint2D& PistonJoint2D::limit(const bool enable)
    {
        m_jointL->EnableLimit(enable);
        return *this;
    }

    PistonJoint2D& PistonJoint2D::setLimits(const float min, const float max)
    {
        m_jointL->EnableLimit(true);
        m_jointL->SetLimits(min, max);
        return *this;
    }

    std::pair<float, float> PistonJoint2D::getLimits()
    {
        return std::make_pair<float, float>(m_jointL->GetLowerLimit(), m_jointL->GetUpperLimit());
    }

    PistonJoint2D& PistonJoint2D::enableMotor(const bool set)
    {
        m_jointL->EnableMotor(set);
        return *this;
    }

    PistonJoint2D& PistonJoint2D::setMotorForces(const float speed, const float force)
    {
        JOP_ASSERT(force >= 0.f, "PistonJoint2D force can not be negative!");

        m_jointL->SetMotorSpeed(speed);
        m_jointL->SetMaxMotorForce(force);
        return *this;
    }

    std::pair<float, float> PistonJoint2D::getMotorForces()
    {
        return std::make_pair<float, float>(m_jointL->GetMotorSpeed(), m_jointL->GetMaxMotorForce());
    }

}