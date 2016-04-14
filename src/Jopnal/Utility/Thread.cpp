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
#include <Jopnal/Precompiled.hpp>

#ifdef JOP_OS_WINDOWS
    #include <Jopnal/Utility/Win32/ThreadImpl.hpp>
#endif

//////////////////////////////////////////////


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

    bool Thread::setPriority(const unsigned int priority)
    {
        return detail::ThreadDetail::setPriority(m_thread, priority);
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
}