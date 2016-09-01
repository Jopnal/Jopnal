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

#ifndef JOP_SUBSYSTEM_HPP
#define JOP_SUBSYSTEM_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Utility/SafeReferenceable.hpp>
#include <Jopnal/Utility/Message.hpp>
#include <string>
#include <memory>
#include <atomic>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Subsystem
    {
    private:

        JOP_DISALLOW_COPY_MOVE(Subsystem);

    public:

        /// \brief Constructor
        ///
        /// \param ID Identifier of this subsystem
        ///
        Subsystem(const uint32 ID);

        /// \brief Virtual destructor
        ///
        virtual ~Subsystem() = 0;


        /// \brief Pre-update
        ///
        /// This will be called before the engine calls the scene's update.
        ///
        /// \param deltaTime Delta time
        ///
        virtual void preUpdate(const float deltaTime);

        /// \brief Post-update
        ///
        /// This will be called after the engine calls the scene's update.
        ///
        /// \param deltaTime Delta time
        ///
        virtual void postUpdate(const float deltaTime);

        /// \brief Draw
        ///
        /// This will be called during rendering.
        ///
        virtual void draw();

        /// \brief Function to handle messages
        ///
        /// \param message The message
        ///
        /// \return The message result
        ///
        Message::Result sendMessage(const Message& message);

        /// \brief Set this sub system active
        ///
        /// \param active True to set active
        ///
        /// \return Reference to self
        ///
        Subsystem& setActive(const bool active);

        /// \brief Check if this sub system is active
        ///
        /// \return True if active
        ///
        bool isActive() const;

        /// \brief Get the ID
        ///
        /// \return The ID
        ///
        uint32 getID() const;

    protected:

        /// \copydoc Component::receiveMessage()
        ///
        virtual Message::Result receiveMessage(const Message& message);

    private:

        const uint32 m_ID;          ///< This subsystem's identifier
        std::atomic<bool> m_active; ///< Sets activity 
    };
}

/// \class jop::SubSystem
/// \ingroup core

#endif