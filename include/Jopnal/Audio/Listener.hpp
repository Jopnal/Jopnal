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

#ifndef JOP_LISTENER_HPP
#define JOP_LISTENER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Component.hpp>
#include <string>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Listener : public Component
    {
    protected:

        /// \brief Copy constructor
        ///
        Listener(const Listener& other, Object& newObj);

        JOP_DISALLOW_COPY_MOVE(Listener);
        JOP_GENERIC_COMPONENT_CLONE(Listener);

    public:

        /// \brief Constructor
        ///
        /// \param object Reference to the object this component will be bound to
        /// \param ID Unique component identifier
        ///
        Listener(Object& object);


        /// \brief Automatically updates position and direction
        ///
        void update(const float deltaTime) override;

        /// \brief Change the global volume of all the sounds and musics
        ///
        /// \param Global volume in range of 0-100.0f
        ///
        void setGlobalVolume(const float volume);

        /// \brief Get the current value of the global volume
        ///
        float getGlobalVolume() const;
    };
}
#endif

/// \class Listener
/// \ingroup Audio
///
/// Handles audio's position distance and volume retrieving according to source/retriever aspect
/// Only one instance of this class can be created at time. SFML listener can't handle multible retrievers.
