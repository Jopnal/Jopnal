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

#ifndef JOP_RIGIDBODY_HPP
#define JOP_RIGIDBODY_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics/Collider.hpp>

//////////////////////////////////////////////


namespace jop
{
    class CollisionShape;

    class JOP_API RigidBody final : public Collider
    {
    private:

        RigidBody(const RigidBody& other, Object& newObj);

        JOP_GENERIC_COMPONENT_CLONE(RigidBody);
        JOP_DISALLOW_COPY_MOVE(RigidBody);

    public:

        enum class Type
        {
            Static,
            Dynamic,
            Kinematic
        };

    public:

        RigidBody(Object& object, World& world, const CollisionShape& shape, const Type type = Type::Static, const float mass = 0.f, const int16 group = 1, const int16 mask = 1);

        ~RigidBody() override;




    private:

        void setActive(const bool active) override;

        const Type m_type;
        const float m_mass;

    };
}

#endif