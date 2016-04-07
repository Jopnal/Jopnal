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
    JOP_DERIVED_COMMAND_HANDLER(Component, SoundStream)

        JOP_BIND_MEMBER_COMMAND_NORETURN((SoundStream& (SoundStream::*)(const bool reset))&SoundStream::play, "playStream");
        JOP_BIND_MEMBER_COMMAND_NORETURN(&SoundStream::pause, "pauseStream");
        JOP_BIND_MEMBER_COMMAND_NORETURN(&SoundStream::stop, "stopStream");
        JOP_BIND_MEMBER_COMMAND_NORETURN(&SoundStream::setOffset, "setStreamOffset");
        JOP_BIND_MEMBER_COMMAND_NORETURN(&SoundStream::setLoop, "setStreamLoop");

    JOP_END_COMMAND_HANDLER(SoundStream)
}

namespace jop
{
    SoundStream::SoundStream(Object& object)
        : SoundSource   (object, "soundstream"),
          m_path        ()
    {
        m_sound = std::make_unique<sf::Music>();
    }

    SoundStream::SoundStream(const SoundStream& other, Object& newObj)
        : SoundSource   (other, newObj),
          m_path        (other.m_path)
    {
        m_sound = std::make_unique<sf::Music>();
       
        auto& s = static_cast<sf::Music&>(*m_sound);
        auto& o = static_cast<const sf::Music&>(*other.m_sound);
        
        s.setPitch              (o.getPitch());
        s.setVolume             (o.getVolume());
        s.setRelativeToListener (o.isRelativeToListener());
        s.setMinDistance        (o.getMinDistance());
        s.setAttenuation        (o.getAttenuation());
        s.setLoop               (o.getLoop());       
        s.openFromFile          (other.m_path);
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::setPath(const std::string& path)
    {
        static const std::string m_relativity = SettingManager::getString("sResourceDirectory", "Resources") + "/";
       
        m_path = m_relativity + path;
        static_cast<sf::Music*>(m_sound.get())->openFromFile(path);

        return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::play(const bool reset)
    {
        if (reset || getStatus() < Status::Playing)
            play();

        return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::play()
    {
        static_cast<sf::Music*>(m_sound.get())->play();
        return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::stop()
    {
        static_cast<sf::Music*>(m_sound.get())->stop();
        return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::pause()
    {
        static_cast<sf::Music*>(m_sound.get())->pause();
        return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::setOffset(const float time)
    {
        sf::Time t(sf::seconds(time));
        static_cast<sf::Music*>(m_sound.get())->setPlayingOffset(t);

        return *this;
    }

    //////////////////////////////////////////////

    float SoundStream::getOffset() const
    {
        return static_cast<sf::Music*>(m_sound.get())->getPlayingOffset().asSeconds();
    }

    //////////////////////////////////////////////

    SoundSource::Status SoundStream::getStatus() const
    {
        return static_cast<Status>(static_cast<sf::Music*>(m_sound.get())->getStatus());
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::setLoop(const bool loop)
    {
        static_cast<sf::Music*>(m_sound.get())->setLoop(loop);
        return *this;
    }
}