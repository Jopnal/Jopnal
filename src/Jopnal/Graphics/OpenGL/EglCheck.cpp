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

#include <Jopnal/Graphics/OpenGL/EglCheck.hpp>

#ifdef JOP_OPENGL_ES

#ifndef JOP_PRECOMPILED_HEADER

#include <Jopnal/Core/DebugHandler.hpp>
#include <EGL/egl.h>

#endif

//////////////////////////////////////////////


namespace jop { namespace detail
{
    void checkEGLError(const char* func, const char* file, const unsigned int line)
    {
        EGLint errCode = eglGetError();

        if (errCode != EGL_SUCCESS)
        {
            const char* errorS = "unknown error";

            switch (errCode)
            {
                case EGL_NOT_INITIALIZED:
                    errorS = "EGL_NOT_INITIALIZED";
                    break;
                case EGL_BAD_ACCESS:
                    errorS = "EGL_BAD_ACCESS";
                    break;
                case EGL_BAD_ALLOC:
                    errorS = "EGL_BAD_ALLOC";
                    break;
                case EGL_BAD_ATTRIBUTE:
                    errorS = "EGL_BAD_ATTRIBUTE";
                    break;
                case EGL_BAD_CONTEXT:
                    errorS = "EGL_BAD_CONTEXT";
                    break;
                case EGL_BAD_CONFIG:
                    errorS = "EGL_BAD_CONFIG";
                    break;
                case EGL_BAD_CURRENT_SURFACE:
                    errorS = "EGL_BAD_CURRENT_SURFACE";
                    break;
                case EGL_BAD_DISPLAY:
                    errorS = "EGL_BAD_DISPLAY";
                    break;
                case EGL_BAD_SURFACE:
                    errorS = "EGL_BAD_SURFACE";
                    break;
                case EGL_BAD_MATCH:
                    errorS = "EGL_BAD_MATCH";
                    break;
                case EGL_BAD_PARAMETER:
                    errorS = "EGL_BAD_PARAMETER";
                    break;
                case EGL_BAD_NATIVE_PIXMAP:
                    errorS = "EGL_BAD_NATIVE_PIXMAP";
                    break;
                case EGL_BAD_NATIVE_WINDOW:
                    errorS = "EGL_BAD_NATIVE_WINDOW";
                    break;
                case EGL_CONTEXT_LOST:
                    errorS = "EGL_CONTEXT_LOST";
            }

            JOP_DEBUG_ERROR("An EGL error occurred after a call to \"" << func << ": " << errorS << " (on line " << line << " in " << file);
        }
    }
}}

#endif