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
#include <Jopnal/Window/Android/InputHandling.hpp>
#endif
#ifdef JOP_OS_ANDROID

    //////////////////////////////////////////////
    namespace jop { namespace input
    {

    int32_t Motion(AInputEvent* event)
    {
        Window* windowRef = &Engine::getCurrentWindow();
        if (windowRef != nullptr)
        {
            int32_t device = AInputEvent_getSource(event);
            int pointerCount = AMotionEvent_getPointerCount(event);

            for (int p = 0; p < pointerCount; p++)
            {  
                if (device & AINPUT_SOURCE_TOUCHSCREEN)
                {
                int id = AMotionEvent_getPointerId(event, p);
                float x = AMotionEvent_getX(event, p);
                float y = AMotionEvent_getY(event, p);

                windowRef->getEventHandler()->touchMotion(id, x, y);
                }
            }
            return 1;
        }
        return 0;
    }

    //////////////////////////////////////////////

    int32_t Scroll(AInputEvent* event)
    {
        Window* windowRef = &Engine::getCurrentWindow();
        if (windowRef != nullptr)
        {
            float x = AMotionEvent_getX(event, 0);
            float y = AMotionEvent_getY(event, 0);

            windowRef->getEventHandler()->touchScrolled(x, y);

            return 1;
        }
        return 0;
    }

    //////////////////////////////////////////////

    int32_t Touch(bool down,int32_t action,AInputEvent* event)
    {
        Window* windowRef = &Engine::getCurrentWindow();
        if (windowRef != nullptr)
        {
            int32_t device = AInputEvent_getSource(_event);
            if (device & AINPUT_SOURCE_TOUCHSCREEN)
            {
                int index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;      
                int id = AMotionEvent_getPointerId(_event, index);
                float x = AMotionEvent_getX(_event, index);
                float y = AMotionEvent_getY(_event, index);

                if(down)
                    windowRef->getEventHandler()->touchEvent(id,x,y);
                else
                    windowRef->getEventHandler()->touchReleased(id,x,y);
               
                return 1;
            }
        }
        return 0;
    }

    //////////////////////////////////////////////

    int32_t onKey(const int32_t& action, const int32_t& meta, const int32_t& key)
    {
        int jopKey = getJopKey(key);

        int mod = 0x0000;
        if (meta & AMETA_ALT_ON)
            mod = 0x0001;
        else if (meta & AMETA_SHIFT_ON)
            mod = 0x0004;

        switch (action)
        {
        case AKEY_EVENT_ACTION_DOWN:
        {
            Window* windowRef = &Engine::getCurrentWindow();
            if (windowRef != nullptr)
            {
                windowRef->getEventHandler()->keyPressed(jopKey, key, mod);
                return 1;
            }
            return 0;
        }
        case AKEY_EVENT_ACTION_UP:
        {
            Window* windowRef = &Engine::getCurrentWindow();
            if (windowRef != nullptr)
            {
                windowRef->getEventHandler()->keyReleased(jopKey, key, mod);
                return 1;
            }
            return 0;
        }
        case AKEY_EVENT_ACTION_MULTIPLE:
        {
            Window* windowRef = &Engine::getCurrentWindow();
            if (windowRef != nullptr)
            {
                windowRef->getEventHandler()->keyPressed(jopKey, key, mod);
                windowRef->getEventHandler()->keyReleased(jopKey, key, mod);
                return 1;
            }
            return 0;
        }
        default:
            break;
        }
        return 0;
    }

    //////////////////////////////////////////////

    int32_t onMotion(const int32_t& action, AInputEvent* event)
    {

        switch (action & AMOTION_EVENT_ACTION_MASK)
        {
        case AMOTION_EVENT_ACTION_SCROLL:
        {
            scroll(event);
            break;
                }
        case AMOTION_EVENT_ACTION_MOVE:
        {
            motion(event);
            break;
        }
        case AMOTION_EVENT_ACTION_POINTER_DOWN:
        case AMOTION_EVENT_ACTION_DOWN:
        {
            touch(true, action, event);
            break;
        }

        case AMOTION_EVENT_ACTION_POINTER_UP:
        case AMOTION_EVENT_ACTION_UP:
        case AMOTION_EVENT_ACTION_CANCEL:
        {
            touch(false, action, event);
            break;
        }
            }
    
        }
        default:
        return 0;
    }
}}
#endif