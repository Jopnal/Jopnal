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

	#include <Jopnal/Core/Subsystem.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_COMMAND_HANDLER(Subsystem)

        JOP_BIND_MEMBER_COMMAND(&Subsystem::setActive, "setActive");

    JOP_END_COMMAND_HANDLER(Subsystem)
}

namespace jop
{
    Subsystem::Subsystem(const uint32 ID)
        : m_ID      (ID),
          m_active  (true)
    {}

    Subsystem::~Subsystem()
    {}

    //////////////////////////////////////////////

    void Subsystem::preUpdate(const float)
    {}

    void Subsystem::postUpdate(const float)
    {}

    //////////////////////////////////////////////

    void Subsystem::draw()
    {}

    //////////////////////////////////////////////

    Message::Result Subsystem::sendMessage(const Message& message)
    {
        if (message.passFilter(Message::Subsystem))
        {
            if (receiveMessage(message) == Message::Result::Escape)
                return Message::Result::Escape;
        }

        return Message::Result::Continue;
    }

    //////////////////////////////////////////////

    Subsystem& Subsystem::setActive(const bool active)
    {
        m_active.store(active);
        return *this;
    }

    //////////////////////////////////////////////

    bool Subsystem::isActive() const
    {
        return m_active.load();
    }

    //////////////////////////////////////////////

    uint32 Subsystem::getID() const
    {
        return m_ID;
    }

    //////////////////////////////////////////////

    Message::Result Subsystem::receiveMessage(const Message& message)
    {
        return JOP_EXECUTE_COMMAND(Subsystem, message.getString(), this);
    }
}