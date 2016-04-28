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

#ifndef JOP_DATETIME_HPP
#define JOP_DATETIME_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <string>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API DateTime
    {
    public:

        struct Date
        {
            uint16 year;
            uint8 month;
            uint8 day;
        };

        struct Time
        {
            uint8 hour;
            uint8 minute;
            uint8 second;
        };

    public:

        DateTime();


        DateTime& update(const bool date = true, const bool time = true);

        const Date& getDate() const;

        const Time& getTime() const;


        bool operator ==(const DateTime& right) const;
        
        bool operator !=(const DateTime& right) const;

    private:

        Date m_date;
        Time m_time;
    };
}

#endif