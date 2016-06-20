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
#include <Jopnal/Header.hpp>
#include <android/native_activity.h>
#include <android/log.h>
#include <jni.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>

//////////////////////////////////////////////


void log(const char* str)
{
    __android_log_write(ANDROID_LOG_INFO, "jopnal-activity", str);
}

const char* getLibraryName(JNIEnv* env, jobject& objectActivityInfo)
{
    
}

void* loadLibrary(const char* name, JNIEnv* env, jobject& ObjectActivityInfo)
{

}

void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize)
{
    jclass ClassNativeActivity = activity->env->GetObjectClass(activity->clazz);
    jclass ClassPackageManager = activity->env->FindClass("android/content/pm/PackageManager");

    jobject ObjectActivityInfo = activity->env->CallObjectMethod(activity->env->CallObjectMethod(activity->clazz, activity->env->GetMethodID(ClassNativeActivity, "getPackageManager", "()Landroid/content/pm/PackageManager;")), activity->env->GetMethodID(ClassPackageManager, "getActivityInfo", "(Landroid/content/ComponentName;I)Landroid/content/pm/ActivityInfo;"), activity->env->CallObjectMethod(activity->env->CallObjectMethod(activity->clazz, activity->env->GetMethodID(ClassNativeActivity, "getIntent", "()Landroid/content/Intent;")), activity->env->GetMethodID(activity->env->FindClass("android/content/Intent"), "getComponent", "()Landroid/content/ComponentName;")), activity->env->GetStaticIntField(ClassPackageManager, activity->env->GetStaticFieldID(ClassPackageManager, "GET_META_DATA", "I")));

    // Libraries must be loaded in reverse dependency order

    #ifdef STL_LIBRARY

        #define JOP_STRINGIZE(token) JOP_STRINGIZEX(token)
        #define JOP_STRINGIZEX(token) #token

        loadLibrary(JOP_STRINGIZE(STL_LIBRARY), activity->env, ObjectActivityInfo);

        #undef JOP_STRINGIZE
        #undef JOP_STRINGIZEX

    #endif

    loadLibrary("openal", activity->env, ObjectActivityInfo);

    #ifndef JOP_STATIC
        #ifdef _DEBUG
            loadLibrary("jopnald", activity->env, ObjectActivityInfo);
        #else
            loadLibrary("jopnal", activity->env, ObjectActivityInfo);
        #endif
    #endif

    typedef void (*OnCreateFunc)(ANativeActivity*, void*, size_t);

    OnCreateFunc onCreate = (OnCreateFunc)dlsym(loadLibrary(getLibraryName(activity->env, ObjectActivityInfo), activity->env, ObjectActivityInfo), "ANativeActivity_onCreate");

    if (!onCreate)
    {
        log("[JOPNAL ACTIVITY] Couldn't find entry point ANativeActivity_onCreate");
        exit(1);
    }

    onCreate(activity, savedState, savedStateSize);
}