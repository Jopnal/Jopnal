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

#include <Jopnal/Window/Vibrator.hpp>
#include <Jopnal/Core/Android/ActivityState.hpp>

//////////////////////////////////////////////

    
namespace jop
{
    void Vibrator::vibrate(const int time_ms)
    {
#if defined (JOP_OS_ANDROID)

        // JVM & JNI
        JavaVM* vm = detail::ActivityState::get()->nativeActivity->vm;
        JNIEnv* jni;

        // Attach thread
        JavaVMAttachArgs args;
        args.version = JNI_VERSION_1_6;
        args.name = "NativeThread";
        args.group = NULL;

        jint result = vm->AttachCurrentThread(&jni, &args);

        if (result == JNI_ERR)
        {
            // Could not attach to thread
            return;
        }

        // Class information
        jclass nativeAct = jni->FindClass("android/app/NativeActivity");
        jclass context = jni->FindClass("android/content/Context");

        // Parameters for getSystemService
        jfieldID fieldID = jni->GetStaticFieldID(context, "VIBRATOR_SERVICE", "Ljava/lang/String;");
        jobject serviceStr = jni->GetStaticObjectField(context, fieldID);

        // getSystemService
        jmethodID getSysServ = jni->GetMethodID(nativeAct, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
        jobject vibrator = jni->CallObjectMethod(detail::ActivityState::get()->nativeActivity->clazz, getSysServ, serviceStr);

        // Get class & method
        jclass vibratorClass = jni->GetObjectClass(vibrator);
        jmethodID vibrate = jni->GetMethodID(vibratorClass, "vibrate", "(J)V");

        // Vibrate!
        jlong time = static_cast<jlong>(time_ms);
        jni->CallVoidMethod(vibrator, vibrate, time);

        // Free references
        jni->DeleteLocalRef(nativeAct);
        jni->DeleteLocalRef(context);
        jni->DeleteLocalRef(serviceStr);
        jni->DeleteLocalRef(vibrator);
        jni->DeleteLocalRef(vibratorClass);
        
        vm->DetachCurrentThread();
        
#endif
    }
}

