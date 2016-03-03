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
    SoundStream::SoundStream(Object& object, const std::string& ID)
        :SoundSource(object, ID)
    {
        m_sound = std::make_unique < sf::Music > ();
    }

    SoundStream::SoundStream(const SoundStream& other)
        : SoundSource(other)
    {
        m_sound = std::make_unique < sf::Music >();
        other.clone()->copy(static_cast<sf::Music*>(m_sound.get()));
    }
    //////////////////////////////////////////////

    SoundStream::~SoundStream()
    {}

    //////////////////////////////////////////////


    SoundStream& SoundStream::setPath(const std::string& path)
    {
        if (static_cast<sf::Music*>(m_sound.get())->openFromFile(path))
        {
            m_path = path;

            return *this;
        }
            JOP_DEBUG_ERROR("Error in SoundStream::setStream: "<<path)

            return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::Play(bool reset)
    {
        if (static_cast<sf::Music*>(m_sound.get())->getStatus() == sf::Sound::Status::Playing)
        {
            if (reset);
            return *this;
        }
        static_cast<sf::Music*>(m_sound.get())->play();
        return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::Play()
    {
        static_cast<sf::Music*>(m_sound.get())->play();
        return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::Stop()
    {
        static_cast<sf::Music*>(m_sound.get())->stop();

        return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::Pause()
    {
        static_cast<sf::Music*>(m_sound.get())->pause();

        return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::setOffset(const float& time)
    {
        sf::Time t(sf::seconds(time));
        static_cast<sf::Music*>(m_sound.get())->setPlayingOffset(t);

        return *this;
    }

    //////////////////////////////////////////////

    const float SoundStream::getOffset()
    {
        return static_cast<sf::Music*>(m_sound.get())->getPlayingOffset().asSeconds();
    }

    //////////////////////////////////////////////

    enum status SoundStream::getStatus()
    {
        return status(static_cast<sf::Music*>(m_sound.get())->getStatus());
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::setLoop(bool loop)
    {
        static_cast<sf::Music*>(m_sound.get())->setLoop(loop);

        return *this;
    }

    //////////////////////////////////////////////

    void SoundStream::copy(sf::Music* sound)
    {
        static_cast<sf::Music*>(sound)->setPlayingOffset(static_cast<sf::Music*>(m_sound.get())->getPlayingOffset());
       
        if (bool listener = static_cast<sf::Music*>(m_sound.get())->isRelativeToListener())
        {
            static_cast<sf::Music*>(sound)->setRelativeToListener(listener);
            static_cast<sf::Music*>(sound)->setPosition(static_cast<sf::Music*>(m_sound.get())->getPosition());
            static_cast<sf::Music*>(sound)->setVolume(static_cast<sf::Music*>(m_sound.get())->getVolume());
            static_cast<sf::Music*>(sound)->setPitch(static_cast<sf::Music*>(m_sound.get())->getPitch());
        }

        static_cast<sf::Music*>(sound)->openFromFile(m_path);
    }
}