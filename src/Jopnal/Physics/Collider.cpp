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


namespace detail
{
    class MotionState final : public btMotionState
    {
    private:

        jop::WeakReference<jop::Object> m_obj;

    public:

        MotionState(jop::Object& obj)
            : m_obj(obj)
        {}

        void getWorldTransform(btTransform& worldTrans) const override
        {
            auto& p = m_obj->getPosition();
            auto& r = m_obj->getRotation();

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

namespace jop
{
    Collider::Collider(Object& object, World& world, const std::string& ID)
        : Component     (object, ID),
          m_motionState (std::make_unique<::detail::MotionState>(object)),
          m_body        (),
          m_worldRef    (world)
    {}

    Collider::Collider(const Collider& other, Object& newObj)
        : Component     (other, newObj),
          m_motionState (std::make_unique<::detail::MotionState>(newObj)),
          m_body        (),
          m_worldRef    (other.m_worldRef)
    {}

    Collider::~Collider()
    {}
}