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

    #include <Jopnal/Physics/RigidBody.hpp>
    
    #include <Jopnal/Core/Object.hpp>
    #include <Jopnal/Physics/World.hpp>
    #include <Jopnal/Physics/Detail/WorldImpl.hpp>
    #include <Jopnal/Physics/Shape/CollisionShape.hpp>
    #include <Jopnal/Utility/CommandHandler.hpp>
    #include <Jopnal/STL.hpp>
    
    #pragma warning(push)
    #pragma warning(disable: 4127)
    
    #include <btBulletCollisionCommon.h>
    #include <btBulletDynamicsCommon.h>
    
    #pragma warning(pop)

#endif

//////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_COMMAND_HANDLER(RigidBody)

        JOP_BIND_MEMBER_COMMAND(&RigidBody::setGravityScale, "setBodyGravity");
        JOP_BIND_MEMBER_COMMAND(&RigidBody::setFixedMovement, "setLinearFactor");
        JOP_BIND_MEMBER_COMMAND(&RigidBody::setFixedRotation, "setFixedRotation");
        JOP_BIND_MEMBER_COMMAND(&RigidBody::applyForce, "applyForce");
        JOP_BIND_MEMBER_COMMAND(&RigidBody::applyImpulse, "applyImpulse");
        JOP_BIND_MEMBER_COMMAND(&RigidBody::applyTorque, "applyTorque");
        JOP_BIND_MEMBER_COMMAND(&RigidBody::applyTorqueImpulse, "applyTorqueImpulse");
        JOP_BIND_MEMBER_COMMAND(&RigidBody::setLinearVelocity, "setLinearVelocity");
        JOP_BIND_MEMBER_COMMAND(&RigidBody::applyCentralForce, "applyCentralForce");
        JOP_BIND_MEMBER_COMMAND(&RigidBody::applyCentralImpulse, "applyCentralImpulse");
        JOP_BIND_MEMBER_COMMAND(&RigidBody::clearForces, "clearForces");

    JOP_END_COMMAND_HANDLER(RigidBody)
}

namespace jop
{
    namespace detail
    {
        class MotionState final : public btMotionState
        {
        private:

            WeakReference<Object> m_obj;

        public:

            explicit MotionState(Object& obj)
                : m_obj(obj)
            {}

            void getWorldTransform(btTransform& worldTrans) const override
            {
                auto& p = m_obj->getGlobalPosition();
                auto& r = m_obj->getGlobalRotation();

                worldTrans.setOrigin(btVector3(p.x, p.y, p.z));
                worldTrans.setRotation(btQuaternion(r.x, r.y, r.z, r.w));
            }

            void setWorldTransform(const btTransform& worldTrans) override
            {
                auto& p = worldTrans.getOrigin();
                auto r = worldTrans.getRotation();

                m_obj->setPosition(p.x(), p.y(), p.z());
                m_obj->setRotation(glm::quat(r.w(), r.x(), r.y(), r.z()));
            }
        };
    }

    //////////////////////////////////////////////

    RigidBody::ConstructInfo::ConstructInfo(const CollisionShape& shape, const Type type, const float mass)
        : group             (1),
          mask              (1),
          friction          (0.5f),
          rollingFriction   (0.f),
          restitution       (0.f),
          m_shape           (shape),
          m_type            (type),
          m_mass            ((type == Type::Dynamic) * mass)
    {}

    //////////////////////////////////////////////

    RigidBody::RigidBody(Object& object, World& world, const ConstructInfo& info)
        : Collider      (object, world, 0),
          m_motionState (std::make_unique<detail::MotionState>(object)),
          m_type        (info.m_type),
          m_mass        (info.m_mass),
          m_rigidBody   (nullptr)
    {
        btVector3 inertia(0.f, 0.f, 0.f);
        if (m_type == Type::Dynamic)
            info.m_shape.m_shape->calculateLocalInertia(m_mass, inertia);

        btRigidBody::btRigidBodyConstructionInfo constInfo(m_mass, m_motionState.get(), info.m_shape.m_shape.get(), inertia);
        constInfo.m_friction = info.friction;
        constInfo.m_rollingFriction = info.rollingFriction;
        constInfo.m_restitution = info.restitution;

        auto rb = std::make_unique<btRigidBody>(constInfo);

        int flags = rb->getCollisionFlags();
        
        if (m_type == Type::Kinematic || m_type == Type::KinematicSensor)
            flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
        else
            object.setIgnoreParent(true);

        flags |= btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK;

        // Remove contact response if body is a sensor
        if (m_type > Type::Kinematic)
            flags |= btCollisionObject::CF_NO_CONTACT_RESPONSE;

        rb->setCollisionFlags(flags);
        rb->setUserPointer(this);

        m_worldRef.m_worldData->world->addRigidBody(rb.get(), info.group, info.mask);

        m_rigidBody = rb.get();
        m_body = std::move(rb);
    }

