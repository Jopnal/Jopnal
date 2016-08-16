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

    #include <Jopnal/Window/SensorManager.hpp>
    #include <Jopnal/Core/DebugHandler.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    SensorManager::SensorManager()
        : m_sensors()
    {
        const auto count = static_cast<std::size_t>(Sensor::Type::__Count);

        m_sensors.reserve(count);
        for (std::size_t i = 0; i < count; ++i)
            m_sensors.emplace_back(static_cast<Sensor::Type>(i));

        SensorImpl::init();
    }

    SensorManager::~SensorManager()
    {
        m_sensors.clear();
        SensorImpl::deInit();
    }

    //////////////////////////////////////////////

    bool SensorManager::isAvailable(const Sensor::Type type) const
    {
        return m_sensors[static_cast<size_t>(type)].isAvailable();
    }

    //////////////////////////////////////////////

    void SensorManager::setEnabled(const Sensor::Type type, const bool enabled)
    {
        m_sensors[static_cast<size_t>(type)].setEnabled(enabled);
    }

    //////////////////////////////////////////////

    bool SensorManager::isEnabled(const Sensor::Type type) const
    {
        return m_sensors[static_cast<size_t>(type)].isEnabled();
    }

    //////////////////////////////////////////////

    glm::vec3 SensorManager::getData(const Sensor::Type type) const
    {
        return m_sensors[static_cast<size_t>(type)].getData();
    }

    //////////////////////////////////////////////

    SensorManager& SensorManager::getInstance()
    {
        static SensorManager instance;
        return instance;
    }
}