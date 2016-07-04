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

	#include <Jopnal/Utility/DirectoryWatcher.hpp>

    #include <Jopnal/STL.hpp>

#endif

#ifdef JOP_OS_WINDOWS

    #include <Jopnal/Utility/Win32/DirectoryWatcherImpl.hpp>

#else

    namespace jop { namespace detail
    {
        class DirectoryWatcherImpl
        {
        public:
    
            DirectoryWatcherImpl(const std::string&, DirectoryWatcher::EventCallback)
            {}
        };
    }}

#endif

//////////////////////////////////////////////


namespace jop
{
    DirectoryWatcher::DirectoryWatcher()
        : m_impl(nullptr)
    {}

    DirectoryWatcher::DirectoryWatcher(const std::string& dir, EventCallback callback)
        : m_impl(nullptr)
    {
        start(dir, callback);
    }

    DirectoryWatcher::~DirectoryWatcher()
    {}

    //////////////////////////////////////////////

    bool DirectoryWatcher::start(const std::string& dir, EventCallback callback)
    {
        m_impl = std::make_unique<detail::DirectoryWatcherImpl>(dir, callback);

        return !hasError();
    }

    //////////////////////////////////////////////

    void DirectoryWatcher::stop()
    {
        m_impl.reset();
    }

    //////////////////////////////////////////////

    void DirectoryWatcher::setActive(const bool active)
    {
    #ifdef JOP_OS_WINDOWS

        if (m_impl)
            m_impl->setActive(active);

    #endif
    }

    //////////////////////////////////////////////

    bool DirectoryWatcher::isActive() const
    {
    #ifdef JOP_OS_WINDOWS
        return m_impl && m_impl->isActive();
    #else
        return false;
    #endif
    }

    //////////////////////////////////////////////

    bool DirectoryWatcher::hasError() const
    {
    #ifdef JOP_OS_WINDOWS
        return !m_impl || m_impl->hasError();
    #else
        return true;
    #endif
    }
}