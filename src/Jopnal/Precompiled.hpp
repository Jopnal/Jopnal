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
    #include <VersionHelpers.h>
    #include <Jopnal/../../project/Resources/resource.h>

#endif

// OpenGL
#ifdef JOP_OPENGL_ES

#else
    #include <Jopnal/gl_4.5.h>
#endif
#include <Jopnal/Window/GlCheck.hpp>

// GLFW
#ifdef JOP_OS_DESKTOP

    #include <GLFW/glfw3.h>

    #define GLFW_EXPOSE_NATIVE_WIN32
    #define GLFW_EXPOSE_NATIVE_WGL
    #include <GLFW/glfw3native.h>

#endif

// GLM
#include <Jopnal/MathInclude.hpp>

// SFML
//#include <SFML/Audio.hpp>

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

// FreeType
//#include <ft2build.h>
//#include FT_FREETYPE_H
//#include FT_GLYPH_H
//#include FT_OUTLINE_H
//#include FT_BITMAP_H

// PhysFS
#include <PhysFS/physfs.h>

// Assimp
//#include <assimp/Importer.hpp>
//#include <assimp/Logger.hpp>
//#include <assimp/DefaultLogger.hpp>
//#include <assimp/IOSystem.hpp>
//#include <assimp/IOStream.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>

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

#ifdef JOP_OS_ANDROID
namespace std
{
template<typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args) {
    return unique_ptr<T>(new T(forward<Args>(args)...));
}
}
#endif

// Jopnal
#include <Jopnal/Jopnal.hpp>

// Implementation classes
#include <Jopnal/Physics/Detail/WorldImpl.hpp>

#endif