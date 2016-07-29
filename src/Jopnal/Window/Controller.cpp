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

#ifdef JOP_OS_DESKTOP
#include <GLFW/glfw3.h>
#endif

#endif

#ifdef JOP_OS_ANDROID
#include <Jopnal/Core/Android/ActivityState.hpp>
#endif

#include <Jopnal/Core/SettingManager.hpp>
#include <Jopnal/Core/Engine.hpp>
#include<Jopnal/Window/InputEnumsImpl.hpp>

//////////////////////////////////////////////

namespace
{
    int ns_maxControllers = 0;
    float ns_deadzone = 0.f;
    unsigned int ns_counter = 0;

	jop::Window* ns_windowRef = nullptr;

	bool validateWindowRef()
	{
		if (ns_windowRef == nullptr)
			ns_windowRef = &jop::Engine::getCurrentWindow();

		return ns_windowRef != nullptr;
	}
}

namespace jop
{
	int Controller::controllersPresent()
	{
		if (validateWindowRef())
		{
		int count = 0;  
       #if defined(JOP_OS_DESKTOP)
        ns_maxControllers = static_cast<int>(std::min(static_cast<unsigned int>(GLFW_JOYSTICK_LAST), SettingManager::get<unsigned int>("engine@Input|Controller|uMaxControllers", 1u)));
        ns_deadzone = SettingManager::get<float>("engine@Input|Controller|fDeadzone", 0.1f);
        ns_counter = 99;
		for (int i = 0; i < ns_maxControllers && (++ns_counter % 100) == 0; ++i)
		{
			if (glfwJoystickPresent(i) == GL_TRUE)
				count += 1;
		}
		return count;
        #elif defined(JOP_OS_ANDROID)
        return detail::ActivityState::get()->activeController;
        #endif
		}
		return 0;;
	}

	//////////////////////////////////////////////

	bool Controller::iscontrollerPresent(const int index)
	{
		if (validateWindowRef())
		{
        #if defined(JOP_OS_DESKTOP)
			return glfwJoystickPresent(index) == GL_TRUE;
        #elif defined(JOP_OS_ANDROID)
            if(index==0)
            return detail::ActivityState::get()->activeController;
        #endif
		}
		return false;
	}

	//////////////////////////////////////////////

	bool Controller::isButtonDown(const int index, const int Button)
	{
		if (validateWindowRef())
		{
        #if defined(JOP_OS_DESKTOP)
			int count = 0;
			const unsigned char* buttons = glfwGetJoystickButtons(index, &count);
			if (Button < count)
				return buttons[Button] == GLFW_PRESS;

			JOP_DEBUG_WARNING("Controller " << index << " has only " << count << " buttons")
				return false;
        #elif defined(JOP_OS_ANDROID)
            using namespace input;
            bool result = detail::ActivityState::get()->activeKey == Button;
            return result;
        #endif
		}
		 return false;
	}

	//////////////////////////////////////////////

    float Controller::getAxisOffset(const int index, const int axis)
	{
		if (validateWindowRef())
		{
        #if defined(JOP_OS_DESKTOP)

            int count = 0;
			const float* axes = glfwGetJoystickAxes(index, &count);

            switch (axis)
            {
            case XBox::Axis::LeftStickX:
                return axes[0];
            case XBox::Axis::LeftStickY:
                return axes[1];
            case XBox::Axis::RightStickX:
                return axes[2];
            case XBox::Axis::RightStickY:
                return axes[3];
            case XBox::Axis::LTrigger:
            {
                if (count >= 6)
                    return ((axes[4]+1)/2.f);
                else
                    break;
            }
            case XBox::Axis::RTrigger:
            {
                if (count >= 6)
                    return ((axes[5] + 1) / 2.f);
                else
                    break;
            }
            default:
                break;
            }
        #elif defined(JOP_OS_ANDROID)
            switch (axis)
            {
            case XBox::Axis::LeftStickX:
            return detail::ActivityState::get()->activeAxes[0];
            case XBox::Axis::LeftStickY:
            return detail::ActivityState::get()->activeAxes[1];
            case XBox::Axis::RightStickX:
            return detail::ActivityState::get()->activeAxes[2];
            case XBox::Axis::RightStickY:
            return detail::ActivityState::get()->activeAxes[3];
            case XBox::Axis::LTrigger:
            return detail::ActivityState::get()->activeAxes[4];
            case XBox::Axis::RTrigger:
            return detail::ActivityState::get()->activeAxes[5];
            default:
                break;
            }
        #endif
			return 0.f;
		}
		return 0.f;
	}
}