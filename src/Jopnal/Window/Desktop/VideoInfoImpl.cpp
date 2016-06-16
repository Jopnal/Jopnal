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

//////////////////////////////////////////////


namespace jop { namespace detail
{
    const std::vector<glm::uvec2>& VideoInfoImpl::getSupportedResolutions()
    {
        static std::vector<glm::uvec2> vec;

        if (vec.empty())
        {
            glfwInit();

            int count;
            auto modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);

            vec.reserve(count);

            auto alreadyExists = [](const int width, const int height, const std::vector<glm::uvec2>& vec) -> bool
            {
                for (auto& i : vec)
                {
                    if (static_cast<int>(i.x) == width && static_cast<int>(i.y) == height)
                        return true;
                }

                return false;
            };

            for (int i = 0; i < count; ++i)
            {
                auto& m = modes[i];

                if (!alreadyExists(m.width, m.height, vec))
                    vec.emplace_back(m.width, m.height);
            }
        }

        return vec;
    }

    //////////////////////////////////////////////

    glm::uvec2 VideoInfoImpl::getDesktopResolution()
    {
        glfwInit();

        auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        return glm::uvec2(mode->width, mode->height);
    }
}}

#endif