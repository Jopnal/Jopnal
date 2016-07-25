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

#include <Jopnal/Physics/Joint/PistonJoint.hpp>

#include <Jopnal/STL.hpp>
#include <BulletDynamics/ConstraintSolver/btSliderConstraint.h>

#endif

//////////////////////////////////////////////


namespace jop
{
    PistonJoint::PistonJoint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB) :
        Joint(worldRef, bodyA, bodyB),
        m_jointL(nullptr)
    {

        

        btTransform frameInA = btTransform::btTransform(getBody(bodyA)->getWorldTransform());
        btTransform frameInB = btTransform::btTransform(getBody(bodyB)->getWorldTransform());

        //btTransform frameInA;
        //frameInA = btTransform::getIdentity();
        //frameInA.setOrigin(btVector3(btScalar(2.), btScalar(-3.), btScalar(2.)));
        frameInA.setRotation(btQuaternion(0., 0., 0.707, 0.707));

        //btTransform frameInB;
        //frameInB = btTransform::getIdentity();

        //frameInB.setOrigin(btVector3(btScalar(2.), btScalar(6.), btScalar(2.))); 
        frameInB.setRotation(btQuaternion(0., 0., 0.707, 0.707));
        

        auto temp = std::make_unique<btSliderConstraint>(*getBody(bodyA), *getBody(bodyB), frameInA, frameInB, true);


        temp->setLowerLinLimit(0.f);
        temp->setUpperLinLimit(btScalar(10.f));
        temp->setPoweredLinMotor(true);
        temp->setMaxLinMotorForce(-50.);


        //temp->setLowerAngLimit(btScalar(0.707));
        //temp->setUpperAngLimit(btScalar(-0.354));
        //temp->setPoweredAngMotor(true); //needs limits to work
        //temp->setMaxAngMotorForce(5.);
        
        //temp->calculateTransforms(getBody(bodyA)->getWorldTransform(), getBody(bodyB)->getWorldTransform()); //no difference



        m_joint = std::move(temp);
        getWorld(worldRef).addConstraint(m_joint.get(), true);

        //  jointDef.Initialize(getBody(bodyA), getBody(bodyB), getBody(bodyA)->GetWorldCenter(), b2Vec2(axis.x, axis.y));
        //  jointDef.collideConnected = collide;
        //  jointDef.userData = this;
        //
        //  m_joint = static_cast<b2PrismaticJoint*>(getBody(bodyA)->GetWorld()->CreateJoint(&jointDef));
        //  m_jointL = static_cast<b2PrismaticJoint*>(m_joint);
    }

    /*PistonJoint& PistonJoint::limit(const bool enable)
    {
    m_jointL->EnableLimit(enable);
    return *this;
    }

    PistonJoint& PistonJoint::setLimits(const float min, const float max)
    {
    m_jointL->EnableLimit(true);
    m_jointL->SetLimits(min, max);
    return *this;
    }

    std::pair<float, float> PistonJoint::getLimits()
    {
    return std::make_pair<float, float>(m_jointL->GetLowerLimit(), m_jointL->GetUpperLimit());
    }

    PistonJoint& PistonJoint::enableMotor(const bool set)
    {
    m_jointL->EnableMotor(set);
    return *this;
    }

    PistonJoint& PistonJoint::setMotorForces(const float speed, const float force)
    {
    JOP_ASSERT(force >= 0.f, "PistonJoint force can not be negative!");

    m_jointL->SetMotorSpeed(speed);
    m_jointL->SetMaxMotorForce(force);
    return *this;
    }

    std::pair<float, float> PistonJoint::getMotorForces()
    {
    return std::make_pair<float, float>(m_jointL->GetMotorSpeed(), m_jointL->GetMaxMotorForce());
    }*/

}