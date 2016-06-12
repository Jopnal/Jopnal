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

#ifndef JOP_EXPORT_HPP
#define JOP_EXPORT_HPP

// Headers
#include <Jopnal/Config.hpp>
#include <Jopnal/OS.hpp>

//////////////////////////////////////////////


#ifdef JOP_STATIC

    #define JOP_API

#else

    #if defined(JOP_OS_WINDOWS)
    
        #define JOP_API_EXPORT __declspec(dllexport)
        #define JOP_API_IMPORT __declspec(dllimport)

    #elif defined(JOP_OS_LINUX) || defined(JOP_OS_ANDROID)

        #if __GNUC__ >= 4

            #define JOP_API_EXPORT __attribute__((__visibility__("default")))
            #define JOP_API_IMPORT __attribute__((__visibility__("default")))

        #else

            #define JOP_API_EXPORT
            #define JOP_API_IMPORT

        #endif

    #endif

    #if defined(JOP_EXPORT_API)
        #define JOP_API JOP_API_EXPORT
    #else
        #define JOP_API JOP_API_IMPORT
    #endif

#endif

#endif