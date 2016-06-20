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

//////////////////////////////////////////////


namespace
{
    jop::detail::ActivityState* ns_instance = nullptr;

    jop::detail::ActivityState* getState(ANativeActivity* activity)
    {
        return static_cast<jop::detail::ActivityState*>(activity->instance);
    }

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

        jclass classView = activity->env->FindClass("android/view/View");

        jint flags = 0;

        if (apiLevel >= 19)
            flags |= activity->env->GetStaticIntField(classView, activity->env->GetStaticFieldID(classView, "SYSTEM_UI_FLAG_IMMERSIVE_STICKY", "I"));

        activity->env->CallVoidMethod(activity->env->CallObjectMethod(activity->env->CallObjectMethod(activity->clazz, activity->env->GetMethodID(activity->env->GetObjectClass(activity->clazz), "getWindow", "()Landroid/view/Window;")), activity->env->GetMethodID(activity->env->FindClass("android/view/Window"), "getDecorView", "()Landroid/view/View;")), activity->env->GetMethodID(classView, "setSystemUiVisibility", "(I)V"), flags);
    }

    // Callbacks
    void onStart(ANativeActivity*)
    {}

    void onResume(ANativeActivity* activity)
    {
        auto state = getState(activity);

        std::lock_guard<decltype(state->mutex)> lock(state->mutex);

        if (state->fullscreen)
            goFullscreen(activity);

        state->eventHandler->mouseEntered();
    }

    void onPause(ANativeActivity* activity)
    {
        auto state = getState(activity);

        std::lock_guard<decltype(state->mutex)> lock(state->mutex);

        state->eventHandler->mouseLeft();
    }

    void onStop(ANativeActivity*)
    {}

    void onDestroy(ANativeActivity* activity)
    {
        auto state = getState(activity);

        {
            std::lock_guard<decltype(state->mutex)> lock(state->mutex);

            if (!state->mainDone)
                state->eventHandler->closed();
        }

        while (!state->terminated.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

        eglTerminate(state->display);

        state->reset();
    }

    void onNativeWindowCreated(ANativeActivity* activity, ANativeWindow* window)
    {
        auto state = getState(activity);

        std::lock_guard<decltype(state->mutex)> lock(state->mutex);

        state->nativeWindow = window;

        state->eventHandler->gainedFocus();

        state->updated.store(false);
        while (!state->updated.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    void onNativeWindowDestroyed(ANativeActivity* activity, ANativeWindow* window)
    {
        auto state = getState(activity);

        std::lock_guard<decltype(state->mutex)> lock(state->mutex);

        state->nativeWindow = nullptr;

        state->eventHandler->lostFocus();

        state->updated.store(false);
        while (!state->updated.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    void onNativeWindowRedrawNeeded(ANativeActivity*, ANativeWindow*)
    {}

    void onNativeWindowResized(ANativeActivity*, ANativeWindow*)
    {}

    void onInputQueueCreated(ANativeActivity* activity, AInputQueue* queue)
    {
        auto state = getState(activity);

        std::lock_guard<decltype(state->mutex)> lock(state->mutex);

        //AInputQueue_attachLooper(queue, state->looper, 1, state->processEvent, nullptr);
        state->inputQueue = queue;
    }

    void onInputQueueDestroyed(ANativeActivity* activity, AInputQueue* queue)
    {
        auto state = getState(activity);

        std::lock_guard<decltype(state->mutex)> lock(state->mutex);

        state->inputQueue = nullptr;
        AInputQueue_detachLooper(queue);
    }

    void onWindowFocusChanged(ANativeActivity*, int)
    {}

    void onContentRectChanged(ANativeActivity* activity, const ARect* rect)
    {
        auto state = getState(activity);

        std::lock_guard<decltype(state->mutex)> lock(state->mutex);

        if (state->nativeWindow)
            state->eventHandler->resized(static_cast<unsigned int>(ANativeWindow_getWidth(state->nativeWindow)),
                                         static_cast<unsigned int>(ANativeWindow_getHeight(state->nativeWindow)));
    }

    void onConfigurationChanged(ANativeActivity*)
    {}

    void* onSaveInstanceState(ANativeActivity*, size_t* outLen)
    {
        return (void*)(*outLen = 0);
    }

    void onLowMemory(ANativeActivity*)
    {}
}

namespace jop { namespace detail
{
    ActivityState::ActivityState(ANativeActivity* act, void* saved, const size_t savedSize)
        : nativeActivity    (act),
          nativeWindow      (nullptr),
          looper            (nullptr),
          inputQueue        (nullptr),
          configuration     (nullptr),
          display           (eglGetDisplay(EGL_DEFAULT_DISPLAY)),
          savedState        (nullptr),
          savedStateSize    (savedSize),
          mutex             (),
          eventHandler      (nullptr),
          fullscreen        (false),
          mainDone          (false),
          terminated        (false),
          init              (false),
          updated           (false),
          screenSize        ()
    {
        JOP_ASSERT(ns_instance == nullptr, "There may only be one ActivityState!");

        if (saved)
        {
            savedState = malloc(savedSize);
            memcpy(savedState, saved, savedSize);
        }

        auto cb = nativeActivity->callbacks;

        cb->onStart     = onStart;
        cb->onResume    = onResume;
        cb->onPause     = onPause;
        cb->onStop      = onStop;
        cb->onDestroy   = onDestroy;

        cb->onNativeWindowCreated       = onNativeWindowCreated;
        cb->onNativeWindowDestroyed     = onNativeWindowDestroyed;

        cb->onNativeWindowRedrawNeeded  = onNativeWindowRedrawNeeded;
        cb->onNativeWindowResized       = onNativeWindowResized;

        cb->onInputQueueCreated         = onInputQueueCreated;
        cb->onInputQueueDestroyed       = onInputQueueDestroyed;

        cb->onWindowFocusChanged        = onWindowFocusChanged;
        cb->onContentRectChanged        = onContentRectChanged;

        cb->onConfigurationChanged      = onConfigurationChanged;
        cb->onSaveInstanceState         = onSaveInstanceState;
        cb->onLowMemory                 = onLowMemory;

        ANativeActivity_setWindowFlags(act, AWINDOW_FLAG_KEEP_SCREEN_ON, AWINDOW_FLAG_KEEP_SCREEN_ON);

        eglInitialize(display, NULL, NULL);

        // Get the screen size
        {
            jclass classDisplayMetrics = act->env->FindClass("android/util/DisplayMetrics");

            jobject objectDisplayMetrics = act->env->NewObject(classDisplayMetrics, act->env->GetMethodID(classDisplayMetrics, "<init>", "()V"));
            jobject objectDisplay = act->env->CallObjectMethod(act->env->CallObjectMethod(act->clazz, act->env->GetMethodID(act->env->GetObjectClass(act->clazz), "getWindowManager", "()Landroid/view/WindowManager;")), act->env->GetMethodID(act->env->FindClass("android/view/WindowManager"), "getDefaultDisplay", "()Landroid/view/Display;"));

            act->env->CallVoidMethod(objectDisplay, act->env->GetMethodID(act->env->FindClass("android/view/Display"), "getMetrics", "(Landroid/util/DisplayMetrics;)V"), objectDisplayMetrics);

            screenSize.x = static_cast<unsigned int>(act->env->GetIntField(objectDisplayMetrics, act->env->GetFieldID(classDisplayMetrics, "widthPixels", "I")));
            screenSize.y = static_cast<unsigned int>(act->env->GetIntField(objectDisplayMetrics, act->env->GetFieldID(classDisplayMetrics, "heightPixels", "I")));
        }
    }

    ActivityState::~ActivityState()
    {
        reset();
    }

    ActivityState* ActivityState::get()
    {
        return ns_instance;
    }

    void ActivityState::reset()
    {
        delete ns_instance;
        ns_instance = nullptr;
    }
}}

#endif