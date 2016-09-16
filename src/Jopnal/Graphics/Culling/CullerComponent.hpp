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

#ifndef JOP_CULLERCOMPONENT_HPP
#define JOP_CULLERCOMPONENT_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics/ContactListener.hpp>
#include <Jopnal/Physics/RigidBody.hpp>

//////////////////////////////////////////////


namespace jop
{
    class Object;
    class World;

    namespace detail
    {
        class CullerComponent final : public RigidBody, public ContactListener
        {
        public:

            enum class Type
            {
                Drawable,
                Camera,
                LightSource,
                EnvironmentRecorder
            };

        public:

            CullerComponent(Object& object, World& world, const Type type, void* component);

            CullerComponent(const CullerComponent& other, Object& newObj, void* newComp);


            Type getType() const;

            bool shouldCollide(const CullerComponent& other) const;

            static bool cullingEnabled();

        private:

            void beginContact(Collider& collider, const ContactInfo& ci) override;

            void endContact(Collider& collider) override;


            const Type m_type;
            void* m_component;
        };
    }
}

/// \class jop::CullerComponent
/// \ingroup graphics

#endif