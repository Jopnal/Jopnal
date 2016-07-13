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
#include <Jopnal/Window/SensorManager.hpp>
#include <Jopnal/Core/DebugHandler.hpp>

//////////////////////////////////////////////


namespace jop
{
    SensorManager::SensorManager()
    {
        // Sensor initialization
        SensorImpl::init();

        // Check which sensors are available and take them to use
        for (size_t i = 0; i < Sensor::Count; ++i)
            m_sensors[i].use(static_cast<Sensor::Type>(i));

        // Disable all after they are in memory
        disableAll();
    }

    SensorManager::~SensorManager()
    {
        SensorImpl::uninit();
    }

    //////////////////////////////////////////////

    bool SensorManager::isAvailable(const Sensor::Type sensorType) const
    {
        return m_sensors[sensorType].available(sensorType);
    }

    //////////////////////////////////////////////

    void SensorManager::update()
    {
        // NO need if ALooper_pollAll is in use somewhere else?
    }

    //////////////////////////////////////////////

    void SensorManager::enable(const Sensor::Type sensorType)
    {
        m_sensors[sensorType].enable();
    }

    //////////////////////////////////////////////

    void SensorManager::disable(const Sensor::Type sensorType)
    {
        m_sensors[sensorType].disable();
    }

    //////////////////////////////////////////////

    void SensorManager::disableAll()
    {
        for (size_t i = 0; i < Sensor::Count; ++i)
            disable(static_cast<Sensor::Type>(i));
    }

    //////////////////////////////////////////////

    bool SensorManager::getStatus(const Sensor::Type sensorType) const
    {
        return m_sensors[sensorType].getStatus();
    }

    //////////////////////////////////////////////

    glm::vec3 SensorManager::getData(const Sensor::Type sensorType) const
    {
        return m_sensors[sensorType].getData(sensorType);
    }

    void SensorManager::lostFocus()
    {
        for (size_t i = 0; i < Sensor::Count; ++i)
        {
            if (m_sensors[i].getStatus())
                m_sensors[i].disable(false);
        }
    }

    //////////////////////////////////////////////

    void SensorManager::gainedFocus()
    {
        for (size_t i = 0; i < Sensor::Count; ++i)
        {
            if (m_sensors[i].getStatus())
                m_sensors[i].enable(false);
        }
    }

    //////////////////////////////////////////////

    SensorManager& SensorManager::getInstance()
    {
        static SensorManager instance;
        return instance;
    }
}