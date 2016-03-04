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
    SoundSource::SoundSource(Object& object, const std::string& ID)
        :Component                                  (object,ID)
    {}

    SoundSource::SoundSource(const SoundSource& other)
        : Component(other),
        m_sound()
    {}

    //////////////////////////////////////////////

    SoundSource::~SoundSource()
    {}

    //////////////////////////////////////////////
    //////////////////////////////////////////////

    void SoundSource::update(const float deltaTime)
    {
        glm::vec3 pos = getObject()->getGlobalPosition();
        m_sound->setPosition(pos.x, pos.y, pos.z);

        if (m_playOnce)
            if (m_playWithSpeed)
                allowSound(deltaTime);
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setVolume(const float vol)
    {
        m_sound->setVolume(vol);

        return *this;
    }

    //////////////////////////////////////////////

    float SoundSource::getVolume()
    {
        return m_sound->getVolume();
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setPitch(const float value)
    {
        m_sound->setPitch(std::max(value,0.f));

        return *this;
    }

    //////////////////////////////////////////////

    float SoundSource::getPitch()
    {
        return m_sound->getPitch();
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setListener(bool toggle)
    {
        m_sound->setRelativeToListener(toggle);

        return *this;
    }

    //////////////////////////////////////////////

    bool SoundSource::getListener()
    {
        return m_sound->isRelativeToListener();
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setAttenuation(const float at)
    {
        m_sound->setAttenuation(at);

        return *this;
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setMinDistance(const float min)
    {
        m_sound->setMinDistance(min);

        return *this;
    }

    //////////////////////////////////////////////

    float SoundSource::getAttenuation()
    {
        return m_sound->getAttenuation();
    }

    //////////////////////////////////////////////

    float SoundSource::getMinDistance()
    {
        return  m_sound->getMinDistance();
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::speedOfSound(const bool use)
    {
        m_playWithSpeed = use;

        return *this;
    }

    //////////////////////////////////////////////

    void SoundSource::calculateSound()
    {
        m_speedCounter.y = 20.0f;
    }

    //////////////////////////////////////////////

    void SoundSource::allowSound(const float deltaTime)
    {
        if (m_speedCounter.y <= 0.0f)
        {
            static_cast<sf::Sound*>(m_sound.get())->play();
            m_playOnce = false;
        }
        else
            std::cout << m_speedCounter.y << "\n";
            m_speedCounter.y -= deltaTime;
    }
}