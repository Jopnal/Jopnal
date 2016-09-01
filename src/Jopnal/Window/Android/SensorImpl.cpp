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
#include <Jopnal/Window/Android/SensorImpl.hpp>

#ifdef JOP_OS_ANDROID

#include <android/looper.h>
#include <Jopnal/Core/DebugHandler.hpp>
#include <glm/vec3.hpp>

//////////////////////////////////////////////


#define ASENSOR_TYPE_GRAVITY             0x00000009
#define ASENSOR_TYPE_LINEAR_ACCELERATION 0x0000000a
#define ASENSOR_TYPE_ORIENTATION         0x00000003

namespace
{
    ALooper* ns_looper                     = nullptr;
    ASensorManager* ns_sensorManager       = nullptr;
    ASensorEventQueue* ns_sensorEventQueue = nullptr;
    glm::vec3 ns_data[static_cast<size_t>(jop::Sensor::Type::__Count)];

    int getSensorEvents(int fd, int events, void* data)
    {
        ASensorEvent event;

        while (ASensorEventQueue_getEvents(ns_sensorEventQueue, &event, 1) > 0)
        {
            using T = jop::Sensor::Type;

            T type = T::__Count;
            glm::vec3 data(0.f);

            switch (event.type)
            {
                case ASENSOR_TYPE_ACCELEROMETER:
                {
                    type = T::Accelerometer;
                    data.x = event.acceleration.x;
                    data.y = event.acceleration.y;
                    data.z = event.acceleration.z;

                    break;
                }
                case ASENSOR_TYPE_GYROSCOPE:
                {
                    type = T::Gyroscope;
                    data.x = event.vector.x;
                    data.y = event.vector.y;
                    data.z = event.vector.z;

                    break;
                }
                case ASENSOR_TYPE_MAGNETIC_FIELD:
                {
                    type = T::Magnetometer;
                    data.x = event.magnetic.x;
                    data.y = event.magnetic.y;
                    data.z = event.magnetic.z;

                    break;
                }
                case ASENSOR_TYPE_GRAVITY:
                {
                    type = T::Gravity;
                    data.x = event.vector.x;
                    data.y = event.vector.y;
                    data.z = event.vector.z;

                    break;
                }
                case ASENSOR_TYPE_LINEAR_ACCELERATION:
                {
                    type = T::LinearAcceleration;
                    data.x = event.acceleration.x;
                    data.y = event.acceleration.y;
                    data.z = event.acceleration.z;

                    break;
                }
                case ASENSOR_TYPE_ORIENTATION:
                {
                    type = T::Orientation;
                    data.x = event.vector.x;
                    data.y = event.vector.y;
                    data.z = event.vector.z;
                }

                default:
                    continue;
            }

            ns_data[static_cast<size_t>(type)] = data;
        }

        return 1;
    }

    //////////////////////////////////////////////

    const ASensor* getDefault(const jop::Sensor::Type type)
    {
        static int types[] = 
        {
            ASENSOR_TYPE_ACCELEROMETER,
            ASENSOR_TYPE_GYROSCOPE,
            ASENSOR_TYPE_MAGNETIC_FIELD,
            ASENSOR_TYPE_GRAVITY,
            ASENSOR_TYPE_LINEAR_ACCELERATION,
            ASENSOR_TYPE_ORIENTATION
        };

        // Retrieve the default android sensor matching the type
        return ASensorManager_getDefaultSensor(ns_sensorManager, types[static_cast<size_t>(type)]);
    }
}

namespace jop
{
    void SensorImpl::init()
    {
        // looper for threading
        ns_looper = ALooper_forThread();

        // get sensor manager
        ns_sensorManager = ASensorManager_getInstance();

        // create sensor event queue & attach to looper
        ns_sensorEventQueue = ASensorManager_createEventQueue(ns_sensorManager, ns_looper, 1, &getSensorEvents, nullptr);
    }

    void SensorImpl::deInit()
    {
        ASensorManager_destroyEventQueue(ns_sensorManager, ns_sensorEventQueue);
    }

    //////////////////////////////////////////////

    SensorImpl::SensorImpl(const Sensor::Type type)
        : m_type    (type),
          m_sensor  (NULL)
    {}

    //////////////////////////////////////////////

    bool SensorImpl::isAvailable() const
    {
        return getDefault(m_type) != NULL;
    }

    //////////////////////////////////////////////

    glm::vec3 SensorImpl::getData() const
    {
        return ns_data[static_cast<size_t>(m_type)];
    }

    //////////////////////////////////////////////

    void SensorImpl::setEnabled(const bool enabled)
    {
        if (enabled == isEnabled() || !isAvailable())
            return;

        if (enabled)
        {
            if (ASensorEventQueue_enableSensor(ns_sensorEventQueue, getDefault(m_type)) >= 0)
            {
                m_sensor = getDefault(m_type);

                if (auto rate = ASensor_getMinDelay(m_sensor))
                    ASensorEventQueue_setEventRate(ns_sensorEventQueue, m_sensor, rate);
            }
        }
        else
        {
            ASensorEventQueue_disableSensor(ns_sensorEventQueue, m_sensor);
            m_sensor = NULL;
        }
    }

    //////////////////////////////////////////////

    bool SensorImpl::isEnabled() const
    {
        return m_sensor != NULL;
    }
}

#endif