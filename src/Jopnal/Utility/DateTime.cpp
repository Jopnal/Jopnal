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

	#include <Jopnal/Utility/DateTime.hpp>

    #include <ctime>

#endif

//////////////////////////////////////////////


namespace jop
{
    DateTime::DateTime()
        : m_date    (),
          m_time    ()
    {
        update();
    }

    //////////////////////////////////////////////

    DateTime& DateTime::update(const bool date, const bool time)
    {
        std::time_t t = std::time(0);

    #ifdef JOP_OS_WINDOWS

        struct tm dt;
        localtime_s(&dt, &t);

    #else

        struct tm* timeInfo;
        timeInfo = localtime(&t);

        tm& dt = *timeInfo;

    #endif
        
        if (date)
        {
            m_date.year = static_cast<uint16>(dt.tm_year + 1900);
            m_date.month = static_cast<uint8>(dt.tm_mon + 1);
            m_date.day = static_cast<uint8>(dt.tm_mday);
        }

        if (time)
        {
            m_time.hour = static_cast<uint8>(dt.tm_hour);
            m_time.minute = static_cast<uint8>(dt.tm_min);
            m_time.second = static_cast<uint8>(dt.tm_sec);
        }

        return *this;
    }

    //////////////////////////////////////////////

    const DateTime::Date& DateTime::getDate() const
    {
        return m_date;
    }

    //////////////////////////////////////////////

    const DateTime::Time& DateTime::getTime() const
    {
        return m_time;
    }

    //////////////////////////////////////////////

    bool DateTime::operator ==(const DateTime& right) const
    {
        return m_date.day      == right.m_date.day        &&
               m_date.month    == right.m_date.month      &&
               m_date.year     == right.m_date.year       &&
               m_time.hour     == right.m_time.hour       &&
               m_time.minute   == right.m_time.minute     &&
               m_time.second   == right.m_time.second;
    }

    //////////////////////////////////////////////

    bool DateTime::operator !=(const DateTime& right) const
    {
        return !(*this == right);
    }
}