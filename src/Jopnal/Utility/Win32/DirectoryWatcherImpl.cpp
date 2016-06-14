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

#ifdef JOP_OS_WINDOWS

#include <Jopnal/Utility/Win32/DirectoryWatcherImpl.hpp>

//////////////////////////////////////////////


namespace jop { namespace detail
{
    DirectoryWatcherImpl::DirectoryWatcherImpl(const std::string& directory, DirectoryWatcher::EventCallback callback)
        : m_fileHandle  (nullptr),
          m_thread      (),
          m_directory   (directory),
          m_lastEvent   (),
          m_callback    (callback),
          m_active      (true),
          m_shouldClose (false),
          m_error       (false)
    {
        std::replace(m_directory.begin(), m_directory.end(), '/', '\\');
    
        m_thread = Thread(&DirectoryWatcherImpl::watch, this);
        m_thread.setPriority(Thread::Priority::Lowest);
    }
    
    DirectoryWatcherImpl::~DirectoryWatcherImpl()
    {
        m_shouldClose = true;
        m_active = false;
    
        if (m_fileHandle)
            CancelIoEx(m_fileHandle, NULL);
    
        m_thread.join();
    }
    
    //////////////////////////////////////////////
    
    void DirectoryWatcherImpl::setActive(const bool active)
    {
        if (!active && m_active && m_fileHandle)
        {
            m_active = false;
            CancelIoEx(m_fileHandle, NULL);
        }
    
        m_active = active;
    }
    
    //////////////////////////////////////////////
    
    bool DirectoryWatcherImpl::isActive() const
    {
        return m_active;
    }
    
    //////////////////////////////////////////////
    
    bool DirectoryWatcherImpl::hasError() const
    {
        return m_error;
    }
    
    //////////////////////////////////////////////
    
    void DirectoryWatcherImpl::watch()
    {
        m_fileHandle = CreateFileA(m_directory.c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
            
        if (m_fileHandle == INVALID_HANDLE_VALUE)
        {
            JOP_DEBUG_ERROR("Failed to start directory watcher, directory: " << m_directory);
            m_active = false;
            m_error = true;
            m_fileHandle = nullptr;
            return;
        }
    
        FILE_NOTIFY_INFORMATION fileNotifyInfo[4];
        DWORD bytesReturned = sizeof(FILE_NOTIFY_INFORMATION);
    
        while (!m_shouldClose)
        {
            while (!m_active)
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                continue;
            }
    
            if (ReadDirectoryChangesW(m_fileHandle,
                &fileNotifyInfo,
                sizeof(fileNotifyInfo),
                TRUE,
                FILE_NOTIFY_CHANGE_LAST_WRITE,
                &bytesReturned,
                NULL,
                NULL))
            {
                if (fileNotifyInfo[0].Action == FILE_ACTION_MODIFIED)
                {
                    DirectoryWatcher::Info info;
    
                    info.filename = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(std::wstring(fileNotifyInfo[0].FileName, fileNotifyInfo[0].FileNameLength / sizeof(wchar_t)));
    
                    if (!(info == m_lastEvent))
                    {
                        m_lastEvent = info;

                        m_callback(m_lastEvent);
                    }
                }
            }
        }
    }
}}

#endif