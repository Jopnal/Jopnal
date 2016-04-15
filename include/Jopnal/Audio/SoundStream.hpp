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
#include <Jopnal/Audio/SoundSource.hpp>

//////////////////////////////////////////////


namespace sf
{
    class Music;
}

namespace jop
{
    class JOP_API SoundStream : public SoundSource
    {
    private:

        JOP_DISALLOW_COPY_MOVE(SoundStream);
        JOP_GENERIC_COMPONENT_CLONE(SoundStream);

    public:

        /// \brief Constructor
        ///
        /// \param object Reference to the object this component will be bound to
        ///
        SoundStream(Object& object);

        /// \brief Copy constructor
        ///
        /// \param other The other sound stream to be copied
        /// \param newObj New object for this stream component
        ///
        SoundStream(const SoundStream& other, Object& newObj);


        /// \brief Stream audio from file
        ///
        /// This will not start playing the stream. You must call play() in addition.
        ///
        /// \param path Path to audio file
        ///
        /// \return True if loaded successfully
        ///
        bool setPath(const std::string& path);

        /// \copydoc SoundEffect::play(const bool)
        ///
        SoundStream& play(const bool reset);

        /// \copydoc SoundEffect::play
        ///
        SoundStream& play();

        /// \copydoc SoundEffect::stop
        ///
        SoundStream& stop();

        /// \copydoc SoundEffect::pause
        ///
        SoundStream& pause();

        /// \copydoc SoundEffect::setOffset
        ///
        SoundStream& setOffset(const float time);

        /// \copydoc SoundEffect::getOffset
        ///
        float getOffset() const;

        /// \copydoc SoundEffect::getStatus
        ///
        Status getStatus() const;

        /// \copydoc SoundEffect::setLoop
        ///
        SoundStream& setLoop(const bool loop);

    private:

        std::string m_path; ///< Remembers streaming path for cloning
    };
}
#endif

/// \class SoundStream
/// \ingroup Audio
///
/// Sound streaming straight from file 
/// Uses SFML Music.hpp streaming instead of FileLoader.hpp