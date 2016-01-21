// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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

        class Time
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

        /// \class Time
        /// \brief Class to store a time value
        ///
        /// #TODO Detailed description

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