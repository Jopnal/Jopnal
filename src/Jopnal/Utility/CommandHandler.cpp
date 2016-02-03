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


namespace
{
    void handleCommand(const std::string& orig, std::string& command, std::string& args)
    {
        std::size_t commandStart = orig.find_first_not_of(" \t\r\n");
        std::size_t commandEnd = orig.find_first_of(" \t\r\n", commandStart);
        std::size_t commandLen = (commandStart != std::string::npos) ? ((commandEnd == std::string::npos ? orig.length() : commandEnd) - commandStart) : 0;

        std::size_t argStart = std::min(commandStart + commandLen + 1, orig.length());
        std::size_t argEnd = orig.find_last_not_of(" \t\r\n");
        std::size_t argLen = std::max(argStart, argEnd) - argStart + 1;

        // Values
        command = (commandLen > 0) ? orig.substr(commandStart, commandLen) : "";
        args = (commandLen > 0) ? orig.substr(argStart, argLen) : "";
    }
}

namespace jop
{
    std::tuple<std::string, std::string> detail::splitFirstArguments(const std::string& args)
    {
        std::size_t pos1 = 0, pos2 = 0, pos3;
        std::size_t length = args.length();

        while (pos1 < length && std::isspace(args[pos1]))
            ++pos1;

        auto errMessage = []()
        {
            JOP_DEBUG_WARNING("Last command didn't contain enough parameters to pass to the bound function");
        };

        if (pos1 == length)
        {
            errMessage();
            return std::make_tuple(std::string(), std::string());
        }

        if (args[pos1] == '\"')
        {
            pos2 = ++pos1;

            while (pos2 < length && args[pos2] != '\"')
                ++pos2;
        }
        /*else if (args[pos1] == '(')
        {
            Command nesting would be handled here. Simply detect if the argument is inside brackets (ignore escape sequences),
            and pass it to the same command handler instance (would need to pass a reference into this function)

            There would also need to be a way to convert jop::Any to string.
        }*/
        else
        {
            pos2 = pos1 + 1;

            while (pos2 < length && !std::isspace(args[pos2]))
                ++pos2;
        }

        pos3 = std::min(pos2 + 1, length);
        while (pos3 < length && isspace(args[pos3]))
            ++pos3;

        if (pos3 == length)
            errMessage();

        return std::make_tuple(args.substr(pos1, pos2 - pos1), args.substr(pos3));
    }

    //////////////////////////////////////////////

    void CommandHandler::execute(const std::string& command, Any instance)
    {
        Any returnWrap(nullptr);
        execute(command, instance, returnWrap);
    }

    //////////////////////////////////////////////

    void CommandHandler::execute(const std::string& command, Any instance, Any returnWrap)
    {
        std::string comm, args;
        handleCommand(command, comm, args);

        {
            auto itr = m_funcParsers.find(comm);
            if (itr != m_funcParsers.end())
                itr->second(args, returnWrap);
        }

        if (instance)
        {
            auto itr = m_memberParsers.find(comm);
            if (itr != m_memberParsers.end())
                itr->second(args, returnWrap, instance);
        }
    }
}