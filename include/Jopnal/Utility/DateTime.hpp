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

        /// \brief Structure representing a date
        ///
        struct Date
        {
            uint16 year;    ///< The year
            uint8 month;    ///< The month
            uint8 day;      ///< The day
        };

        struct Time
        {
            uint8 hour;     ///< The hour
            uint8 minute;   ///< The minute
            uint8 second;   ///< The second
        };

    public:

        /// \brief Constructor
        ///
        /// Calls update().
        ///
        DateTime();


        /// \brief Update the date and/or time with the current, real world date and time values
        ///
        /// \param date Update date?
        /// \param time Update time?
        ///
        /// \return Reference to self
        ///
        DateTime& update(const bool date = true, const bool time = true);

        /// \brief Get the cached date value
        ///
        /// \return The cached date value
        ///
        const Date& getDate() const;

        /// \brief Get the cached time value
        ///
        /// \return The cached time value
        ///
        const Time& getTime() const;

        /// \brief Equality operator
        ///
        /// \param right The other DateTime to compare against
        ///
        /// \return True if the two date-times match
        ///
        bool operator ==(const DateTime& right) const;

        /// \brief Inequality operator
        ///
        /// \param right The other DateTime to compare against
        ///
        /// \return True if the two date-times don't match
        ///
        bool operator !=(const DateTime& right) const;

    private:

        Date m_date;    ///< Date
        Time m_time;    ///< Time
    };
}

/// \class jop::DateTime
/// \ingroup utility

#endif