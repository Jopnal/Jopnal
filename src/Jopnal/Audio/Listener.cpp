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
    #include <Jopnal/Core/DebugHandler.hpp>
    #include <Jopnal/Utility/CommandHandler.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_COMMAND_HANDLER(Listener)

        JOP_BIND_COMMAND_ESCAPE(&Listener::setGlobalVolume, "setGlobalVolume");

    JOP_END_COMMAND_HANDLER(Listener)
}

namespace jop
{
    Listener::Listener(Object& object)
        : Component(object, 0)
    {}

    Listener::Listener(const Listener& other, Object& newObj)
        : Component(other, newObj)
    {
        JOP_DEBUG_WARNING("jop::Listener component is not meant to be copied, sounds will likely not behave correctly");
    }

    //////////////////////////////////////////////

    void Listener::update(const float)
    {
    }

    ///////////////////////////////////////

    void Listener::setGlobalVolume(const float volume)
    {
    }

    ///////////////////////////////////////

    float Listener::getGlobalVolume()
    {
        return 0.f;
    }

    //////////////////////////////////////////////

    Message::Result Listener::receiveMessage(const Message& message)
    {
        if (JOP_EXECUTE_COMMAND(Listener, message.getString(), this) == Message::Result::Escape)
            return Message::Result::Escape;

        return Component::receiveMessage(message);
    }
}