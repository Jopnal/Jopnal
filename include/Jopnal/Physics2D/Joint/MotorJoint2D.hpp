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

#ifndef JOP_MOTORJOINT2D_HPP
#define JOP_MOTORJOINT2D_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics2D/Joint2D.hpp>

//////////////////////////////////////////////

class b2MotorJoint;

namespace jop
{
    class JOP_API MotorJoint2D : public Joint2D
    {
    public:

        MotorJoint2D(World2D& worldRef, RigidBody2D& bodyA, RigidBody2D& bodyB);

        glm::vec2 getLinearOffset();
        float getAngularOffset();
        MotorJoint2D& setLinearOffset(const glm::vec2& offset);
        MotorJoint2D& setAngularOffset(const float offset);
        MotorJoint2D& setForces(const float force, const float torque);


    private:
        b2MotorJoint* m_jointL;


    };
}

#endif