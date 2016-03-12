// Jopnal Editor C++ Application
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
#include <Jopnal/OS.hpp>

// Windows
#ifdef JOP_OS_WINDOWS

    // Disable warning C4512 and C4503
    #pragma warning(disable: 4512)
    #pragma warning(disable: 4503)

    #define _CRT_SECURE_NO_WARNINGS

    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef VC_EXTRALEAN
        #define VC_EXTRALEAN
    #endif

    #include <windows.h>
    #include <GL/wgl_jop.hpp>

#endif

// OpenGL
#include <GL/GL.hpp>

// Standard
#include <iostream>
#include <thread>

// Nana
#include <nana/gui/wvl.hpp> 
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/timer.hpp>

// Jopnal Engine
#include <Jopnal/Jopnal.hpp>

// Jopnal Editor
#include <Jopnal/Editor/Forms/MainWindow.hpp>
#include <Jopnal/Editor/CommandBuffer.hpp>
#include <Jopnal/Editor/WindowUpdater.hpp>
#include <Jopnal/Editor/Forms/OpenGLWindow.hpp>
#include <Jopnal/Editor/Command.hpp>
#include <Jopnal/Editor/Forms/PropertyWindow.hpp>
#include <Jopnal/Editor/Forms/ToolWindow.hpp>