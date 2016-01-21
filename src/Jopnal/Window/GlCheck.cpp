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