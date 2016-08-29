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

#ifndef JOP_VERTEXBUFFER_HPP
#define JOP_VERTEXBUFFER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Buffer.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API VertexBuffer : public Buffer
    {
    public:

        /// \brief Constructor 
        ///
        /// \param type Buffer type
        /// \param usage Usage type
        ///
        VertexBuffer(const Type type, const Usage usage = Buffer::StaticDraw);

        /// \brief Copy constructor
        ///
        VertexBuffer(const VertexBuffer& other);

        /// \brief Copy assignment operator
        ///
        VertexBuffer& operator =(const VertexBuffer& other);

        /// \brief Move constructor
        ///
        VertexBuffer(VertexBuffer&& other);

        /// \brief Move assignment operator
        ///
        VertexBuffer& operator =(VertexBuffer&& other);


        /// \brief Set the buffer data
        ///
        /// \param data Pointer to the data
        /// \param size Size of the buffer data in bytes
        ///
        void setData(const void* data, const std::size_t size);
        
        /// \brief Set buffer sub data
        ///
        /// \param data Pointer to the data
        /// \param offset The start position in the buffer
        /// \param size Size of the sub data in bytes
        ///
        void setSubData(const void* data, const std::size_t offset, const std::size_t size);
    };
}

/// \class jop::VertexBuffer
/// \ingroup graphics

#endif