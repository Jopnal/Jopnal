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


#define LOG_ERROR(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "jopnal-activity", __VA_ARGS__))
#define LOG_INFO(...) ((void)__android_log_print(ANDROID_LOG_INFO, "jopnal-activity", __VA_ARGS__))

const char* getLibraryName(JNIEnv* env, jobject& objectActivityInfo)
{
    static char name[256];

    jstring valueString = (jstring)env->CallObjectMethod(env->GetObjectField(objectActivityInfo, env->GetFieldID(env->FindClass("android/content/pm/ActivityInfo"), "metaData", "Landroid/os/Bundle;")), env->GetMethodID(env->FindClass("android/os/Bundle"), "getString", "(Ljava/lang/String;)Ljava/lang/String;"), env->NewStringUTF("jopnal.app.lib_name"));

    if (!valueString)
    {
        LOG_ERROR("\"jopnal.app.lib_name\" not found in AndroidManifest.xml");
        exit(1);
    }

    const char* appName = env->GetStringUTFChars(valueString, NULL);
    const jsize length = env->GetStringUTFLength(valueString);

    if (length >= 256)
    {
        LOG_ERROR("The value of \"jopnal.app.lib_name\" is longer than 256 characters");
        exit(1);
    }

    strncpy(name, appName, length);
    name[length] = '\0';

    env->ReleaseStringUTFChars(valueString, appName);

    return name;
}

void* loadLibrary(const char* name, JNIEnv* env, jobject& activityInfo)
{
    jclass systemClass = env->FindClass("java/lang/System");
    jclass fileClass = env->FindClass("java/io/File");

    jstring javaLibPath = static_cast<jstring>(env->CallObjectMethod(env->NewObject(fileClass, env->GetMethodID(fileClass, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V"), env->GetObjectField(env->GetObjectField(activityInfo, env->GetFieldID(env->FindClass("android/content/pm/ActivityInfo"), "applicationInfo", "Landroid/content/pm/ApplicationInfo;")), env->GetFieldID(env->FindClass("android/content/pm/ApplicationInfo"), "nativeLibraryDir", "Ljava/lang/String;")), env->CallStaticObjectMethod(systemClass, env->GetStaticMethodID(systemClass, "mapLibraryName", "(Ljava/lang/String;)Ljava/lang/String;"), env->NewStringUTF(name))), env->GetMethodID(fileClass, "getPath", "()Ljava/lang/String;")));

    const char* libPath = env->GetStringUTFChars(javaLibPath, NULL);
    void * handle = dlopen(libPath, RTLD_NOW | RTLD_GLOBAL);

    if (!handle)
    {
        LOG_ERROR("Failed to load library \"%s\": %s", libPath, dlerror());
        exit(1);
    }
    
    LOG_INFO("Successfully loaded library \"%s\"", libPath);

    env->ReleaseStringUTFChars(javaLibPath, libPath);

    return handle;
}

void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize)
{
    LOG_INFO("Entered jopnal-activity");
    
    jclass nativeActivityClass = activity->env->GetObjectClass(activity->clazz);
    jclass packageManagerClass = activity->env->FindClass("android/content/pm/PackageManager");

    jobject activityInfo = activity->env->CallObjectMethod(activity->env->CallObjectMethod(activity->clazz, activity->env->GetMethodID(nativeActivityClass, "getPackageManager", "()Landroid/content/pm/PackageManager;")), activity->env->GetMethodID(packageManagerClass, "getActivityInfo", "(Landroid/content/ComponentName;I)Landroid/content/pm/ActivityInfo;"), activity->env->CallObjectMethod(activity->env->CallObjectMethod(activity->clazz, activity->env->GetMethodID(nativeActivityClass, "getIntent", "()Landroid/content/Intent;")), activity->env->GetMethodID(activity->env->FindClass("android/content/Intent"), "getComponent", "()Landroid/content/ComponentName;")), activity->env->GetStaticIntField(packageManagerClass, activity->env->GetStaticFieldID(packageManagerClass, "GET_META_DATA", "I")));

    // Libraries must be loaded in reverse dependency order

    #ifdef STL_LIBRARY

        #define JOP_STRINGIZE(token) JOP_STRINGIZEX(token)
        #define JOP_STRINGIZEX(token) #token

        loadLibrary(JOP_STRINGIZE(STL_LIBRARY), activity->env, activityInfo);

        #undef JOP_STRINGIZE
        #undef JOP_STRINGIZEX

    #endif

    loadLibrary("openal", activity->env, activityInfo);

    #ifndef JOP_STATIC
        #ifdef _DEBUG
            loadLibrary("jopnald", activity->env, activityInfo);
        #else
            loadLibrary("jopnal", activity->env, activityInfo);
        #endif
    #endif

    typedef void (*OnCreateFunc)(ANativeActivity*, void*, size_t);

    OnCreateFunc onCreate = (OnCreateFunc)dlsym(loadLibrary(getLibraryName(activity->env, activityInfo), activity->env, activityInfo), "ANativeActivity_onCreate");

    if (!onCreate)
    {
        LOG_ERROR("Couldn't find entry point ANativeActivity_onCreate");
        exit(1);
    }

    onCreate(activity, savedState, savedStateSize);
}