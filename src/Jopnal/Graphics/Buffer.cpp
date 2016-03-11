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
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace 
{
    static const int ns_bufferType[] =
    {
        gl::ARRAY_BUFFER,
        gl::ELEMENT_ARRAY_BUFFER,
        gl::TEXTURE_BUFFER,
        gl::TRANSFORM_FEEDBACK_BUFFER,
        gl::UNIFORM_BUFFER
    };

    static const int ns_usageType[] =
    {
        gl::STATIC_DRAW,
        gl::DYNAMIC_DRAW,
        gl::STREAM_DRAW
    };
}

namespace jop
{

    Buffer::Buffer(const Type type, const Usage usage)
        : m_bytesAllocated  (0),
          m_buffer          (0),
          m_bufferType      (ns_bufferType[static_cast<int>(type)]),
          m_usage           (ns_usageType[static_cast<int>(usage)])
    {}

    Buffer::Buffer(const Buffer& other)
        : m_bufferType  (other.m_bufferType),
          m_usage       (other.m_usage)
    {
        *this = other;
    }

    Buffer& Buffer::operator =(const Buffer& other)
    {
        JOP_ASSERT(m_bufferType == other.m_bufferType, "Cannot assign a buffer of different type!");

        m_bytesAllocated = other.m_bytesAllocated;

        return *this;
    }

    Buffer::Buffer(Buffer&& other)
        : m_bufferType  (other.m_bufferType),
          m_usage       (other.m_usage)
    {
        *this = std::move(other);
    }

    Buffer& Buffer::operator =(Buffer&& other)
    {
        JOP_ASSERT(m_bufferType == other.m_bufferType, "Cannot assign a buffer of different type!");

        m_buffer = other.m_buffer;
        other.m_buffer = 0;

        m_bytesAllocated = other.m_bytesAllocated;

        return *this;
    }

    Buffer::~Buffer()
    {
        destroy();
    }

    //////////////////////////////////////////////

    void Buffer::bind() const
    {
        if (!m_buffer)
            glCheck(gl::GenBuffers(1, &m_buffer));

        glCheck(gl::BindBuffer(m_bufferType, m_buffer));
    }

    //////////////////////////////////////////////

    void Buffer::unbind(const Type type)
    {
        glCheck(gl::BindBuffer(ns_bufferType[static_cast<const unsigned int>(type)], 0));
    }

    //////////////////////////////////////////////

    void Buffer::clear()
    {
        if (m_buffer && m_bytesAllocated > 0)
        {
            bind();

            glCheck(gl::BufferData(m_bufferType, 0, NULL, m_usage));

            m_bytesAllocated = 0;
        }
    }

    //////////////////////////////////////////////

    void Buffer::destroy()
    {
        if (m_buffer)
        {
            glCheck(gl::BindBuffer(m_bufferType, m_buffer));
            glCheck(gl::DeleteBuffers(1, &m_buffer));
            m_bytesAllocated = 0;
            m_buffer = 0;
        }
    }

    //////////////////////////////////////////////

    std::size_t Buffer::getAllocatedSize() const
    {
        return m_bytesAllocated;
    }
}