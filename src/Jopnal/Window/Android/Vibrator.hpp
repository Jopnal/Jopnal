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

#ifndef JOP_VIBRATOR_HPP
#define JOP_VIBRATOR_HPP

// Headers
#include <Jopnal/Header.hpp>

//////////////////////////////////////////////


namespace jop
{
    class Vibrator
    {
    public:

        /// \brief Vibrate for given time (in milliseconds)
        ///
        static void vibrate(const int time_ms);

        /// \brief Vibrate with given pattern
        ///
        /// The first value of the pattern indicates the number of milliseconds to wait before turning the vibration on.
        /// The next value indicates the number of milliseconds for which to keep the vibrator on before turning it off.
        /// The values after these alternate between durations to turn the vibrator off or to turn the vibrator on.
        ///
        /// \param pattern Vibration pattern (Durations for which to turn on or off the vibration in milliseconds)
        /// \param repeat Repeat pattern? (-1 = Don't repeat, 0 = repeat infinitely, 1 = repeat once)
        ///
        //static void vibrate(const long[] pattern, const int repeat);

        /// \brief Stop vibrating
        ///
        //static void stop();

    };

}

#endif