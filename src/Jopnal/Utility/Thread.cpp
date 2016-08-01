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

    #include <Jopnal/Utility/Thread.hpp>

    #include <Jopnal/Core/DebugHandler.hpp>
    #include <Jopnal/Core/Android/ActivityState.hpp>

    #ifdef JOP_OS_ANDROID

        #include <jni.h>
        #include <unordered_map>

    #endif

#endif

#ifdef JOP_OS_WINDOWS
    #include <Jopnal/Utility/Win32/ThreadImpl.hpp>
#else
    #include <Jopnal/Utility/Unix/ThreadImpl.hpp>
#endif

//////////////////////////////////////////////


#ifdef JOP_OS_ANDROID

namespace
{
    std::unordered_map<std::thread::id, _JNIEnv*> ns_javaEnvs;
}

#endif

namespace jop
{
    Thread::Thread()
        : m_thread()
    {}

    Thread::Thread(Thread&& other)
        : m_thread(std::move(other.m_thread))
    {}

    Thread& Thread::operator=(Thread&& other)
    {
        m_thread = std::move(other.m_thread);
        return *this;
    }

    Thread::~Thread()
    {
        join();
    }

    //////////////////////////////////////////////

    void Thread::join()
    {
        if (isJoinable())
            m_thread.join();
    }

    //////////////////////////////////////////////

    bool Thread::isJoinable() const
    {
        return m_thread.joinable();
    }

    //////////////////////////////////////////////

    void Thread::detach()
    {
        if (isJoinable())
            m_thread.detach();
    }

    //////////////////////////////////////////////

    bool Thread::setPriority(const Priority priority)
    {
        return detail::ThreadDetail::setPriority(m_thread, static_cast<unsigned int>(priority));
    }

    //////////////////////////////////////////////

    void Thread::terminate()
    {
        detail::ThreadDetail::terminate(m_thread);
    }

    //////////////////////////////////////////////

    std::thread::id Thread::getId() const
    {
        return m_thread.get_id();
    }

    //////////////////////////////////////////////

    void Thread::attachJavaThread(void* vm)
    {
    #ifdef JOP_OS_ANDROID

        if (!vm)
            vm = detail::ActivityState::get()->nativeActivity->vm;

        JNIEnv* env;

        JavaVMAttachArgs args;
        args.version = JNI_VERSION_1_6;
        args.name = "NativeThread";
        args.group = NULL;
        jint res = static_cast<JavaVM*>(vm)->AttachCurrentThread(&env, &args);

        if (res == JNI_ERR)
        {
            JOP_DEBUG_ERROR("Failed to attach thread \"" << std::this_thread::get_id() << "\" to JNI");
            return;
        }

        ns_javaEnvs[std::this_thread::get_id()] = env;

    #else

        vm;

    #endif
    }

    //////////////////////////////////////////////

    void Thread::detachJavaThread(void* vm)
    {
    #ifdef JOP_OS_ANDROID

        static_cast<JavaVM*>(vm)->DetachCurrentThread();

        ns_javaEnvs.erase(std::this_thread::get_id());

    #else

        vm;

    #endif
    }

    //////////////////////////////////////////////

    _JNIEnv* Thread::getCurrentJavaEnv()
    {
    #ifdef JOP_OS_ANDROID

        auto itr = ns_javaEnvs.find(std::this_thread::get_id());

        if (itr != ns_javaEnvs.end())
            return itr->second;

    #endif

        return nullptr;
    }
}