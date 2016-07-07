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

#ifndef JOP_ACTIVITYSTATE_HPP
#define JOP_ACTIVITYSTATE_HPP

// Headers
#include <Jopnal/Header.hpp>

#ifdef JOP_OS_ANDROID

#include <Jopnal/Window/WindowEventHandler.hpp>
#include <glm/vec2.hpp>
#include <android/native_activity.h>
#include <mutex>

//////////////////////////////////////////////


namespace jop { namespace detail
{
    struct JOP_API ActivityState
    {
        ActivityState() = default;

        static ActivityState* create(ANativeActivity* activity);

        static ActivityState* get();

        static void reset();


        std::mutex mutex;

        ANativeActivity* nativeActivity;
        ANativeWindow* nativeWindow;

        Window* window;
        glm::uvec2 screenSize;
    };
}}

#endif
#endif