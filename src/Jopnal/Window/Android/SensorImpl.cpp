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
//////////////////////////////////////////////

#define ASENSOR_TYPE_GRAVITY             0x00000009
#define ASENSOR_TYPE_LINEAR_ACCELERATION 0x0000000a
#define ASENSOR_TYPE_ORIENTATION         0x00000003

namespace jop
{

    namespace
    {
        ALooper*            looper              = nullptr;
        ASensorManager*     sensorManager       = nullptr;
        ASensorEventQueue*  sensorEventQueue    = nullptr;
        glm::vec3           m_data[Sensor::Type::Count];
    }


    //////////////////////////////////////////////

    void SensorImpl::init()
    {
        // looper for threading
        looper = ALooper_forThread();

        // get sensor manager
        sensorManager = ASensorManager_getInstance();

        // create sensor event queue & attach to looper
        sensorEventQueue = ASensorManager_createEventQueue(sensorManager, looper, 1, &getSensorEvents, nullptr);
    }

    //////////////////////////////////////////////

    void SensorImpl::uninit()
    {
        ASensorManager_destroyEventQueue(sensorManager, sensorEventQueue);
    }

    //////////////////////////////////////////////

    bool SensorImpl::use(Sensor::Type sensorType)
    {
        m_sensor = getDefault(sensorType);

        std::string name = std::string(ASensor_getName(m_sensor), sizeof(ASensor_getName(m_sensor)));

        // Check if sensor is available
        if(!available(sensorType))
        {
            JOP_DEBUG_ERROR("Sensor: (" <<  name << ") not available.");
            return false;
        }
        
        // Set rate (60 events / second) use ASensor_getMinDelay() instead?
        int status = ASensorEventQueue_setEventRate(sensorEventQueue, m_sensor, (1000 / 60) * 1000);

        if (status < 0)
            JOP_DEBUG_WARNING("Setting event rate on sensor: " << name << " failed.");

        return true;
    }

    //////////////////////////////////////////////

    bool SensorImpl::available(Sensor::Type sensorType)
    {
        if (getDefault(sensorType) == nullptr)
        {
            return false;
        }
        else
            return true;
    }

    //////////////////////////////////////////////

    void SensorImpl::disable(bool updateStatus)
    {
        int status = ASensorEventQueue_disableSensor(sensorEventQueue, m_sensor);

        std::string name = std::string(ASensor_getName(m_sensor), sizeof(ASensor_getName(m_sensor)));

        if(status < 0)
            JOP_DEBUG_WARNING("Disabling sensor: " << name << " failed.");
        if(updateStatus)
            m_enabled = false;
    }

    //////////////////////////////////////////////

    void SensorImpl::enable(bool updateStatus)
    {
        int status = ASensorEventQueue_enableSensor(sensorEventQueue, m_sensor);

        std::string name = std::string(ASensor_getName(m_sensor), sizeof(ASensor_getName(m_sensor)));

        if(status < 0)
            JOP_DEBUG_WARNING("Enabling sensor: " << name << " failed.");
        if(updateStatus)
            m_enabled = true;
    }

    //////////////////////////////////////////////

    glm::vec3 SensorImpl::getData(Sensor::Type sensorType) const
    {
        return m_data[sensorType];
    }

    //////////////////////////////////////////////

    bool SensorImpl::getStatus() const
    {
        return m_enabled;
    }

    //////////////////////////////////////////////

    ASensor const* SensorImpl::getDefault(Sensor::Type sensorType)
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
            unsigned int type = Sensor::Type::Count;
            glm::vec3 data;

            switch (event.type)
            {
            case ASENSOR_TYPE_ACCELEROMETER:
            {
                type = Sensor::Accelerometer;
                data.x = event.acceleration.x;
                data.y = event.acceleration.y;
                data.z = event.acceleration.z;
                break;
            }
            case ASENSOR_TYPE_GYROSCOPE:
            {
                type = Sensor::Gyroscope;
                data.x = event.vector.x;
                data.y = event.vector.y;
                data.z = event.vector.z;
                break;
            }
            case ASENSOR_TYPE_MAGNETIC_FIELD:
            {
                type = Sensor::Magnetometer;
                data.x = event.magnetic.x;
                data.y = event.magnetic.y;
                data.z = event.magnetic.z;
                break;
            }
            case ASENSOR_TYPE_GRAVITY:
            {
                type = Sensor::Gravity;
                data.x = event.vector.x;
                data.y = event.vector.y;
                data.z = event.vector.z;
                break;
            }
            case ASENSOR_TYPE_LINEAR_ACCELERATION:
            {
                type = Sensor::LinearAcceleration;
                data.x = event.acceleration.x;
                data.y = event.acceleration.y;
                data.z = event.acceleration.z;
                break;
            }
            case ASENSOR_TYPE_ORIENTATION:
            {
                type = Sensor::Orientation;
                data.x = event.vector.x;
                data.y = event.vector.y;
                data.z = event.vector.z;
                break;
            }
            }

            if (type == Sensor::Count)
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