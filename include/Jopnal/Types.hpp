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
    // Float (32 bit)
    typedef float float32;

    // Float (64 bit)
    typedef double float64;

    // Ints (8, 16, 32 64 bit)
    typedef signed char int8;
    typedef signed short int16;
    typedef signed int int32;
    typedef signed long long int64;

    // Uints (8, 16, 32, 64 bit)
    typedef unsigned char uint8;
    typedef unsigned short uint16;
    typedef unsigned int uint32;
    typedef unsigned long long uint64;
}

#endif