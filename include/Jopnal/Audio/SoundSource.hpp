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

        /// \brief Virtual destructor
        ///
        ~SoundSource();

        JOP_GENERIC_CLONE(SoundSource);

    
        /// \brief setSound
        ///
        /// \param Path to audio file
        ///
        void setSound(const std::string& path);

        /// \brief setSound with listener
        ///
        /// \param Path to audio file and position xyz
        ///
        void setSound(const std::string& path, const float& x, const float& y, const float& z);

        /// \brief Toggle sound on/off
        ///
        /// \param Boolean true iquals on and false iquals off
        ///
        void toggleSoundLoop(bool loop);

        /// \brief Toggle sound on/off and update position
        ///
        /// \param Boolean On/off and position xyz
        ///
        void toggleSoundLoop(bool loop, const float& x, const float& y, const float& z);

        /// \brief Toggle listener on/off
        ///
        /// \param Boolean true iquals on and false iquals off
        ///
        void toggleStreamListener(bool toggle);

    private:
        std::unique_ptr<sf::Sound> m_sound;      ///< Unique audio sample
    };
}

#endif

/// \class SoundSource
/// \ingroup Audio
///
/// Audio component that plays sound