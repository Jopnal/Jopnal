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
    }

    SoundEffect::SoundEffect(const SoundEffect& other)
        : SoundSource(other)
    {
        m_sound = std::make_unique < sf::Sound >();
        other.clone()->copy(static_cast<sf::Sound*>(m_sound.get()));
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

    SoundEffect& SoundEffect::Play(bool reset)
    {
        if (static_cast<sf::Sound*>(m_sound.get())->getStatus() == sf::Sound::Status::Playing)
        {
            if (reset);
            return *this;
        }
        static_cast<sf::Sound*>(m_sound.get())->play();
        return *this;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::Play()
    {
        static_cast<sf::Sound*>(m_sound.get())->play();
        return *this;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::Stop()
    {
        static_cast<sf::Sound*>(m_sound.get())->stop();

        return *this;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::Pause()
    {
        static_cast<sf::Sound*>(m_sound.get())->pause();

        return *this;
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::setOffset(const float& time)
    {
        sf::Time t(sf::seconds(time));
        static_cast<sf::Sound*>(m_sound.get())->setPlayingOffset(t);

        return *this;
    }

    //////////////////////////////////////////////

    const float SoundEffect::getOffset()
    {
        return static_cast<sf::Sound*>(m_sound.get())->getPlayingOffset().asSeconds();
    }

    //////////////////////////////////////////////

    enum status SoundEffect::getStatus()
    {
        return status(static_cast<sf::Sound*>(m_sound.get())->getStatus());
    }

    //////////////////////////////////////////////

    SoundEffect& SoundEffect::setLoop(bool loop)
    {
        static_cast<sf::Sound*>(m_sound.get())->setLoop(loop);

        return *this;
    }

    //////////////////////////////////////////////

    void SoundEffect::copy(sf::Sound* sound)
    {
        static_cast<sf::Sound*>(sound)=static_cast<sf::Sound*>(m_sound.get());
    }
}