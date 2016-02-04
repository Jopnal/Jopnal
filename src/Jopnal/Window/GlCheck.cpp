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
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////

#ifdef JOP_OPENGL_ERROR_CHECKS

namespace jop { namespace detail
{
    void detail::openGlCheck(const char* func, const char* file, const unsigned int line)
    {
        GLenum errCode = gl::GetError();

        if (errCode != gl::NO_ERROR_)
        {
            const char* errorS = "unknown error";

            switch (errCode)
            {
                case gl::INVALID_ENUM:
                    errorS = "GL_INVALID_ENUM";
                    break;
                case gl::INVALID_VALUE:
                    errorS = "GL_INVALID_VALUE";
                    break;
                case gl::INVALID_OPERATION:
                    errorS = "GL_INVALID_OPERATION";
                    break;
                case gl::STACK_OVERFLOW:
                    errorS = "GL_STACK_OVERFLOW";
                    break;
                case gl::STACK_UNDERFLOW:
                    errorS = "GL_STACK_UNDERFLOW";
                    break;
                case gl::OUT_OF_MEMORY:
                    errorS = "GL_OUT_OF_MEMORY";
                    break;
                case gl::INVALID_FRAMEBUFFER_OPERATION:
                    errorS = "GL_INVALID_FRAMEBUFFER_OPERATION";
            }

            JOP_DEBUG_ERROR("An OpenGL error occurred after a call to \"" << func << ": " << errorS << " (on line " << line << " in " << file);
        }
    }
}}

#endif