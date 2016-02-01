// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//////////////////////////////////////////////

// Headers
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
    Message::Message(const std::string& message, PtrWrapper ptr)
        : m_command(),
          m_id(),
          m_ptr(ptr),
          m_filterBits(Filter::Global),
          m_idMatchMethod(nullptr)
    {
        if (!message.empty())
        {
            setFilter(message);
            std::size_t startPos = message.find_first_of(']');
            m_command << (startPos == std::string::npos ? message : message.substr(message.find_first_not_of(' ', startPos)));
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

    std::string Message::getString() const
    {
        return m_command.str();
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

        return m_idMatchMethod(m_id, id);
    }

    //////////////////////////////////////////////

    Message& Message::setFilter(const std::string& filter)
    {
        m_filterBits = Filter::Global;
        m_id = "";

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

            m_id = filter.substr(fBegin + 1, endPos - fBegin - 1);
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

    PtrWrapper& Message::getReturnPointer() const
    {
        return m_ptr;
    }

    //////////////////////////////////////////////

    Message::operator bool() const
    {
        return m_command.rdbuf()->in_avail() > 0;
    }
}