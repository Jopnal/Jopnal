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

#ifndef JOP_CONTACTLISTENER2D_HPP
#define JOP_CONTACTLISTENER2D_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics2D/Collider2D.hpp>
#include <Jopnal/Physics2D/ContactInfo2D.hpp>

//////////////////////////////////////////////


namespace jop
{
    class Collider2D;

    class JOP_API ContactListener2D 
    {
    private:

        friend class Collider2D;

    public:

        /// \brief Virtual destructor
        ///
        virtual ~ContactListener2D() = 0;

        /// \brief Begin contact callback
        ///
        /// This is called when two fixtures begin to overlap. This event can only occur inside the time step.
        /// This is called for sensors and non-sensors.
        ///
        /// \param collider Reference to the collider which is being collided with
        /// \param ci Contact info containing the contact point and contact normal
        ///
        virtual void beginContact(Collider2D& collider, const ContactInfo2D& ci);

        /// \brief End contact callback
        ///
        /// This is called when two fixtures cease to overlap. This is called for sensors and non-sensors. 
        /// This may be called when a body is destroyed, so this event can occur outside the time step.
        ///
        /// \param collider Reference to the collider which was being collided with
        ///
        virtual void endContact(Collider2D& collider);
        
    private:

        Collider2D* m_collider; ///< Pointer to the collider which this listener is registered for
    };
}

/// \class jop::ContactListener2D
/// \ingroup physics2d

#endif