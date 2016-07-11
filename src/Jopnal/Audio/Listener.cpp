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

    #include <Jopnal/Audio/Listener.hpp>

    #include <Jopnal/Audio/AlTry.hpp>
    #include <Jopnal/Audio/SoundSource.hpp>
    #include <Jopnal/Core/DebugHandler.hpp>
    #include <Jopnal/Core/Object.hpp>
    #include <Jopnal/Utility/CommandHandler.hpp>
    #include <AL/al.h>

#endif

//////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_COMMAND_HANDLER(Listener)

        JOP_BIND_MEMBER_COMMAND((Listener& (Listener::*)(float))&Listener::setGlobalVolume, "setGlobalVolume");

    JOP_END_COMMAND_HANDLER(Listener)
}

namespace jop
{
    Listener::Listener(Object& object)
        : Component         (object, 0),
          m_dopplerEffect   (false),
          m_doppler         (1.f),
          m_lastPos         (0.f)
    {
        const glm::vec3 pos = object.getGlobalPosition();
        const ALfloat var[] = {pos.x,pos.y,pos.z};
        alTry(alListenerfv(AL_POSITION, var));

        const ALfloat speed[] = {0.f, 0.f, 0.f};
        alTry(alListenerfv(AL_VELOCITY, speed));

        const glm::vec3 vec = object.getGlobalUp();
        const ALfloat direction[] = {pos.x, pos.y, pos.z, vec.x, vec.y, vec.z};
        alTry(alListenerfv(AL_ORIENTATION, direction));
    }

    Listener::Listener(const Listener& other, Object& newObj)
        : Component         (other, newObj),
          m_dopplerEffect   (other.m_dopplerEffect),
          m_doppler         (other.m_doppler),
          m_lastPos         (other.m_lastPos)
    {
        JOP_DEBUG_WARNING("jop::Listener component is not meant to be copied, sounds will likely not behave correctly");
    }

    //////////////////////////////////////////////

    void Listener::update(const float)
    {
        glm::vec3 pos = getObject()->getGlobalPosition();
        const ALfloat var[] = {pos.x, pos.y, pos.z};
        alTry(alListenerfv(AL_POSITION, var));

        if (m_dopplerEffect)
        {
            m_lastPos -= glm::abs(pos);
            const ALfloat speed[] = {m_lastPos.x, m_lastPos.y, m_lastPos.z};
            alTry(alListenerfv(AL_VELOCITY, speed));
            m_lastPos = glm::abs(pos);
        }
        
        pos = getObject()->getGlobalFront();
        const glm::vec3 up = getObject()->getGlobalUp();
        const ALfloat direction[] = {pos.x, pos.y, pos.z, up.x, up.y, up.z};
        alTry(alListenerfv(AL_ORIENTATION, direction));
    }

    ///////////////////////////////////////

    Listener& Listener::setGlobalVolume(const float volume)
    {
        alTry(alListenerf(AL_GAIN, glm::clamp(volume, 0.f, 100.f) * 0.01f));

        return *this;
    }

    ///////////////////////////////////////

    float Listener::getGlobalVolume() const
    {
        float volume;
        alTry(alGetListenerf(AL_GAIN, &volume));

        return volume * 100.f;
    }

    ///////////////////////////////////////

    Listener& Listener::setDopplerEffect(const float dop)
    {
        m_doppler = std::min(0.f, dop);
        alTry(alDopplerFactor(m_doppler));

        return *this;
    }

    ///////////////////////////////////////

    float Listener::getDopplerEffect() const
    {
        return m_doppler;
    }

    ///////////////////////////////////////

    Listener& Listener::setDopplerEffectToDefault()
    {
        m_doppler = 1.f;
        alTry(alDopplerFactor(m_doppler));

        return *this;
    }

    ///////////////////////////////////////

    Listener& Listener::setSpeedOfSound(const float speed)
    {
        const float sos = std::max(0.f, speed);
        SoundSource::setSpeedForSound(sos);
        alTry(alSpeedOfSound(sos));

        return *this;
    }

    ///////////////////////////////////////

    float Listener::getSpeedOfSound() const
    {
        return SoundSource::getSpeedForSound();
    }

    ///////////////////////////////////////

    Listener& Listener::useDopplerEffect(const bool use)
    {
        m_dopplerEffect = use;
        SoundSource::calculateDopplerEffect(use);

        return *this;
    }

    ///////////////////////////////////////

    Listener& Listener::useSpeedOfSound(const bool use)
    {
        SoundSource::calculateSpeedOfSound(use);

        return *this;
    }

    ///////////////////////////////////////

    bool Listener::isSpeedOfSound() const
    {
        return SoundSource::isSpeedOfSound();
    }

    ///////////////////////////////////////

    bool Listener::isDopplerEffect() const
    {
        return m_dopplerEffect;
    }

    ///////////////////////////////////////

    Listener& Listener::setSpeedOfSoundToDefault()
    {
        SoundSource::setSpeedForSound(343.3f);
        alTry(alSpeedOfSound(343.3f));

        return *this;
    }
}