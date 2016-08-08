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

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Window/Keyboard.hpp>

    #include <Jopnal/Core/Android/ActivityState.hpp>
    #include <Jopnal/Core/Engine.hpp>
    #include <Jopnal/Window/InputEnumsImpl.hpp>

    #ifdef JOP_OS_DESKTOP
        #include <GLFW/glfw3.h>
    #endif

#endif

//////////////////////////////////////////////


namespace
{
    jop::Window* ns_windowRef = nullptr;

    bool validateWindowRef()
    {
        if (ns_windowRef == nullptr)
            ns_windowRef = &jop::Engine::getMainWindow();

        return ns_windowRef != nullptr;
    }
}

namespace jop
{
    std::string Keyboard::getKeyName(const int scanCode)
    {
    #ifdef JOP_OS_DESKTOP
        return std::string(glfwGetKeyName(GLFW_KEY_UNKNOWN, scanCode));
    #else
        return std::string("UNKNOWN");
    #endif
    }

    //////////////////////////////////////////////

    bool Keyboard::isKeyDown(const Key key)
    {
        using namespace Input;

        if (validateWindowRef())
        {
        #if defined(JOP_OS_DESKTOP)
            return glfwGetKey(ns_windowRef->getLibraryHandle(), getGlKey(key)) == GLFW_PRESS;

        #elif defined(JOP_OS_ANDROID)

            return detail::ActivityState::get()->activeKey == key;

        #endif
        }
        return false;
    }

    //////////////////////////////////////////////

    void Keyboard::showVirtualKeyboard(const bool show)
    {
    #ifdef JOP_OS_ANDROID

        auto state = detail::ActivityState::get();
        auto env = Thread::getCurrentJavaEnv();

        if (!env)
        {
            JOP_DEBUG_ERROR("No current java environment, function \"" << __func__ << "\"");
            return;
        }

        jint flags = 0;

        jclass classNativeActivity = env->GetObjectClass(state->nativeActivity->clazz);
        jclass ClassContext = env->FindClass("android/content/Context");
        jfieldID fieldInput = env->GetStaticFieldID(ClassContext,
                                                    "INPUT_METHOD_SERVICE", "Ljava/lang/String;");
        jobject inputService = env->GetStaticObjectField(ClassContext,
                                                         fieldInput);
        env->DeleteLocalRef(ClassContext);
        jclass classInputManager =
            env->FindClass("android/view/inputmethod/InputMethodManager");
        jmethodID systemService = env->GetMethodID(classNativeActivity,
                                                   "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
        jobject inputManager = env->CallObjectMethod(state->nativeActivity->clazz,
                                                     systemService, inputService);
        env->DeleteLocalRef(inputService);

        jmethodID javaWindow = env->GetMethodID(classNativeActivity,
                                                "getWindow", "()Landroid/view/Window;");
        jobject javaWindowObject = env->CallObjectMethod(state->nativeActivity->clazz, javaWindow);
        jclass classWindow = env->FindClass("android/view/Window");
        jmethodID decorView = env->GetMethodID(classWindow,
                                               "getDecorView", "()Landroid/view/View;");
        jobject decorViewObject = env->CallObjectMethod(javaWindowObject, decorView);
        env->DeleteLocalRef(javaWindowObject);
        env->DeleteLocalRef(classWindow);

        if (show)
        {
            jmethodID MethodShowSoftInput = env->GetMethodID(classInputManager,
                                                             "showSoftInput", "(Landroid/view/View;I)Z");
            jboolean result = env->CallBooleanMethod(inputManager,
                                                     MethodShowSoftInput, decorViewObject, flags);
        }
        else
        {
            jclass classView = env->FindClass("android/view/View");
            jmethodID javaWindowToken = env->GetMethodID(classView,
                                                         "getWindowToken", "()Landroid/os/IBinder;");
            jobject binder = env->CallObjectMethod(decorViewObject,
                                                   javaWindowToken);
            env->DeleteLocalRef(classView);


            jmethodID MethodHideSoftInput = env->GetMethodID(classInputManager,
                                                             "hideSoftInputFromWindow", "(Landroid/os/IBinder;I)Z");
            jboolean res = env->CallBooleanMethod(inputManager,
                                                  MethodHideSoftInput, binder, flags);
            env->DeleteLocalRef(binder);
        }

        env->DeleteLocalRef(classNativeActivity);
        env->DeleteLocalRef(classInputManager);
        env->DeleteLocalRef(decorViewObject);

    #else

        show;

    #endif
    }
}