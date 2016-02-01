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

        ///
        template<typename T>
        PtrWrapper(T* ptr);

        ///
        PtrWrapper(const PtrWrapper& other);

        ///
        PtrWrapper(std::nullptr_t);


        ///
        template<typename T>
        PtrWrapper& operator =(const T& data);

        ///
        template<typename T>
        T* cast();

        ///
        operator bool() const;

        ///
        bool operator ==(const std::type_info& otherType) const;

    private:

        void* m_ptr;
        const std::type_info& m_type;

    };

    // Include the template implementation file
    #include <Jopnal/Utility/Inl/PtrWrapper.inl>
}

#endif