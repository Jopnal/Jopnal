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