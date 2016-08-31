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

#ifndef JOP_JOINT2D_HPP
#define JOP_JOINT2D_HPP

// Headers
#include <Jopnal/Header.hpp>

//////////////////////////////////////////////


class b2Joint;
class b2Body;

namespace jop
{
    class RigidBody2D;
    class World2D;

    class JOP_API Joint2D
    {
        friend class RigidBody2D;
        friend class GearJoint2D;

    public:
        
        /// \brief Joint2D constructor
        ///
        /// \param worldRef Reference to the current world. Obtained and handled by the engine.
        /// \param bodyA RigidBody2D where to attach this joint. Received automatically from the calling function.
        /// \param bodyB RigidBody2D in which to attach the second end of the joint. Given by user as an argument.
        /// \param collide Boolean whether the linked bodies should collide with each other.
        ///
        Joint2D(World2D& worldRef, RigidBody2D& bodyA, RigidBody2D& bodyB, const bool collide);

        virtual ~Joint2D() = 0;

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
        Joint2D& setID(const unsigned int id);

        World2D* m_worldRef; ///< Pointer to the world.
        RigidBody2D* m_bodyA; ///< Pointer to bodyA.
        RigidBody2D* m_bodyB; ///< Pointer to bodyB.

    protected:

        /// \brief Return engine internal pointer to body.
        ///
        /// \param body RigidBody2D.
        ///
        /// \return Returns pointer to Box2D body.
        ///
        static b2Body* getBody(RigidBody2D& body); 

        bool m_collide; ///< Should the linked bodies collide.
        b2Joint* m_joint; ///< Pointer to Box2D joint.
        unsigned int m_ID; ///< Joint ID.
    };
}

/// \class jop::Joint2D
/// \ingroup physics2d

#endif