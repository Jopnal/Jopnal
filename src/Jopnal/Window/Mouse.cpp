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

#include <Jopnal/Window/Mouse.hpp>

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
	bool Mouse::isDown(Button button)
	{
		if (validateWindowRef())
		{
        #if defined(JOP_OS_DESKTOP)
			return glfwGetMouseButton(ns_windowRef->getLibraryHandle(), getGlButton(button)) == GLFW_PRESS;
        #elif defined(JOP_OS_ANDROID)
			return false;
        #else
			return false;
        #endif
		}
		return false;
	}

	//////////////////////////////////////////////

	glm::vec2 Mouse::scrollOffset()
	{
		if (validateWindowRef())
		{
        #if defined(JOP_OS_DESKTOP)
			auto result = ns_windowRef->getEventHandler()->m_scrollOffset;
			jop::Engine::getSubsystem<jop::Window>()->getEventHandler()->m_scrollOffset = { NULL, NULL };
			return result;
        #elif defined(JOP_OS_ANDROID)
			return glm::vec2(NULL);
        #else
			return glm::vec2(NULL);
        #endif
		}
		return glm::vec2(NULL);
	}

	//////////////////////////////////////////////

	glm::vec2 getPosition()
	{
		if (validateWindowRef())
		{
         #if defined(JOP_OS_DESKTOP)
			double x, y = NULL; 
			glfwGetCursorPos(ns_windowRef->getLibraryHandle(),&x,&y);
			return glm::vec2(x,y);
        #elif defined(JOP_OS_ANDROID)
			return glm::vec2(NULL);
        #else
			return glm::vec2(NULL);
        #endif
		}
		return glm::vec2(NULL);
	}
}