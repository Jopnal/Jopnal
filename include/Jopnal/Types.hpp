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

#ifndef JOP_TYPES_HPP
#define JOP_TYPES_HPP

//////////////////////////////////////////////


namespace jop
{
    typedef float               float32; ///< 32 bit float (single-precision)
    typedef double              float64; ///< 64 bit float (double precision)

    typedef signed char         int8;  ///< 8 bit signed integer     
    typedef signed short        int16; ///< 16 bit signed integer
    typedef signed int          int32; ///< 32 bit signed integer
    typedef signed long long    int64; ///< 64 bit signed integer

    // Uints (8, 16, 32, 64 bit)
    typedef unsigned char       uint8;  ///< 8 bit unsigned integer
    typedef unsigned short      uint16; ///< 16 bit unsigned integer
    typedef unsigned int        uint32; ///< 32 bit unsigned integer
    typedef unsigned long long  uint64; ///< 64 bit unsigned integer
}

#endif