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

#ifndef JOP_PRECOMPILED_HPP
#define JOP_PRECOMPILED_HPP

//******** HEADERS ********//

// Needed for configuration
#include <Jopnal/Config.hpp>
#include <Jopnal/OS.hpp>

// Windows
#ifdef JOP_OS_WINDOWS

    #include <Jopnal/Core/Win32/Win32.hpp>
    #include <io.h>
    #include <fcntl.h>

#endif

// Android
#ifdef JOP_OS_ANDROID

    #include <android/asset_manager.h>

#endif

// OpenGL
#include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
#include <Jopnal/Graphics/OpenGL/GlCheck.hpp>

// GLFW
#ifdef JOP_OS_DESKTOP

    #include <GLFW/glfw3.h>

    #define GLFW_EXPOSE_NATIVE_WIN32
    #define GLFW_EXPOSE_NATIVE_WGL
    #include <GLFW/glfw3native.h>

#endif

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

// OpenAlSoft
#include <OpenAlSoft/AL/al.h>
#include <OpenAlSoft/AL/alc.h>
#include <OpenAlSoft/AL/alext.h>

// Bullet
#pragma warning(push)
#pragma warning(disable: 4127)
#include <Bullet/btBulletCollisionCommon.h>
#include <Bullet/btBulletDynamicsCommon.h>
#include <Bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#pragma warning(pop)

// RapidJSON
#pragma warning(push)
#pragma warning(disable: 4244)
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#pragma warning(pop)

// PhysFS
#include <PhysFS/physfs.h>

// Standard headers
#include <algorithm>
#include <iostream>
#include <sstream>
#include <array>
#include <string>
#include <vector>
#include <cctype>
#include <climits>
#include <locale>
#include <codecvt>
#include <cstring>

// Additional STL header
#include <Jopnal/STL.hpp>

// Jopnal
#include <Jopnal/Jopnal.hpp>

// Implementation classes
#include <Jopnal/Physics/Detail/WorldImpl.hpp>

#endif