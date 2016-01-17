// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//////////////////////////////////////////////

#ifndef JOP_PRECOMPILED_HPP
#define JOP_PRECOMPILED_HPP

//******** HEADERS ********//

// Needed for configuration
#include <Jopnal/Config.hpp>
#include <Jopnal/OS.hpp>

// Windows
#if defined(JOP_OS_WINDOWS)

    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef VC_EXTRALEAN
        #define VC_EXTRALEAN
    #endif

    #include <Windows.h>
    #include <io.h>
    #include <fcntl.h>

#endif

// OpenGL
#include <GL/GL.hpp>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>

// RapidJSON
#pragma warning(push)
#pragma warning(disable: 4244)
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#pragma warning(pop)

// PhysicsFS
#include <PhysicsFS/physfs.h>

// Standard headers
#include <algorithm>
#include <iostream>
#include <sstream>


//** Jopnal **\

// Audio

// Core
#include <Jopnal/Core/DebugHandler.hpp>
#include <Jopnal/Core/Engine.hpp>
#include <Jopnal/Core/SettingManager.hpp>

// Graphics
#include <Jopnal/Graphics/Color.hpp>

// Utility
#include <Jopnal/Utility/Assert.hpp>
#include <Jopnal/Utility/Clock.hpp>

// Window
#include <Jopnal/Window/Window.hpp>

#endif