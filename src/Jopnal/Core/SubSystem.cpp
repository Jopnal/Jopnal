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
    JOP_REGISTER_COMMAND_HANDLER(Subsystem)

        JOP_BIND_MEMBER_COMMAND(&Subsystem::setID, "setID");

    JOP_END_COMMAND_HANDLER(Subsystem)
}

namespace jop
{
    Subsystem::Subsystem(const std::string& ID)
        : std::enable_shared_from_this<Subsystem>   (),
          m_ID                                      (ID)
    {}

    Subsystem::~Subsystem()
    {}

    //////////////////////////////////////////////

    void Subsystem::preFixedUpdate(const float)
    {}

    void Subsystem::postFixedUpdate(const float)
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

    MessageResult Subsystem::sendMessage(const std::string& message)
    {
        Any wrap;
        return sendMessage(message, wrap);
    }

    //////////////////////////////////////////////

    MessageResult Subsystem::sendMessage(const std::string& message, Any& returnWrap)
    {
        const Message msg(message, returnWrap);
        return sendMessage(msg);
    }

    MessageResult Subsystem::sendMessage(const Message& message)
    {
        if (message.passFilter(Message::Subsystem, getID()))
        {
            if (message.passFilter(Message::Command))
            {
                Any instance(this);
                JOP_EXECUTE_COMMAND(Subsystem, message.getString(), instance, message.getReturnWrapper());
            }

            if (message.passFilter(Message::Custom))
                return sendMessageImpl(message);
        }

        return MessageResult::Continue;
    }

    //////////////////////////////////////////////

    void Subsystem::setID(const std::string& ID)
    {
        m_ID = ID;
    }

    //////////////////////////////////////////////

    const std::string& Subsystem::getID() const
    {
        return m_ID;
    }

    //////////////////////////////////////////////

    MessageResult Subsystem::sendMessageImpl(const Message&)
    {
        return MessageResult::Continue;
    }
}