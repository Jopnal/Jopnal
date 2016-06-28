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
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Audio/SoundEffect.hpp>

    #include <Jopnal/Audio/SoundBuffer.hpp>
    #include <Jopnal/Utility/CommandHandler.hpp>

#endif

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



        return *this;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::stop()
    {
        m_playOnce = false;

        return *this;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::pause()
    {
        return *this;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::setOffset(const float time)
    {

        return *this;
    }

    //////////////////////////////////////////////

    float SoundEffect::getOffset() const
    {
        return 0.f;
    }

    //////////////////////////////////////////////

    SoundSource::Status SoundEffect::getStatus() const
    {
        return Status::Stopped;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::setLoop(const bool loop)
    {
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
    }

    //////////////////////////////////////////////

    void SoundEffect::allowSound(const float deltaTime)
    {
        if ((m_speedCounter -= deltaTime) <= 0.0f)
        {

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