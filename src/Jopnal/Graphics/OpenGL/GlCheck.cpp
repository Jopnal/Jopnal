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

// Headers
#include JOP_PRECOMPILED_HEADER_FILE

#include <Jopnal/Graphics/OpenGL/GlCheck.hpp>

#ifdef JOP_OPENGL_ERROR_CHECKS

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Core/DebugHandler.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>

#endif

//////////////////////////////////////////////


namespace
{
    bool ns_error = false;
}

namespace jop { namespace detail
{
    void openGlCheck(const char* func, const char* file, const unsigned int line)
    {
    #if JOP_CONSOLE_VERBOSITY < 0

        func;
        file;
        line;

    #else

        GLenum errCode = glGetError();

        if (errCode != GL_NO_ERROR)
        {
            ns_error = true;
            const char* errorS = "unknown error";

            switch (errCode)
            {
                case GL_INVALID_ENUM:
                    errorS = "GL_INVALID_ENUM";
                    break;
                case GL_INVALID_VALUE:
                    errorS = "GL_INVALID_VALUE";
                    break;
                case GL_INVALID_OPERATION:
                    errorS = "GL_INVALID_OPERATION";
                    break;

            #ifndef JOP_OPENGL_ES

                case GL_STACK_OVERFLOW:
                    errorS = "GL_STACK_OVERFLOW";
                    break;
                case GL_STACK_UNDERFLOW:
                    errorS = "GL_STACK_UNDERFLOW";
                    break;

            #endif

                case GL_OUT_OF_MEMORY:
                    errorS = "GL_OUT_OF_MEMORY";
                    break;
                case GL_INVALID_FRAMEBUFFER_OPERATION:
                    errorS = "GL_INVALID_FRAMEBUFFER_OPERATION";
            }

            JOP_DEBUG_ERROR("An OpenGL error occurred after a call to \"" << func << ": " << errorS << " (on line " << line << " in " << file);
        }
        else
            ns_error = false;

    #endif
    }
}}

#endif

namespace jop { namespace gl
{
    bool hasError()
    {
    #ifdef JOP_OPENGL_ERROR_CHECKS
        return ns_error;
    #else
        return false;
    #endif
    }
}}