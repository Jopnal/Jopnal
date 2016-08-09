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

    #include <Jopnal/Physics2D/Joint/RopeJoint2D.hpp>

    #include <Jopnal/Utility/Assert.hpp>
    #include <Jopnal/STL.hpp>
    #include <Box2D/Dynamics/Joints/b2RopeJoint.h>
    #include <Box2D/Dynamics/Joints/b2DistanceJoint.h>
    #include <Box2D/Dynamics/b2Body.h>
    #include <Box2D/Dynamics/b2World.h>
    #include <cmath>

#endif

//////////////////////////////////////////////

namespace jop
{
    RopeJoint2D::RopeJoint2D(World2D& worldRef, RigidBody2D& bodyA, RigidBody2D& bodyB, const bool collide,
        const glm::vec2& localAnchorA, const glm::vec2& localAnchorB) :
        Joint2D(worldRef, bodyA, bodyB, collide),
        m_jointL(nullptr)
    {
        b2Vec2 ancA(localAnchorA.x, localAnchorA.y);
        b2Vec2 ancB(localAnchorB.x, localAnchorB.y);

        b2DistanceJointDef jointDef;
        jointDef.bodyA = getBody(bodyA);
        jointDef.bodyB = getBody(bodyB);
        jointDef.collideConnected = collide;
        jointDef.length = std::abs(b2Distance(getBody(bodyA)->GetWorldPoint(ancA), getBody(bodyB)->GetWorldPoint(ancB)));
        jointDef.localAnchorA = ancA;
        jointDef.localAnchorB = ancB;
        jointDef.userData = this;

        m_joint = static_cast<b2DistanceJoint*>(getBody(bodyA)->GetWorld()->CreateJoint(&jointDef));
        m_jointL = static_cast<b2DistanceJoint*>(m_joint);
    }

    //////////////////////////////////////////////

    std::pair<float, float> RopeJoint2D::getDamping() const
    {
        return std::make_pair(m_jointL->GetFrequency(), m_jointL->GetDampingRatio());
    }

    //////////////////////////////////////////////

    RopeJoint2D& RopeJoint2D::setDamping(const float frequency, const float damping)
    {
        m_jointL->SetFrequency(frequency);
        m_jointL->SetDampingRatio(damping);
        return *this;
    }
}