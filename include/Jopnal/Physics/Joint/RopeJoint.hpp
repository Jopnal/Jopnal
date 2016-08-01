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

#ifndef JOP_ROPEJOINT_HPP
#define JOP_ROPEJOINT_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics/Joint.hpp>

//////////////////////////////////////////////

class btGeneric6DofConstraint;

namespace jop
{
    class JOP_API RopeJoint : public Joint
    {
    public:
        /// \brief RopeJoint constructor.
        ///
        /// Length of the rope is calculated from the difference in global coordinates between the two bodies.
        /// Longer ropes, where accurate physics computations are needed, should be created by creating separate (rigid)bodies and attaching RopeJoint to them one after another.
        ///
        /// \param collide Joined bodies collide with each other.
        ///
        RopeJoint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide);
        
    private:
        btGeneric6DofConstraint* m_jointL;
    };
}

#endif