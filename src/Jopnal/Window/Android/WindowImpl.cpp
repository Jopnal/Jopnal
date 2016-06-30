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

#include <Jopnal/Window/Android/WindowImpl.hpp>

#ifdef JOP_OS_ANDROID

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Core/Android/ActivityState.hpp>

#endif

//////////////////////////////////////////////


namespace
{
    void goFullscreen(ANativeActivity* activity)
    {
        int apiLevel = 0;

        {
            jclass versionClass = activity->env->FindClass("android/os/Build$VERSION");

            if (versionClass)
            {
                jfieldID sdkIntFieldID = activity->env->GetStaticFieldID(versionClass, "SDK_INT", "I");

                if (sdkIntFieldID)
                    apiLevel = activity->env->GetStaticIntField(versionClass, sdkIntFieldID);
            }
        }

        jclass viewClass = activity->env->FindClass("android/view/View");

        jint flags = 0;

        if (apiLevel >= 19)
            flags |= activity->env->GetStaticIntField(viewClass, activity->env->GetStaticFieldID(viewClass, "SYSTEM_UI_FLAG_IMMERSIVE_STICKY", "I"));
        
        activity->env->CallVoidMethod(activity->env->CallObjectMethod(activity->env->CallObjectMethod(activity->clazz, activity->env->GetMethodID(activity->env->GetObjectClass(activity->clazz), "getWindow", "()Landroid/view/Window;")), activity->env->GetMethodID(activity->env->FindClass("android/view/Window"), "getDecorView", "()Landroid/view/View;")), activity->env->GetMethodID(viewClass, "setSystemUiVisibility", "(I)V"), flags);
    }
}

namespace jop { namespace detail
{
    WindowImpl::WindowImpl(const Window::Settings& settings, Window& windowPtr)
    {
    }

    WindowImpl::~WindowImpl()
    {
        
    }

    //////////////////////////////////////////////

    void WindowImpl::swapBuffers()
    {
        
    }

    //////////////////////////////////////////////

    WindowLibHandle WindowImpl::getLibraryHandle()
    {
        return getNativeHandle();
    }

    //////////////////////////////////////////////

    WindowHandle WindowImpl::getNativeHandle()
    {
        auto state = ActivityState::get();
        std::lock_guard<decltype(state->mutex)> lock(state->mutex);

        return state->nativeWindow;
    }

    //////////////////////////////////////////////

    void WindowImpl::pollEvents()
    {
        
    }

    //////////////////////////////////////////////

    void WindowImpl::setMouseMode(const Mouse::Mode)
    {}

    //////////////////////////////////////////////

    void WindowImpl::setPosition(const int, const int)
    {}

    //////////////////////////////////////////////

    glm::ivec2 WindowImpl::getPosition() const
    {
        return glm::ivec2();
    }

    //////////////////////////////////////////////

    void WindowImpl::setSize(const int, const int)
    {}

    //////////////////////////////////////////////

    glm::uvec2 WindowImpl::getSize() const
    {
        return m_size;
    }

    //////////////////////////////////////////////

    Window* WindowImpl::getCurrentContextWindow()
    {
        return nullptr;
    }
}}

#endif