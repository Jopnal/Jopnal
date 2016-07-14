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
#include <Jopnal/Header.hpp>

//////////////////////////////////////////////

extern int main(int argc, char* argv[]);

#if defined(JOP_OS_WINDOWS)

    #include <Jopnal/Core/Win32/Win32.hpp>
    #include <cstdlib>

    int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
    {
        return main(__argc, __argv);
    }

#elif defined(JOP_OS_ANDROID)

    #include <Jopnal/Core/Android/ActivityState.hpp>
    #include <Jopnal/Core/Engine.hpp>
    #include <Jopnal/Core/DebugHandler.hpp>
    #include <Jopnal/Main/Android/android_native_app_glue.c>
    #include<Jopnal/Window/Window.hpp>
    #include <thread>
    #include <atomic>  

#include <Jopnal/Window/InputEnumsImpl.hpp>
namespace jop
{
    ///////////////////////////////////////////////////////////////////////

    int32_t motion(AInputEvent* event)
    {  
        Window* windowRef = &Engine::getCurrentWindow();
        if (windowRef != nullptr)
        {
            int32_t device = AInputEvent_getSource(event);
            int pointerCount = AMotionEvent_getPointerCount(event);

            for (int p = 0; p < pointerCount; p++)
            {                   
                    int id = AMotionEvent_getPointerId(event, p);
                    float x = AMotionEvent_getX(event, p);
                    float y = AMotionEvent_getY(event, p);
                   
                    if (device & AINPUT_SOURCE_TOUCHSCREEN)
                    windowRef->getEventHandler()->touchMotion(id, x, y);
                    else if(device & AINPUT_SOURCE_MOUSE)
                        windowRef->getEventHandler()->mouseMoved(x, y);
                    else
                        return 0;
            }
            return 1;
        }
        return 0;
    }

    //////////////////////////////////////////////

    int32_t scroll(AInputEvent* event)
    {   
        Window* windowRef = &Engine::getCurrentWindow();
        if (windowRef != nullptr)
        {
            int32_t device = AInputEvent_getSource(event);

            float x = AMotionEvent_getX(event, 0);
            float y = AMotionEvent_getY(event, 0);

            if (device & AINPUT_SOURCE_TOUCHSCREEN)
            windowRef->getEventHandler()->touchScrolled(x, y);
            else if(device & AINPUT_SOURCE_MOUSE)
                windowRef->getEventHandler()-> mouseScrolled(x, y);
            else
                return 0;

            return 1;
        }
        return 0;
    }

    //////////////////////////////////////////////

    int32_t touch(bool down,int32_t action,AInputEvent* event)
    {
        
        Window* windowRef = &Engine::getCurrentWindow();
        if (windowRef != nullptr)
        {
            int32_t device = AInputEvent_getSource(event);
            if (device & AINPUT_SOURCE_TOUCHSCREEN)
            {
                int index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;      
                int id = AMotionEvent_getPointerId(event, index);
                float x = AMotionEvent_getX(event, index);
                float y = AMotionEvent_getY(event, index);

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
        int jopKey = input::getJopKey(key);

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
            return scroll(event);
            break;
        }
        case AMOTION_EVENT_ACTION_MOVE:
        {
            return motion(event);
            break;
        }
        case AMOTION_EVENT_ACTION_POINTER_DOWN:
        case AMOTION_EVENT_ACTION_DOWN:
        {
            return touch(true, action, event);
            break;
        }

        case AMOTION_EVENT_ACTION_POINTER_UP:
        case AMOTION_EVENT_ACTION_UP:
        case AMOTION_EVENT_ACTION_CANCEL:
        {
            return touch(false, action, event);
            break;
        }
        }

    }}
///////////////////////////////////////////////////////////////////////
    namespace jop { namespace detail
    {

        android_app* ns_app=nullptr;
        std::atomic<bool> ns_ready(false);
   
        void onAppCmd(struct android_app* app, int32_t cmd)
        { 
            auto state = ActivityState::get();
            std::lock_guard<decltype(state->mutex)> lock(state->mutex);

            switch (cmd)
            {
                case APP_CMD_INIT_WINDOW:
                {
                    state->nativeWindow = app->window;
                    //state->windowSize.x = ANativeWindow_getWidth(app->window);
                    //state->windowSize.y = ANativeWindow_getHeight(app->window);

                    ns_ready.store(true);
                }
            }
        }

        void onAppCmdRunning(struct android_app* app, int32_t cmd)
        {


            switch (cmd)
            {
                case APP_CMD_INIT_WINDOW:
                {

                }

                case APP_CMD_TERM_WINDOW:
                {

                }

                case APP_CMD_WINDOW_RESIZED:
                {

                }

                case APP_CMD_DESTROY:
                {
                    ANativeActivity_finish(app->activity);
                    Engine::exit();
                }
            }
        }

        int32_t onInputEvent(struct android_app* app, AInputEvent* event)
        {
			int32_t type = AInputEvent_getType(event);
			int32_t action = AKeyEvent_getAction(event);

			if (type == AINPUT_EVENT_TYPE_KEY)
			{
                int32_t metakey = AKeyEvent_getMetaState(event);
                int32_t key = AKeyEvent_getKeyCode(event);
                return  onKey(action,metakey,key);
			}
			else if (type == AINPUT_EVENT_TYPE_MOTION)
			{
                return  onMotion(action, event);
			}

            return 0;
        }

        void pollFunc()
        { 
            android_poll_source* event = nullptr;
            while (ALooper_pollAll(0, NULL, NULL, reinterpret_cast<void**>(&event)) >= 0)
            {
                if (event)
                    event->process(ns_app, event);
            }
        }

        void main(struct android_app* app)
        {

            ns_app=app;
            JOP_DEBUG_INFO("Android activity started, waiting for window...");
            while (!ns_ready.load())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                pollFunc();
            }
            app->onAppCmd       = onAppCmdRunning;
            app->onInputEvent   = onInputEvent;       
            ActivityState::get()->pollFunc=&pollFunc;

            JOP_DEBUG_INFO("Android activity is ready, entering application main()");

            char v = '\0';
            char* argv[] = {&v};
            ::main(1, argv);
        }
    }}

    void android_main(struct android_app* app)
    {
        JOP_DEBUG_INFO("Entered jopnal-main");

        app->onAppCmd       = jop::detail::onAppCmd;

        app->userData = jop::detail::ActivityState::create(app->activity);

        jop::detail::main(app);
    }

#endif