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

#if defined(JOP_OS_DESKTOP)
#include<Jopnal\Window\Desktop\InputEnumsImpl.cpp>
#elif defined(JOP_OS_ANDROID)
#include<Jopnal\Window\Desktop\InputEnumsImpl.cpp>
#endif

//////////////////////////////////////////////

namespace
{
	jop::Window* ns_windowRef = nullptr;

	bool  validateWindowRef()
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
		static const int maxControllers = static_cast<int>(std::min(unsigned int(GLFW_JOYSTICK_LAST), SettingManager::get<unsigned int>("engine@Input|Controller|uMaxControllers", 1)));
		static const float deadzone = SettingManager::get<float>("engine@Input|Controller|fDeadzone", 0.1f);
		static unsigned int counter = 99;
      
       #if defined(JOP_OS_DESKTOP)
		for (int i = 0; i < maxControllers && (++counter % 100) == 0; ++i)
		{
			if (glfwJoystickPresent(i) == GL_TRUE)
				count += 1;
		}
		return count;
        #elif defined(JOP_OS_ANDROID)
		return NULL;
        #else
		return NULL;
        #endif
		}
		return NULL;;
	}

	//////////////////////////////////////////////

	bool Controller::iscontrollerPresent(const int index)
	{
		if (validateWindowRef())
		{
        #if defined(JOP_OS_DESKTOP)
			return glfwJoystickPresent(index) == GL_TRUE;
        #elif defined(JOP_OS_ANDROID)
			return false;
        #else
			return false;
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
			return false;
        #else
			return false;
        #endif
		}
		 return false;
	}

	//////////////////////////////////////////////

	glm::vec2 Controller::rightStickOffset(const int index)
	{
		if (validateWindowRef())
		{
        #if defined(JOP_OS_DESKTOP)
			int count = 0;
			const float* axes = glfwGetJoystickAxes(index, &count);
			if (count = 4)
				return glm::vec2(axes[2], axes[3]);
			else
				return glm::vec2(NULL);
        #elif defined(JOP_OS_ANDROID)
			return glm::vec2(NULL);
        #else
			return glm::vec2(NULL);
        #endif
			return glm::vec2(NULL);
		}
		return glm::vec2(NULL);
	}

	//////////////////////////////////////////////

	glm::vec2 Controller::leftStickOffset(const int index)
	{
		if (validateWindowRef())
		{
        #if defined(JOP_OS_DESKTOP)
			int count = 0;
			const float* axes = glfwGetJoystickAxes(index, &count);
			if (count = 4)
				return glm::vec2(axes[0], axes[1]);
			else
				return glm::vec2(NULL);
        #elif defined(JOP_OS_ANDROID)
			return glm::vec2(NULL);
        #else
			return glm::vec2(NULL);
        #endif
			return glm::vec2(NULL);
		}
		return glm::vec2(NULL);
	}

}