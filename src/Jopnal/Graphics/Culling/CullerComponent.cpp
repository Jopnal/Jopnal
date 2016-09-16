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
    class DummyShape : public CollisionShape
    {
    public:

        DummyShape()
            : CollisionShape("")
        {
            m_shape = std::make_unique<btEmptyShape>();
        }
    };

    DummyShape& getDummyShape()
    {
        static DummyShape shape;
        return shape;
    }

    //////////////////////////////////////////////

    CullerComponent::CullerComponent(Object& object, World& world, const Type type, void* component)
        : RigidBody         (object, world, RigidBody::ConstructInfo(getDummyShape(), RigidBody::Type::KinematicSensor)),
          ContactListener   (),
          m_type            (type),
          m_component       (component)
    {
        if (type != Type::Drawable)
            registerListener(*this);
    }

    CullerComponent::CullerComponent(const CullerComponent& other, Object& newObj, void* newComp)
        : RigidBody         (other, newObj),
          ContactListener   (),
          m_type            (other.m_type),
          m_component       (newComp)
    {
        if (m_type != Type::Drawable)
            registerListener(*this);
    }

    //////////////////////////////////////////////

    CullerComponent::Type CullerComponent::getType() const
    {
        return m_type;
    }

    //////////////////////////////////////////////

    bool CullerComponent::shouldCollide(const CullerComponent& other) const
    {
        switch (getType())
        {
            case Type::Camera:
            {
                if (other.getType() == Type::Drawable)
                    return (static_cast<const Camera*>(m_component)->getRenderMask() & 1 << static_cast<const Drawable*>(other.m_component)->getRenderGroup()) != 0;

                else if (other.getType() == Type::LightSource)
                    return true;
            }

            case Type::LightSource:
                return other.getType() == Type::Drawable &&
                      (static_cast<const LightSource*>(m_component)->getRenderMask() & 1 << static_cast<const Drawable*>(other.m_component)->getRenderGroup()) != 0;

            //case Type::EnvironmentRecorder:
            //    return other.getType() == Type::Drawable;
        }

        return false;
    }

    //////////////////////////////////////////////

    bool CullerComponent::cullingEnabled()
    {
        static const bool culling = SettingManager::get<bool>("engine@Graphics|bCulling", false);
        return culling;
    }

    //////////////////////////////////////////////

    void CullerComponent::beginContact(Collider& collider, const ContactInfo& ci)
    {
        switch (getType())
        {
            case Type::Camera:
            {
                auto cam = static_cast<Camera*>(m_component);
                auto& other = static_cast<CullerComponent&>(collider);

                switch (other.getType())
                {
                    case Type::Drawable:
                        cam->m_drawables.insert(static_cast<const Drawable*>(other.m_component));
                        break;
                    case Type::LightSource:
                        cam->m_lights.insert(static_cast<const LightSource*>(other.m_component));
                }

                break;
            }
            case Type::LightSource:
            {
                break;
            }
            //case Type::EnvironmentRecorder:
            //{
            //}
        }
    }

    //////////////////////////////////////////////

    void CullerComponent::endContact(Collider& collider)
    {
        switch (getType())
        {
            case Type::Camera:
            {
                auto cam = static_cast<Camera*>(m_component);
                auto& other = static_cast<CullerComponent&>(collider);

                switch (other.getType())
                {
                    case Type::Drawable:
                        cam->m_drawables.erase(static_cast<const Drawable*>(other.m_component));
                        break;
                    case Type::LightSource:
                        cam->m_lights.erase(static_cast<const LightSource*>(other.m_component));
                }

                break;
            }
            case Type::LightSource:
            {
                break;
            }
            case Type::EnvironmentRecorder:
            {
            }
        }
    }
}}