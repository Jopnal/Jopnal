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

//////////////////////////////////////////////


#ifdef JOP_OPENGL_ES

    #include <GLES3/gl3.h>
    #include <GLES2/gl2ext.h>
    #include <GLES3/gl3platform.h>

    namespace jop { namespace detail
    {
        bool checkEGLExtension(const char* ext);
    }}

    #define JOP_CHECK_GL_EXTENSION(extension) (::jop::detail::checkEGLExtension(#extension))

#else

    #include <Jopnal/Graphics/OpenGL/gl_4.5.h>

    #define JOP_CHECK_GL_EXTENSION(extension) ((ogl_ext_##extension)  != ogl_LOAD_FAILED)

#endif

#endif