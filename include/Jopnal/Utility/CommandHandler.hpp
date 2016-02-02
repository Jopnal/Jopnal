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
#include <Jopnal/Utility/Message.hpp>
#include <unordered_map>
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

    class CommandHandler final
    {
    public:

        /// \brief Bind a new free function using a custom parser
        ///
        /// \param command The command name
        /// \param func The function to bind
        /// \param parser The parser to use
        ///
        template<typename Func, typename Parser>
        void bind(const std::string& command, const Func& func, const Parser& parser);

        /// \brief Bind a new free function
        ///
        /// \param command The command name
        /// \param func The function object to bind
        ///
        template<typename Ret, typename ... FuncArgs>
        void bind(const std::string& command, const std::function<Ret(FuncArgs...)>& func);

        /// \brief Bind a new free function
        ///
        /// \param command The command name
        /// \param func The function pointer to bind
        ///
        template<typename Ret, typename ... FuncArgs>
        void bind(const std::string& command, Ret (*func)(FuncArgs...));


        /// \brief Bind a new member function
        ///
        /// \param command The command name
        /// \param func The function to bind
        /// \param parser The parser to use
        ///
        template<typename Func, typename Parser>
        void bindMember(const std::string& command, const Func& func, const Parser& parser);

        /// \brief Bind a new member function
        /// 
        /// \param command The command name
        /// \param func The function object to bind
        ///
        template<typename Ret, typename Class, typename ... FuncArgs>
        void bindMember(const std::string& command, const std::function<Ret(Class&, FuncArgs...)>& func);

        /// \brief Bind a new member function
        ///
        /// \param command The command name
        /// \param func The function pointer to bind
        ///
        template<typename Ret, typename Class, typename ... FuncArgs>
        void bindMember(const std::string& command, Ret (Class::*func)(FuncArgs...));


        /// \brief Execute a command
        ///
        /// \param command The command name
        /// \param instance The class instance to call the command on. Can be nullptr to only consider free functions
        ///
        void execute(const std::string& command, PtrWrapper instance);

        /// \brief Execute a command and get the return value
        ///
        /// \param command The command name
        /// \param instance The class instance to call the command on. Can be nullptr to only consider free functions
        /// \param returnWrap PtrWrapper to hold the return value
        ///
        void execute(const std::string& command, PtrWrapper instance, PtrWrapper returnWrap);


    private:

        std::unordered_map<std::string, std::function<void(const std::string&, PtrWrapper&, PtrWrapper&)>> m_memberParsers; ///< Parsers for member functions
        std::unordered_map<std::string, std::function<void(const std::string&, PtrWrapper&)>> m_funcParsers;                ///< Parsers for free functions

    };

    // Include the template implementation file
    #include <Jopnal/Utility/Inl/CommandHandler.inl>
}

/// \brief Register a new command handler
///
/// This macro must be followed with JOP_END_COMMAND_HANDLER
///
#define JOP_REGISTER_COMMAND_HANDLER(className) jop::CommandHandler ns_##className##_commandHandler; \
                                                struct ns_##className##_registrar{                   \
                                                ns_##className##_registrar(jop::CommandHandler& handler){

/// \brief End a command handler registration
///
#define JOP_END_COMMAND_HANDLER(className) }}; static ns_##className##_registrar ns_##className##_reg(ns_##className##_commandHandler);

/// \brief Bind a member command
///
#define JOP_BIND_MEMBER_COMMAND(function, funcName) handler.bindMember(funcName, &function)

/// brief Bind a free function command
///
#define JOP_BIND_COMMAND(function, funcName) handler.bind(funcName, &function)

/// \brief Execute a free function command
///
#define JOP_EXECUTE_COMMAND(className, command, returnPtr) ns_##className##_commandHandler.execute(command, nullptr, returnPtr)

/// \brief Execute a member command
///
#define JOP_EXECUTE_MEMBER_COMMAND(className, command, instance, returnPtr) ns_##className##_commandHandler.execute(command, instance, returnPtr)


#endif

/// \class CommandHandler
/// \ingroup utility
///
/// #TODO Detailed description