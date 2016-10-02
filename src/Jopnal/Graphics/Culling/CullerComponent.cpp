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

    #include <Jopnal/Graphics/Culling/CullerComponent.hpp>

    #include <Jopnal/Core/SettingManager.hpp>
    #include <Jopnal/Physics/Shape/CollisionShape.hpp>
    #include <Jopnal/Graphics/Camera.hpp>
    #include <Jopnal/Graphics/Drawable.hpp>
    #include <Jopnal/Graphics/LightSource.hpp>
    #include <Jopnal/Graphics/EnvironmentRecorder.hpp>
    #include <Jopnal/STL.hpp>
    #include <BulletCollision/CollisionShapes/btEmptyShape.h>

#endif

//////////////////////////////////////////////


namespace jop { namespace detail
{
    btEmptyShape& getDummyShape()
    {
        static btEmptyShape shape;
        return shape;
    }

    //////////////////////////////////////////////

    inline short getMaskBits(const CullerComponent::Type type)
    {
        using Type = CullerComponent::Type;

        switch (type)
        {
            case Type::Drawable:
                return SHRT_MAX & ~(1 << static_cast<short>(Type::Drawable));
            case Type::Camera:
                return (SHRT_MAX & ~(1 << static_cast<short>(Type::Camera))) * CullerComponent::cullingEnabled();
        }

        return 0;
    }

    inline short getGroupBit(const CullerComponent::Type type)
    {
        return 1 << static_cast<short>(type);
    }

    //////////////////////////////////////////////

    CullerComponent::CullerComponent(Object& obj, CullingWorld* world, const Type type)
        : m_objRef      (obj),
          m_worldRef    (world),
          m_body        (),
          m_type        (type)
    {
        if (world)
        {
            if (type == Type::Drawable)
                m_body = std::make_unique<btCollisionObject>();
            else
                m_body = std::make_unique<btPairCachingGhostObject>();

            m_body->setCollisionShape(&getDummyShape());
            m_body->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
            m_body->setUserPointer(this);
            m_body->setActivationState(ACTIVE_TAG);

            world->m_worldData->world->addCollisionObject(m_body.get(), getGroupBit(type), getMaskBits(type));
        }
    }

    CullerComponent::CullerComponent(const CullerComponent& other, Object& newObj)
        : m_objRef      (newObj),
          m_worldRef    (other.m_worldRef),
          m_body        (),
          m_type        (other.m_type)
    {
        if (other.m_worldRef)
        {
            if (m_type == Type::Drawable)
                m_body = std::make_unique<btCollisionObject>();
            else
                m_body = std::make_unique<btPairCachingGhostObject>();

            m_body->setCollisionShape(other.m_body->getCollisionShape());
            m_body->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
            m_body->setUserPointer(this);
            m_body->setActivationState(ACTIVE_TAG);

            m_worldRef->m_worldData->world->addCollisionObject(m_body.get(), getGroupBit(m_type), getMaskBits(m_type));
        }
    }

    CullerComponent::~CullerComponent()
    {
        if (m_worldRef)
            m_worldRef->m_worldData->world->removeCollisionObject(m_body.get());
    }

    //////////////////////////////////////////////

    void CullerComponent::updateTransform()
    {
        if (m_worldRef)
        {
            auto& pos = m_objRef->getGlobalPosition();
            auto& rot = m_objRef->getGlobalRotation();

            m_body->setWorldTransform(btTransform(btQuaternion(rot.x, rot.y, rot.z, rot.w), btVector3(pos.x, pos.y, pos.z)));
        }
    }

    //////////////////////////////////////////////

    CullerComponent::Type CullerComponent::getType() const
    {
        return m_type;
    }

    //////////////////////////////////////////////

    void CullerComponent::setCollisionShape(CollisionShape& shape)
    {
        m_body->setCollisionShape(shape.m_shape.get());
    }

    //////////////////////////////////////////////

    void CullerComponent::updateWorldBounds()
    {
        m_body->setCollisionShape(m_body->getCollisionShape());
        m_worldRef->m_worldData->world->updateSingleAabb(m_body.get());
    }

    //////////////////////////////////////////////

    bool CullerComponent::cullingEnabled()
    {
        static const bool culling = SettingManager::get<bool>("engine@Graphics|bCulling", false);
        return culling;
    }

    //////////////////////////////////////////////

    bool CullerComponent::shouldCollide(const CullerComponent&) const
    {
        return false;
    }
}}