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

#ifndef JOP_SOUNDSTREAM_HPP
#define JOP_SOUNDSTREAM_HPP

// Headers
#include <Jopnal/Header.hpp>

//////////////////////////////////////////////

namespace sf
{
    class Music;
}

namespace jop
{
    class Object;

    class JOP_API SoundStream : public SoundSource
    {
    public:

        /// \brief Constructor
        ///
        /// \param object Reference to the object this component will be bound to
        /// \param ID Unique component identifier
        ///
        SoundStream(Object& object, const std::string& ID);

        /// \brief Copy constructor
        ///
        SoundStream(const SoundStream& other);

        /// \brief Check for copy constructor's need
        ///
        JOP_GENERIC_CLONE(SoundStream);

        /// \brief Virtual destructor
        ///
        ~SoundStream();
        /// \brief Stream audio from file
        ///
        /// Needs relative path to asset flodder
        ///
        /// \param Path to audio file
        ///
        SoundStream& setPath(const std::string& path);

        /// \brief Get information from member variables
        ///
        /// \param SoundStream component to copy to
        ///

        /// \brief Play sound
        ///
        /// \param Boolean Does sound start from beginning or continue if already playing
        ///
        /// If true audio doesn't start over from beginging
        ///
        SoundStream& Play(bool reset);

        /// \brief Play sound from start
        ///
        SoundStream& Play();

        /// \brief Stop playing sound
        ///
        SoundStream& Stop();

        /// \brief Pause sound
        ///
        SoundStream& Pause();

        /// \brief Set from which point as seconds the sound starts playing
        ///
        /// \param Float time
        ///
        SoundStream& setOffset(const float& time);

        /// \brief Returns point where sound is playing as seconds
        ///
        const float getOffset();

        /// \brief Returns status Stopped (0), Paused (1) or Playing (2) as enum
        ///
        enum status getStatus();

        /// \brief Toggle sound on/off
        ///
        /// \param Boolean true iquals on and false iquals off
        ///
        SoundStream& setLoop(bool loop);

        /// \brief Copy information from stream component
        ///
        /// \param sf::music object
        ///
        void copy(sf::Music* sound);

    private:
        std::string m_path;                     ///< Remembers streaming path for cloning
    };
}
#endif

/// \class SoundStream
/// \ingroup Audio
///
/// Sound streaming straight from file 
/// Uses SFML Music.hpp streaming instead of FileLoader.hpp