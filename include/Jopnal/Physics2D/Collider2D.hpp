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

#ifndef JOP_COLLIDER2D_HPP
#define JOP_COLLIDER2D_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Component.hpp>
#include <glm/vec2.hpp>
#include <set>

//////////////////////////////////////////////

class b2Body;

namespace jop
{
    class Joint2D;
    class World2D;
    class ContactListener2D;

    namespace detail
    {
        struct ContactListener2DImpl;
    }

    class JOP_API Collider2D : public Component
    {
    private:

        friend class ContactListener2D;
        friend struct detail::ContactListener2DImpl;

        JOP_DISALLOW_COPY_MOVE(Collider2D);

        friend class Joint2D;

    protected:

        /// \brief Constructor
        ///
        /// \param object Reference to the object
        /// \param world Reference to the physics world
        /// \param ID Id of this component
        ///
        Collider2D(Object& object, World2D& world, const uint32 ID);

        /// \brief Copy constructor
        ///
        /// \param other The other collider to copy
        /// \param newObj The new object
        ///
        Collider2D(const Collider2D& other, Object& newObj);

        /// \brief Virtual destructor
        ///
        virtual ~Collider2D() override = 0;

        /// \brief Update inherited from Component
        ///
        void update(const float deltaTime) override;

    public:

        /// \brief Check if this collider overlaps with another
        ///
        /// This will compare the axis-aligned bounding boxes. To check if two
        /// colliders are actually touching, use checkContact().
        ///
        /// \param other The other collider to check against
        ///
        /// \return True if the two colliders are overlapping
        ///
        bool checkOverlap(const Collider2D& other) const;

        /// \brief Check if this collider is in contact with another
        ///
        /// \param other The other collider to check against
        ///
        /// \return True if the two colliders are in contact
        ///
        bool checkContact(const Collider2D& other) const;

        /// \brief Check if a ray pierces this collider
        ///
        /// \param start The start position of the ray
        /// \param ray Ray to be shot from start
        ///
        /// \return True if the ray pierces this collider
        ///
        bool checkRay(const glm::vec2& start, const glm::vec2& ray) const;

        /// \brief Register a listener for this collider
        ///
        /// Single collider can have multiple listeners
        ///
        /// \param listener Reference to the listener which is to be registered for this collider 
        ///
        void registerListener(ContactListener2D& listener);

        /// \brief Get the world this collider belongs to
        ///
        /// \return Reference to the 2D world
        ///
        World2D& getWorld();
     
        /// \copydoc getWorld()
        ///
        const World2D& getWorld() const;

    protected:

        b2Body* m_body;         ///< A RigidBody2D
        World2D& m_worldRef2D;  ///< Reference to the world
        std::set<ContactListener2D*> m_listeners;
    };
}
#endif