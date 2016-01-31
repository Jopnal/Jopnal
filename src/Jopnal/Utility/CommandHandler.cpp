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
    std::tuple<std::string, std::string> detail::splitFirstArguments(const std::string& args)
    {
        std::size_t pos1 = 0, pos2 = 0, pos3;
        std::size_t length = args.length();

        while (pos1 < length && std::isspace(args[pos1]))
            ++pos1;

        if (pos1 == length)
            return std::make_tuple(std::string(), std::string());


        if (args[pos1] == '\"')
        {
            pos2 = ++pos1;

            while (pos2 < length && args[pos2] != '\"')
                ++pos2;
        }
        else
        {
            pos2 = pos1 + 1;

            while (pos2 < length && !std::isspace(args[pos2]))
                ++pos2;
        }

        pos3 = std::min(pos2 + 1, length);
        while (pos3 < length && isspace(args[pos3]))
            ++pos3;

        return std::make_tuple(args.substr(pos1, pos2 - pos1), args.substr(pos3));
    }

    //////////////////////////////////////////////

    void CommandHandler::execute(const std::string& command)
    {
        detail::VoidWrapper returnWrap(nullptr);
        execute(command, returnWrap);
    }

    //////////////////////////////////////////////

    void CommandHandler::execute(const std::string& command, detail::VoidWrapper& returnWrap)
    {
        // Command name
        std::size_t name_pos = command.find_first_not_of(" \t\r\n");
        std::size_t name_end = command.find_first_of(" \t\r\n", name_pos);
        std::size_t name_len =
            (name_pos != std::string::npos) ?
            ((name_end == std::string::npos ? command.length() : name_end) - name_pos) : 0;

        // Arguments
        std::size_t args_pos = std::min(name_pos + name_len + 1, command.length());
        std::size_t args_end = command.find_last_not_of(" \t\r\n");
        std::size_t args_len = std::max(args_pos, args_end) - args_pos + 1;

        // Values
        std::string cmd_name = (name_len > 0) ? command.substr(name_pos, name_len) : "";
        std::string cmd_args = (name_len > 0) ? command.substr(args_pos, args_len) : "";

        // Execution
        if (m_funcParsers.count(cmd_name) > 0)
            m_funcParsers[cmd_name](cmd_args, returnWrap);
    }
}