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
        : Component         (object, ID)
    {}

    SoundSource::SoundSource(const SoundSource& other, Object& newObj)
        : Component         (other, newObj)
    {}

    SoundSource::~SoundSource()
    {}

    //////////////////////////////////////////////

    void SoundSource::update(const float)
    {
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setVolume(const float vol)
    {
        return *this;
    }

    //////////////////////////////////////////////

    float SoundSource::getVolume() const
    {
        return 0.f;
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setPitch(const float value)
    {
        return *this;
    }

    //////////////////////////////////////////////

    float SoundSource::getPitch() const
    {
        return 0.f;
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setSpatialized(const bool set)
    {
        return *this;
    }

    //////////////////////////////////////////////

    bool SoundSource::isSpatialized() const
    {
        return false;
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setAttenuation(const float at)
    {
        return *this;
    }

    //////////////////////////////////////////////

    SoundSource& SoundSource::setMinDistance(const float min)
    {
        return *this;
    }

    //////////////////////////////////////////////

    float SoundSource::getAttenuation() const
    {
        return 0.f;
    }

    //////////////////////////////////////////////

    float SoundSource::getMinDistance() const
    {
        return 0.f;
    }

    //////////////////////////////////////////////

    Message::Result SoundSource::receiveMessage(const Message& message)
    {
        if (JOP_EXECUTE_COMMAND(SoundSource, message.getString(), this) == Message::Result::Escape)
            return Message::Result::Escape;

        return Component::receiveMessage(message);
    }
}