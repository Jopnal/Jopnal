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
    #include <Jopnal/Window/SensorManager.hpp>
    #include <Jopnal/Main/Android/android_native_app_glue.c>
    #include <Jopnal/Core/Android/GooglePlayService.hpp>
    #include <Jopnal/Utility/Thread.hpp>
    #include <Jopnal/Window/Window.hpp>
    #include <Jopnal/Window/Android/WindowImpl.hpp>
    #include <thread>
    #include <atomic>  

    namespace jop { namespace detail
    {
        android_app* ns_app = nullptr;
        unsigned int ns_readyState = 0;
   
        void onAppCmd(struct android_app* app, int32_t cmd)
        {
            static auto lastRunningState = Engine::getState();
            auto state = ActivityState::get();

            switch (cmd)
            {
                case APP_CMD_INIT_WINDOW:
                {
                    ns_readyState |= ActivityState::FocusState::Window;
                    state->nativeWindow = app->window;

                    break;
                }
                
                case APP_CMD_TERM_WINDOW:
                {
                    ns_readyState &= ~ActivityState::FocusState::Window;
                    state->fullFocus = false;

                    state->nativeWindow = nullptr;

                    if (state->handleSurfaceDestruction)
                        state->handleSurfaceDestruction();

                    break;
                }

                case APP_CMD_WINDOW_RESIZED:
                case APP_CMD_CONFIG_CHANGED:
                {
                    static int lastOrient = -1;

                    if (state->window && lastOrient != AConfiguration_getOrientation(app->config))
                    {
                        state->window->handleResize();
                        lastOrient = AConfiguration_getOrientation(app->config);
                    }

                    break;   
                }

                case APP_CMD_RESUME:
                {
                    ns_readyState |= ActivityState::FocusState::Running;

                    if (state->window)
                        state->window->getEventHandler()->lostFocus();

                    break;
                }

                case APP_CMD_PAUSE:
                case APP_CMD_STOP:
                {
                    ns_readyState &= ~ActivityState::FocusState::Running;
                    state->fullFocus = false;

                    if (state->window)
                        state->window->getEventHandler()->lostFocus();

                    break;
                }

                case APP_CMD_DESTROY:
                {
                    app->onAppCmd           = nullptr;
                    app->onInputEvent       = nullptr;
                    state->pollFunc         = nullptr;
                    state->destroyRequested = true;
                    state->fullFocus        = false;

                    Engine::exit();

                    break;
                }

                case APP_CMD_GAINED_FOCUS:
                {
                    ns_readyState |= ActivityState::FocusState::Focus;
                    //SensorManager::getInstance().gainedFocus();

                    if (state->window)
                        state->window->getEventHandler()->mouseEntered();

                    break;
                }
                case APP_CMD_LOST_FOCUS:
                {
                    ns_readyState &= ~ActivityState::FocusState::Focus;

                    if (state->window)
                        state->window->getEventHandler()->mouseLeft();

                    //SensorManager::getInstance().lostFocus();
                }
            }

            if (!state->fullFocus && ns_readyState == ActivityState::FullFocus)
            {
                state->fullFocus = true;

                if (state->window)
                {
                    state->nativeWindow = app->window;
                    state->handleSurfaceCreation();
                }
            }
        }

        int32_t onInputEvent(struct android_app* app, AInputEvent* event)
        {
            int32_t type = AInputEvent_getType(event);
            int32_t action = AKeyEvent_getAction(event);

            if (type == AINPUT_EVENT_TYPE_KEY)
            { 
                if (action == AKEY_EVENT_ACTION_DOWN)
                    getUnicode(event);

                return onKey(action, event);
            }
            else if (type == AINPUT_EVENT_TYPE_MOTION)
                return onMotion(action, event);

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
            ns_app = app;
            auto state = detail::ActivityState::get();

            JOP_DEBUG_INFO("Android activity started, waiting for window...");

            while (!state->fullFocus)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                pollFunc();
            }

            app->onInputEvent   = onInputEvent;
            state->pollFunc     = pollFunc;

            JOP_DEBUG_INFO("Android activity is ready, entering application main()");

            // Dummy command line arguments. Can't be null
            // because PhysFS wants the first one
            char v = '\0';
            char* argv[] = {&v};
            ::main(1, argv);
        }
    }}

    void android_main(struct android_app* app)
    {
        JOP_DEBUG_INFO("Entered jopnal-main");

        //jop::GooglePlayService::init(app, app->activity);

        app->onAppCmd = jop::detail::onAppCmd;
        app->userData = jop::detail::ActivityState::create(app->activity);

        jop::detail::main(app);

        jop::detail::ActivityState::reset();

        exit(0);
    }

#endif