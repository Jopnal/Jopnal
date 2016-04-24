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

#ifndef JOP_DIRECTORYWATCHERIMPL_HPP
#define JOP_DIRECTORYWATCHERIMPL_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Utility/DirectoryWatcher.hpp>
#include <Jopnal/Utility/Thread.hpp>
#include <atomic>

/////////////////////////////////////////////////////////////////////////////////////////////////


namespace jop { namespace detail
{
    class DirectoryWatcherImpl
    {
    private:
    
        JOP_DISALLOW_COPY_MOVE(DirectoryWatcherImpl);
    
    public:
    
        DirectoryWatcherImpl(const std::string& directory, DirectoryWatcher::EventCallback callback);
    
        ~DirectoryWatcherImpl();
    
    
        void setActive(const bool active);
    
        bool isActive() const;
    
        bool hasError() const;
    
    private:
    
        void watch();
    
    
        void* m_fileHandle;
        Thread m_thread;
        std::string m_directory;
        DirectoryWatcher::Info m_lastEvent;
        DirectoryWatcher::EventCallback m_callback;
        std::atomic<bool> m_active;
        std::atomic<bool> m_shouldClose;
        std::atomic<bool> m_error;
    };
}}

#endif