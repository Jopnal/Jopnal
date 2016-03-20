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
#include <memory>

//////////////////////////////////////////////


class btCollisionObject;
class btMotionState;
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

        bool checkOverlap(const Collider& other) const;

        bool checkContact(const Collider& other) const;

        bool checkRay(const glm::vec3& start, const glm::vec3& ray);

    private:

        void setActive(const bool) override = 0;

    protected:

        std::unique_ptr<btMotionState> m_motionState;   ///< The motion state
        std::unique_ptr<btCollisionObject> m_body;      ///< Body data
        World& m_worldRef;                              ///< Reference to the world
    };
}

#endif