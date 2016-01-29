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

#ifndef JOP_COMMANDHANDLER_HPP
#define JOP_COMMANDHANDLER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <unordered_map>

//////////////////////////////////////////////


namespace jop
{
    #define JOP_COMMAND_HANDLER(classType)
    #define JOP_END_COMMAND_HANDLER }

    #define JOP_BIND_MEMBER_COMMAND(function) 
    #define JOP_BIND_COMMAND(function)


    template<typename T>
    class CommandHandler final
    {
    public:



    private:

        std::unordered_map<std::string, void (T::*)(T&, const std::string&)> m_memberParsers;
        std::unordered_map<std::string, void (*)(const std::string&)> m_funcParsers;

    };
}

#endif

/// \class CommandHandler
/// \ingroup utility
///
/// #TODO Detailed description