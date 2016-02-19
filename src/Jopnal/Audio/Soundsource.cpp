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
        :Component                                  (object,ID),
        m_sound(std::make_unique<sf::Sound>())
    {}

    SoundSource::SoundSource(const SoundSource& other)
        : Component(other),
        m_sound()
    {
        *m_sound = other.clone()->clone();
    }
    //////////////////////////////////////////////

    SoundSource::~SoundSource()
    {}

    //////////////////////////////////////////////
    //////////////////////////////////////////////

    void SoundSource::update(const float)
    {
        m_sound->setPosition(getObject().getPosition().x, getObject().getPosition().y,getObject().getPosition().z);
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setBuffer(const std::string& path)
    {
       m_sound->setBuffer(ResourceManager::getResource<SoundBuffer>(path).getSfmlBuffer());

       return *this;
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::Play(bool reset)
    {
        if (m_sound->getStatus() == sf::Sound::Status::Playing)
        {
            if (reset);
            return *this;
        }

        m_sound->play();
        return *this;
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::Stop()
    {
        m_sound->stop();

        return *this;
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::Pause()
    {
        m_sound->pause();

        return *this;
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setVolume(const float& vol)
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

    SoundSource& SoundSource::setOffset(const float& time)
    {
        m_sound->setPlayingOffset(&time);

        return *this;
    }

    //////////////////////////////////////////////

    const float SoundSource::getOffset()
    {
       return m_sound->getPlayingOffset().asSeconds();
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setPitch(const float& value)
    {

        return *this;
    }

    //////////////////////////////////////////////

    const float SoundSource::getPitch()
    {
        return m_sound->getPitch();
    }

    //////////////////////////////////////////////

    int SoundSource::getStatus()
    {
       m_sound->getStatus();
       return 999;
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setLoop(bool loop)
    {
        m_sound->setLoop(loop);

        return *this;
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setListener(bool toggle)
    {
        m_sound->setRelativeToListener(toggle);

        return *this;
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setAttenuationAndMinDistance(const float& at, const float& min)
    {
        m_sound->setAttenuation(at);
        m_sound->setMinDistance(min);

        return *this;
    }

    //////////////////////////////////////////////

    sf::Sound SoundSource::clone()
    {
        return *m_sound;
    }
}