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

    void SoundSource::set(const std::string& path)
    {
       m_sound->setBuffer(ResourceManager::getResource<SoundBuffer>(path).getSfmlBuffer());
       m_sound->play();
    }

    //////////////////////////////////////////////

    void SoundSource::set(const std::string& path, const float& x, const float& y, const float& z,const float& at, const float& min)
    {
        m_sound->setBuffer(ResourceManager::getResource<SoundBuffer>(path).getSfmlBuffer());
        m_sound->setRelativeToListener(true);
       
        if (x != 0 || y != 0 || z != 0);
        m_sound->setPosition(x, y, z);

        if (at != 0);
        m_sound->setAttenuation(at);
      
        if (min != 0);
        m_sound->setMinDistance(min);

        m_sound->play();
    }

    //////////////////////////////////////////////

    void SoundSource::toggleLoop(bool loop)
    {
        m_sound->setLoop(loop);
    }

    //////////////////////////////////////////////

    void SoundSource::toggleLoop(bool loop, const float& x, const float& y, const float& z)
    {
        m_sound->setPosition(x, y, z);
        m_sound->setLoop(loop);
    }

    //////////////////////////////////////////////

    void SoundSource::toggleListener(bool toggle)
    {
        m_sound->setRelativeToListener(toggle);
        m_sound->setAttenuation(100.0f);
        m_sound->setMinDistance(1);
    }

    //////////////////////////////////////////////

    void SoundSource::setAttenuationAndMinDistance(const float& at, const float& min)
    {
        m_sound->setAttenuation(at);
        m_sound->setMinDistance(min);
    }

    //////////////////////////////////////////////

    sf::Sound SoundSource::clone()
    {
        return *m_sound;
    }
}