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
    namespace detail
    {
        std::tuple<std::string, std::string> parseArgumentPair(const std::string& args)
        {
            std::size_t pos = 0;
            std::size_t length = args.length();

            while (pos < length && std::isspace(args[pos]))
                ++pos;

            if (pos == length)
                return std::make_tuple(std::string(), std::string());

            std::size_t pos2 = 0;
            if (args[pos] == '\"')
            {
                pos2 = ++pos;

                while (pos2 < length && args[pos2] != '\"')
                    ++pos2;
            }
            else
            {
                pos2 = pos + 1;

                while (pos2 < length && !std::isspace(args[pos2]))
                    ++pos2;
            }

            std::size_t pos3 = std::min(pos2 + 1, length);
            while (pos3 < length && std::isspace(args[pos3]))
                ++pos3;

            return std::make_tuple(args.substr(pos, pos2 - pos), args.substr(pos3));
        }

        namespace DefaultParser
        {
            template<>
            void parse<>(const std::function<void()>& func, const std::string&)
            {
                func();
            }
        }
    }

    //////////////////////////////////////////////

    CommandHandler::CommandHandler()
        : m_parsers()
    {}
}