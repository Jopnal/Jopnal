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
#include <mutex>

//////////////////////////////////////////////


namespace jop
{
    // Error verbosity
    #if JOP_CONSOLE_VERBOSITY >= 0
        #define JOP_DEBUG_ERROR(stream)   {jop::DebugHandler::getInstance() << jop::Color::Red << jop::DebugHandler::Severity::Error << stream << std::endl;}
    #else
        #define JOP_DEBUG_ERROR(stream)   {}
    #endif

    // Warning verbosity
    #if JOP_CONSOLE_VERBOSITY >= 1
        #define JOP_DEBUG_WARNING(stream) {jop::DebugHandler::getInstance() << jop::Color::Yellow << jop::DebugHandler::Severity::Warning << stream << std::endl;}
    #else
        #define JOP_DEBUG_WARNING(stream) {}
    #endif

    // Info verbosity
    #if JOP_CONSOLE_VERBOSITY >= 2
        #define JOP_DEBUG_INFO(stream)    {jop::DebugHandler::getInstance() << jop::Color::White << jop::DebugHandler::Severity::Info << stream << std::endl;}
    #else
        #define JOP_DEBUG_INFO(stream)    {}
    #endif

    // Diagnostic verbosity
    #if JOP_CONSOLE_VERBOSITY >= 3
        #define JOP_DEBUG_DIAG(stream)    {jop::DebugHandler::getInstance() << jop::Color::Gray << jop::DebugHandler::Severity::Diagnostic << stream << std::endl;}
    #else
        #define JOP_DEBUG_DIAG(stream)    {}
    #endif

    class JOP_API DebugHandler
    {
    private:

        /// Private constructor since this is a singleton class
        ///
        DebugHandler();

    public:

        /// The severity levels
        ///
        enum class Severity
        {
            Error,
            Warning,
            Info,
            Diagnostic
        };

    public:

        /// \brief Get the DebugHandler instance
        ///
        /// \return Reference to the instance
        ///
        static DebugHandler& getInstance();


        /// \brief Check if the console is enabled
        ///
        /// \return True if console is enabled
        ///
        bool isConsoleEnabled();

        /// \brief Enable/disable the console
        ///
        /// \param enabled True to enable
        ///
        void setEnabled(const bool enabled);

        /// \brief Set the verbosity level
        ///
        /// \param severity The verbosity to set
        ///
        void setVerbosity(const Severity severity);

        /// \brief Get the verbosity level
        ///
        /// \return The verbosity level
        ///
        Severity getSeverity() const;

        /// \brief Set the reduce spam flag
        ///
        /// \param set True to enable
        ///
        void setReduceSpam(const bool set);

        /// \brief Set debugger logger attachment
        ///
        /// \param set True to enable debugger output
        ///
        void setDebuggerOutput(const bool set);


        /// \brief Operator for setting the severity level for the next text object
        ///
        /// \param severity The severity level
        ///
        /// \return Reference to self
        ///
        DebugHandler& operator <<(const Severity severity);

        /// \brief Flushes the stream using std::endl
        ///
        /// \return Reference to self
        ///
        DebugHandler& operator <<(std::basic_ostream<char, std::char_traits<char>>& (*)(std::basic_ostream<char, std::char_traits<char>>&));

        /// \brief Sets the color for the next text object
        ///
        /// \param color The color to set
        ///
        /// \return Reference to self
        ///
        DebugHandler& operator <<(const jop::Color& color);

        /// \brief An operator for inputting data into the stream
        ///
        /// \param data The data to be output
        ///
        /// \return Reference to self
        ///
        template<typename T>
        DebugHandler& operator <<(const T& data);

    private:

        std::stringstream m_stream;     ///< The stream object
        Severity m_displaySeverity;     ///< The current severity
        Severity m_lastSeverity;        ///< The last set severity
        std::string m_last;             ///< Last string entered. Used for culling repeating messages
        bool m_consoleEnabled;          ///< Is the console enabled?
        bool m_noSpam;                  ///< Is the no spam mode set?
        bool m_debuggerOutput;          ///< Use debugger output?
        std::recursive_mutex m_mutex;   ///< Mutex
    };

    // Include the template implementation file
    #include <Jopnal/Core/Inl/DebugHandler.inl>
}

#endif

/// \class DebugHandler
/// \ingroup core
///
/// 