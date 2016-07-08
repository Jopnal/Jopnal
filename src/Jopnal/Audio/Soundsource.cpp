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

    #include <Jopnal/Audio/SoundSource.hpp>

    #include <Jopnal/Utility/CommandHandler.hpp>

#endif

//////////////////////////////////////////////


namespace
{
    bool ns_isSpeedOfSound = false;
    bool ns_isDopplerEffect = false;
    float ns_speedForSound = 343.3f;
}

namespace jop
{
    JOP_REGISTER_COMMAND_HANDLER(SoundSource)

    JOP_BIND_MEMBER_COMMAND(&SoundSource::setVolume, "setVolume");
    JOP_BIND_MEMBER_COMMAND(&SoundSource::setPitch, "setPitch");
    JOP_BIND_MEMBER_COMMAND(&SoundSource::setMinDistance, "setMinDistance");
    JOP_BIND_MEMBER_COMMAND(&SoundSource::setAttenuation, "setAttenuation");

    JOP_END_COMMAND_HANDLER(SoundSource)
}

namespace jop
{
    SoundSource::SoundSource(Object& object, const uint32 ID)
        : Component         (object, ID),
          m_source          (NULL),
          m_lastPos         (NULL),
          m_isDirection     (false),
          m_calculateDelay  (false),
          m_delayCounter    (-1.f)
    {
        update(0.f);
    }

    SoundSource::SoundSource(const SoundSource& other, Object& newObj)
        : Component         (other, newObj),
          m_source          (NULL),
          m_lastPos         (NULL),
          m_isDirection     (false),
          m_calculateDelay  (false),
          m_delayCounter    (-1.f)
    {
        setPitch(other.getPitch());
        setVolume(other.getVolume());
        setSpatialization(other.isSpatialized());
        setMinDistance(other.getMinDistance());
        setAttenuation(other.getAttenuation());

        update(0.f);
    }

    SoundSource::~SoundSource()
    {
        alTry(alSourcei(m_source, AL_BUFFER, 0));
        alTry(alDeleteSources(1, &m_source));
    }

    //////////////////////////////////////////////

    void SoundSource::update(const float deltaTime)
    {
        glm::vec3 pos = getObject()->getGlobalPosition();    
        alTry(alSource3f(m_source, AL_POSITION, pos.x, pos.y, pos.z));

        if (ns_isDopplerEffect)
        {
            glm::vec3 pos = getObject()->getGlobalPosition();
            m_lastPos -= glm::abs(pos);
            ALfloat speed[] = { m_lastPos.x, m_lastPos.y, m_lastPos.z };
            alTry(alSourcefv(m_source, AL_VELOCITY, speed));
            m_lastPos = glm::abs(pos);
        }

        if (m_isDirection)
        {
            glm::vec3 front = getObject()->getGlobalFront();
            glm::vec3 up = getObject()->getGlobalUp();
            ALfloat  direction[] = { front.x, front.y, front.z, up.x, up.y, up.z };
            alTry(alSourcefv(m_source, AL_ORIENTATION, direction));
        }
        if (m_calculateDelay)
        {
            if (m_delayCounter < -0.5f)
                calculateSound();
            else
                m_delayCounter -= deltaTime;
        }
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setVolume(const float vol)
    {
        alTry(alSourcef(m_source, AL_GAIN, glm::clamp(vol, 0.f, 100.f) * 0.01f));

        return *this;
    }

    //////////////////////////////////////////////

    float SoundSource::getVolume() const
    {
        ALfloat vol;
        alTry(alGetSourcef(m_source, AL_GAIN, &vol));

        return vol * 100.f;

    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setPitch(const float value)
    {
        alTry(alSourcef(m_source, AL_PITCH, std::max(value, 0.f)));
        
        return *this;
    }

    //////////////////////////////////////////////

    float SoundSource::getPitch() const
    {
        ALfloat pitch;
        alTry(alGetSourcef(m_source, AL_PITCH, &pitch));
      
        return pitch;
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setSpatialization(const bool toggle)
    {
        alTry(alSourcei(m_source, AL_SOURCE_RELATIVE, !toggle));
       
        return *this;
    }

    //////////////////////////////////////////////

    bool SoundSource::isSpatialized() const
    {
        ALint relative;
        alTry(alGetSourcei(m_source, AL_SOURCE_RELATIVE, &relative));

        return !relative;
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setAttenuation(const float at)
    {
        alTry(alSourcef(m_source, AL_ROLLOFF_FACTOR, glm::clamp(at, 0.f, 100.f)));

        return *this;
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setMinDistance(const float min)
    {
        alTry(alSourcef(m_source, AL_REFERENCE_DISTANCE, std::max(1.f, min)));

        return *this;
    }

    //////////////////////////////////////////////

    float SoundSource::getAttenuation() const
    {
        ALfloat attenuation;
        alTry(alGetSourcef(m_source, AL_ROLLOFF_FACTOR, &attenuation));

        return attenuation;
    }

    //////////////////////////////////////////////

    float SoundSource::getMinDistance() const
    {
        ALfloat distance;
        alTry(alGetSourcef(m_source, AL_REFERENCE_DISTANCE, &distance));

        return distance;
    }

    //////////////////////////////////////////////

    SoundSource::Status SoundSource::getStatus() const
    {
        ALint status;
        alTry(alGetSourcei(m_source, AL_SOURCE_STATE, &status));

        switch (status)
        {
            case AL_INITIAL:
            case AL_STOPPED: return Status::Stopped;
            case AL_PAUSED:  return Status::Paused;
            case AL_PLAYING: return Status::Playing;
        }

        return Status::Stopped;
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::useDirection(bool use)
    {
        m_isDirection = use;

        return *this;
    }

    //////////////////////////////////////////////

    bool SoundSource::isDirection() const
    {
        return m_isDirection;
    }

    //////////////////////////////////////////////

    void SoundSource::calculateSpeedOfSound(const bool use)
    {
        ns_isSpeedOfSound = use;
    }

    //////////////////////////////////////////////

    bool SoundSource::isSpeedOfSound()
    {
        return ns_isSpeedOfSound;
    }

    //////////////////////////////////////////////

    void SoundSource::calculateDopplerEffect(const bool use)
    {
        ns_isDopplerEffect = use;
    }

    //////////////////////////////////////////////

    void SoundSource::setSpeedForSound(float speed)
    {
        ns_speedForSound = speed;
    }

    //////////////////////////////////////////////

    float SoundSource::getSpeedForSound()
    {
        return ns_speedForSound;
    }

    //////////////////////////////////////////////

    void SoundSource::calculateSound()
    {
        ALfloat target[3];
        alTry(alGetListenerfv(AL_POSITION, target));
        const float lenght = glm::length(glm::vec3(target[0], target[1], target[2]) - getObject()->getGlobalPosition());
        m_delayCounter = lenght / ns_speedForSound;
    }
}