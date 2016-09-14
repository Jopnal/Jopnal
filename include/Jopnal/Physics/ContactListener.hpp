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

#ifndef JOP_CONTACTLISTENER_HPP
#define JOP_CONTACTLISTENER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics/Collider.hpp>
#include <Jopnal/Physics/ContactInfo.hpp>

/////////////////////////////////////////////


namespace jop
{
    class Collider;

    class JOP_API ContactListener
    {
    private:

        friend class Collider;

    public:

        /// \brief Constructor
        ///
        ContactListener();

        /// \brief Virtual destructor
        ///
        /// Calls unregisterSelf().
        ///
        virtual ~ContactListener() = 0;


        /// \brief Unregister this listener from the collider it's bound to
        ///
        void unregisterSelf();

        /// \brief Begin contact callback
        ///
        /// \param collider Reference to the collider which registered collider is colliding with
        /// \param ci Contact info containing the contact point and contact normal
        ///
        virtual void beginContact(Collider& collider, const ContactInfo& ci);

        /// \brief End contact callback
        ///
        /// \param collider Reference to the collider which registered collider was colliding with
        ///
        virtual void endContact(Collider& collider);

        /// \brief AABB's begin overlapping
        ///
        /// \param collider Reference to the collider that is being overlapped with
        ///
        virtual void beginOverlap(Collider& collider);

        /// \brief AABB's stopped overlapping
        ///
        /// \param collider Reference to the collider that was being overlapped with
        ///
        virtual void endOverlap(Collider& collider);

    private:

        Collider* m_collider; ///< Pointer to the collider which this listener is registered for
    };
}

/// \class jop::ContactListener
/// \ingroup physics

#endif