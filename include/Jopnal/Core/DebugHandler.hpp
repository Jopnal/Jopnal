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

#ifndef JOP_DEBUGHANDLER_HPP
#define JOP_DEBUGHANDLER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Color.hpp>
#include <sstream>

//////////////////////////////////////////////


namespace jop
{
    #define JOP_DEBUG_ERROR(stream)   {jop::DebugHandler::getInstance() << jop::Color::Red << jop::DebugHandler::Severity::Error << stream << std::endl;}
    #define JOP_DEBUG_WARNING(stream) {jop::DebugHandler::getInstance() << jop::Color::Yellow << jop::DebugHandler::Severity::Warning << stream << std::endl;}
    #define JOP_DEBUG_INFO(stream)    {jop::DebugHandler::getInstance() << jop::Color::White << jop::DebugHandler::Severity::Info << stream << std::endl;}

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
/// \brief A class to handle debug output
/// \addtogroup Core
///
/// #TODO Detailed description