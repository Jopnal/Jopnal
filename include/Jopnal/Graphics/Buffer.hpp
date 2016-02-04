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
#include <Jopnal/Jopnal.hpp>

//////////////////////////////////////////////

namespace jop
{

    class Buffer
    {
    public:
        
        ///\brief enum class consisting of buffer types
        ///
        enum class BufferType
        {
            ArrayBuffer,
            ElementArrayBuffer,
            TextureBuffer,
            TransformFeedBackBuffer,
            UniformBuffer
        };

        ///\brief enum of usage types
        ///
        enum UsageType
        {
            StaticDraw,
            DynamicDraw,
            StreamDraw
        };

    public:

        ///\brief default constructor 
        ///
        ///\param type Buffer type 
        ///
        Buffer(BufferType type);

        ///\brief default deconstructor
        ///
        ~Buffer();

        ///\brief constructor to another buffer
        ///
        ///\param other reference to buffer constructor 
        ///
        Buffer(const Buffer& other);
           
        ///\brief operator to overload reference to buffer
        ///
        ///\param other reference to buffer constructor
        ///
        Buffer& operator =(const Buffer& other);
        
        ///\brief move constructor 
        ///
        ///\param other 
        ///
        Buffer(Buffer&& other);

        ///\brief operator to overload move constructor
        ///
        ///\param other 
        ///
        Buffer& operator =(Buffer&& other);

        ///\brief method to generate and bind buffers by its type
        ///
        void bind()const;

        ///\brief static method to unbind buffer by its type
        ///
        ///\param type buffer type 
        ///
        static void unbind(BufferType type);

        ///\brief method to clear buffers
        ///
        void clear();

        ///\brief method to destroy buffers
        ///
        void destroy();

        ///\brief method that returns m_bytesAllocated
        ///
        std::size_t getAllocatedSize()const;

    protected:

        ///\brief method to unbind 
        ///
        ///\param type const unsigned int 
        ///
        static void unbind(const unsigned int& type);
  
        std::size_t m_bytesAllocated;   
        mutable unsigned int m_buffer;  
        const int m_bufferType;          
    };
}
#endif

/// \class Buffer
/// \ingroup Graphics
///
/// Do explanation about the class