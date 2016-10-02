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
#include <memory>

//////////////////////////////////////////////


class btCollisionObject;

namespace jop
{
    class Object;
    class CollisionShape;

    namespace detail
    {
        class CullingWorld;
        
        class CullerComponent
        {
        private:

            friend struct CullBroadphaseCallback;

        public:

            enum class Type
            {
                Drawable,
                Camera,
                LightSource,
                EnvironmentRecorder
            };

        public:

            CullerComponent(Object& obj, CullingWorld* world, const Type type);

            CullerComponent(const CullerComponent& other, Object& newObj);

            virtual ~CullerComponent();


            void updateTransform();

            Type getType() const;

            void setCollisionShape(CollisionShape& shape);

            void updateWorldBounds();

            static bool cullingEnabled();

        private:

            virtual bool shouldCollide(const CullerComponent& other) const;

        protected:

            WeakReference<Object> m_objRef;
            CullingWorld* m_worldRef;
            std::unique_ptr<btCollisionObject> m_body;
            const Type m_type;
        };
    }
}

/// \class jop::CullerComponent
/// \ingroup graphics

#endif