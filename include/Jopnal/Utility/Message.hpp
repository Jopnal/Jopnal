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

#ifndef JOP_MESSAGEUTIL_HPP
#define JOP_MESSAGEUTIL_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <string>
#include <typeinfo>

//////////////////////////////////////////////


namespace jop
{
    enum class MessageResult
    {
        Continue,
        Escape
    };

    namespace detail
    {
        class VoidWrapper
        {
        private:

            JOP_DISALLOW_COPY_MOVE(VoidWrapper);

        public:

            template<typename T>
            VoidWrapper(T* ptr);


            template<typename T>
            void operator =(T& data);

            operator bool() const;

        private:

            void* m_ptr;
            const std::type_info& m_type;

        };
}

    class JOP_API Message
    {
    public:

        Message(const std::string& message, detail::VoidWrapper& ptr);

        /// \brief Check if the message has the given filter symbol
        ///
        /// This function is used to check how a message should be filtered.
        /// If the message doesn't contain a proper filtering field, this test will pass.
        ///
        /// \param message The message in its entirety
        /// \param symbol The filter symbol to check
        ///
        /// \return True if symbol was found
        static bool hasFilterSymbol(const std::string& message, const std::string& symbol);

    };
}

#endif

/// \class Resource
/// \ingroup utility