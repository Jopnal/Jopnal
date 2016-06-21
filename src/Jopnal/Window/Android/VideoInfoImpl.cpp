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

#include <Jopnal/Window/Android/VideoInfoImpl.hpp>

#ifdef JOP_OS_ANDROID

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Core/Android/ActivityState.hpp>

#endif

//////////////////////////////////////////////


namespace jop { namespace detail
{
    const std::vector<glm::uvec2>& VideoInfoImpl::getSupportedResolutions()
    {
        static std::vector<glm::uvec2> vec;

        if (vec.empty())
        {
            const glm::uvec2 res = getDesktopResolution();

            vec.push_back(res);
            vec.push_back(glm::uvec2(res.y, res.x));
        }

        return vec;
    }

    //////////////////////////////////////////////

    glm::uvec2 VideoInfoImpl::getDesktopResolution()
    {
        auto state = detail::ActivityState::get();

        std::lock_guard<decltype(state->mutex)> lock(state->mutex);

        return state->screenSize;
    }
}}

#endif