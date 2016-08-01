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

#include <Jopnal/Physics2D/Joint/WeldJoint2D.hpp>

#include <Jopnal/STL.hpp>
#include <Box2D/Dynamics/Joints/b2Joint.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{

    WeldJoint2D::WeldJoint2D(World2D& worldRef, RigidBody2D& bodyA, RigidBody2D& bodyB, const bool collide) :
        Joint2D(worldRef, bodyA, bodyB),
        m_jointL(nullptr)
    {
        b2WeldJointDef jointDef;
        jointDef.Initialize(getBody(bodyA), getBody(bodyB), getBody(bodyA)->GetWorldCenter());
        jointDef.collideConnected = collide;
        jointDef.userData = this;

        m_joint = static_cast<b2WeldJoint*>(getBody(bodyA)->GetWorld()->CreateJoint(&jointDef));
        m_jointL = static_cast<b2WeldJoint*>(m_joint);
    }

    WeldJoint2D& WeldJoint2D::setDamping(const float frequency, const float damping)
    {
        JOP_ASSERT(frequency >= 0.f, "WeldJoint2D damping frequency can not be negative!");
        JOP_ASSERT(damping >= 0.f, "WeldJoint2D damping ratio can not be negative!");

        m_jointL->SetFrequency(frequency);
        m_jointL->SetDampingRatio(damping);
        return *this;
    }

    std::pair<float, float> WeldJoint2D::getDamping() const
    {
        return std::make_pair(m_jointL->GetFrequency(), m_jointL->GetDampingRatio());
    }
}