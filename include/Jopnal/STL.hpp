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

#ifndef JOP_STL_HPP
#define JOP_STL_HPP

// Headers
#include <Jopnal/Header.hpp>

#ifdef JOP_OS_ANDROID

#include <memory>

//////////////////////////////////////////////


namespace std
{
    // Android STL doesn't have make_unique, so implement it here
    template<typename T, typename ... Args>
    inline unique_ptr<T> make_unique(Args&&... args)
    {
        return unique_ptr<T>(new T(forward<Args>(args)...));
    }
}

#endif
#endif