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

#ifndef JOP_CONFIG_HPP
#define JOP_CONFIG_HPP

//////////////////////////////////////////////

// This file contains the configuration options for the engine compilation

// OpenGL
#define JOP_OPENGL_VERSION_MAJOR 3
#define JOP_OPENGL_VERSION_MINOR 3

// Define to enable gl error checks
#define JOP_OPENGL_ERROR_CHECKS

// Debug
#define JOP_DEBUG_MODE

// Console
#ifdef JOP_DEBUG_MODE
    #define JOP_CONSOLE_VERBOSITY 3
#else
    #define JOP_CONSOLE_VERBOSITY 0
#endif

// Asserts
#define JOP_ENABLE_ASSERTS

// Version
#define JOP_VERSION_MAJOR 0
#define JOP_VERSION_MINOR 2
#define JOP_VERSION_STRING "indev 0.2"

#endif