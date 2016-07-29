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

    #include <Jopnal/Window/Controller.hpp>

    #include <Jopnal/Core/Android/ActivityState.hpp>
    #include <Jopnal/Core/SettingManager.hpp>
    #include <Jopnal/Core/Engine.hpp>
    #include <Jopnal/Window/InputEnumsImpl.hpp>

    #ifdef JOP_OS_DESKTOP
        #include <GLFW/glfw3.h>
    #endif

#endif

//////////////////////////////////////////////


namespace
{
    int ns_maxControllers = 0;
    float ns_deadzone = 0.f;
    unsigned int ns_counter = 0;

    jop::Window* ns_windowRef = nullptr;

    bool validateWindowRef()
    {
        if (!ns_windowRef)
            ns_windowRef = &jop::Engine::getCurrentWindow();

        return ns_windowRef != nullptr;
    }
}

namespace jop
{
    int Controller::controllersPresent()
    {
    #ifdef JOP_OS_DESKTOP

        if (validateWindowRef())
        {
            int count = 0;

            ns_maxControllers = static_cast<int>(std::min(static_cast<unsigned int>(GLFW_JOYSTICK_LAST), SettingManager::get<unsigned int>("engine@Input|Controller|uMaxControllers", 1u)));
            ns_deadzone = SettingManager::get<float>("engine@Input|Controller|fDeadzone", 0.1f);
            ns_counter = 99;

            for (int i = 0; i < ns_maxControllers; ++i)
            {
                if (glfwJoystickPresent(i) == GL_TRUE)
                    count += 1;
            }

            return count;
        }

    #endif

        return 0;
    }

    //////////////////////////////////////////////

    bool Controller::isControllerPresent(const int index)
    {
    #ifdef JOP_OS_DESKTOP

        return glfwJoystickPresent(index) == GL_TRUE;

    #else

        index;
        return false;

    #endif
    }

    //////////////////////////////////////////////

    bool Controller::isButtonDown(const int index, const int button)
    {
        if (validateWindowRef())
        {
        #if defined(JOP_OS_DESKTOP)

            int count = 0;
            const unsigned char* buttons = glfwGetJoystickButtons(index, &count);

            if (button < count)
                return buttons[button] == GLFW_PRESS;

        #elif defined(JOP_OS_ANDROID)

            auto state = detail::ActivityState::get();

            const bool result = state->activeKey == button;
            state->activeKey=-1;

            return result;

        #endif
        }
        
        return false;
    }

    //////////////////////////////////////////////

    glm::vec2 Controller::leftStickOffset(const int index)
    {
        if (validateWindowRef())
        {
        #if defined(JOP_OS_DESKTOP)

            int count = 0;
            const float* axes = glfwGetJoystickAxes(index, &count);
            if (count == 4)
                return glm::vec2(axes[0], axes[1]);

        #elif defined(JOP_OS_ANDROID)

            glm::vec2 result={detail::ActivityState::get()->activeAxes[0],detail::ActivityState::get()->activeAxes[1]};
            detail::ActivityState::get()->activeAxes[0]=0.f;
            detail::ActivityState::get()->activeAxes[1]=0.f;

            return result;

        #endif
        }

        return glm::vec2(0,0);
    }

    //////////////////////////////////////////////

    glm::vec2 Controller::rightStickOffset(const int index)
    {
        if (validateWindowRef())
        {
        #if defined(JOP_OS_DESKTOP)

            int count = 0;
            const float* axes = glfwGetJoystickAxes(index, &count);
            if (count == 4)
                return glm::vec2(axes[2], axes[3]);

        #elif defined(JOP_OS_ANDROID)

            using namespace Input;
            glm::vec2 result={detail::ActivityState::get()->activeAxes[2],detail::ActivityState::get()->activeAxes[3]};
            detail::ActivityState::get()->activeAxes[2]=0.f;
            detail::ActivityState::get()->activeAxes[3]=0.f;

            return result;
        #endif
        }
        return glm::vec2(0,0);
    }

}