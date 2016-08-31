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

    #include <Jopnal/Audio/AlTry.hpp>
    #include <Jopnal/Audio/SoundBuffer.hpp>
    #include <Jopnal/Utility/CommandHandler.hpp>
    #include <glm/common.hpp>
    #include <AL/al.h>

#endif

//////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_COMMAND_HANDLER(SoundEffect)

        JOP_BIND_MEMBER_COMMAND(&SoundEffect::play, "playEffect");
        JOP_BIND_MEMBER_COMMAND(&SoundEffect::pause, "pauseEffect");
        JOP_BIND_MEMBER_COMMAND(&SoundEffect::stop, "stopEffect");
        JOP_BIND_MEMBER_COMMAND(&SoundEffect::setOffset, "setEffectOffset");
        JOP_BIND_MEMBER_COMMAND(&SoundEffect::setLoop, "setEffectLoop");

    JOP_END_COMMAND_HANDLER(SoundEffect)
}

namespace jop
{
    SoundEffect::SoundEffect(Object& object)
        : SoundSource   (object, 0),
          m_resetSound  (false),
          m_buffer      ()
    {
        alTry(alGenSources(1, &m_source));
        alTry(alSourcei(m_source, AL_BUFFER, 0));

        setBuffer(SoundBuffer::getDefault());
    }

    SoundEffect::SoundEffect(const SoundEffect& other, Object& newObj)
        : SoundSource   (other, newObj),
          m_resetSound  (false),
          m_buffer      (other.m_buffer)
    {
        alTry(alGenSources(1, &m_source));
        alTry(alSourcei(m_source, AL_BUFFER, 0));
    }

    SoundEffect::~SoundEffect()
    {
        stop();

        if (!m_buffer.expired())
            m_buffer->detachSound(this);
    }

    //////////////////////////////////////////////

    void SoundEffect::update(const float deltaTime)
    {
        SoundSource::update(deltaTime);
        
        if (m_delayCounter < 0.f && m_delayCounter > -0.5f)
        {
            playReset();
            m_delayCounter = -1.f;
        }
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::setBuffer(const SoundBuffer& buffer)
    {
        if (!m_buffer.expired())
        {
            stop();
            m_buffer->detachSound(this);
        }

        m_buffer = static_ref_cast<const SoundBuffer>(buffer.getReference());
        m_buffer->attachSound(this);

        alTry(alSourcei(m_source, AL_BUFFER, m_buffer->m_bufferId));

        return *this;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::play()
    {
        if (getStatus() < Status::Playing)
            return playReset();

        return *this;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::playReset()
    {
        if (isSpeedOfSound() && m_buffer->m_info.channelCount == 1)
        {
            if (!m_calculateDelay)
            {
                m_calculateDelay = true;
                return *this;
            }
            else
                m_calculateDelay = false;
        }
        
        alTry(alSourcePlay(m_source));

        return *this;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::stop()
    {
        alTry(alSourceStop(m_source));

        return *this;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::pause()
    {
        alTry(alSourcePause(m_source));

        return *this;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::setOffset(const float time)
    {
        alTry(alSourcef(m_source, AL_SEC_OFFSET, glm::clamp(time, 0.f, m_buffer->m_duration)));

        return *this;
    }

    //////////////////////////////////////////////

    float SoundEffect::getOffset() const
    {
        ALfloat secs = 0.f;
        alTry(alGetSourcef(m_source, AL_SEC_OFFSET, &secs));

        return secs;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::setLoop(const bool loop)
    {
        alTry(alSourcei(m_source, AL_LOOPING, loop));

        return *this;
    }

    //////////////////////////////////////////////

    bool SoundEffect::isLooping() const
    {
        ALint loop;
        alTry(alGetSourcei(m_source, AL_LOOPING, &loop));

        return loop != 0;
    }
}