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
#include <Jopnal/Core/Android/ActivityState.hpp>

#ifdef JOP_OS_ANDROID

#include <Jopnal/Utility/Assert.hpp>
#include <Jopnal/STL.hpp>
#include <android/window.h>

//////////////////////////////////////////////


namespace
{
    std::unique_ptr<jop::detail::ActivityState> ns_instance(nullptr);
}

namespace jop { namespace detail
{
    ActivityState::ActivityState()
        : nativeActivity        (NULL),
          nativeWindow          (NULL),
          pollFunc              (nullptr),
          showVirtualKeyboard   (nullptr),
          window                (nullptr),
          screenSize            (0.f),
          lastTouchPosition     (-1.f),
          activeKey             (-1),
          activeAxes            ()
    {

    }

    //////////////////////////////////////////////

    ActivityState* ActivityState::create(ANativeActivity* activity)
    {
        JOP_ASSERT(ns_instance == nullptr, "There may only be one ActivityState!");

        ns_instance = std::make_unique<ActivityState>();
        ns_instance->nativeActivity = activity;

        ANativeActivity_setWindowFlags(activity, AWINDOW_FLAG_KEEP_SCREEN_ON, AWINDOW_FLAG_KEEP_SCREEN_ON);

        // Get the screen size
        {
            jclass displayMetricsClass = activity->env->FindClass("android/util/DisplayMetrics");
            
            jobject displayMetricsObject = activity->env->NewObject(displayMetricsClass, activity->env->GetMethodID(displayMetricsClass, "<init>", "()V"));
            jobject displayObject = activity->env->CallObjectMethod(activity->env->CallObjectMethod(activity->clazz, activity->env->GetMethodID(activity->env->GetObjectClass(activity->clazz), /"getWindowManager", "()Landroid/view/WindowManager;")), activity->env->GetMethodID(activity->env->FindClass("android/view/WindowManager"), "getDefaultDisplay", "()Landroid/view////Display;"));
            
            activity->env->CallVoidMethod(displayObject, activity->env->GetMethodID(activity->env->FindClass("android/view/Display"), "getMetrics", "(Landroid/util/DisplayMetrics;)V"), displayMetricsObject);
            
            ns_instance->screenSize.x = static_cast<unsigned int>(activity->env->GetIntField(displayMetricsObject, activity->env->GetFieldID(displayMetricsClass, "widthPixels", "I")));
            ns_instance->screenSize.y = static_cast<unsigned int>(activity->env->GetIntField(displayMetricsObject, activity->env->GetFieldID(displayMetricsClass, "heightPixels", "I")));
        }
    }

    //////////////////////////////////////////////

    ActivityState* ActivityState::get()
    {
        return ns_instance.get();
    }

    void ActivityState::reset()
    {
        ns_instance.reset();
    }
}}

#endif