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

#ifndef JOP_ANY_HPP
#define JOP_ANY_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Utility/Assert.hpp>
#include <memory>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Any
    {
    public:

        struct Concept
        {
            virtual ~Concept() = 0;
            virtual Concept* clone() const = 0;
            virtual const std::type_info& getType() const = 0;
        };

        template<typename T>
        class Data final : public Concept
        {
        private:

            JOP_DISALLOW_COPY_MOVE(Data);

        public:

            Data() = default;

            Data(const T& data);


            Data* clone() const override;

            T& getData();

            const T& getData() const;

            const std::type_info& getType() const override;

        private:

            T m_data;
        };

    public:

        Any() = default;

        template<typename T>
        Any(const T& data);

        Any(std::nullptr_t);

        Any(const Any& other);

        Any& operator =(const Any& other);


        template<typename T>
        T& cast();

        template<typename T>
        const T& cast() const;

        void clear();

        bool isValid() const;

        operator bool() const;

        const std::type_info& getType() const;

        template<typename T>
        Any& operator =(const T& data);

        //template<typename T>
        //Any& operator =(T&& data);

        bool operator ==(const std::type_info& type) const;


    private:

        std::unique_ptr<Concept> m_data;

    };

    // Include the template implementation file
    #include <Jopnal/Utility/Inl/Any.inl>
}

#endif

/// \class Any
/// \ingroup utility