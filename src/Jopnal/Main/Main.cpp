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
    #include <Jopnal/Window/Android/WindowImpl.hpp>
    #include <thread>
    #include <atomic>  

    namespace jop { namespace detail
    {

        android_app* ns_app=nullptr;

        JavaVM* ns_virtualMachine = nullptr;
        JNIEnv* ns_environment = nullptr;
        jobject ns_nativeActivity =nullptr;
        JavaVMAttachArgs ns_vmArgs;

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
                if(action==AKEY_EVENT_ACTION_DOWN)
                    getUnicode(event, ns_virtualMachine, ns_environment, &ns_vmArgs);

                return  onKey(action, event);
			}
            else if (type == AINPUT_EVENT_TYPE_MOTION)
            {
                return  onMotion(action, event);
            }
            else 
                JOP_DEBUG_INFO("Unknown input type");

            return 0;
        }

        void showVirtualKeyboard(const bool show)
        {
            showVirtualKeyboard(show, ns_virtualMachine, ns_environment, &ns_nativeActivity, &ns_vmArgs);
        }

        void initJavaEnvironment()
        {
            ns_virtualMachine = ns_app->activity->vm;
            ns_environment = ns_app->activity->env;
            ns_nativeActivity = ns_app->activity->clazz;

            ns_vmArgs.version = JNI_VERSION_1_6;
            ns_vmArgs.name = "nativeThread";
            ns_vmArgs.group = NULL;
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
            ActivityState::get()->showVirtualKeyboard=&showVirtualKeyboard;
            initJavaEnvironment();

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