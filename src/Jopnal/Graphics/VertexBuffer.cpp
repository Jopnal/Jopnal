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

// Headers
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

	#include <Jopnal/Graphics/VertexBuffer.hpp>

    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>
    #include <Jopnal/Utility/Assert.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    VertexBuffer::VertexBuffer(const Type type, const Usage usage)
        : Buffer(type, usage)
    {
        JOP_ASSERT(type == Type::ArrayBuffer || type == Type::ElementArrayBuffer, "Invalid buffer type specified in VertexBuffer. Must be either ArrayBuffer or IndexBuffer");
    }

    VertexBuffer::VertexBuffer(const VertexBuffer& other)
        : Buffer(other)
    {
        *this = other;
    }

    VertexBuffer& VertexBuffer::operator =(const VertexBuffer& other)
    {
        if (m_bytesAllocated)
        {
            bind();

            setData(NULL, m_bytesAllocated);
            glBindBuffer(GL_COPY_READ_BUFFER, other.m_buffer);
            glBindBuffer(GL_COPY_WRITE_BUFFER, m_buffer);
            glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, m_bytesAllocated);

            Buffer::operator=(other);
        }

        return *this;
    }

    VertexBuffer::VertexBuffer(VertexBuffer&& other)
        : Buffer(std::move(other))
    {}

    VertexBuffer& VertexBuffer::operator =(VertexBuffer&& other)
    {
        destroy();

        Buffer::operator=(std::move(other));

        return *this;
    }
  
    //////////////////////////////////////////////

    void VertexBuffer::setData(const void* data, const std::size_t bytes)
    {
        bind();

        if (m_buffer && bytes > 0 && data)
        {
            glCheck(glBufferData(m_bufferType, bytes, NULL, m_usage));
            m_bytesAllocated = bytes;
            setSubData(data, 0, bytes);
            return;
        }
    }

    //////////////////////////////////////////////

    void VertexBuffer::setSubData(const void* data, const std::size_t offset, const std::size_t size)
    {
        if (m_buffer && data && size)
            glCheck(glBufferSubData(m_bufferType, offset, size, data));
    }
}