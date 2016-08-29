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

        /// File change information
        ///
        struct JOP_API Info
        {
            std::string filename;   ///< The file that was changed
            DateTime datetime;      ///< Date and time

            /// \brief Equality operator
            ///
            /// \param right The other Info to compare against
            ///
            /// \return True if the infos match
            ///
            bool operator ==(const Info& right) const;
        };

        /// File change event callback
        ///
        typedef std::function<void(Info)> EventCallback;

    public:

        /// \brief Constructor
        ///
        /// Won't start observing any directory.
        ///
        DirectoryWatcher();

        /// \brief Overloaded constructor
        ///
        /// Calls start() with the given arguments.
        ///
        /// \param dir The directory to start watching
        /// \param callback The event callback
        ///
        DirectoryWatcher(const std::string& dir, EventCallback callback);

        /// \brief Destructor
        ///
        ~DirectoryWatcher();

        
        /// \brief Start watching a directory
        ///
        /// Calls stop() before opening the new directory.
        ///
        /// \param dir The directory to start watching
        /// \param callback The event callback
        ///
        /// \return True if the directory was opened successfully
        ///
        bool start(const std::string& dir, EventCallback callback);

        /// \brief Stop watching a directory
        ///
        void stop();

        /// \brief Set this watcher active/inactive
        ///
        /// \param active True to set active
        ///
        void setActive(const bool active);

        /// \brief Check if this watcher is active
        ///
        /// \return True if active
        ///
        bool isActive() const;

        /// \brief Check if this watcher is in an error condition
        ///
        /// \return True if this watcher has an error
        ///
        bool hasError() const;

    private:

        std::unique_ptr<detail::DirectoryWatcherImpl> m_impl;   ///< implementation
    };
}

/// \class jop::DirectoryWatcher
/// \ingroup utility

#endif