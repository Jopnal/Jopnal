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
    class Clock
    {
    public:

        struct Time
        {
            Time(const uint64 nsec);


            float64 asSeconds() const;

            uint32 asMilliseconds() const;

            uint64 asMicroseconds() const;

            uint64 asNanoseconds() const;


        private:

            uint64 m_nanoseconds;

        };

    public:

        Clock();


        Time reset();

        Time getElapsedTime() const;

    private:

        std::chrono::high_resolution_clock m_clock;
        std::chrono::time_point<decltype(m_clock)> m_lastTime;

    };

}

#endif