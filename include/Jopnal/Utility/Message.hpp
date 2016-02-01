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

#ifndef JOP_MESSAGE_HPP
#define JOP_MESSAGE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Utility/Assert.hpp>
#include <Jopnal/Utility/PtrWrapper.hpp>
#include <string>
#include <typeinfo>
#include <sstream>

//////////////////////////////////////////////


namespace jop
{
    /// 
    enum class MessageResult
    {
        Continue,
        Escape
    };

    class JOP_API Message
    {
    private:

        JOP_DISALLOW_COPY_MOVE(Message);

    public:

        enum Filter : unsigned short
        {
            Engine = 1,
            Subsystem = Engine << 1,
            SharedScene = Subsystem << 1,
            Scene = SharedScene << 1,
            Layer = Scene << 1,
            Object = Layer << 1,
            Component = Object << 1,
            Command = Component << 1,
            Custom = Command << 1,
            Global = 0xFFFF
        };

    public:

        Message(const std::string& message, PtrWrapper ptr);


        template<typename T>
        Message& push(const T& arg);

        Message& push(const std::string& str);

        Message& push(const char* str);

        Message& pushPointer(const void* ptrArg);

        template<typename T>
        Message& pushReference(const T& ref);


        std::string getString() const;;

        bool passFilter(const unsigned short filter, const std::string& id) const;

        bool passFilter(const unsigned short filter) const;

        bool passFilter(const std::string& id) const;

        Message& setFilter(const std::string& filter);

        PtrWrapper& getReturnPointer() const;

        operator bool() const;

    private:

        std::ostringstream m_command;
        std::string m_idPattern;
        mutable PtrWrapper m_ptr;
        unsigned short m_filterBits;
        bool (*m_idMatchMethod)(const std::string&, const std::string&);

    };

    // Include the template implementation file
    #include <Jopnal/Utility/Inl/Message.inl>
}

#endif

/// \class Resource
/// \ingroup utility