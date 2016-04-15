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

#ifndef JOP_SOUNDEFFECT_HPP
#define JOP_SOUNDEFFECT_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Audio/SoundSource.hpp>

//////////////////////////////////////////////


namespace sf
{
    class Sound;
}

namespace jop
{
    class JOP_API SoundEffect : public SoundSource
    {
    private:

        JOP_DISALLOW_COPY_MOVE(SoundEffect);
        JOP_GENERIC_COMPONENT_CLONE(SoundEffect);

    public:

        /// \brief Constructor
        ///
        /// \param object Reference to the object this component will be bound to
        ///
        SoundEffect(Object& object);

        /// \brief Copy constructor
        ///
        /// \param other The other sound to be copied
        /// \param newObj The new object
        ///
        SoundEffect(const SoundEffect& other, Object& newObj);


        /// \brief Update
        ///
        /// Automatically updates position.
        ///
        /// \param deltaTime The delta time
        ///
        void update(const float deltaTime) override;

        
        /// \brief Set the sound buffer
        ///
        /// \warning The bound buffer has to stay alive as long as it's being used
        ///
        /// \param buffer Reference to the buffer
        ///
        /// \return Reference to self
        ///
        SoundEffect& setBuffer(const SoundBuffer& buffer);

        /// \brief Play sound
        ///
        /// \comm playEffect
        ///
        /// \param reset Does sound start from beginning (true) or continue if already playing (false)
        ///
        /// \return Reference to self
        ///
        SoundEffect& play(const bool reset);

        /// \brief Play sound from start
        ///
        /// This is the same as calling play(true).
        ///
        /// \return Reference to self
        ///
        /// \see stop
        /// \see pause
        ///
        SoundEffect& play();

        /// \brief Stop playing sound
        ///
        /// After this call, the playing offset will be at the beginning.
        ///
        /// \comm stopEffect
        ///
        /// \return Reference to self
        ///
        /// \see play
        /// \see pause
        ///
        SoundEffect& stop();

        /// \brief Pause sound
        ///
        /// The playing offset will be left at the current position.
        ///
        /// \comm pauseEffect
        ///
        /// \return Reference to self
        ///
        /// \see play
        /// \see stop
        ///
        SoundEffect& pause();

        /// \brief Set from which point the sound starts playing
        ///
        /// The value will be clamped to fit the buffer's length.
        ///
        /// \comm setEffectOffset
        ///
        /// \param time The time point as seconds
        ///
        /// \return Reference to self
        ///
        SoundEffect& setOffset(const float time);

        /// \brief Returns point where sound is playing
        ///
        /// \return The playing offset in seconds
        ///
        float getOffset() const;

        /// \brief Get the status
        ///
        /// \return The playing status
        ///
        Status getStatus() const;

        /// \brief Enable/disable looping
        ///
        /// If looping is enabled, the sound will loop indefinitely until stopped.
        ///
        /// \comm setEffectLoop
        ///
        /// \param loop True to set this sound to loop
        ///
        /// \return Reference to self
        ///
        SoundEffect& setLoop(const bool loop);


        /// \brief Use speed of sound
        ///
        /// When speed of sound is enabled, the sound simulates the time it takes the sound wave to travel.
        /// This means that when a sound is far away, it will only be played when it actually reaches the listener.
        ///
        /// (default = 343.0f * global * personal)
        ///
        /// \comm speedOfSound
        ///
        /// \param use True to enable
        ///
        /// \return Reference to self
        ///
        /// \see setGlobalSpeedOfSound
        /// \see setPersonalSpeed
        ///
        SoundEffect& speedOfSound(const bool use);

        /// \brief Change global speed multiplier for sounds
        ///
        /// \param speed Speed multiplier
        ///
        static void setGlobalSpeedOfSound(const float speed);

        /// \brief Returns global speed multiplier for sounds
        ///
        /// \return The global speed multiplier
        ///
        static float getGlobalSpeedOfSound();


        /// \brief Change sound's personal speed multiplier
        ///
        /// \comm setPersonalSpeed
        ///
        /// \param speed Speed multiplier
        ///
        /// \return Reference to self
        ///
        SoundEffect& setPersonalSpeed(const float speed);

        /// \brief Returns sound's personal speed multiplier
        ///
        /// \return The personal speed multiplier
        ///
        float getPersonalSpeed() const;

    private:

        /// \brief Automated calculation when sound is allowed to play
        ///
        void calculateSound();

        /// \brief Automatic check if sound is allowed to play
        ///
        /// \param deltaTime The delta time
        ///
        void allowSound(const float deltaTime);


        float m_speedCounter;   ///< Counter for speed of sound
        bool m_playWithSpeed;   ///< Calculate when sound is allowed to play
        bool m_playOnce;        ///< Breaks link from update to calculate sound()
        bool m_resetSound;      ///< When using speed of sound saves value from play(bool)
        float m_personalSpeed;  ///< Sound's own personal speed of sound multiplier
    };
}
#endif

/// \class SoundEffect
/// \ingroup Audio
///
/// Audio component that plays sound