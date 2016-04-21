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

#ifndef JOP_CLOCK_HPP
#define JOP_CLOCK_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <chrono>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Clock
    {
    public:

        /// \brief Class to store a time value
        ///
        class JOP_API Time
        {
        public:

            /// \brief Constructor
            ///
            /// \param nsec The initial time value in nanoseconds
            ///
            Time(const uint64 nsec);


            /// \brief Get the time value as seconds
            ///
            /// \return The time value as seconds
            ///
            float64 asSeconds() const;

            /// \brief Get the time value as milliseconds
            ///
            /// \return The time value as milliseconds
            ///
            uint32 asMilliseconds() const;

            /// \brief Get the time value as microseconds
            ///
            /// \return The time value as microseconds
            ///
            uint64 asMicroseconds() const;

            /// \brief Get the time value as nanoseconds
            ///
            /// \return The time value as nanoseconds
            ///
            uint64 asNanoseconds() const;

        private:

            uint64 m_nanoseconds; ///< The internal time value in nanoseconds
        };

    public:

        /// \brief Default constructor
        ///
        /// Sets the initial time to zero and starts counting.
        ///
        Clock();


        /// \brief Reset the clock
        ///
        /// \return The elapsed time since last reset
        ///
        Time reset();

        /// \brief Get the elapsed time
        ///
        /// \return The elapsed time since last reset
        ///
        Time getElapsedTime() const;

    private:

        std::chrono::high_resolution_clock m_clock;             ///< The standard clock object
        std::chrono::time_point<decltype(m_clock)> m_lastTime;  ///< The time point of the last reset
    };
}

#endif

/// \class Clock
/// \ingroup utility
///
/// #TODO Detailed description