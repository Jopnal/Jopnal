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

#include <Jopnal/Physics2D/Joint/PulleyJoint2D.hpp>

#include <Jopnal/STL.hpp>
#include <Box2D/Dynamics/Joints/b2Joint.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    PulleyJoint2D::PulleyJoint2D(World2D& worldRef, RigidBody2D& bodyA, RigidBody2D& bodyB,
        const bool collide, const float ratio,
        const glm::vec2& groundAnchorA, const glm::vec2& groundAnchorB,
        const glm::vec2& localAnchorA, const glm::vec2& localAnchorB
        ) :
        Joint2D(worldRef, bodyA, bodyB),
        m_jointL(nullptr)
    {
        JOP_ASSERT(ratio > 0.f, "PulleyJoint2D ratio can not be exactly zero or negative!");

        b2PulleyJointDef jointDef;

        jointDef.Initialize(getBody(bodyA), getBody(bodyB),
            b2Vec2(groundAnchorA.x, groundAnchorA.y),
            b2Vec2(groundAnchorB.x, groundAnchorB.y),
            b2Vec2(localAnchorA.x, localAnchorA.y),
            b2Vec2(localAnchorB.x, localAnchorB.y),
            ratio);
        jointDef.collideConnected = collide;

        //Re-apply local positions because box2D adds global coords to these and changes +/-
        jointDef.localAnchorA = b2Vec2(localAnchorA.x, localAnchorA.y);
        jointDef.localAnchorB = b2Vec2(localAnchorB.x, localAnchorB.y);
        jointDef.userData = this;

        m_joint = static_cast<b2PulleyJoint*>(getBody(bodyA)->GetWorld()->CreateJoint(&jointDef));
        m_jointL = static_cast<b2PulleyJoint*>(m_joint);
    }

    std::pair<float, float> PulleyJoint2D::getRopeLengths()
    {
        return std::make_pair(m_jointL->GetCurrentLengthA(), m_jointL->GetCurrentLengthB());
    }
}