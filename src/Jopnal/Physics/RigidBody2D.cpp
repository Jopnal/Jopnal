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
#include <Jopnal/Physics/RigidBody2D.hpp>
#include <Jopnal/Physics/World2D.hpp>

//////////////////////////////////////////////


//namespace jop
//{
//    JOP_REGISTER_COMMAND_HANDLER(RigidBody)
//
//        JOP_BIND_MEMBER_COMMAND(&RigidBody::setGravity, "setBodyGravity");
//        JOP_BIND_MEMBER_COMMAND(&RigidBody::setLinearFactor, "setLinearFactor");
//        JOP_BIND_MEMBER_COMMAND(&RigidBody::setAngularFactor, "setAngularFactor");
//        JOP_BIND_MEMBER_COMMAND(&RigidBody::applyForce, "applyForce");
//        JOP_BIND_MEMBER_COMMAND(&RigidBody::applyImpulse, "applyImpulse");
//        JOP_BIND_MEMBER_COMMAND(&RigidBody::applyTorque, "applyTorque");
//        JOP_BIND_MEMBER_COMMAND(&RigidBody::applyTorqueImpulse, "applyTorqueImpulse");
//        JOP_BIND_MEMBER_COMMAND(&RigidBody::setLinearVelocity, "setLinearVelocity");
//        JOP_BIND_MEMBER_COMMAND(&RigidBody::setAngularFactor, "setAngularVelocity");
//        JOP_BIND_MEMBER_COMMAND(&RigidBody::applyCentralForce, "applyCentralForce");
//        JOP_BIND_MEMBER_COMMAND(&RigidBody::applyCentralImpulse, "applyCentralImpulse");
//        JOP_BIND_MEMBER_COMMAND(&RigidBody::clearForces, "clearForces");
//
//    JOP_END_COMMAND_HANDLER(RigidBody)
//}

namespace jop
{
    RigidBody2D::ConstructInfo2D::ConstructInfo2D(const CollisionShape2D& shape, const Type type, const float mass)
        :
        group       (1),
        mask        (1),
        friction    (0.2f),
        restitution (0.5f),
        m_shape     (shape),
        m_type      (type),
        m_mass      ((type == Type::Dynamic) * mass)
    {
    }

    //////////////////////////////////////////////

    RigidBody2D::RigidBody2D(Object& object, World2D& world, const ConstructInfo2D& info)
        : Collider2D(object, world, 0)
    {

        b2BodyDef bd;
        b2FixtureDef fdf;

        auto& pos = getObject()->getGlobalPosition();

        bd.angle = glm::eulerAngles(getObject()->getGlobalRotation()).z;
        bd.position = b2Vec2(pos.x, pos.y);
        bd.userData = this;


        b2BodyType Types[3] = { b2BodyType::b2_staticBody, b2BodyType::b2_dynamicBody, b2BodyType::b2_kinematicBody };

        switch (info.m_type)
        {
        case Type::StaticSensor:
            bd.type = b2BodyType::b2_staticBody;
            object.setIgnoreParent(true);
            fdf.isSensor = true;
            break;

        case Type::KinematicSensor:
            bd.type = b2BodyType::b2_kinematicBody;
            fdf.isSensor = true;
            break;

        default:
            bd.type = Types[static_cast<int>(info.m_type)];
            object.setIgnoreParent(true);
        }

        bd.allowSleep = bd.type != b2_kinematicBody;

        m_body = world.m_worldData2D->CreateBody(&bd);

        fdf.filter.groupIndex = info.group;
        fdf.filter.maskBits = info.mask;
        fdf.friction = info.friction;
        fdf.restitution = info.restitution;

        fdf.shape = info.m_shape.m_shape.get();
        fdf.density = 1;// info.m_mass / fdf.shape->m_radius;

        m_body->CreateFixture(&fdf);
        setActive(isActive());
    }

    RigidBody2D::RigidBody2D(const RigidBody2D& other, Object& newObj)
        : Collider2D(other, newObj)
    {
        b2Body* body = other.m_body; //check if fixtures are copied too
    }

    RigidBody2D::~RigidBody2D()
    {
        m_worldRef2D.m_worldData2D->DestroyBody(m_body);
    }

    //////////////////////////////////////////////

    RigidBody2D& RigidBody2D::setGravityScale(const float scale)
    {
        m_body->SetGravityScale(scale);
        return *this;
    }

    //////////////////////////////////////////////

    float RigidBody2D::getGravityScale()const
    {
        return m_body->GetGravityScale();
    }

    //////////////////////////////////////////////

    RigidBody2D& RigidBody2D::setLinearVelocity(const glm::vec2& linearVelocity)
    {
        m_body->SetLinearVelocity(b2Vec2(linearVelocity.x, linearVelocity.y));
        return *this;
    }

    //////////////////////////////////////////////

    glm::vec2 RigidBody2D::getLinearVelocity()const
    {
        return glm::vec2(m_body->GetLinearVelocity().x, m_body->GetLinearVelocity().y);
    }

    //////////////////////////////////////////////

    RigidBody2D& RigidBody2D::setAngularVelocity(const float angularVelocity)
    {
        m_body->SetAngularVelocity(angularVelocity);
        return *this;
    }

    //////////////////////////////////////////////

    float RigidBody2D::getAngularVelocity()const
    {

        return m_body->GetAngularVelocity();
    }

    //////////////////////////////////////////////

    RigidBody2D& RigidBody2D::applyForce(const glm::vec2& force, const glm::vec2& worldPoint)
    {
        m_body->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(worldPoint.x, worldPoint.y), true);
        return *this;
    }

    //////////////////////////////////////////////

    RigidBody2D& RigidBody2D::applyCentralForce(const glm::vec2& force)
    {
        m_body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
        return *this;
    }

    //////////////////////////////////////////////

    RigidBody2D& RigidBody2D::applyAngularImpulse(const float& impulse)
    {
        m_body->ApplyAngularImpulse(impulse, true);
        return *this;
    }

    //////////////////////////////////////////////

    RigidBody2D& RigidBody2D::applyLinearImpulse(const glm::vec2& impulse, const glm::vec2& point)
    {
        m_body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(point.x, point.y), true);
        return *this;
    }

    //////////////////////////////////////////////

    RigidBody2D& RigidBody2D::applyCentralImpulse(const glm::vec2& impulse)
    {
        m_body->ApplyLinearImpulseToCenter(b2Vec2(impulse.x, impulse.y), true);
        return *this;
    }

    //////////////////////////////////////////////

    RigidBody2D& RigidBody2D::applyTorque(const float torque)
    {
        m_body->ApplyTorque(torque, true);
        return *this;
    }

    //////////////////////////////////////////////

    RigidBody2D& RigidBody2D::setFixedRotation(const bool rot)
    {
        m_body->SetFixedRotation(rot);
        return *this;
    }



    //////////////////////////////////////////////

    void RigidBody2D::setActive(const bool active)
    {
        m_body->SetActive(active);
    }

    //////////////////////////////////////////////

    Message::Result RigidBody2D::receiveMessage(const Message& message)
    {
       // if (JOP_EXECUTE_COMMAND(RigidBody2D, message.getString(), this) == Message::Result::Escape)
       //     return Message::Result::Escape;

        return Component::receiveMessage(message);
    }
}