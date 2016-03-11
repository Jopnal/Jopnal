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
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
    RigidBody::RigidBody(Object& object, World& world, const CollisionShape& shape, const Type type, const float mass, const int16 group, const int16 mask)
        : Collider  (object, world, "rigidbody"),
          m_type    (type),
          m_mass    (type == Type::Static ? 0.f : mass)
    {
        btVector3 inertia(0.f, 0.f, 0.f);
        if (type == Type::Dynamic)
            shape.m_shape->calculateLocalInertia(mass, inertia);

        auto rb = std::make_unique<btRigidBody>(m_mass, m_motionState.get(), shape.m_shape.get(), inertia);
        
        if (type == Type::Kinematic)
            rb->setCollisionFlags(rb->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);

        m_worldRef.m_worldData->world->addRigidBody(rb.get(), group, mask);

        rb->setUserPointer(this);
        m_body = std::move(rb);

        setActive(isActive());
    }

    RigidBody::RigidBody(const RigidBody& other, Object& newObj)
        : Collider  (other, newObj),
          m_type    (other.m_type),
          m_mass    (other.m_mass)
    {
        btVector3 inertia(0.f, 0.f, 0.f);
        if (m_type == Type::Dynamic)
            other.m_body->getCollisionShape()->calculateLocalInertia(m_mass, inertia);

        auto rb = std::make_unique<btRigidBody>(m_mass, m_motionState.get(), other.m_body->getCollisionShape(), inertia);
        rb->setCollisionFlags(other.m_body->getCollisionFlags());

        m_worldRef.m_worldData->world->addRigidBody(rb.get(), other.m_body->getBroadphaseHandle()->m_collisionFilterGroup, other.m_body->getBroadphaseHandle()->m_collisionFilterMask);

        rb->setUserPointer(this);
        m_body = std::move(rb);

        setActive(isActive());
    }

    RigidBody::~RigidBody()
    {
        m_worldRef.m_worldData->world->removeRigidBody(btRigidBody::upcast(m_body.get()));
    }

    //////////////////////////////////////////////

    void RigidBody::setActive(const bool active)
    {
        m_body->forceActivationState(active ? (m_body->isKinematicObject() ? DISABLE_DEACTIVATION : ACTIVE_TAG) : DISABLE_SIMULATION);
    }
}