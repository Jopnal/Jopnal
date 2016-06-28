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
#include <Jopnal/Core/DebugHandler.hpp>


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
    #include <Jopnal/Utility/Thread.hpp>

    namespace jop { namespace detail
    {
        void startMain(ActivityState* state)
        {
            std::lock_guard<decltype(state->mutex)> lock(state->mutex);

            state->looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);

            state->configuration = AConfiguration_new();
            AConfiguration_fromAssetManager(state->configuration, state->nativeActivity->assetManager);

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        void endMain(ActivityState* state)
        {
            std::lock_guard<decltype(state->mutex)> lock(state->mutex);

            state->mainDone = true;
            ANativeActivity_finish(state->nativeActivity);
        }

        void main(ActivityState* state)
        {
            startMain(state);

            ::main(0, NULL);

            endMain(state);

            state->terminated = true;
        }
    }}

    void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize)
    {
        JOP_DEBUG_INFO("Entered Android entry point");

        auto state = new jop::detail::ActivityState(activity, savedState, savedStateSize);

        jop::Thread mainThread(jop::detail::main, state);
        mainThread.setPriority(jop::Thread::Priority::Highest);
        mainThread.detach();

        while (!state->init.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

        activity->instance = state;
    }

#endif