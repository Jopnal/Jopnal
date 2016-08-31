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

#ifndef JOP_JOINT_HPP
#define JOP_JOINT_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <glm/vec3.hpp>
#include <memory>

//////////////////////////////////////////////


class btCollisionObject;
class btDiscreteDynamicsWorld;
class btRigidBody;
class btTypedConstraint;

namespace jop
{
    class RigidBody;
    class World;

    class JOP_API Joint
    {
        friend class RigidBody;

    public:

        /// \brief Joint constructor.
        ///
        /// \param worldRef Reference to the current world. Obtained and handled by the engine.
        /// \param bodyA RigidBody where to attach this joint. Received automatically from the calling function.
        /// \param bodyB RigidBody in which to attach the second end of the joint. Given by user as the first argument.
        /// \param collide Boolean whether the linked bodies should collide with each other.
        ///
        Joint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide);

        virtual ~Joint() = 0;

        ///\brief Get joint ID.
        ///
        /// \return Returns ID of the joint.
        ///
        unsigned int getID() const;

        /// \brief Set new ID for the joint.
        ///
        /// \param id New ID to set.
        ///
        /// \return Returns reference to self.
        ///
        Joint& setID(const unsigned int id);

        World* m_worldRef;  ///< Pointer to the world.
        RigidBody* m_bodyA; ///< Pointer to bodyA.
        RigidBody* m_bodyB; ///< Pointer to bodyB.

    protected:

        /// \brief Return engine internal pointer to body.
        ///
        /// \param body RigidBody2D.
        ///
        /// \return Returns pointer to Box2D body.
        ///
        btRigidBody* getBody(RigidBody& body) const;

        /// \brief Return engine internal pointer to world.
        ///
        /// \param world World.
        ///
        /// \return Returns reference to Bullet world.
        ///
        btDiscreteDynamicsWorld& getWorld(World& world) const;

        /// Checks if the joint position is default (declared in the function)
        /// Returns the current position if not default.
        /// Returns the middle between the m_bodies if default.
        ///
        /// \param jPos Current position of the joint.
        ///
        /// \return Returns the joint position as glm::vec3.
        /// 
        glm::vec3 defaultCenter(const glm::vec3& jPos) const;

        /// \brief Calculates the point in the middle between this' m_bodies.
        ///
        /// \return Returns the point as glm::vec3.
        ///
        glm::vec3 computeCenter() const;

        bool m_collide;                             /// < Should the linked bodies collide.
        std::unique_ptr<btTypedConstraint> m_joint; ///< Pointer to Bullet joint.
        unsigned int m_ID;                          ///< Joint ID.
    };
}

/// \class jop::Joint
/// \ingroup physics

#endif