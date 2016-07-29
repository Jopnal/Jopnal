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
#include <sstream>
#include <fstream>
#include <mutex>
#include <array>

//////////////////////////////////////////////


namespace jop
{
    namespace detail
    {
        class FileSystemInitializer;
    }

    #define __JOP_DEBUG_BASE(sev, stream) \
        {::jop::DebugHandler::getInstance() << ::jop::DebugHandler::Severity::sev << stream << std::endl;}

    #define __JOP_DEBUG_BASE_EMPTY {}

    // Error verbosity
    #if JOP_CONSOLE_VERBOSITY >= 0
        #define JOP_DEBUG_ERROR(stream)   __JOP_DEBUG_BASE(Error, stream)
    #else
        #define JOP_DEBUG_ERROR(stream)   __JOP_DEBUG_BASE_EMPTY
    #endif

    // Warning verbosity
    #if JOP_CONSOLE_VERBOSITY >= 1
        #define JOP_DEBUG_WARNING(stream) __JOP_DEBUG_BASE(Warning, stream)
    #else
        #define JOP_DEBUG_WARNING(stream) __JOP_DEBUG_BASE_EMPTY
    #endif

    // Info verbosity
    #if JOP_CONSOLE_VERBOSITY >= 2
        #define JOP_DEBUG_INFO(stream)    __JOP_DEBUG_BASE(Info, stream)
    #else
        #define JOP_DEBUG_INFO(stream)    __JOP_DEBUG_BASE_EMPTY
    #endif

    // Diagnostic verbosity
    #if JOP_CONSOLE_VERBOSITY >= 3
        #define JOP_DEBUG_DIAG(stream)    __JOP_DEBUG_BASE(Diagnostic, stream)
    #else
        #define JOP_DEBUG_DIAG(stream)    __JOP_DEBUG_BASE_EMPTY
    #endif

    #define __JOP_DEBUG_ONCE_BASE(SEV, stream)      \
        {                                           \
            static bool __jop_debugonce = false;    \
                                                    \
            if (!__jop_debugonce)                   \
                JOP_DEBUG_##SEV(stream);            \
        }

    #define JOP_DEBUG_ERROR_ONCE(stream)    __JOP_DEBUG_ONCE_BASE(ERROR, stream)
    #define JOP_DEBUG_WARNING_ONCE(stream)  __JOP_DEBUG_ONCE_BASE(WARNING, stream)
    #define JOP_DEBUG_INFO_ONCE(stream)     __JOP_DEBUG_ONCE_BASE(INFO, stream)
    #define JOP_DEBUG_DIAG_ONCE(stream)     __JOP_DEBUG_ONCE_BASE(DIAG, stream)

    class JOP_API DebugHandler
    {
    private:

        friend class detail::FileSystemInitializer;

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
            Diagnostic,

            __Always
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

        /// \brief Enable/disable file logging
        ///
        /// \param set True to enable
        ///
        void setFileLogging(const bool set);

        /// \brief Check if file logging is enabled
        ///
        /// \return True if enabled
        ///
        bool fileLoggingEnabled() const;


        std::recursive_mutex& getMutex();


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

        /// \brief An operator for inputting data into the stream
        ///
        /// \param data The data to be output
        ///
        /// \return Reference to self
        ///
        template<typename T>
        DebugHandler& operator <<(const T& data);

    private:

        void openFileHandles();

        void closeFileHandles();

        std::stringstream m_stream;                 ///< The stream object
        Severity m_displaySeverity;                 ///< The current severity
        Severity m_lastSeverity;                    ///< The last set severity
        std::string m_last;                         ///< Last string entered. Used for culling repeating messages
        bool m_consoleEnabled;                      ///< Is the console enabled?
        bool m_noSpam;                              ///< Is the no spam mode set?
        bool m_debuggerOutput;                      ///< Use debugger output?
        bool m_fileLogging;                         ///< Is file logging enabled?
        std::recursive_mutex m_mutex;               ///< Mutex
        std::array<std::ofstream, 4> m_fileHandles; ///< Log file handles
    };

    // Include the template implementation file
    #include <Jopnal/Core/Inl/DebugHandler.inl>
}

#endif

/// \class DebugHandler
/// \ingroup core
///
/// 