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

#ifndef JOP_COMMANDHANDLER_HPP
#define JOP_COMMANDHANDLER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Utility/Message.hpp>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <string>
#include <tuple>

//////////////////////////////////////////////


namespace jop
{
    namespace detail
    {
        JOP_API std::tuple<std::string, std::string> splitFirstArguments(const std::string& args);

        // Include the helper inl file
        #include <Jopnal/Utility/Inl/CommandParser.inl>
    }

    class JOP_API CommandHandler final
    {
    public:

        /// \brief Bind a new free function using a custom parser
        ///
        /// \param command The command name
        /// \param func The function to bind
        /// \param parser The parser to use
        /// \param result The message result this function should return
        ///
        template<typename Func, typename Parser>
        void bind(const std::string& command, const Func& func, const Parser& parser, const Message::Result result);

        /// \brief Bind a new free function
        ///
        /// \param command The command name
        /// \param func The function object to bind
        /// \param result The message result this function should return
        ///
        template<typename Ret, typename ... FuncArgs>
        void bind(const std::string& command, const std::function<Ret(FuncArgs...)>& func, const Message::Result result);

        /// \brief Bind a new free function
        ///
        /// \param command The command name
        /// \param func The function pointer to bind
        /// \param result The message result this function should return
        ///
        template<typename Ret, typename ... FuncArgs>
        void bind(const std::string& command, Ret(*func)(FuncArgs...), const Message::Result result);


        /// \brief Bind a new member function
        ///
        /// \param command The command name
        /// \param func The function to bind
        /// \param parser The parser to use
        /// \param result The message result this function should return
        ///
        template<typename Func, typename Parser>
        void bindMember(const std::string& command, const Func& func, const Parser& parser, const Message::Result result);

        /// \brief Bind a new member function
        /// 
        /// \param command The command name
        /// \param func The function object to bind
        /// \param result The message result this function should return
        ///
        template<typename Ret, typename Class, typename ... FuncArgs>
        void bindMember(const std::string& command, const std::function<Ret(Class&, FuncArgs...)>& func, const Message::Result result);

        /// \brief Bind a new member function
        ///
        /// \param command The command name
        /// \param func The function pointer to bind
        /// \param result The message result this function should return
        ///
        template<typename Ret, typename Class, typename ... FuncArgs>
        void bindMember(const std::string& command, Ret(Class::*func)(FuncArgs...), const Message::Result result);

        /// \brief Bind a new const member function
        ///
        /// \param command The command name
        /// \param func The function pointer to bind
        /// \param result The message result this function should return
        /// 
        template<typename Ret, typename Class, typename ... FuncArgs>
        void bindMember(const std::string& command, Ret(Class::*func)(FuncArgs...) const, const Message::Result result);


        /// \brief Execute a command
        ///
        /// \param command The command name
        /// \param instance The class instance to call the command on. Can be nullptr to only consider free functions
        ///
        /// \return The message result
        ///
        Message::Result execute(const std::string& command, void* instance);

    private:

        std::unordered_map<std::string, std::pair<std::function<void(const std::string&, void*)>, Message::Result>> m_memberParsers;    ///< Parsers for member functions
        std::unordered_map<std::string, std::pair<std::function<void(const std::string&)>, Message::Result>> m_funcParsers;             ///< Parsers for free functions

    };

    // Include the template implementation file
    #include <Jopnal/Utility/Inl/CommandHandler.inl>
}

/// \brief Register a new command handler
///
/// This macro must be followed with JOP_END_COMMAND_HANDLER
///
#define JOP_REGISTER_COMMAND_HANDLER(handlerName) ::jop::CommandHandler ns_##handlerName##_commandHandler;  \
                                                  struct ns_##handlerName##_registrar{                      \
                                                  ns_##handlerName##_registrar(){                           \
                                                  auto& handler = ns_##handlerName##_commandHandler;

/// \brief End a command handler registration
///
#define JOP_END_COMMAND_HANDLER(handlerName) }}; static ns_##handlerName##_registrar ns_##handlerName##_reg;

/// \brief Bind a member command
///
#define JOP_BIND_MEMBER_COMMAND(function, funcName) handler.bindMember(funcName, function, ::jop::Message::Result::Continue)

/// \brief Bind a member command with escape result
///
#define JOP_BIND_MEMBER_COMMAND_ESCAPE(function, funcName) handler.bindMember(funcName, function, ::jop::Message::Result::Escape)

/// \brief Bind a free function command
///
#define JOP_BIND_COMMAND(function, funcName) handler.bind(funcName, function, ::jop::Message::Result::Continue)

/// \brief Bind a free function command with escape result
///
#define JOP_BIND_COMMAND_ESCAPE(function, funcName) handler.bind(funcName, function, ::jop::Message::Result::Escape)

/// \brief Execute a command
///
/// This will search for both free and member functions. This will need to be placed in the
/// same namespace as the command handler.
///
#define JOP_EXECUTE_COMMAND(handlerName, command, instance) ns_##handlerName##_commandHandler.execute(command, instance)

/// \class jop::CommandHandler
/// \ingroup utility

#endif