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
#include <android/native_window.h>
#include <android/configuration.h>
#include <android/window.h>
#include <EGL/egl.h>
#include <thread>
#include <mutex>
#include <atomic>

//////////////////////////////////////////////


namespace jop { namespace detail
{
    struct JOP_API ActivityState
    {
        ActivityState(ANativeActivity* act, void* saved, const size_t savedSize);

        ~ActivityState();

        static ActivityState* get();

        void reset();


        ANativeActivity* nativeActivity;
        ANativeWindow* nativeWindow;
        ALooper* looper;
        AInputQueue* inputQueue;
        AConfiguration* configuration;
        
        EGLDisplay display;

        void* savedState;
        size_t savedStateSize;

        std::mutex mutex;

        WindowEventHandler* eventHandler;

        bool fullscreen;
        bool mainDone;
        std::atomic<bool> terminated;
        std::atomic<bool> init;
        std::atomic<bool> updated;

        glm::uvec2 screenSize;
    };
}}

#endif
#endif