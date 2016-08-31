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

#ifndef JOP_WELDJOINT_HPP
#define JOP_WELDJOINT_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics/Joint/Joint.hpp>

//////////////////////////////////////////////

class btFixedConstraint;

namespace jop
{
    class JOP_API WeldJoint : public Joint
    {
    public:
        /// \brief WeldJoint constructor.
        ///
        /// Welds two bodies together. Moving one body moves the other in similiar way.
        ///
        WeldJoint(World& worldRef, RigidBody& bodyA, RigidBody& bodyB, const bool collide);

        /// \return Returns currently used damping.
        ///
        float getDamping() const;

        /// \param damping Set new damping value. Please use values between 0.f - 1.f.
        ///
        /// \return Returns reference to self.
        ///
        WeldJoint& setDamping(const float damping);

    private:
        btFixedConstraint* m_jointL;
        float m_damping;
    };
}

/// \class jop::WeldJoint
/// \ingroup physics

#endif