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
#include <Jopnal/Window/Android/Sensor.hpp>
#ifdef JOP_OS_ANDROID

//////////////////////////////////////////////


namespace jop
{

    namespace
    {
        ALooper*            looper              = nullptr;
        ASensorManager*     sensorManager       = nullptr;
        ASensorEventQueue*  sensorEventQueue    = nullptr;
    }

    SensorImpl::SensorImpl()
    {}

    //////////////////////////////////////////////

    void SensorImpl::init()
    {
        // looper for threading
        looper = ALooper_forThread();

        // get sensor manager
        sensorManager = ASensorManager_getInstance();

        // create sensor event queue & attach to looper
        sensorEventQueue = ASensorManager_createEventQueue(sensorManager, looper, LOOPER_ID_USER, &getSensorEvents, nullptr);
    }

    //////////////////////////////////////////////

    void SensorImpl::update()
    {
        for (auto& i : m_data)
        {
            // Only update available sensors
            if(m_sensors[i])
            {
                ALooper_pollAll(0, NULL, NULL, NULL);   
            }
        }
    }

    //////////////////////////////////////////////

    bool SensorImpl::use(SensorType sensorType)
    {
        m_sensors[sensorType] = getDefault(sensorType);

        // Check if sensor is available
        if(!m_sensors[sensorType])
        {
            JOP_DEBUG_ERROR("Sensor: (" <<  m_sensors[sensorType].ASensor_getName() << ") not available.");
            return false;
        }

        // Enable it
        enable(sensorType);

        return true;
    }

    //////////////////////////////////////////////

    void SensorImpl::disable(SensorType sensorType)
    {
        int status = ASensorEventQueue_disableSensor(sensorEventQueue, m_sensors[sensorType]);

        if(status < 0)
            JOP_DEBUG_WARNING("Disabling sensor: " << m_sensors[sensorType].ASensor_getName() << " failed.");
    }

    //////////////////////////////////////////////

    static void SensorImpl::disableAll()
    {
        for (auto& i : m_sensors)
        {
            disable(i);
        }
    }

    //////////////////////////////////////////////

    void SensorImpl::enable(SensorType sensorType)
    {
        int status = ASensorEventQueue_enableSensor(sensorEventQueue, m_sensors[sensorType]);

        if(status < 0)
            JOP_DEBUG_WARNING("Enabling sensor: " <<  m_sensors[sensorType].ASensor_getName() << " failed.");

        // Set rate (60 events / second) use ASensor_getMinDelay() instead?
        status = ASensorEventQueue_setEventRate(sensorEventQueue, m_sensors[sensorType], (1000L / 60) * 1000);

        if(status < 0)
            JOP_DEBUG_WARNING("Setting event rate on sensor: " <<  m_sensors[sensorType].ASensor_getName() << " failed.");
    }

    //////////////////////////////////////////////

    static void SensorImpl::enableAll()
    {
        // enable sensors
        for (auto& i : m_sensors)
        {
            enable(i);
        }
    }

    //////////////////////////////////////////////

    const glm::vec3 SensorImpl::getData(SensorType sensorType) const
    {
        return m_data[sensorType];
    }

    //////////////////////////////////////////////

    ASensor const* SensorImpl::getDefault(SensorType sensorType)
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
        return ASensorManager_getDefaultSensor(sensorManager, types[sensorType]);
    }

    //////////////////////////////////////////////

    int SensorImpl::getSensorEvents(int fd, int events, void* data)
    {
        ASensorEvent event;

        while (ASensorEventQueue_getEvents(sensorEventQueue, &event, 1) > 0)
        {
            unsigned int type = SensorType::Count;
            glm::vec3 data;

            switch (event.type)
            {
            case ASENSOR_TYPE_ACCELEROMETER:
            {
                type = SensorType::Accelerometer;
                data.x = event.acceleration.x;
                data.y = event.acceleration.y;
                data.z = event.acceleration.z;
                break;
            }
            case ASENSOR_TYPE_GYROSCOPE:
            {
                type = SensorType::Gyroscope;
                data.x = event.vector.x;
                data.y = event.vector.y;
                data.z = event.vector.z;
                break;
            }
            case ASENSOR_TYPE_MAGNETIC_FIELD:
            {
                type = SensorType::Magnetometer;
                data.x = event.magnetic.x;
                data.y = event.magnetic.y;
                data.z = event.magnetic.z;
                break;
            }
            case ASENSOR_TYPE_GRAVITY:
            {
                type = SensorType::Gravity;
                data.x = event.vector.x;
                data.y = event.vector.y;
                data.z = event.vector.z;
                break;
            }
            case ASENSOR_TYPE_LINEAR_ACCELERATION:
            {
                type = SensorType::LinearAcceleration;
                data.x = event.acceleration.x;
                data.y = event.acceleration.y;
                data.z = event.acceleration.z;
                break;
            }
            case ASENSOR_TYPE_ORIENTATION:
            {
                type = SensorType::Orientation;
                data.x = event.vector.x;
                data.y = event.vector.y;
                data.z = event.vector.z;
                break;
            }
            }

            if (type == SensorType::Count)
            {
                JOP_DEBUG_ERROR("Unknown sensor event detected.");
                continue;
            }

            m_data[type] = data;
        }

        return 1;

    }
}

#endif