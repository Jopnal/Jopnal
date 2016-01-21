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

#ifndef JOP_EXPORT_HPP
#define JOP_EXPORT_HPP

// Headers
#include <Jopnal/OS.hpp>

//////////////////////////////////////////////


#ifdef JOP_DYNAMIC

    #if defined(JOP_OS_WINDOWS)
    
        #define JOP_API_EXPORT __declspec(dllexport)
        #define JOP_API_IMPORT __declspec(dllimport)

    #endif

    #if defined(JOP_EXPORT_API)
        #define JOP_API JOP_API_EXPORT
    #else
        #define JOP_API JOP_API_IMPORT
    #endif

#else

    #define JOP_API

#endif

#endif