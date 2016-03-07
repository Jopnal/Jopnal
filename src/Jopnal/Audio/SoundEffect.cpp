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
    SoundEffect::SoundEffect(Object& object, const std::string& ID)
        :SoundSource(object, ID)
    {
        m_sound = std::make_unique < sf::Sound >();
        m_playOnce = false;
        m_playWithSpeed = false;
    }

    SoundEffect::SoundEffect(const SoundEffect& other)
        : SoundSource(other)
    {
        m_sound = std::make_unique < sf::Sound >();
        m_playOnce = false;
		m_ms = 343.0f;
      //  dynamic_cast<sf::Sound*>(*m_sound.get) = dynamic_cast<sf::Sound*>(other.m_sound.get());
        m_playWithSpeed = other.m_playWithSpeed;
    }
    //////////////////////////////////////////////

    SoundEffect::~SoundEffect()
    {}

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::setBuffer(const std::string& path)
    {
        static_cast<sf::Sound*>(m_sound.get())->setBuffer(ResourceManager::getResource<SoundBuffer>(path).getSfmlBuffer());

        return *this;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::play(bool reset)
    {
        m_resetSound = reset;

        if (!m_playWithSpeed)
        {
            if (static_cast<sf::Sound*>(m_sound.get())->getStatus() == sf::Sound::Status::Playing)
            {
                if (reset)
                    return *this;
            }
            static_cast<sf::Sound*>(m_sound.get())->play();
            return *this;
        }
        else
        {
            if (m_playOnce)
            {
                if (static_cast<sf::Sound*>(m_sound.get())->getStatus() == sf::Sound::Status::Playing)
                {
                    if (reset)
                        return *this;
                }
                static_cast<sf::Sound*>(m_sound.get())->play();
                return *this;
            }
            else
            {
                calculateSound();
                m_playOnce = true;
                return *this;
            }
        }
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::play()
    {
        m_resetSound = false;

        if (!m_playWithSpeed)
        {
            static_cast<sf::Sound*>(m_sound.get())->play();
            return *this;
        }
        else
        {
            if (m_playOnce)
            {
                static_cast<sf::Sound*>(m_sound.get())->play();
                return *this;
            }
            else
            {
                calculateSound();
                m_playOnce = true;
                return *this;
            }
        }

    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::stop()
    {
        static_cast<sf::Sound*>(m_sound.get())->stop();

        return *this;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::pause()
    {
        static_cast<sf::Sound*>(m_sound.get())->pause();

        return *this;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::setOffset(const float time)
    {
        sf::Time t(sf::seconds(time));
        static_cast<sf::Sound*>(m_sound.get())->setPlayingOffset(t);

        return *this;
    }

    //////////////////////////////////////////////

    float SoundEffect::getOffset()
    {
        return static_cast<sf::Sound*>(m_sound.get())->getPlayingOffset().asSeconds();
    }

    //////////////////////////////////////////////

    enum status SoundEffect::getStatus()
    {
        return status(static_cast<sf::Music*>(m_sound.get())->getStatus());
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::setLoop(const bool loop)
    {
        static_cast<sf::Sound*>(m_sound.get())->setLoop(loop);

        return *this;
    }
}