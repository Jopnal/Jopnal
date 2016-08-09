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
        //friend class GearJoint;

    public:
        /// \param bodyA RigidBody where to attach this joint. Received automatically from the calling function.
        /// \param bodyB RigidBody in which to attach the second end of the joint. Given by user as the first argument.
        ///
        Joint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide);
        virtual ~Joint() = 0;

        unsigned int getID() const;
        Joint& setID(const unsigned int id);

        World* m_worldRef;

        RigidBody* m_bodyA;
        RigidBody* m_bodyB;

    protected:

        btRigidBody* getBody(RigidBody& body) const; 
        btDiscreteDynamicsWorld& getWorld(World& world) const;

        glm::vec3 defaultCenter(const glm::vec3& jPos) const;
        glm::vec3 computeCenter() const;

        bool m_collide;
        std::unique_ptr<btTypedConstraint> m_joint;
        unsigned int m_ID;
    };
}
#endif