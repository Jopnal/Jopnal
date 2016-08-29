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

#ifndef JOP_OPENGL_HPP
#define JOP_OPENGL_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <string>

//////////////////////////////////////////////


#ifdef JOP_OPENGL_ES

    #ifdef JOP_OS_ANDROID

        #include <android/api-level.h>

        #if __ANDROID_API__ >= 18

            #include <GLES3/gl3.h>

            #if __ANDROID_API__ >= 23
                #include <GLES3/gl31.h>
            #endif

            #if __ANDROID_API__ >= 24
                #include <GLES3/gl32.h>
            #endif

            #define __gl2_h_ // Workaround for broken gl headers
            #include <GL_ES/gl2ext.h>
            #include <GLES3/gl3platform.h>

        #else

            #include <GL_ES/gl2.h>
            #include <GL_ES/gl2ext.h>
            #include <GLES2/gl2platform.h>

        #endif

    #endif

    namespace jop { namespace detail
    {
        JOP_API bool checkGLESExtension(const char* ext);

        JOP_API bool checkEGLExtension(const char* ext);
    }}

    #define JOP_CHECK_GL_EXTENSION(extension) (::jop::detail::checkGLESExtension(#extension))
    #define JOP_CHECK_EGL_EXTENSION(extension) (::jop::detail::checkEGLExtension(#extension))

#else

    #include <Jopnal/Graphics/OpenGL/gl_4.5.h>

    #define JOP_CHECK_GL_EXTENSION(extension) ((ogl_ext_##extension) != ogl_LOAD_FAILED)
    #define JOP_CHECK_EGL_EXTENSION(extension) false

#endif

/// \def JOP_CHECK_GL_EXTENSION
/// \brief Check if an OpenGL extension is supported
///
/// \param extension The name of the extension without quotes
/// \return True if the extension is supported

/// \def JOP_CHECK_EGL_EXTENSION
/// \brief Check if an EGL extension is supported
///
/// \copydetails JOP_CHECK_GL_EXTENSION

namespace jop { namespace gl
{
    /// \brief Get the major OpenGL version
    ///
    /// \return Major OpenGL version number
    ///
    JOP_API unsigned int getVersionMajor();

    /// \brief Get the minor OpenGL version
    ///
    /// \return Minor OpenGL version number
    ///
    JOP_API unsigned int getVersionMinor();

    /// \brief Get the GLSL version
    ///
    /// The version will be represented by a single number, e.g. 300.
    ///
    /// \return GLSL version number
    ///
    JOP_API unsigned int getGLSLVersion();

    /// \brief This will be true when using OpenGL ES
    ///
    JOP_API extern const bool es;
}}

#endif