    RigidBody::RigidBody(const RigidBody& other, Object& newObj)
        : Collider      (other, newObj),
          m_motionState (std::make_unique<detail::MotionState>(newObj)),
          m_type        (other.m_type),
          m_mass        (other.m_mass),
          m_rigidBody   (nullptr)
    {
        btRigidBody::btRigidBodyConstructionInfo constInfo(m_mass, m_motionState.get(), other.m_body->getCollisionShape(), other.m_rigidBody->getLocalInertia());
        constInfo.m_friction = other.m_body->getFriction();
        constInfo.m_rollingFriction = other.m_body->getRollingFriction();
        constInfo.m_restitution = other.m_body->getRestitution();

        auto rb = std::make_unique<btRigidBody>(constInfo);

        rb->setCollisionFlags(other.m_body->getCollisionFlags());
        rb->setUserPointer(this);

        auto bpHandle = other.m_body->getBroadphaseHandle();
        m_worldRef.m_worldData->world->addRigidBody(rb.get(), bpHandle->m_collisionFilterGroup, bpHandle->m_collisionFilterMask);

        m_rigidBody = rb.get();
        m_body = std::move(rb);
        
        newObj.setIgnoreParent(other.getObject()->ignoresParent());
    }

    RigidBody::~RigidBody()
    {
        for (auto& i : m_joints)
        {
            auto& body = i->m_bodyA == this ? i->m_bodyB : i->m_bodyA;

            body->m_joints.erase(i);

            auto& thisBody = i->m_bodyA == this ? i->m_bodyA : i->m_bodyB;
            thisBody = nullptr;
        }
        m_joints.clear();

        m_worldRef.m_worldData->world->removeRigidBody(btRigidBody::upcast(m_body.get()));
    }

    //////////////////////////////////////////////

    RigidBody& RigidBody::setGravityScale(const glm::vec3& acceleration)
    {
        m_rigidBody->setGravity(btVector3(acceleration.x, acceleration.y, acceleration.z));
        return *this;
    }

    //////////////////////////////////////////////

    glm::vec3 RigidBody::getGravityScale()const
    {
        auto& gg = m_rigidBody->getGravity();
        return glm::vec3(gg.x(), gg.y(), gg.z());
    }

    //////////////////////////////////////////////

    RigidBody& RigidBody::setFixedMovement(const glm::bvec3& fixed)
    {
        m_rigidBody->setLinearFactor(btVector3(fixed.x, fixed.y, fixed.z));
        return *this;
    }

    //////////////////////////////////////////////

    glm::bvec3 RigidBody::hasFixedMovement() const
    {
        auto& lf = m_rigidBody->getLinearFactor();
        return glm::bvec3(lf.x() < 1.f, lf.y() < 1.f, lf.z() < 1.f);
    }

    //////////////////////////////////////////////

    RigidBody& RigidBody::setFixedRotation(const glm::bvec3& axis)
    {
        m_rigidBody->setAngularFactor(btVector3(axis.x, axis.y, axis.z));
        return *this;
    }

    //////////////////////////////////////////////

    glm::bvec3 RigidBody::hasFixedRotation() const
    {
        auto& af = m_rigidBody->getAngularFactor();
        return glm::bvec3(af.x() < 1.f, af.y() < 1.f, af.z() < 1.f);
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

    glm::vec3 RigidBody::getLinearVelocity() const
    {
        auto& vel = m_rigidBody->getLinearVelocity();

        return glm::vec3(vel.x(), vel.y(), vel.z());
    }

    //////////////////////////////////////////////

    RigidBody& RigidBody::setAngularVelocity(const glm::vec3& angularVelocity)
    {
        m_rigidBody->activate();
        m_rigidBody->setAngularVelocity(btVector3(angularVelocity.x, angularVelocity.y, angularVelocity.z));

        return *this;
    }

    //////////////////////////////////////////////

    glm::vec3 RigidBody::getAngularVelocity() const
    {
        auto& vel = m_rigidBody->getAngularVelocity();

        return glm::vec3(vel.x(), vel.y(), vel.z());
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

    RigidBody& RigidBody::clearForces()
    {
        m_rigidBody->clearForces();
        return *this;
    }

    //////////////////////////////////////////////

    RigidBody& RigidBody::synchronizeTransform()
    {
        if (!m_body->isStaticOrKinematicObject())
        {
            auto& pos = getObject()->getGlobalPosition();
            auto& rot = getObject()->getGlobalRotation();

            m_body->setWorldTransform(btTransform(btQuaternion(rot.x, rot.y, rot.z, rot.w), btVector3(pos.x, pos.y, pos.z)));
        }
        return *this;
    }

    //////////////////////////////////////////////

    std::pair<glm::vec3, glm::vec3> RigidBody::getLocalBounds() const
    {
        btVector3 min;
        btVector3 max;
        m_rigidBody->getAabb(min, max);

        return std::make_pair(glm::vec3(min.x(), min.y(), min.z()), glm::vec3(max.x(), max.y(), max.z()));
    }

    //////////////////////////////////////////////

    Message::Result RigidBody::receiveMessage(const Message& message)
    {
        if (JOP_EXECUTE_COMMAND(RigidBody, message.getString(), this) == Message::Result::Escape)
            return Message::Result::Escape;

        return Component::receiveMessage(message);
    }
}