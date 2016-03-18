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

namespace
{
    float ns_globalFactor=1.f;
}

namespace jop
{
    SoundEffect::SoundEffect(Object& object, const std::string& ID)
        :SoundSource(object, ID)
    {
        m_sound = std::make_unique < sf::Sound >();
        m_playOnce = false;
        m_playWithSpeed = false;
        m_personalSpeed = 1.f;
    }

    SoundEffect::SoundEffect(const SoundEffect& other)
        : SoundSource(other)
    {
        m_sound = std::make_unique < sf::Sound >(static_cast<const sf::Sound&>(*other.m_sound));
        m_playOnce = false;
        m_playWithSpeed = other.m_playWithSpeed;
        m_personalSpeed = 1.f;
    }
    //////////////////////////////////////////////

    SoundEffect::~SoundEffect()
    {}

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::setBuffer(const std::string& path)
    {
        static_cast<sf::Sound*>(m_sound.get())->setBuffer(*ResourceManager::getResource<SoundBuffer>(path).m_soundBuf);
        if (sizeof(static_cast<sf::Sound*>(m_sound.get())->getBuffer()) < 1)
        {
            JOP_DEBUG_WARNING("Size of " << getID() << "'s buffer is smaller than one.");
        }
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
                if (reset)
                    return *this;
                else
                    m_playOnce = true;
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
                m_playOnce = true;
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
        m_playOnce = false;

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
        return status(static_cast<sf::Sound*>(m_sound.get())->getStatus());
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::setLoop(const bool loop)
    {
        static_cast<sf::Sound*>(m_sound.get())->setLoop(loop);

        return *this;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::speedOfSound(const bool use)
    {
        m_playWithSpeed = use;

        return *this;
    }

    //////////////////////////////////////////////

    void SoundEffect::calculateSound()
    {
        glm::vec3 origin = getObject()->getGlobalPosition();
        glm::vec3 target = Camera::getDefault().getObject()->getGlobalPosition();
        float lenght = sqrt(pow(target.x - origin.x, 2.f) + pow(target.y - origin.y, 2.f) + pow(target.z - origin.z, 2.f));
        float multiplier = 343.f*ns_globalFactor*m_personalSpeed;

        m_speedCounter = lenght / multiplier;

        if (multiplier < 0.f)
            JOP_DEBUG_WARNING("SoundEffect: "<<getID()<<"'s speed of sound multiplier is negative. Global speed multiplier: "<<ns_globalFactor<<" Personal speed multiplier: "<<m_personalSpeed);
    }

    //////////////////////////////////////////////

    void SoundEffect::setGlobalSpeedOfSound(float speed)
    {
        ns_globalFactor = speed;
    }


    //////////////////////////////////////////////

    float SoundEffect::getGlobalSpeedOfSound()
    {
       return ns_globalFactor;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::setPersonalSpeed(float speed)
    {
        m_personalSpeed = speed;

        return *this;
    }


    //////////////////////////////////////////////

    float SoundEffect::getPersonalSpeed()
    {
        return m_personalSpeed;
    }
}