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
#include <Jopnal/Utility/Message.hpp>
#include <memory>
#include <string>

//////////////////////////////////////////////

namespace sf
{
    class Sound;
}

namespace jop
{
    class Object;

    class SoundSource : public Component
    {
    public:

        /// \brief Constructor
        ///
        /// \param object Reference to the object this component will be bound to
        /// \param ID Unique component identifier
        ///
        SoundSource(Object& object, const std::string& ID);


        /// \brief Copy constructor
        ///
        SoundSource(const SoundSource& other);

        /// \brief Check for copy constructor's need
        ///
        JOP_GENERIC_CLONE(SoundSource);

        /// \brief Virtual destructor
        ///
        ~SoundSource();

        /// \brief Automatically updates position
        ///
        void update(const float deltaTime)override;

        /// \brief Load sound buffer from resource manager
        ///
        /// Can not be constant
        ///
        /// \param Path to audio file
        ///
        SoundSource& setBuffer(const std::string& path);

        /// \brief Play sound
        ///
        /// \param Enum Does sound start from beginning or continaue if already playing
        ///
        SoundSource& Play(bool reset);

        /// \brief Stop playing sound
        ///
        SoundSource& Stop();

        /// \brief Pause sound
        ///
        SoundSource& Pause();

        /// \brief Set sound's volume
        ///
        /// \param Float 0-100.0f default is 100.0f
        ///
        SoundSource& setVolume(const float& vol);

        /// \brief Returns float volume
        ///
        float getVolume();

        /// \brief Set from which point as seconds the sound starts playing
        ///
        /// \param Float time
        ///
        SoundSource& setOffset(const float& time);

        /// \brief Returns point where sound is playing as seconds
        ///
        const float getOffset();

        /// \brief Set pitch
        ///
        /// \param Float default is 1
        ///
        SoundSource& setPitch(const float& value);

        /// \brief Returns pitch
        ///
        const float getPitch();

        /// \brief Returns status Playing, Paused or Stopped as enum
        ///
        int getStatus();

        /// \brief Toggle sound on/off
        ///
        /// \param Boolean true iquals on and false iquals off
        ///
        SoundSource& setLoop(bool loop);

        /// \brief Toggle listener on/off
        ///
        /// \param Boolean true iquals on and false iquals off
        ///
        SoundSource& setListener(bool toggle);

        /// \brief Change stream's fade and distance of max volume
        ///
        /// \param Attenuation 0-100.0f and minDistance 1<x
        ///
        SoundSource& setAttenuationAndMinDistance(const float& at, const float& min);

        /// \brief Clone m_sound
        ///
        sf::Sound clone();

    private:
        std::unique_ptr<sf::Sound> m_sound;      ///< Unique audio sample
    };
}

#endif

/// \class SoundSource
/// \ingroup Audio
///
/// Audio component that plays sound