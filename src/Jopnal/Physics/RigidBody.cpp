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
    RigidBody::ConstructInfo::ConstructInfo(const CollisionShape& shape, const Type type, const float mass)
        : group                 (1),
          mask                  (1),
          friction              (0.5f),
          rollingFriction       (0.f),
          restitution           (0.f),
          enableContactCallback (false),
          m_shape               (shape),
          m_type                (type),
          m_mass                ((type == Type::Dynamic) * mass)
    {}

    //////////////////////////////////////////////

    RigidBody::RigidBody(Object& object, World& world, const ConstructInfo& info)
        : Collider                  (object, world, "rigidbody"),
          m_type                    (info.m_type),
          m_mass                    (info.m_mass),
          m_rigidBody               (nullptr)
    {
        btVector3 inertia(0.f, 0.f, 0.f);
        if (m_type == Type::Dynamic)
            info.m_shape.m_shape->calculateLocalInertia(m_mass, inertia);
        
        btRigidBody::btRigidBodyConstructionInfo constInfo(m_mass, m_motionState.get(), info.m_shape.m_shape.get(), inertia);
        constInfo.m_friction = info.friction;
        constInfo.m_rollingFriction = info.rollingFriction;
        constInfo.m_restitution = info.restitution;

        auto rb = std::make_unique<btRigidBody>(constInfo);
        
        if (m_type == Type::Kinematic || m_type == Type::KinematicSensor)
            rb->setCollisionFlags(rb->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        else
            object.setIgnoreParent(true);
        
        rb->setCollisionFlags(rb->getCollisionFlags() | (info.enableContactCallback * btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK));

        // Remove contact response if body is a sensor
        if (m_type > Type::Kinematic)
            rb->setCollisionFlags(rb->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

        m_worldRef.m_worldData->world->addRigidBody(rb.get(), info.group, info.mask);

        m_rigidBody = rb.get();
        rb->setUserPointer(this);
        m_body = std::move(rb);

        setActive(isActive());
    }

    RigidBody::RigidBody(const RigidBody& other, Object& newObj)
        : Collider                  (other, newObj),
          m_type                    (other.m_type),
          m_mass                    (other.m_mass),
          m_rigidBody               (nullptr)
    {
        btRigidBody::btRigidBodyConstructionInfo constInfo(m_mass, m_motionState.get(), other.m_body->getCollisionShape(), other.m_rigidBody->getLocalInertia());
        constInfo.m_friction = other.m_body->getFriction();
        constInfo.m_rollingFriction = other.m_body->getRollingFriction();
        constInfo.m_restitution = other.m_body->getRestitution();

        auto rb = std::make_unique<btRigidBody>(constInfo);
        rb->setCollisionFlags(other.m_body->getCollisionFlags());

        auto bpHandle = other.m_body->getBroadphaseHandle();
        m_worldRef.m_worldData->world->addRigidBody(rb.get(), bpHandle->m_collisionFilterGroup, bpHandle->m_collisionFilterMask);

        m_rigidBody = rb.get();
        rb->setUserPointer(this);
        m_body = std::move(rb);

        newObj.setIgnoreParent(other.getObject()->ignoresParent());
        setActive(isActive());
    }

    RigidBody::~RigidBody()
    {
        m_worldRef.m_worldData->world->removeRigidBody(btRigidBody::upcast(m_body.get()));
    }

    //////////////////////////////////////////////

    RigidBody& RigidBody::setGravity(const glm::vec3& acceleration)
    {
        m_rigidBody->setGravity(btVector3(acceleration.x, acceleration.y, acceleration.z));
        return *this;
    }

    //////////////////////////////////////////////

    glm::vec3 RigidBody::getGravity()const
    {
        auto& gg = m_rigidBody->getGravity();
        return glm::vec3(gg.x(), gg.y(), gg.z());
    }

    //////////////////////////////////////////////

    RigidBody& RigidBody::setLinearFactor(const glm::vec3& linearFactor)
    {
        m_rigidBody->setLinearFactor(btVector3(linearFactor.x, linearFactor.y, linearFactor.z));
        return *this;
    }
    
    //////////////////////////////////////////////

    glm::vec3 RigidBody::getLinearFactor()const
    {
        auto& glf = m_rigidBody->getLinearFactor();
        return glm::vec3(glf.x(), glf.y(), glf.z());
    }

    //////////////////////////////////////////////

    RigidBody& RigidBody::setAngularFactor(const glm::vec3& angularFactor)
    {
        m_rigidBody->setAngularFactor(btVector3(angularFactor.x, angularFactor.y, angularFactor.z));
        return *this;
    }

    //////////////////////////////////////////////

    glm::vec3 RigidBody::getAngularFactor()const
    {
        auto& gaf = m_rigidBody->getAngularFactor();
        return glm::vec3(gaf.x(), gaf.y(), gaf.z());
    }

    //////////////////////////////////////////////

    RigidBody& RigidBody::applyForce(const glm::vec3& force, const glm::vec3& rel_pos)
    {
        m_rigidBody->activate();
        m_rigidBody->applyForce(btVector3(force.x, force.y, force.z), btVector3(rel_pos.x, rel_pos.y, rel_pos.z));

        return *this;
    }

    //////////////////////////////////////////////

    RigidBody& RigidBody::applyImpulse(const glm::vec3& impulse, const glm::vec3& rel_pos)
    {
        m_rigidBody->activate();
        m_rigidBody->applyImpulse(btVector3(impulse.x, impulse.y, impulse.z), btVector3(rel_pos.x, rel_pos.y, rel_pos.z));

        return *this;
    }

    //////////////////////////////////////////////

    RigidBody& RigidBody::applyTorque(const glm::vec3& torque)
    {
        m_rigidBody->activate();
        m_rigidBody->applyTorque(btVector3(torque.x, torque.y, torque.z));

        return *this;
    }

    //////////////////////////////////////////////

    RigidBody& RigidBody::applyTorqueImpulse(const glm::vec3& torque)
    {
        m_rigidBody->activate();
        m_rigidBody->applyTorqueImpulse(btVector3(torque.x, torque.y, torque.z));

        return *this;
    }

    //////////////////////////////////////////////

    RigidBody& RigidBody::setLinearVelocity(const glm::vec3& linearVelocity)
    {
        m_rigidBody->activate();
        m_rigidBody->setLinearVelocity(btVector3(linearVelocity.x, linearVelocity.y, linearVelocity.z));

        return *this;
    }

    //////////////////////////////////////////////

    RigidBody& RigidBody::setAngularVelocity(const glm::vec3& angularVelocity)
    {
        m_rigidBody->activate();
        m_rigidBody->setAngularVelocity(btVector3(angularVelocity.x, angularVelocity.y, angularVelocity.z));

        return *this;
    }

    //////////////////////////////////////////////

    RigidBody& RigidBody::applyCentralForce(const glm::vec3& force)
    {
        m_rigidBody->activate();
        m_rigidBody->applyCentralForce(btVector3(force.x, force.y, force.z));

        return *this;
    }

    //////////////////////////////////////////////

    RigidBody& RigidBody::applyCentralImpulse(const glm::vec3& impulse)
    {
        m_rigidBody->activate();
        m_rigidBody->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));

        return *this;
    }

    //////////////////////////////////////////////

    void RigidBody::clearForces()
    {
        m_rigidBody->clearForces();
    }

    //////////////////////////////////////////////

    void RigidBody::setActive(const bool active)
    {
        m_body->forceActivationState(active ? (m_body->isKinematicObject() ? DISABLE_DEACTIVATION : ACTIVE_TAG) : DISABLE_SIMULATION);
    }

}