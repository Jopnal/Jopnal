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

#ifndef JOP_MESSAGE_HPP
#define JOP_MESSAGE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Utility/Assert.hpp>
#include <Jopnal/Utility/Any.hpp>
#include <string>
#include <typeinfo>
#include <sstream>

//////////////////////////////////////////////


namespace jop
{
    /// Message result
    ///
    enum class MessageResult
    {
        Continue,   ///< Continue to process the message
        Escape      ///< Return from sendMessage
    };

    class JOP_API Message
    {
    private:

        JOP_DISALLOW_COPY_MOVE(Message);

    public:

        /// System filtering bits
        ///
        enum Filter : unsigned short
        {
            Engine = 1,
            Subsystem = Engine << 1,
            SharedScene = Subsystem << 1,
            Scene = SharedScene << 1,
            Layer = Scene << 1,
            Object = Layer << 1,
            Component = Object << 1,
            Command = Component << 1,
            Custom = Command << 1,
            Global = 0xFFFF
        };

    public:

        /// \brief Constructor
        /// 
        /// \param message The message. Can include just a filter or be empty,
        ///                             in which case you must push the command & arguments
        ///                             in a separate context.
        /// \param ptr The return value pointer
        ///
        Message(const std::string& message, Any ptr);


        /// \brief Push an argument value
        ///
        /// \param arg The argument to push
        ///
        /// \return Reference to this
        ///
        template<typename T>
        Message& push(const T& arg);

        /// \brief Push a string argument
        ///
        /// This can be used to push the command, if the message passed to the constructor didn't have it.
        ///
        /// \param arg The argument to push
        ///
        /// \return Reference to this
        ///
        Message& push(const std::string& str);

        /// \copydoc Message::push(const std::string)
        ///
        Message& push(const char* str);

        /// \brief Push a pointer argument
        ///
        /// The pointer will be converted into a hex string and then
        /// interpreted by the command handler to recreate the pointer.
        ///
        /// \param ptrArg The pointer to push
        ///
        /// \return Reference to self
        ///
        Message& pushPointer(const void* ptrArg);

        /// \brief Push a reference argument
        ///
        /// Essentially does the same as pushPointer().
        ///
        /// \param ref The reference to push
        ///
        /// \return Reference to self
        ///
        template<typename T>
        Message& pushReference(const T& ref);


        /// \brief Get a string with the command and arguments
        ///
        /// \return A new string with the internal buffer's contents
        ///
        std::string getString() const;


        /// \brief Check if the given arguments should pass the filter
        ///
        /// \param filter The bits to check
        /// \param id The id to pass to the filter
        ///
        /// \return True if both arguments got past the filter
        ///
        bool passFilter(const unsigned short filter, const std::string& id) const;

        /// \brief Check if the given bit field should pass the filter
        ///
        /// \param filter The bits to check. Only one match is required for a pass
        ///
        /// \return True if the filter was passed
        ///
        bool passFilter(const unsigned short filter) const;

        /// \brief Check if the given id should pass the filter
        ///
        /// \param id The id to check
        ///
        /// \return True if the filter was passed
        ///
        bool passFilter(const std::string& id) const;

        /// \brief Set the filter
        ///
        /// \param filter A filter field in the correct form
        ///
        /// \return Reference to self
        ///
        Message& setFilter(const std::string& filter);

        /// \brief Get the wrapper containing the return value pointer
        ///
        /// \return Reference to the internal return value pointer
        ///
        Any& getReturnWrapper() const;

        /// \brief Check the internal command/argument buffer
        ///
        /// \return True if the buffer has at least a command
        ///
        operator bool() const;

    private:

        std::ostringstream m_command;                                       ///< Buffer containing the command and arguments in string form
        std::string m_idPattern;                                            ///< The id filter to compare any passed ids against
        mutable Any m_ptr;                                                  ///< Any object to store a possible return value
        unsigned short m_filterBits;                                        ///< Bit field with the system filter bits
        bool (*m_idMatchMethod)(const std::string&, const std::string&);    ///< Function to use in comparing the filter id and the passed id

    };

    // Include the template implementation file
    #include <Jopnal/Utility/Inl/Message.inl>
}

#endif

/// \class Resource
/// \ingroup utility