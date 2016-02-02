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

#ifndef JOP_PTRWRAPPER_HPP
#define JOP_PTRWRAPPER_HPP

// Headers
#include <Jopnal/Header.hpp>

//////////////////////////////////////////////


namespace jop
{
    class PtrWrapper
    {
    private:

        JOP_DISALLOW_MOVE(PtrWrapper);

        void operator =(const PtrWrapper&) = delete;

    public:

        /// \brief Constructor
        ///
        /// \param ptr The pointer to be assigned to the internal one
        ///
        template<typename T>
        PtrWrapper(T* ptr);

        /// \brief Copy constructor
        ///
        PtrWrapper(const PtrWrapper& other);

        /// \brief Constructor for nullptr initialization
        ///
        PtrWrapper(std::nullptr_t);


        /// \brief Assign new data for this wrapper
        ///
        /// \data The data to assign
        ///
        /// \reference to this
        ///
        template<typename T>
        PtrWrapper& operator =(const T& data);

        /// \brief Cast this object into a pointer of the given type
        ///
        /// This function will fail an assertion if the type is invalid.
        ///
        /// \return A cast pointer to the internal data
        ///
        template<typename T>
        T* cast();

        /// \brief Check if this object has any associated data
        ///
        /// \return True if the internal pointer is not nullptr
        ///
        operator bool() const;

        /// \brief Compare this object with type_info
        ///
        /// \param otherType The other type's info to compare against
        ///
        /// \return True if the types match
        ///
        bool operator ==(const std::type_info& otherType) const;

    private:

        void* m_ptr;                    ///< The pointer
        const std::type_info& m_type;   ///< Type info

    };

    // Include the template implementation file
    #include <Jopnal/Utility/Inl/PtrWrapper.inl>
}

#endif

/// \class PtrWrapper
/// \ingroup utility
///
/// Wrapper for void* with type info