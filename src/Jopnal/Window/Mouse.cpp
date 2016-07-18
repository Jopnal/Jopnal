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

#ifdef JOP_OS_DESKTOP
#include <Windows.h>
#endif

#include <Jopnal/Core/Engine.hpp>
#include<Jopnal/Window/InputEnumsImpl.hpp>

//////////////////////////////////////////////

namespace
{
    jop::Window* ns_windowRef = nullptr;
    glm::ivec4 ns_restrictions = { -1, -1, -1, -1};

	bool  validateWindowRef()
	{
		if (ns_windowRef == nullptr)
			ns_windowRef = &jop::Engine::getCurrentWindow();

		return ns_windowRef != nullptr;
	}
}

namespace jop
{
	bool Mouse::isButtonDown(Button button)
	{
		using namespace input;

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

    glm::vec2 Mouse::getPosition()
	{
		if (validateWindowRef())
		{
         #if defined(JOP_OS_DESKTOP)
			double x, y = NULL; 
			glfwGetCursorPos(ns_windowRef->getLibraryHandle(),&x,&y);
			return glm::vec2(x,y);
        #elif defined(JOP_OS_ANDROID)
			return glm::vec2(0,0);
        #else
			return glm::vec2(0,0);
        #endif
		}
		return glm::vec2(0,0);
	}

    //////////////////////////////////////////////

    void Mouse::setMouseMode(const Mode mode)
    {
        if (validateWindowRef())
        {
            ns_windowRef->setMouseMode(mode);
        }
    }

    //////////////////////////////////////////////

    void Mouse::setClipping()
    {
      #if defined(JOP_OS_DESKTOP)
                if (validateWindowRef())
        {
            glm::ivec2 winPos = {NULL,NULL};
            glfwGetWindowPos(ns_windowRef->getLibraryHandle(), &winPos.x, &winPos.y);

            if (!isClipping())
            {
                glm::ivec2 winSize = { NULL, NULL };
                glfwGetWindowSize(ns_windowRef->getLibraryHandle(), &winSize.x, &winSize.y);
                ns_restrictions = { 0, winSize.x, 0, winSize.y };
            }
            RECT res{ winPos.x + ns_restrictions.x, winPos.y + ns_restrictions.z, winPos.x + ns_restrictions.y, winPos.y + ns_restrictions.w };
            ClipCursor(&res);
        }
       #elif defined(JOP_OS_ANDROID)
        JOP_DEBUG_INFO("Android os doens't have ability to restrict mouse clipping")
        return;
       #else
        JOP_DEBUG_INFO("This operation system doens't have ability to restrict mouse clipping")
        return;
       #endif
    }

    //////////////////////////////////////////////

    void Mouse::setClipping(int x, int y, int size)
    {
        int s = size / 2;
        setClipping(glm::ivec4(x-s, x+s, y-s, y+s));
    }

    //////////////////////////////////////////////

    void Mouse::setClipping(int minX, int maxX, int minY, int maxY)
    {
        setClipping(glm::ivec4(minX, maxX, minY, maxY));
    }

    //////////////////////////////////////////////

    void Mouse::setClipping(glm::ivec2 x, glm::ivec2 y)
    {
        setClipping(glm::ivec4(x.x, x.y, y.x, y.y));
    }

    //////////////////////////////////////////////

    void Mouse::setClipping(glm::ivec4 clipping)
    {
        #if defined(JOP_OS_DESKTOP)
        if (validateWindowRef())
        {
            glm::ivec2 winSize = { NULL, NULL };
            glfwGetWindowSize(ns_windowRef->getLibraryHandle(), &winSize.x, &winSize.y);

            if (clipping.y > winSize.x)
                clipping.y = winSize.x;
            if (clipping.x > winSize.x)
                clipping.x = winSize.x;
            if (clipping.w > winSize.y)
                clipping.w = winSize.y;
            if (clipping.z > winSize.y)
                clipping.z = winSize.y;

            ns_restrictions = clipping;
            setClipping();
        }
        #elif defined(JOP_OS_ANDROID)
        JOP_DEBUG_INFO("Android os doens't have ability to restrict mouse clipping")
        return;
        #else
        JOP_DEBUG_INFO("This operation system doens't have ability to restrict mouse clipping")
        return;
        #endif
    }

    //////////////////////////////////////////////

    glm::ivec4 Mouse::getClipping()
    {
        return ns_restrictions;
    }

    //////////////////////////////////////////////

    bool Mouse::isClipping()
    {
        if (ns_restrictions.x != -1 && ns_restrictions.y != -1 && ns_restrictions.z != -1 && ns_restrictions.w != -1)
            return true;

        return false;
    }

    //////////////////////////////////////////////

    void Mouse::releaseClipping()
    {
        #if defined(JOP_OS_DESKTOP)
        ClipCursor(NULL);
        ns_restrictions = { -1, -1, -1, -1};
        #elif defined(JOP_OS_ANDROID)
        JOP_DEBUG_INFO("Android os doens't have ability to restrict mouse clipping")
        ns_restrictions = { -1, -1, -1, -1};
        return;
        #else
        JOP_DEBUG_INFO("This operation system doens't have ability to restrict mouse clipping")
        return;
        #endif
    }
}