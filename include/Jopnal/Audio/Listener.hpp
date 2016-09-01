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
#include <glm/vec3.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Listener : public Component
    {
    private:

        JOP_GENERIC_COMPONENT_CLONE(Listener);

    public:

        /// \brief Constructor
        ///
        /// \param object Reference to the object this component will be bound to
        ///
        Listener(Object& object);


        /// \brief Update
        ///
        /// Automatically updates position, speed and direction.
        ///
        /// \param deltaTime The delta time
        ///
        void update(const float deltaTime) override;

        /// \brief Change the global volume of all the sounds and musics
        ///
        /// The individual volume of all sounds will be multiplied by this value.
        ///
        /// \param volume Global volume in range of 0-100.0f
        ///
        /// \see getGlobalVolume
        ///
        static void setGlobalVolume(const float volume);

        /// \brief Get the current value of the global volume
        ///
        /// \return The global volume
        ///
        static float getGlobalVolume();

        /// \brief Set doppler effect
        ///
        /// \param dop The doppler value. Default is 1
        ///
        Listener& setDopplerEffect(const float dop);

        /// \brief Get doppler effect's value
        ///
        /// \return Float multiplier of doppler effect
        ///
        float getDopplerEffect() const;

        /// \brief Sets doppler effect to default
        /// 
        /// \return Reference to self
        ///
        Listener& setDopplerEffectToDefault();

        /// \brief Set speed for sound
        ///
        /// \param speed The speed of sound. Default is 343.3
        /// 
        /// \return Reference to self
        ///
        Listener& setSpeedOfSound(const float speed);

        /// \brief Get speed of sound
        ///
        /// \return Speed as meters per second
        ///
        float getSpeedOfSound() const;

        /// \brief Calculate doppler effect for sound
        ///
        /// \param use True to use doppler effect, false to ignore
        /// 
        /// \return Reference to self
        ///
        Listener& useDopplerEffect(const bool use);

        /// \brief Calculate doppler speed for sounds
        ///
        /// \param use True to use speed, false to ignore
        /// 
        /// \return Reference to self
        ///
        Listener& useSpeedOfSound(const bool use);

        /// \brief Get boolean to know if speed of sound is in use
        /// 
        /// \return True if using speed of sound
        ///
        bool isSpeedOfSound() const;

        /// \brief Get boolean to know if doppler effect is in use
        ///
        /// \return True if using doppler effect
        ///
        bool isDopplerEffect() const;

        /// \brief Sets speed of sound to default
        /// 
        /// \return Reference to self
        ///
        Listener& setSpeedOfSoundToDefault();

    private:

        bool m_dopplerEffect;   ///< Check if sound components need to calculate velocity
        float m_doppler;        ///< Multiplier for doppler effect
        glm::vec3 m_lastPos;    ///< Used to calculate velocity
    };
}

/// \class jop::Listener
/// \ingroup audio
///
/// Handles audio's position distance and volume retrieving according to source/retriever aspect
/// Only one instance of this class can be created at time. SFML listener can't handle multiple retrievers.

#endif