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

//////////////////////////////////////////////

namespace
{
	jop::Window* ns_windowRef = nullptr;

	bool  validateWindowRef()
	{
		if (ns_windowRef == nullptr)
		{
			if (jop::Engine::hasCurrentWindow())
			{
				ns_windowRef = &jop::Engine::getCurrentWindow();
				return true;
			}
			JOP_DEBUG_ERROR("Couldn't retrieve window context for input")
				return false;
		}
		return true;
	}
}

namespace jop
{
	bool Mouse::isDown(Button button)
	{
		if (validateWindowRef())
		return ns_windowRef->getEventHandler()->mouseButtonDown(button) == GLFW_PRESS;
	}

	//////////////////////////////////////////////

	glm::vec2 Mouse::scrollOffset()
	{
		if (validateWindowRef())
		{
			auto result = ns_windowRef->getEventHandler()->m_scrollOffset;
			jop::Engine::getSubsystem<jop::Window>()->getEventHandler()->m_scrollOffset = { NULL, NULL };
			return result;
		}
	}

	//////////////////////////////////////////////

	glm::vec2 getPosition()
	{
		if (validateWindowRef())
		return ns_windowRef->getEventHandler()->getCursorPosition();

		return glm::vec2(NULL);
	}
}