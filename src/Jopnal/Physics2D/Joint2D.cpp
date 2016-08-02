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

#include <Jopnal/Physics2D/Joint2D.hpp>

#include <Box2D/Dynamics/Joints/b2Joint.hpp>

#endif

//////////////////////////////////////////////

namespace jop
{

    Joint2D::Joint2D(World2D& worldRef, RigidBody2D& bodyA, RigidBody2D& bodyB, const bool collide) :
        m_bodyA     (&bodyA),
        m_bodyB     (&bodyB),
        m_worldRef  (&worldRef),
        m_collide   (collide),
        m_joint     (nullptr),
        m_ID        (0)
    {
    }

    Joint2D::~Joint2D()
    {
          m_worldRef->m_worldData2D->DestroyJoint(m_joint);
    }

    unsigned int Joint2D::getID() const
    {
        return m_ID;
    }

    Joint2D& Joint2D::setID(const unsigned int id)
    {
        m_ID = id;
        return *this;
    }

    b2Body* Joint2D::getBody(RigidBody2D& body) //(std::weak_ptr<RigidBody2D>& body)
    {
        return body./*lock()->*/m_body;
    }

}