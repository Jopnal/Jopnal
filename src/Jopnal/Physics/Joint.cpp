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

#include <Jopnal/Physics/Joint.hpp>



#endif

//////////////////////////////////////////////

namespace jop
{

    Joint::Joint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB) :
        m_bodyA(&bodyA),
        m_bodyB(&bodyB),
        m_worldRef(&worldRef),
        m_ID(0)
    {
    }

    Joint::~Joint()
    {
        m_worldRef->m_worldData->world->removeConstraint(m_joint.get());
    }

    unsigned int Joint::getID() const
    {
        return m_ID;
    }

    Joint& Joint::setID(const unsigned int id)
    {
        m_ID = id;
        return *this;
    }

    btRigidBody* Joint::getBody(RigidBody& body) const
    {
        return body.m_rigidBody;
    }

    btDiscreteDynamicsWorld& Joint::getWorld(World& world) const
    {
        return *world.m_worldData->world;
    }

    glm::vec3 Joint::defaultCenter(const glm::vec3& jPos) const
    {
        return jPos == glm::vec3(0.f, 0.f, FLT_MAX) ?
            computeCenter() : jPos;
    }

    glm::vec3 Joint::computeCenter() const
    {
        return (m_bodyA->getObject()->getGlobalPosition() + m_bodyB->getObject()->getGlobalPosition()) * 0.5f;
    }



}