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

#ifndef JOP_SENSORMANAGER_HPP
#define JOP_SENSORMANAGER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Window/Sensor.hpp>

#if defined(JOP_OS_ANDROID)
    #include <Jopnal/Window/Android/SensorImpl.hpp>
#else
    #include <Jopnal/Window/Desktop/SensorImpl.hpp>
#endif

//////////////////////////////////////////////


namespace jop
{
    class JOP_API SensorManager
    {
    public:
        
        /// \brief Is sensor available or not?
        ///
        bool isAvailable(const Sensor::Type sensorType) const;

        /// \brief Process sensor events and update their data
        ///
        void update();

        /// \brief Enable sensor of a type
        ///
        void enable(const Sensor::Type sensorType);

        /// \brief Disable sensor of a type
        ///
        void disable(const Sensor::Type sensorType);

        /// \brief Disable all sensors
        ///
        void disableAll();

        /// \brief Get sensor status (True = enabled, False = disabled)
        ///
        bool getStatus(const Sensor::Type) const;

        /// \brief Get sensor data
        ///
        glm::vec3 getData(const Sensor::Type sensorType) const;

        /// \brief If application loses focus - call this to disable sensors
        ///
        void lostFocus();

        /// \brief After losing focus - call this to enable the sensors that were in use prior to that
        ///
        void gainedFocus();

        /// \brief Get instance
        ///
        static SensorManager& getInstance();

    private:

        SensorManager();

        ~SensorManager();

        SensorImpl m_sensors[static_cast<std::size_t>(Sensor::Type::Count)]; ///< Sensors
    };
}

#endif