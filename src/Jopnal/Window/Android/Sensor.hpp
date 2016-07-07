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

#ifdef JOP_OS_ANDROID

#include <android/sensor.h>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Sensor
    {
    private:

        typedef std::unordered_map<SensorType, const ASensor*> SensorMap;
        typedef std::unordered_map<SensorType, glm::vec3> SensorDataMap;


    public:

        enum SensorType
        {
            Accelerometer,      ///< Measures the raw acceleration (meter/second^2)
            Gyroscope,          ///< Measures the raw rotation rates (degrees/second)
            Magnetometer,       ///< Measures the ambient magnetic field (micro-teslas)
            Gravity,            ///< Measures the direction and intensity of gravity, independent of device acceleration (meter/second^2)
            LinearAcceleration, ///< Measures the direction and intensity of device acceleration, independent of the gravity (meter/second^2)
            Orientation,        ///< Measures the absolute 3D orientation (degrees)

            Count             ///< Total number of sensor types
        };

    public:
        Sensor();


        /// \brief Initialize
        ///
        void init();


        void update();

        /// \brief Use this sensor
        ///
        /// Checks if its available and enables it if so
        ///
        bool use(SensorType sensorType);

        /// \brief Disable a single sensor
        ///
        void disable(SensorType sensorType);

        /// \brief Disable sensors
        ///
        /// Used when program loses focus or sensors no longer needed
        ///
        void disableAll();

        /// \brief Enable single sensor
        ///
        /// Used to enable sensor if it has been disabled
        ///
        void enable(SensorType sensorType);

        /// \brief Enable all sensors
        ///
        void enableAll();

        /// \brief Get data from sensor
        ///
        const glm::vec3 getData(SensorType sensorType) const;

        // https://github.com/SFML/SFML/blob/e00d160224fbdf519aa9e420e57cc8e29e244f31/src/SFML/Window/Android/SensorImpl.cpp
        // https://github.com/Team-Innis/UtH-Engine/blob/master/src/Platform/Android/InputSensor.cpp
        // https://github.com/Grimcode/Peramoottori/blob/2ea3a2c7a7b569f8b1f898df076ed64e50093b54/Peramoottori/jni/core/CommandCenter.cpp
        // https://developer.android.com/ndk/reference/group___sensor.html#gaa6e89b6d69dc3e07f2d7e72e81ec7937

    private:

        ASensor const* getDefault(SensorType sensorType);

        /// \brief Get events and save data
        ///
        int getSensorEvents(int fd, int events, void* data);

        SensorMap m_sensors; ///< Map with sensors
        SensorDataMap m_data;
    };


}

#endif
#endif