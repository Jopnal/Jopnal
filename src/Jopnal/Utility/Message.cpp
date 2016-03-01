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
    Message::Message(const std::string& message, Any& ptr)
        : m_command         (),
          m_commandStr      (),
          m_idPattern       (),
          m_ptr             (ptr),
          m_filterBits      (Filter::Global),
          m_idMatchMethod   (nullptr)
    {
        if (!message.empty())
        {
            setFilter(message);
            std::size_t startPos = message.find_first_of(']');
            m_command << (startPos == std::string::npos ? message : message.substr(message.find_first_not_of(' ', startPos + 1)));
        }
    }

    //////////////////////////////////////////////

    Message& Message::push(const std::string& str)
    {
        m_command << ' ' << str;
        return *this;
    }

    //////////////////////////////////////////////

    Message& Message::push(const char* str)
    {
        m_command << ' ' << str;
        return *this;
    }

    //////////////////////////////////////////////

    Message& Message::pushPointer(const void* ptrArg)
    {
        m_command << ' ' << std::hex << ptrArg;
        return *this;
    }

    //////////////////////////////////////////////

    const std::string& Message::getString() const
    {
        if (m_commandStr.empty())
            return (m_commandStr = m_command.str());

        return m_commandStr;
    }

    //////////////////////////////////////////////

    bool Message::passFilter(const unsigned short filter, const std::string& id) const
    {
        return passFilter(filter) && passFilter(id);
    }

    //////////////////////////////////////////////

    bool Message::passFilter(const unsigned short filter) const
    {
        return (m_filterBits & filter) != 0;
    }

    //////////////////////////////////////////////

    bool Message::passFilter(const std::string& id) const
    {
        if (!m_idMatchMethod)
            return true;

        return m_idMatchMethod(id, m_idPattern);
    }

    //////////////////////////////////////////////

    Message& Message::setFilter(const std::string& filter)
    {
        m_filterBits = Filter::Global;
        m_idPattern = "";

        if (filter.empty() || filter[0] != '[')
            return *this;

        std::size_t endPos = filter.find_first_of(']');

        if (endPos == std::string::npos)
        {
            JOP_DEBUG_ERROR("Message filter bracket unmatched. Message: \"" << m_command.str() << "\"");
            return *this;
        }

        // Id filtering
        std::size_t fBegin = filter.find_last_of("=*", endPos);
        if (fBegin != std::string::npos && (endPos - fBegin) > 1)
        {
            if (filter[fBegin] == '=')
                m_idMatchMethod = [](const std::string& compAgainst, const std::string& comp) -> bool
                {
                    return compAgainst == comp;
                };
            else
                m_idMatchMethod = [](const std::string& compAgainst, const std::string& comp) -> bool
                {
                    return compAgainst.find(comp) != std::string::npos;
                };

            m_idPattern = filter.substr(fBegin + 1, endPos - fBegin - 1);
        }

        // Built-in/overridden filtering
        fBegin = filter.find_last_of('-', endPos);
        if (fBegin != std::string::npos && (endPos - fBegin) > 1)
        {
            if (filter[fBegin + 1] == 'c')
                m_filterBits &= ~(Filter::Custom);
            else if (filter[fBegin + 1] == 'm')
                m_filterBits &= ~(Filter::Command);
        }

        // System filters
        fBegin = fBegin == std::string::npos ? endPos - 1 : fBegin - 1;
        if (filter[fBegin] != '[')
        {
            static const unsigned short systemBitsInv = static_cast<unsigned short>(~(Engine | Subsystem | SharedScene | Scene | Layer | Object | Component));
            m_filterBits &= systemBitsInv;

            static const char* symbols[]
            {
                "En",
                "Su",
                "Sh",
                "Sc",
                "La",
                "Ob",
                "Co"
            };

            for (int i = 0; i < sizeof(symbols) / sizeof(symbols[0]); ++i)
            {
                if (filter.rfind(symbols[i], fBegin) != std::string::npos)
                    m_filterBits |= (Filter::Engine << i);
            }
        }

        return *this;
    }

    //////////////////////////////////////////////

    Any& Message::getReturnWrapper() const
    {
        return m_ptr;
    }

    //////////////////////////////////////////////

    Message::operator bool() const
    {
        return m_command.rdbuf()->in_avail() > 0;
    }
}