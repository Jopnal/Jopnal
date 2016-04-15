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

//////////////////////////////////////////////


class btMotionState;
class btCollisionObject;
namespace detail
{
    struct GhostCallback;
}

namespace jop
{
    class World;

    class JOP_API Collider : public Component
    {
    private:

        JOP_DISALLOW_COPY_MOVE(Collider);

        friend struct ::detail::GhostCallback;

    protected:

        /// \brief Constructor
        ///
        /// \param object Reference to the object
        /// \param world Reference to the physics world
        /// \param ID Id of this component
        ///
        Collider(Object& object, World& world, const std::string& ID);

        /// \brief Copy constructor
        ///
        /// \param other The other collider to copy
        /// \param newObj The new object
        ///
        Collider(const Collider& other, Object& newObj);

        /// \brief Virtual destructor
        ///
        virtual ~Collider() override = 0;

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

        /// \brief Get the world this collider belongs to
        ///
        /// \return Reference to the world
        ///
        World& getWorld();

        /// \copydoc getWorld()
        ///
        const World& getWorld() const;

    private:

        /// \brief Overlap begin callback
        ///
        /// \param other The other collider that was overlapped
        ///
        virtual void beginOverlap(const Collider& other);

        /// \brief Overlap end callback
        ///
        /// \param other The other collider that was overlapping
        ///
        virtual void endOverlap(const Collider& other);

        /// \brief Activity setter
        ///
        /// For internal use
        ///
        virtual void setActive(const bool) override = 0;

    protected:

        std::unique_ptr<btMotionState> m_motionState;   ///< The motion state
        std::unique_ptr<btCollisionObject> m_body;      ///< Body data
        World& m_worldRef;                              ///< Reference to the world
    };
}

#endif