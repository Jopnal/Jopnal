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

        /// Polymorphic base class for data
        ///
        struct Concept
        {
            virtual ~Concept() = 0;
            virtual Concept* clone() const = 0;
            virtual const std::type_info& getType() const = 0;
        };

        /// Specialized class for storing any type.
        ///
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

        /// \brief Default constructor
        ///
        /// Doesn't initialize any data.
        ///
        Any() = default;

        /// \brief Constructor for initialization
        ///
        /// \param data The data to be copied and stored
        ///
        template<typename T>
        Any(const T& data);

        /// \brief Constructor for use with nullptr
        ///
        /// This will do the same as the default constructor.
        ///
        Any(std::nullptr_t);

        /// \brief Copy constructor
        ///
        Any(const Any& other);

        /// \brief Copy assignment operator
        ///
        /// \return Reference to this
        ///
        Any& operator =(const Any& other);


        /// \brief Cast this object
        ///
        /// This function asserts that the data matches the type of the template argument.
        /// If you're unsure, you should check the type before calling this.
        ///
        /// \return Reference to the internal data
        ///
        template<typename T>
        T& cast();

        /// \copydoc Any::cast()
        ///
        template<typename T>
        const T& cast() const;

        /// \brief Delete the contained data
        ///
        void clear();

        /// \brief Check if this object contains any data
        ///
        /// \return True if any data exits
        ///
        bool isValid() const;

        /// \copydoc Any::isValid()
        ///
        operator bool() const;

        /// \brief Get the type info of the contained object
        ///
        /// \return type_info of the object
        ///
        const std::type_info& getType() const;

        /// \brief Assign new data with an lvalue reference
        ///
        /// This function will assign the data normally if the type matches or the assignee is a
        /// pointer of the same type. The data will be replaced with the new type if it doesn't match.
        ///
        /// \param data The data to be assigned
        ///
        /// \return Reference to this
        ///
        template<typename T>
        Any& operator =(const T& data);

        /// \brief Assign new data with an rvalue reference
        ///
        /// This function will assign the data normally if the type matches or the assignee is a
        /// pointer of the same type. The data will be replaced with the new type if it doesn't match.
        ///
        /// \param data The data to be assigned
        ///
        /// \return Reference to this
        ///
        template<typename T>
        Any& operator =(T&& data);

        /// \brief Check this object against type_info
        ///
        /// Returns true if the given type matches the contained type exactly.
        ///
        /// \param type The type info to compare against
        ///
        /// \return True if the type matches and this object is not empty
        ///
        bool operator ==(const std::type_info& type) const;

    private:

        std::unique_ptr<Concept> m_data;    ///< Pointer holding the data

    };

    // Include the template implementation file
    #include <Jopnal/Utility/Inl/Any.inl>
}

#endif

/// \class Any
/// \ingroup utility