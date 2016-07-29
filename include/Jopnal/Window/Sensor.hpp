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

#ifndef JOP_SENSOR_HPP
#define JOP_SENSOR_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <glm/vec3.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Sensor
    {
    public:

        enum class Type
        {
            Accelerometer,      ///< Measures the raw acceleration (meter/second^2)
            Gyroscope,          ///< Measures the raw rotation rates (degrees/second)
            Magnetometer,       ///< Measures the ambient magnetic field (micro-teslas)
            Gravity,            ///< Measures the direction and intensity of gravity, independent of device acceleration (meter/second^2)
            LinearAcceleration, ///< Measures the direction and intensity of device acceleration, independent of the gravity (meter/second^2)
            Orientation,        ///< Measures the absolute 3D orientation (degrees)

            Count               ///< Total number of sensor types
        };

    public:

        /// \brief Use this sensor
        ///
        /// \return True if successfully taken in to use, false if not
        ///
        static bool use(const Type sensorType);

        /// \brief Enable sensor of a type
        ///
        static void enable(const Type sensorType);

        /// \brief Disable sensor of a type
        ///
        static void disable(const Type sensorType);

        /// \brief Get data from sensor
        ///
        static glm::vec3 getData(const Type sensorType);

        /// \brief Get sensor status
        ///
        /// \return True if enabled
        ///
        static bool getStatus(const Type sensorType);
    };
}

#endif

/// \class jop::Sensor
/// \ingroup window
///
///