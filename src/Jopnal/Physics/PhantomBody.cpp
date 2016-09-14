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
    #include <Jopnal/Physics/World.hpp>
    #include <Jopnal/Physics/Shape/CollisionShape.hpp>
    #include <Jopnal/Physics/Detail/WorldImpl.hpp>
    #include <Jopnal/STL.hpp>

    #pragma warning(push)
    #pragma warning(disable: 4127)

    #include <btBulletCollisionCommon.h>
    #include <BulletCollision/CollisionDispatch/btGhostObject.h>

    #pragma warning(pop)

#endif

//////////////////////////////////////////////


namespace jop
{
    PhantomBody::PhantomBody(Object& object, World& world, CollisionShape& shape)
        : Collider(object, world, 0)
    {
        auto ghost = std::make_unique<btGhostObject>();

        ghost->setCollisionShape(shape.m_shape.get());
        ghost->setCollisionFlags(ghost->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

        m_worldRef.m_worldData->world->addCollisionObject(ghost.get());

        ghost->setUserPointer(this);
        m_body = std::move(ghost);

    }

    PhantomBody::PhantomBody(const PhantomBody& other, Object& newObj)
        : Collider(other, newObj)
    {
        auto otherGhost = static_cast<const btGhostObject*>(other.m_body.get());

        auto ghost = std::make_unique<btGhostObject>(*otherGhost);

        ghost->setUserPointer(this);
        m_body = std::move(ghost);
    }

    PhantomBody::~PhantomBody()
    {
        m_worldRef.m_worldData->world->removeCollisionObject(m_body.get());
    }

    //////////////////////////////////////////////

    void PhantomBody::update(const float deltaTime)
    {
        Collider::update(deltaTime);

        auto& rot = getObject()->getGlobalRotation();
        auto& pos = getObject()->getGlobalPosition();

        m_body->setWorldTransform(btTransform(btQuaternion(rot.x, rot.y, rot.z, rot.w), btVector3(pos.x, pos.y, pos.z)));
    }

    //////////////////////////////////////////////

    std::vector<Collider*> PhantomBody::getOverlaps()
    {
        std::vector<Collider*> vec;
        vec.reserve(getOverlapAmount());

        auto overlaps = static_cast<btGhostObject&>(*m_body).getOverlappingPairs();

        for (int i = 0; i < overlaps.size(); ++i)
            vec.push_back(static_cast<Collider*>(overlaps[i]->getUserPointer()));

        return vec;
    }

    //////////////////////////////////////////////

    std::vector<const Collider*> PhantomBody::getOverlaps() const
    {
        std::vector<const Collider*> vec;
        vec.reserve(getOverlapAmount());

        auto overlaps = static_cast<const btGhostObject&>(*m_body).getOverlappingPairs();

        for (int i = 0; i < overlaps.size(); ++i)
            vec.push_back(static_cast<const Collider*>(overlaps[i]->getUserPointer()));

        return vec;
    }

    //////////////////////////////////////////////

    unsigned int PhantomBody::getOverlapAmount() const
    {
        return static_cast<const btGhostObject&>(*m_body).getNumOverlappingObjects();
    }
}