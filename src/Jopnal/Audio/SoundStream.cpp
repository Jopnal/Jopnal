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

    #include <Jopnal/Audio/SoundStream.hpp>

    #include <Jopnal/Utility/CommandHandler.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_COMMAND_HANDLER(SoundStream)

        JOP_BIND_MEMBER_COMMAND((SoundStream& (SoundStream::*)(const bool reset))&SoundStream::play, "playStream");
        JOP_BIND_MEMBER_COMMAND(&SoundStream::pause, "pauseStream");
        JOP_BIND_MEMBER_COMMAND(&SoundStream::stop, "stopStream");
        JOP_BIND_MEMBER_COMMAND(&SoundStream::setOffset, "setStreamOffset");
        JOP_BIND_MEMBER_COMMAND(&SoundStream::setLoop, "setStreamLoop");

    JOP_END_COMMAND_HANDLER(SoundStream)
}

namespace jop
{
    SoundStream::SoundStream(Object& object)
        : SoundSource   (object, 0),
          m_path        ()
    {
    }

    SoundStream::SoundStream(const SoundStream& other, Object& newObj)
        : SoundSource   (other, newObj),
          m_path        ()
    {
        
    }

    SoundStream::~SoundStream()
    {
    }

    //////////////////////////////////////////////

    bool SoundStream::setPath(const std::string& path)
    {
        return false;
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
        return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::stop()
    {
        return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::pause()
    {
        return *this;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::setOffset(const float time)
    {

        return *this;
    }

    //////////////////////////////////////////////

    float SoundStream::getOffset() const
    {return 0.f;
    }

    //////////////////////////////////////////////

    SoundSource::Status SoundStream::getStatus() const
    {
        return Status::Stopped;
    }

    //////////////////////////////////////////////

    SoundStream& SoundStream::setLoop(const bool loop)
    {
        return *this;
    }

    //////////////////////////////////////////////

    Message::Result SoundStream::receiveMessage(const Message& message)
    {
        if (JOP_EXECUTE_COMMAND(SoundStream, message.getString(), this) == Message::Result::Escape)
            return Message::Result::Escape;

        return SoundSource::receiveMessage(message);
    }
}