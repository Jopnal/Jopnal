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

#include <Jopnal/Window/Desktop/VideoInfoImpl.hpp>

#ifdef JOP_OS_DESKTOP

#ifndef JOP_PRECOMPILED_HEADER

    #include <GLFW/glfw3.h>

#endif

//////////////////////////////////////////////


namespace jop { namespace detail
{
    const std::vector<VideoInfo>& VideoInfoImpl::getSupportedModes()
    {
        static std::vector<VideoInfo> vec;

        if (vec.empty())
        {
            glfwInit();

            int count;
            auto modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);
            
            vec.reserve(count);

            auto alreadyExists = [](const int width, const int height, const int refreshRate, const std::vector<VideoInfo>& vec) -> bool
            {
                for (auto& i : vec)
                {
                    if (static_cast<int>(i.resolution.x) == width && static_cast<int>(i.resolution.y) == height && static_cast<int>(i.refreshRate) == refreshRate)
                        return true;
                }

                return false;
            };

            for (int i = 0; i < count; ++i)
            {
                auto& m = modes[i];

                if (!alreadyExists(m.width, m.height, m.refreshRate, vec))
                    vec.push_back(VideoInfo{glm::uvec2(glm::ivec2(m.width, m.height)), static_cast<unsigned int>(m.refreshRate)});
            }
        }

        return vec;
    }

    //////////////////////////////////////////////

    VideoInfo VideoInfoImpl::getDesktopMode()
    {
        glfwInit();

        auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        return VideoInfo{glm::uvec2(mode->width, mode->height), static_cast<unsigned int>(mode->refreshRate)};
    }
}}

#endif