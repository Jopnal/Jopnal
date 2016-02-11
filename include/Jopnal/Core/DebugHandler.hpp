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

#ifndef JOP_DEBUGHANDLER_HPP
#define JOP_DEBUGHANDLER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Color.hpp>
#include <sstream>

//////////////////////////////////////////////


namespace jop
{
    #ifdef JOP_ENABLE_CONSOLE
        #define JOP_DEBUG_ERROR(stream)   {jop::DebugHandler::getInstance() << jop::Color::Red << jop::DebugHandler::Severity::Error << stream << std::endl;}
        #define JOP_DEBUG_WARNING(stream) {jop::DebugHandler::getInstance() << jop::Color::Yellow << jop::DebugHandler::Severity::Warning << stream << std::endl;}
        #define JOP_DEBUG_INFO(stream)    {jop::DebugHandler::getInstance() << jop::Color::White << jop::DebugHandler::Severity::Info << stream << std::endl;}
    #else
        #define JOP_DEBUG_ERROR(stream)   {}
        #define JOP_DEBUG_WARNING(stream) {}
        #define JOP_DEBUG_INFO(stream)    {}
    #endif

    class JOP_API DebugHandler
    {
    private:

        // Private constructor since this is a singleton class
        //
        DebugHandler();

        // Get the internal stream object. This is used by the template << operator
        //
        std::ostringstream& getStream();

    public:

        friend class Engine;

        /// The severity levels
        ///
        enum class Severity
        {
            Error,
            Warning,
            Info
        };

    public:

        /// \brief Get the DebugHandler instance
        ///
        static DebugHandler& getInstance();

        /// \brief Check if the console is enabled
        ///
        static bool isConsoleEnabled();

        /// \brief Operator for setting the severity level for the next text object
        ///
        /// \param verbosity The verbosity level.
        ///
        DebugHandler& operator <<(const Severity severity);

        /// \brief Flushes the stream using std::endl
        ///
        DebugHandler& operator <<(std::basic_ostream<char, std::char_traits<char>>& (*)(std::basic_ostream<char, std::char_traits<char>>&));

        /// \brief Sets the color for the next text object
        ///
        DebugHandler& operator <<(const jop::Color& color);

        /// \brief An operator for inputting data into the stream
        ///
        /// \param data The data to be output
        ///
        template<typename T>
        DebugHandler& operator <<(const T& data);
    };

    // Include the template implementation file
    #include <Jopnal/Core/Inl/DebugHandler.inl>
}

#endif

/// \class DebugHandler
/// \ingroup core
///
/// #TODO Detailed description