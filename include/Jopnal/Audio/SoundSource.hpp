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

#ifndef JOP_SOUNDSOURCE_HPP
#define JOP_SOUNDSOURCE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Component.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <string>

//////////////////////////////////////////////

namespace jop
{

    class JOP_API SoundSource : public Component
    {
    private:

        friend class Listener;

    protected:

        /// \brief Copy constructor
        ///
        /// \param other The other sound source to be copied
        /// \param newObj The new object for this sound source
        ///
        SoundSource(const SoundSource& other, Object& newObj);

        JOP_DISALLOW_COPY_MOVE(SoundSource);

    public:

        /// Sound status
        ///
        enum class Status
        {
            Stopped,    ///< Sound is stopped
            Paused,     ///< Sound is paused
            Playing     ///< Sound is playing
        };

    public:

        /// \brief Constructor
        ///
        /// \param object Reference to the object this component will be bound to
        /// \param ID Component identifier
        ///
        SoundSource(Object& object, const uint32 ID);

        /// \brief Virtual destructor
        ///
        virtual ~SoundSource() override = 0;


        /// \brief Update
        ///
        /// Automatically updates position.
        ///
        /// \param deltaTime The delta time
        ///
        void update(const float deltaTime) override;

        /// \brief Set sound's volume
        ///
        /// The value will be clamped inside the appropriate range.
        ///
        /// \comm setVolume
        ///
        /// \param vol Float 0-100.0f default is 100.0f
        ///
        /// \return Reference to self
        ///
        SoundSource& setVolume(const float vol);

        /// \brief Get the volume
        ///
        /// \return The volume
        ///
        float getVolume() const;

        /// \brief Set pitch
        ///
        /// Be careful with this function. Setting the pitch too high or low can lead to
        /// sound artifacts or even crashes. The safe range seems to be between 0.1 and 8.
        ///
        /// \comm setPitch
        ///
        /// \param value The pitch value. Default is 1
        ///
        /// \return Reference to self
        ///
        SoundSource& setPitch(const float value);

        /// \brief Get the pitch
        ///
        /// \return The pitch value
        ///
        float getPitch() const;

        /// \brief Toggle listener on/off
        ///
        /// \comm setListener
        ///
        /// \param toggle True enables spatialization(default) and makes sound relative to listener
        ///
        /// \return Reference to self
        ///
        SoundSource& setSpatialization(const bool toggle);

        /// \brief Returns relativity to listener
        ///
        /// \return True if spatialized false if relative to listener
        ///
        bool isSpatialized() const;

        /// \brief Change sound's fade-out distance
        ///
        /// Higher values mean that the sound will fade out more quickly.
        ///
        /// \comm setAttenuation
        ///
        /// \param at Attenuation 0-100.0f
        ///
        /// \return Reference to self
        ///
        SoundSource& setAttenuation(const float at);
        
        /// \brief Change sound's distance when it is heard in max volume
        ///
        /// \comm setMinDistance
        ///
        /// \param min MinDistance 1<x
        ///
        /// \return Reference to self
        ///
        SoundSource& setMinDistance(const float min);
       
        /// \brief Returns attenuation value
        ///
        /// \return Attenuation value
        ///
        float getAttenuation() const;

        /// \brief Returns distance of max volume
        ///
        /// \return Minimum distance
        ///
        float getMinDistance() const;

        /// \brief Returns status of the sound (Stopped,Paused,Playing)
        ///
        /// \return enum
        ///
        Status getStatus() const;

        /// \brief Use object's direction for sound
        ///
        /// \param Bool true will make sound to use direction
        ///
        SoundSource& useDirection(bool use);

        /// \brief Check if sound has direction
        ///
        /// \return Is direction calculated for sound
        ///
        bool isDirection() const;

    protected:

        /// \brief Private check is speed of sound calculated
        ///
        /// \return Boolean if true speed is calculated
        ///
        static bool isSpeedOfSound();

        unsigned int m_source;   ///< Sound source
        float m_delayCounter;    ///< Sound's propagation delay
        bool m_calculateDelay;   ///< Check if delay should be calculated

    private:

        /// \brief Private handling for speed of sound for source
        ///
        /// \param Boolean if true speed of sound will be calculated
        ///
        static void calculateSpeedOfSound(const bool use);

        /// \brief Private handling for doppler effect for source
        ///
        /// \param Boolean if true doppler effect will be calculated
        ///
        static void calculateDopplerEffect(const bool use);

        /// \brief Private method to change speed of sound
        ///
        /// \return Param speed for sound
        ///
        static void setSpeedForSound(float speed);

        /// \brief Private method to get speed of sound
        ///
        /// \return Speed of sound as float
        ///
        static float getSpeedForSound();

        /// \brief Private calculation when sound must be played.
        ///
        void calculateSound();


        bool m_isDirection;     ///< Does sound have direction
        glm::vec3 m_lastPos;    ///< Used in calculating velocity
    };
}

#endif

/// \class SoundSource
/// \ingroup Audio
///
/// Base class for audio component