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

	#include <Jopnal/Utility/Clock.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    Clock::Time::Time(const uint64 nsec)
        : m_nanoseconds(nsec)
    {}

    //////////////////////////////////////////////

    float64 Clock::Time::asSeconds() const
    {
        return static_cast<float64>(m_nanoseconds) / 1000000000.0;
    }

    //////////////////////////////////////////////

    uint32 Clock::Time::asMilliseconds() const
    {
        return static_cast<uint32>(m_nanoseconds / 1000000);
    }

    //////////////////////////////////////////////

    uint64 Clock::Time::asMicroseconds() const
    {
        return m_nanoseconds / 1000;
    }

    //////////////////////////////////////////////

    uint64 Clock::Time::asNanoseconds() const
    {
        return m_nanoseconds;
    }


    //////////////////////////////////////////////


    Clock::Clock()
        : m_clock(),
          m_lastTime(m_clock.now())
    {}

    //////////////////////////////////////////////

    Clock::Time Clock::reset()
    {
        const Time time(getElapsedTime());

        m_lastTime = m_clock.now();

        return time;
    }

    //////////////////////////////////////////////

    Clock::Time Clock::getElapsedTime() const
    {
        return Time(static_cast<uint64>(std::chrono::nanoseconds(m_clock.now() - m_lastTime).count()));
    }
}