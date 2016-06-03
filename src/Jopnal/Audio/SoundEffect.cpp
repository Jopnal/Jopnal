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
    JOP_REGISTER_COMMAND_HANDLER(SoundEffect)

        JOP_BIND_MEMBER_COMMAND((SoundEffect& (SoundEffect::*)(const bool reset))&SoundEffect::play, "playEffect");
        JOP_BIND_MEMBER_COMMAND(&SoundEffect::pause, "pauseEffect");
        JOP_BIND_MEMBER_COMMAND(&SoundEffect::stop, "stopEffect");
        JOP_BIND_MEMBER_COMMAND(&SoundEffect::setOffset, "setEffectOffset");
        JOP_BIND_MEMBER_COMMAND(&SoundEffect::setLoop, "setEffectLoop");
        JOP_BIND_MEMBER_COMMAND(&SoundEffect::speedOfSound, "speedOfSound");
        JOP_BIND_MEMBER_COMMAND(&SoundEffect::setPersonalSpeed, "setPersonalSpeed");

    JOP_END_COMMAND_HANDLER(SoundEffect)
}

namespace
{
    static float ns_globalFactor = 1.f;
}

namespace jop
{
    SoundEffect::SoundEffect(Object& object)
        : SoundSource       (object, 0),
          m_personalSpeed   (1.f),
          m_speedCounter    (0.f),
          m_playWithSpeed   (false),
          m_playOnce        (false),
          m_resetSound      (false)
    {
        m_sound = std::make_unique<sf::Sound>();

        m_playOnce = false;
        m_playWithSpeed = false;
    }

    SoundEffect::SoundEffect(const SoundEffect& other, Object& newObj)
        : SoundSource       (other, newObj),
          m_personalSpeed   (other.m_personalSpeed),
          m_speedCounter    (other.m_speedCounter),
          m_playWithSpeed   (other.m_playWithSpeed),
          m_playOnce        (false),
          m_resetSound      (false)
    {
        m_sound = std::make_unique<sf::Sound>(static_cast<const sf::Sound&>(*other.m_sound));
    }

    //////////////////////////////////////////////

    void SoundEffect::update(const float deltaTime)
    {
        SoundSource::update(deltaTime);

        if (m_playOnce && m_playWithSpeed)
            allowSound(deltaTime);
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::setBuffer(const SoundBuffer& buffer)
    {
        static_cast<sf::Sound*>(m_sound.get())->setBuffer(*buffer.m_soundBuf);
        return *this;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::play(const bool reset)
    {
        m_resetSound = reset;

        if (!m_playWithSpeed)
        {
            if (reset || getStatus() < Status::Playing)
                play();
        }
        else
        {
            if (m_playOnce && !reset)
                m_playOnce = true;
            else
            {
                calculateSound();
                m_playOnce = true;
            }
        }

        return *this;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::play()
    {
        m_resetSound = false;

        if (!m_playWithSpeed)
            static_cast<sf::Sound*>(m_sound.get())->play();

        else if (!m_playOnce)
        {
            calculateSound();
            m_playOnce = true;
        }

        return *this;
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
        auto& sound = *static_cast<sf::Sound*>(m_sound.get());

        sf::Time t(sf::seconds(glm::clamp(time, 0.f, sound.getBuffer()->getDuration().asSeconds())));
        sound.setPlayingOffset(t);

        return *this;
    }

    //////////////////////////////////////////////

    float SoundEffect::getOffset() const
    {
        return static_cast<sf::Sound*>(m_sound.get())->getPlayingOffset().asSeconds();
    }

    //////////////////////////////////////////////

    SoundSource::Status SoundEffect::getStatus() const
    {
        return static_cast<Status>(static_cast<sf::Sound*>(m_sound.get())->getStatus());
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

    void SoundEffect::setGlobalSpeedOfSound(const float speed)
    {
        ns_globalFactor = speed;
    }

    //////////////////////////////////////////////

    float SoundEffect::getGlobalSpeedOfSound()
    {
       return ns_globalFactor;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::setPersonalSpeed(const float speed)
    {
        m_personalSpeed = speed;
        return *this;
    }

    //////////////////////////////////////////////

    float SoundEffect::getPersonalSpeed() const
    {
        return m_personalSpeed;
    }

    //////////////////////////////////////////////

    void SoundEffect::calculateSound()
    {
        const auto target = sf::Listener::getPosition();
        const float lenght = glm::length(glm::vec3(target.x, target.y, target.z) - getObject()->getGlobalPosition());
        const float multiplier = std::max(FLT_MIN, 343.f * ns_globalFactor * m_personalSpeed);

        m_speedCounter = lenght / multiplier;
    }

    //////////////////////////////////////////////

    void SoundEffect::allowSound(const float deltaTime)
    {
        if ((m_speedCounter -= deltaTime) <= 0.0f)
        {
            if (getStatus() != Status::Playing || m_resetSound)
                static_cast<sf::Sound*>(m_sound.get())->play();

            m_playOnce = false;
        }
    }

    //////////////////////////////////////////////

    Message::Result SoundEffect::receiveMessage(const Message& message)
    {
        if (JOP_EXECUTE_COMMAND(SoundEffect, message.getString(), this) == Message::Result::Escape)
            return Message::Result::Escape;

        return SoundSource::receiveMessage(message);
    }
}