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

// Headers
#include <Jopnal/Precompiled.hpp>

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