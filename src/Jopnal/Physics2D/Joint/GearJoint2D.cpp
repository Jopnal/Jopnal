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

    #include <Jopnal/Physics2D/Joint/GearJoint2D.hpp>

    #include <Jopnal/Utility/Assert.hpp>
    #include <Jopnal/STL.hpp>
    #include <Box2D/Dynamics/Joints/b2GearJoint.h>
    #include <Box2D/Dynamics/b2Body.h>
    #include <Box2D/Dynamics/b2World.h>

#endif

//////////////////////////////////////////////

namespace jop
{

    GearJoint2D::GearJoint2D(World2D& worldRef, RigidBody2D& bodyA, RigidBody2D& bodyB, const bool collide, const float ratio, const Joint2D& joint1, const Joint2D& joint2) :
        Joint2D(worldRef, bodyA, bodyB),
        m_jointL(nullptr)
    {
        b2GearJointDef jointDef;
        jointDef.bodyA = getBody(bodyA);
        jointDef.bodyB = getBody(bodyB);
        jointDef.collideConnected = collide;
        jointDef.ratio = ratio;

        if ((joint1.m_joint->GetType() == b2JointType::e_prismaticJoint || joint1.m_joint->GetType() == b2JointType::e_revoluteJoint) &&
            (joint2.m_joint->GetType() == b2JointType::e_prismaticJoint || joint2.m_joint->GetType() == b2JointType::e_revoluteJoint))
        {
            jointDef.joint1 = static_cast<b2Joint*>(joint1.m_joint);
            jointDef.joint2 = static_cast<b2Joint*>(joint2.m_joint);
        }
        else
            JOP_ASSERT(false, "GearJoint2D can only be constructed with any combination of PistonJoint2Ds and RotationJoint2Ds.");

        m_joint = static_cast<b2GearJoint*>(getBody(bodyA)->GetWorld()->CreateJoint(&jointDef));
        m_jointL = static_cast<b2GearJoint*>(m_joint);
    }

    void GearJoint2D::setRatio(const float ratio)
    {
        m_jointL->SetRatio(ratio);
    }
}