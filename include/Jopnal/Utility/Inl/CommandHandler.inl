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


template<typename Func, typename Parser>
void CommandHandler::bind(const std::string& command, const Func& func, const Parser& parser)
{
    JOP_ASSERT(!command.empty(), "Tried to register an empty command!");
    m_funcParsers[command] = std::bind(parser, func, std::placeholders::_1, std::placeholders::_2);
}

//////////////////////////////////////////////

template<typename Ret, typename ... FuncArgs>
void CommandHandler::bind(const std::string& command, const std::function<Ret(FuncArgs...)>& func)
{
    bind(command, func, &detail::DefaultParser::parse<Ret, FuncArgs...>);
}

//////////////////////////////////////////////

template<typename Ret, typename ... FuncArgs>
void CommandHandler::bind(const std::string& command, Ret(*func)(FuncArgs...))
{
    bind(command, func, &detail::DefaultParser::parse<Ret, FuncArgs...>);
}

//////////////////////////////////////////////

template<typename Func, typename Parser>
void CommandHandler::bindMember(const std::string& command, const Func& func, const Parser& parser)
{
    JOP_ASSERT(!command.empty(), "Tried to register an empty member command!");
    m_memberParsers[command] = std::bind(parser, func, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

//////////////////////////////////////////////

template<typename Ret, typename Class, typename ... FuncArgs>
void CommandHandler::bindMember(const std::string& command, const std::function<Ret(Class&, FuncArgs...)>& func)
{
    bindMember(command, func, &detail::DefaultParser::parseMember<Ret, Class, FuncArgs...>);
}

//////////////////////////////////////////////

template<typename Ret, typename Class, typename ... FuncArgs>
void CommandHandler::bindMember(const std::string& command, Ret(Class::*func)(FuncArgs...))
{
    // Have to use mem_fn due to a bug in VS
    bindMember(command, std::mem_fn(func), &detail::DefaultParser::parseMember<Ret, Class, FuncArgs...>);
}