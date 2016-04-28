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

#ifndef JOP_DIRECTORYWATCHER_HPP
#define JOP_DIRECTORYWATCHER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Utility/DateTime.hpp>
#include <memory>
#include <functional>

//////////////////////////////////////////////


namespace jop
{
    namespace detail
    {
        class DirectoryWatcherImpl;
    }

    class JOP_API DirectoryWatcher 
    {
    private:

        JOP_DISALLOW_COPY_MOVE(DirectoryWatcher);

    public:

        struct Info
        {
            std::string filename;
            DateTime datetime;

            bool operator ==(const Info& right) const
            {
                return (datetime == right.datetime) &&
                       (filename == right.filename);
            }
        };

        typedef std::function<void(Info)> EventCallback;

    public:

        DirectoryWatcher();

        DirectoryWatcher(const std::string& dir, EventCallback callback);

        ~DirectoryWatcher();

        
        bool start(const std::string& dir, EventCallback callback);

        void stop();

        void setActive(const bool active);

        bool isActive() const;

        bool hasError() const;

    private:

        std::unique_ptr<detail::DirectoryWatcherImpl> m_impl;
    };
}

#endif