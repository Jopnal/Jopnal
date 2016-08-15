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

#include <Jopnal/Core/DebugHandler.hpp>
#include <Jopnal/Utility/Assert.hpp>
#include <Jopnal/Utility/Thread.hpp>
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
        : nativeActivity            (NULL),
          nativeWindow              (NULL),
          pollFunc                  (nullptr),
          handleSurfaceCreation     (nullptr),
          handleSurfaceDestruction  (nullptr),
          window                    (nullptr),
          screenSize                (0.f),
          activeKey                 (-1),
          controllerPresent         (false),
          destroyRequested          (false)
    {
        for (int i = 0; i < sizeof(lastTouchPosition) / sizeof(lastTouchPosition[0]); ++i)
            lastTouchPosition[i] = glm::vec2(-1.f);

        for (int i = 0; i < sizeof(activeAxes) / sizeof(activeAxes[0]); ++i)
            activeAxes[i] = 0.f;

        for (int i = 0; i < sizeof(activeControllerButtons) / sizeof(activeControllerButtons[0]); ++i)
            activeControllerButtons[i] = false;
    }

    ActivityState::~ActivityState()
    {
        Thread::detachJavaThread(nativeActivity->vm);
    }

    //////////////////////////////////////////////

    ActivityState* ActivityState::create(ANativeActivity* activity)
    {
        JOP_ASSERT(ns_instance == nullptr, "There must only be one ActivityState!");

        ns_instance.reset(new ActivityState);
        ns_instance->nativeActivity = activity;

        ANativeActivity_setWindowFlags(activity, AWINDOW_FLAG_KEEP_SCREEN_ON, AWINDOW_FLAG_KEEP_SCREEN_ON);

        Thread::attachJavaThread(activity->vm, activity->env);

        // Get the screen size
        {
            auto env = Thread::getCurrentJavaEnv();

            if (!env)
            {
                JOP_DEBUG_ERROR("No current java environment, function \"" << __func__ << "\"");
                return get();
            }

            jclass activityClass = env->GetObjectClass(activity->clazz);

            jclass displayMetricsClass = env->FindClass("android/util/DisplayMetrics");
            jmethodID displayMetricsInit = env->GetMethodID(displayMetricsClass, "<init>", "()V");
            jobject displayMetricsObject = env->NewObject(displayMetricsClass, displayMetricsInit);

            jmethodID getWindowManagerMethod = env->GetMethodID(activityClass, "getWindowManager", "()Landroid/view/WindowManager;");
            jobject windowManagerObject = env->CallObjectMethod(activity->clazz, getWindowManagerMethod);

            jclass windowManagerClass = env->FindClass("android/view/WindowManager");
            jmethodID getDefaultDisplayMethod = env->GetMethodID(windowManagerClass, "getDefaultDisplay", "()Landroid/view/Display;");
            jobject displayObject = env->CallObjectMethod(windowManagerObject, getDefaultDisplayMethod);

            jclass displayClass = env->FindClass("android/view/Display");
            jmethodID getMetricsMethod = env->GetMethodID(displayClass, "getMetrics", "(Landroid/util/DisplayMetrics;)V");
            env->CallVoidMethod(displayObject, getMetricsMethod, displayMetricsObject);

            jfieldID pixelsWidth = env->GetFieldID(displayMetricsClass, "widthPixels", "I");
            jfieldID pixelsHeight = env->GetFieldID(displayMetricsClass, "heightPixels", "I");

            ns_instance->screenSize.x = env->GetIntField(displayMetricsObject, pixelsWidth);
            ns_instance->screenSize.y = env->GetIntField(displayMetricsObject, pixelsHeight);

            env->DeleteLocalRef(activityClass);
            env->DeleteLocalRef(displayMetricsObject);
            env->DeleteLocalRef(windowManagerObject);
            env->DeleteLocalRef(windowManagerClass);
            env->DeleteLocalRef(displayObject);
            env->DeleteLocalRef(displayClass);
        }

        return get();
    }

    //////////////////////////////////////////////

    ActivityState* ActivityState::get()
    {
        return ns_instance.get();
    }

    //////////////////////////////////////////////

    void ActivityState::reset()
    {
        ns_instance.reset();
    }

    //////////////////////////////////////////////

    unsigned int ActivityState::getAPILevel()
    {
        auto env = Thread::getCurrentJavaEnv();

        if (!env)
        {
            JOP_DEBUG_ERROR("No current java environment, function \"" << __func__ << "\"");
            return 0;
        }

        static unsigned int api = 0;

        if (!api)
        {
            jclass versionClass = env->FindClass("android/os/Build$VERSION");
            if (!versionClass)
            {
                JOP_DEBUG_ERROR("Failed to get Android API level");
                return 0;
            }

            jfieldID IDField = env->GetStaticFieldID(versionClass, "SDK_INT", "I");
            if (!IDField)
            {
                JOP_DEBUG_ERROR("Failed to get Android API level");

                env->DeleteLocalRef(versionClass);
                return 0;
            }

            api = static_cast<unsigned int>(env->GetStaticIntField(versionClass, IDField));

            env->DeleteLocalRef(versionClass);
        }

        return api;
    }
}}

#endif