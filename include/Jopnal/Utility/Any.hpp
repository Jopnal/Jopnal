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

        /// \brief Cast this object dynamically
        ///
        /// This will check the type before cast.
        ///
        /// \return The cast data
        ///
        template<typename T>
        T dynamicCast();

        /// \copydoc dynamicCast
        ///
        template<typename T>
        const T dynamicCast() const;

        /// \brief Delete the contained data
        ///
        void clear();

        /// \brief Check if this object contains any data
        ///
        /// \return True if any data exits
        ///
        bool isValid() const;

        /// \copydoc isValid
        ///
        operator bool() const;

        /// \brief Get the type info of the contained object
        ///
        /// \return type_info of the object
        ///
        const std::type_info& getType() const;

        /// \brief Assign new data with an lvalue reference
        ///
        /// This function will assign the data normally if the type matches.
        /// The data will be replaced with the new type if it doesn't match.
        ///
        /// \param data The data to be assigned
        ///
        /// \return Reference to this
        ///
        template<typename T>
        Any& operator =(const T& data);

        /// \brief Assign new data with an rvalue reference
        ///
        /// This function will assign the data normally if the type matches. 
        /// The data will be replaced with the new type if it doesn't match.
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