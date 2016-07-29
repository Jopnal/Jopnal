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

#ifndef JOP_ROPEJOINT2D_HPP
#define JOP_ROPEJOINT2D_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics2D/Joint2D.hpp>

//////////////////////////////////////////////

class b2RopeJoint;

namespace jop
{
    class JOP_API RopeJoint2D : public Joint2D
    {
    public:

        /// \brief RopeJoint2D constructor.
        ///
        /// \param collide Joined bodies collide with each other.
        /// \param length Length of the rope.
        /// \param stiff True: Bodies are connected like with a solid block. False: Bodies are connected like with a real rope.
        ///
        RopeJoint2D(World2D& worldRef, RigidBody2D& bodyA, RigidBody2D& bodyB, const bool collide, const float length, const bool stiff);

        /// \param frequency Damping frequency in Hz.
        /// \param damping Damping ratio. Please use values between 0.f - 1.0f.
        ///
        /// \return Returns reference to self.
        ///
        RopeJoint2D& setDamping(const float frequency, const float damping);

    private:
        unsigned int m_type;

    };
}

#endif