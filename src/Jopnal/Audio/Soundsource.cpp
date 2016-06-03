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
    JOP_REGISTER_COMMAND_HANDLER(SoundSource)

        JOP_BIND_MEMBER_COMMAND(&SoundSource::setVolume, "setVolume");
        JOP_BIND_MEMBER_COMMAND(&SoundSource::setPitch, "setPitch");
        JOP_BIND_MEMBER_COMMAND(&SoundSource::setSpatialized, "setSpatialized");
        JOP_BIND_MEMBER_COMMAND(&SoundSource::setMinDistance, "setMinDistance");
        JOP_BIND_MEMBER_COMMAND(&SoundSource::setAttenuation, "setAttenuation");

    JOP_END_COMMAND_HANDLER(SoundSource)
}

namespace jop
{
    SoundSource::SoundSource(Object& object, const uint32 ID)
        : Component         (object, ID),
          m_sound           ()
    {}

    SoundSource::SoundSource(const SoundSource& other, Object& newObj)
        : Component         (other, newObj),
          m_sound           ()
    {}

    SoundSource::~SoundSource()
    {}

    //////////////////////////////////////////////

    void SoundSource::update(const float)
    {
        glm::vec3 pos = getObject()->getGlobalPosition();
        m_sound->setPosition(pos.x, pos.y, pos.z);
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setVolume(const float vol)
    {
        m_sound->setVolume(glm::clamp(vol, 0.f, 100.f));
        return *this;
    }

    //////////////////////////////////////////////

    float SoundSource::getVolume() const
    {
        return m_sound->getVolume();
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setPitch(const float value)
    {
        m_sound->setPitch(std::max(value, 0.f));
        return *this;
    }

    //////////////////////////////////////////////

    float SoundSource::getPitch() const
    {
        return m_sound->getPitch();
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setSpatialized(const bool set)
    {
        m_sound->setRelativeToListener(!set);
        return *this;
    }

    //////////////////////////////////////////////

    bool SoundSource::isSpatialized() const
    {
        return !m_sound->isRelativeToListener();
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setAttenuation(const float at)
    {
        m_sound->setAttenuation(std::max(at, 0.f));
        return *this;
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setMinDistance(const float min)
    {
        m_sound->setMinDistance(std::max(FLT_MIN, min));
        return *this;
    }

    //////////////////////////////////////////////

    float SoundSource::getAttenuation() const
    {
        return m_sound->getAttenuation();
    }

    //////////////////////////////////////////////

    float SoundSource::getMinDistance() const
    {
        return m_sound->getMinDistance();
    }

    //////////////////////////////////////////////

    Message::Result SoundSource::receiveMessage(const Message& message)
    {
        if (JOP_EXECUTE_COMMAND(SoundSource, message.getString(), this) == Message::Result::Escape)
            return Message::Result::Escape;

        return Component::receiveMessage(message);
    }
}