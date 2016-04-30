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

// Headers
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
    JOP_DERIVED_COMMAND_HANDLER(Component, SoundStream)

        JOP_BIND_MEMBER_COMMAND_NORETURN((SoundStream& (SoundStream::*)(const bool reset))&SoundStream::play, "playStream");
        JOP_BIND_MEMBER_COMMAND_NORETURN(&SoundStream::pause, "pauseStream");
        JOP_BIND_MEMBER_COMMAND_NORETURN(&SoundStream::stop, "stopStream");
        JOP_BIND_MEMBER_COMMAND_NORETURN(&SoundStream::setOffset, "setStreamOffset");
        JOP_BIND_MEMBER_COMMAND_NORETURN(&SoundStream::setLoop, "setStreamLoop");

    JOP_END_COMMAND_HANDLER(SoundStream)
}

namespace jop
{
    namespace detail
    {
        class AudioStream : public sf::InputStream
        {
            FileLoader m_loader;

        public:

            AudioStream(const std::string& path)
                : sf::InputStream   (),
                  m_loader          (path)
            {}

            operator bool() const
            {
                return m_loader.isValid();
            }

            //////////////////////////////////////////////

            sf::Int64 read(void* data, sf::Int64 size) override
            {
                return m_loader.read(data, size);
            }

            sf::Int64 seek(sf::Int64 position) override
            {
                m_loader.seek(position);
                return tell();
            }

            sf::Int64 tell() override
            {
                return m_loader.tell();
            }

            sf::Int64 getSize() override
            {
                return m_loader.getSize();
            }
        };
    }

    //////////////////////////////////////////////

    SoundStream::SoundStream(Object& object)
        : SoundSource   (object, "soundstream"),
          m_path        (),
          m_stream      ()
    {
        m_sound = std::make_unique<sf::Music>();
    }

    SoundStream::SoundStream(const SoundStream& other, Object& newObj)
        : SoundSource   (other, newObj),
          m_path        (),
          m_stream      ()
    {
        auto& s = static_cast<sf::Music&>(*(m_sound = std::make_unique<sf::Music>()));
        auto& o = static_cast<const sf::Music&>(*other.m_sound);
        
        s.setPitch              (o.getPitch());
        s.setVolume             (o.getVolume());
        s.setRelativeToListener (o.isRelativeToListener());
        s.setMinDistance        (o.getMinDistance());
        s.setAttenuation        (o.getAttenuation());
        s.setLoop               (o.getLoop());

        if (!other.m_path.empty())
            setPath(other.m_path);
    }

    SoundStream::~SoundStream()
    {
        // Sound needs to be freed before the file stream
        m_sound.reset();
    }

    //////////////////////////////////////////////

    bool SoundStream::setPath(const std::string& path)
    {
        if (path.empty())
        {
            JOP_DEBUG_ERROR("Failed to open sound stream (object \"" << getObject()->getID() << "\"), file path is empty");
            return false;
        }

        auto stream = std::make_unique<detail::AudioStream>(path);

        if (stream->operator bool() && static_cast<sf::Music*>(m_sound.get())->openFromStream(*stream))
        {
            JOP_DEBUG_INFO("Successfully opened sound source: " << path);
            m_path = path;
            m_stream = std::move(stream);

            return true;
        }

        JOP_DEBUG_ERROR("Failed to open sound stream, unable to open file stream: " << path);

        return false;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::play(const bool reset)
    {
        if (reset || getStatus() < Status::Playing)
            play();

        return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::play()
    {
        static_cast<sf::Music*>(m_sound.get())->play();
        return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::stop()
    {
        static_cast<sf::Music*>(m_sound.get())->stop();
        return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::pause()
    {
        static_cast<sf::Music*>(m_sound.get())->pause();
        return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::setOffset(const float time)
    {
        sf::Time t(sf::seconds(time));
        static_cast<sf::Music*>(m_sound.get())->setPlayingOffset(t);

        return *this;
    }

    //////////////////////////////////////////////

    float SoundStream::getOffset() const
    {
        return static_cast<sf::Music*>(m_sound.get())->getPlayingOffset().asSeconds();
    }

    //////////////////////////////////////////////

    SoundSource::Status SoundStream::getStatus() const
    {
        return static_cast<Status>(static_cast<sf::Music*>(m_sound.get())->getStatus());
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::setLoop(const bool loop)
    {
        static_cast<sf::Music*>(m_sound.get())->setLoop(loop);
        return *this;
    }
}