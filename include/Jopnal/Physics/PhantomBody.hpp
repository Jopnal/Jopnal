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

#ifndef JOP_PHANTOMBODY_HPP
#define JOP_PHANTOMBODY_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics/Collider.hpp>
#include <vector>

//////////////////////////////////////////////


namespace jop
{
    class CollisionShape;

    class JOP_API PhantomBody : public Collider
    {
    public:

        JOP_GENERIC_COMPONENT_CLONE(PhantomBody);

    public:
    
        /// \brief Constructor
        ///
        /// \param object The object to bind this component into
        /// \param world The physics world
        ///
        PhantomBody(Object& object, World& world, CollisionShape& shape);

        /// \brief Destructor
        ///
        virtual ~PhantomBody() override;


        /// \copydoc Component::update()
        ///
        void update(const float deltaTime) override;

        /// \brief Get all the overlapping colliders
        ///
        /// \return The overlapping colliders
        ///
        std::vector<Collider*> getOverlaps();

        /// \copydoc getOverlaps()
        ///
        std::vector<const Collider*> getOverlaps() const;

        /// \brief Get the number of overlapping colliders
        ///
        /// \return The number of overlapping colliders
        ///
        unsigned int getOverlapAmount() const;
    };
}

/// \class jop::PhantomBody
/// \ingroup physics

#endif