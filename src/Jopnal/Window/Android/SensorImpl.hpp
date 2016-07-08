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
#include <unordered_map>
#include <glm/vec3.hpp>
#include <android/sensor.h>

//////////////////////////////////////////////


namespace jop
{
    class SensorImpl
    {

    public:

        /// \brief Initialize
        ///
        static void init();
            
        /// \brief After we are done with sensor - detach sensor event queue and destroy it
        ///
        static void uninit();

        /// \brief Start sensor
        ///
        /// Checks if its available and sets event ratio afterwards
        ///
        bool use(Sensor::Type sensorType);

        /// \brief Is sensor available or not
        ///
        bool available(Sensor::Type sensorType);

        /// \brief Disable a single sensor
        ///
        void disable(bool updateStatus = true);

        /// \brief Enable single sensor
        ///
        /// Used to enable sensor if it has been disabled
        ///
        void enable(bool updateStatus = true);

        /// \brief Get data from sensor
        ///
        glm::vec3 getData(Sensor::Type sensorType) const;

        /// \brief Get sensor status (True = Enabled, False = Disabled)
        ///
        bool getStatus() const;

    private:

        /// \brief Get default sensor of type
        ///
        static ASensor const* getDefault(Sensor::Type sensorType);

        /// \brief Get events and save data
        ///
        static int getSensorEvents(int fd, int events, void* data);

        bool m_enabled;
        const ASensor* m_sensor;
    };


}

#endif
#endif