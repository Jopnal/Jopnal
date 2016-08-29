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

#ifndef JOP_BUFFER_HPP
#define JOP_BUFFER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <cstddef>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Buffer
    {
    public:
        
        /// \brief Enum class consisting of buffer types
        ///
        enum class Type
        {
            ArrayBuffer,        ///< Array buffer (vertex data)
            ElementArrayBuffer, ///< Element buffer (index data)
            UniformBuffer       ///< Uniform buffer
        };

        /// \brief Enum of usage types
        ///
        enum Usage
        {
            StaticDraw,     ///< Static buffer, use when the data rarely changes
            DynamicDraw,    ///< Dynamic buffer, use when the data changes often
            StreamDraw      ///< Stream buffer, use when the data changes all the time
        };

    public:

        /// \brief Constructor 
        ///
        /// \param type Buffer type 
        /// \param usage Usage type
        ///
        Buffer(const Type type, const Usage usage);

        /// \brief Copy constructor
        ///
        Buffer(const Buffer& other);

        /// \brief Copy assignment operator
        ///
        Buffer& operator =(const Buffer& other);

        /// \brief Move constructor 
        ///
        Buffer(Buffer&& other);

        /// \brief Move assignment operator
        ///
        Buffer& operator =(Buffer&& other);

        /// \brief Virtual destructor
        ///
        virtual ~Buffer() = 0;


        /// \brief Bind this buffer
        ///
        void bind() const;

        /// \brief Unbind a buffer
        ///
        /// \param type The buffer type to unbind
        ///
        static void unbind(const Type type);

        /// \brief Clear this buffer
        ///
        /// The buffer itself won't be destroyed, only the data.
        ///
        void clear();

        /// \brief Destroy this buffer
        ///
        void destroy();

        /// \brief Get the allocated size in bytes
        ///
        /// \return The allocated size
        ///
        std::size_t getAllocatedSize() const;

    protected:
  
        std::size_t m_bytesAllocated;   ///< Size of the allocated buffer
        mutable unsigned int m_buffer;  ///< Buffer's OpenGL handle
        const int m_bufferType;         ///< Type of the buffer
        const int m_usage;              ///< The usage type
    };
}

/// \class jop::Buffer
/// \ingroup graphics

#endif