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

#ifndef JOP_OS_HPP
#define JOP_OS_HPP

//////////////////////////////////////////////


// Detect operating system
#if defined(_WIN32)

    #pragma warning(disable: 4251) // Export warnings for standard classes
    #pragma warning(disable: 4201) // Nameless struct/union
    #pragma warning(disable: 4503) // Decorated name truncation
    #pragma warning(disable: 4068) // Unrecognized pragma
        
    #define JOP_OS_WINDOWS
    #define JOP_OS_DESKTOP

#elif defined(__unix__)

    #if defined(__ANDROID__)

        #define JOP_OS_ANDROID

    #elif defined(__linux__)

        #define JOP_OS_LINUX
        #define JOP_OS_DESKTOP

    #else

        #error Jopnal: unsupported unix operating system

    #endif

#else

    #error Jopnal: unsupported operating system

#endif

// Detect compiler
#if defined(_MSC_VER)

    #define JOP_COMPILER_MSVC

#elif defined(__GNUC__)

    #define JOP_COMPILER_GNU

#endif

#endif