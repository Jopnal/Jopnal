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

    #include <Jopnal/Physics/PhantomBody.hpp>

    #include <Jopnal/Core/Object.hpp>
    #include <Jopnal/Physics/Shape/CollisionShape.hpp>
    #include <Jopnal/STL.hpp>
    #include <btBulletCollisionCommon.h>
    #include <BulletCollision/CollisionDispatch/btGhostObject.h>

#endif

//////////////////////////////////////////////


namespace jop
{
    PhantomBody::PhantomBody(Object& object, World& world, CollisionShape& shape)
        : Collider(object, world, 0)
    {
        auto ghost = std::make_unique<btPairCachingGhostObject>();

        ghost->setCollisionShape(shape.m_shape.get());
    }

    PhantomBody::PhantomBody(const PhantomBody& other, Object& newObj)
        : Collider(other, newObj)
    {

    }

    //////////////////////////////////////////////

    void PhantomBody::update(const float)
    {
        auto& rot = getObject()->getGlobalRotation();
        auto& pos = getObject()->getGlobalPosition();
        btTransform transform(btQuaternion(rot.x, rot.y, rot.z, rot.w), btVector3(pos.x, pos.y, pos.z));

        m_body->setWorldTransform(transform);
    }    
    
    //////////////////////////////////////////////

    void PhantomBody::setActive(const bool active)
    {
        m_body->setActivationState(active ? DISABLE_DEACTIVATION : DISABLE_SIMULATION);
    }
}