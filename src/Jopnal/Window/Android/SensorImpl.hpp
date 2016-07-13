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

#ifndef JOP_SENSORIMPL_HPP
#define JOP_SENSORIMPL_HPP

// Headers
#include <Jopnal/Header.hpp>

#ifdef JOP_OS_ANDROID

#include <Jopnal/Window/Sensor.hpp>
#include <glm/vec3.hpp>
#include <android/sensor.h>
#include <unordered_map>

//////////////////////////////////////////////


namespace jop
{
    class SensorImpl
    {
    public:

        /// \brief Initialize sensor implementation
        ///
        static void init();
            
        /// \brief After we are done with sensor - detach sensor event queue and destroy it
        ///
        static void uninit();

        /// \brief Use this sensor
        ///
        /// Checks if its available and sets event ratio afterwards
        ///
        /// \return True if successfully taken use, false if it is not available!
        ///
        bool use(const Sensor::Type sensorType);

        /// \brief Is sensor available or not
        ///
        bool available(const Sensor::Type sensorType) const;

        /// \brief Disable sensor
        ///
        void disable(const bool updateStatus = true);

        /// \brief Enable sensor
        ///
        /// Used to enable sensor if it has been disabled
        ///
        void enable(const bool updateStatus = true);

        /// \brief Get data from sensor
        ///
        glm::vec3 getData(const Sensor::Type sensorType) const;

        /// \brief Get sensor status (True = Enabled, False = Disabled)
        ///
        bool getStatus() const;

    private:

        /// \brief Get default sensor of a type
        ///
        static const ASensor* getDefault(Sensor::Type sensorType);

        /// \brief Process events and save data
        ///
        static int getSensorEvents(int fd, int events, void* data);

        bool m_enabled;             ///< Sensor status
        const ASensor* m_sensor;    ///< Sensor
    };
}

#endif
#endif