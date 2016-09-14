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

#ifndef JOP_COLLIDER_HPP
#define JOP_COLLIDER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Component.hpp>
#include <Jopnal/Physics/ContactInfo.hpp>
#include <memory>
#include <set>

//////////////////////////////////////////////


class btCollisionObject;

namespace jop
{
    namespace detail
    {
        struct GhostCallback;
        struct ContactListenerImpl;
    }
    class CollisionShape;
    class ContactListener;
    class Joint;
    class World;
    
    class JOP_API Collider : public Component, public SafeReferenceable<Collider>
    {
    private:

        JOP_DISALLOW_COPY_MOVE(Collider);

        friend struct detail::GhostCallback;
        friend struct detail::ContactListenerImpl;
        friend class ContactListener;
        friend class Joint;
        
    protected:

        /// \brief Constructor
        ///
        /// \param object Reference to the object
        /// \param world Reference to the physics world
        /// \param ID Id of this component
        ///
        Collider(Object& object, World& world, const uint32 ID);

        /// \brief Copy constructor
        ///
        /// \param other The other collider to copy
        /// \param newObj The new object
        ///
        Collider(const Collider& other, Object& newObj);

        /// \brief Virtual destructor
        ///
        virtual ~Collider() override = 0;


        /// \brief Update
        ///
        /// \param deltaTime The delta time
        ///
        void update(const float deltaTime) override;

        /// \brief Set whether the collider is allowed to sleep
        ///
        /// \param allow True to allow sleep. True by default
        ///
        void setAllowSleep(const bool allow);

        /// \brief Check if sleep is allowed
        ///
        /// \return True if allowed
        ///
        bool isSleepAllowed() const;

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
        bool checkOverlap(const Collider& other) const;

        /// \brief Check if this collider is in contact with another
        ///
        /// \param other The other collider to check against
        ///
        /// \return True if the two colliders are in contact
        ///
        bool checkContact(const Collider& other) const;

        /// \brief Check if a ray pierces this collider
        ///
        /// \param start The start position of the ray
        /// \param ray Ray to be shot from start
        ///
        /// \return True if the ray pierces this collider
        ///
        bool checkRay(const glm::vec3& start, const glm::vec3& ray) const;

        /// \brief Register a listener for this collider
        ///
        /// Single collider can have multiple listeners.
        ///
        /// \param listener Reference to the listener which is to be registered for this collider 
        ///
        void registerListener(ContactListener& listener);

        /// \brief Get the world this collider belongs to
        ///
        /// \return Reference to the world
        ///
        World& getWorld();

        /// \copydoc getWorld()
        ///
        const World& getWorld() const;

        /// \brief Detach this body from its world
        ///
        void detachFromWorld();

        /// \brief Attach this body to its world if it was previously detached
        ///
        void attachToWorld();

        /// \brief Check if this body is currently detached from its world
        ///
        /// \return True if detached
        ///
        bool isDetachedFromWorld() const;

        /// \brief Force update of the world space bounds
        ///
        /// You'll usually want to call this when you've called
        /// CollisionShape::setLocalScale() for the bound collision shape.
        ///
        void updateWorldBounds();

        /// \brief Set the collision shape
        ///
        /// \param shape The new shape to set
        ///
        void setCollisionShape(CollisionShape& shape);

        /// \brief Get the collision shape
        ///
        /// \return Pointer to the shape. nullptr if none bound
        ///
        const CollisionShape* getCollisionShape() const;

    protected:

        std::unique_ptr<btCollisionObject> m_body;          ///< Body data
        World& m_worldRef;                                  ///< Reference to the world
        std::set<ContactListener*> m_listeners;             ///< Listeners registered for this collider
        bool m_detached;                                    ///< Is this body detached from the world?
        bool m_allowSleep;                                  ///< Is sleep allowed?
    };
}

/// \class jop::Collider
/// \ingroup physics

#endif