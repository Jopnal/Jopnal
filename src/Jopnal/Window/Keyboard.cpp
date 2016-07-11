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

    #include <Jopnal/Window/Keyboard.hpp>

    #ifdef JOP_OS_DESKTOP
        #include <GLFW/glfw3.h>
    #endif

#endif

#include<Jopnal/Window/InputEnumsImpl.hpp>
#include <Jopnal/Core/Engine.hpp>

//////////////////////////////////////////////

namespace
{
	jop::Window* ns_windowRef=nullptr;

	bool  validateWindowRef()
	{
		if (ns_windowRef == nullptr)
				ns_windowRef = &jop::Engine::getCurrentWindow();

				return ns_windowRef != nullptr;
	}
}

namespace jop
{
    std::string Keyboard::getKeyName(const int scanCode)
    {
    #if defined(JOP_OS_DESKTOP)
        return std::string(glfwGetKeyName(GLFW_KEY_UNKNOWN, scanCode));
    #else
        return std::string("UNKNOWN");
    #endif
    }

	//////////////////////////////////////////////

	bool Keyboard::isKeyDown(Key key)
	{
		using namespace input;

		if (validateWindowRef())
		{
         #if defined(JOP_OS_DESKTOP)
			return glfwGetKey(ns_windowRef->getLibraryHandle(), getGlKey(key)) == GLFW_PRESS;
         #elif defined(JOP_OS_ANDROID)
			return false;
         #else
			return false;
         #endif
		}
		return false;
	}
}