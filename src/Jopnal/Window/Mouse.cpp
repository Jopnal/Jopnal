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

    #include <Jopnal/Core/Engine.hpp>
    #include<Jopnal/Window/InputEnumsImpl.hpp>

    #ifdef JOP_OS_DESKTOP
        #include <GLFW/glfw3.h>
    #endif

#endif

//////////////////////////////////////////////

namespace
{
    jop::Window* ns_windowRef = nullptr;
    glm::ivec4 ns_restrictions(-1);

    bool validateWindowRef()
    {
        if (!ns_windowRef)
            ns_windowRef = &jop::Engine::getCurrentWindow();

        return ns_windowRef != nullptr;
    }
}

namespace jop
{
    bool Mouse::isButtonDown(const Button button)
    {
        using namespace Input;

    #ifdef JOP_OS_DESKTOP

        if (validateWindowRef())
            return glfwGetMouseButton(ns_windowRef->getLibraryHandle(), getGlMouseButton(button)) == GLFW_PRESS;

    #endif

        return false;
    }

    //////////////////////////////////////////////

    glm::vec2 Mouse::getPosition()
    {
        if (validateWindowRef())
        {
        #ifdef JOP_OS_DESKTOP

            double x, y; 
            glfwGetCursorPos(ns_windowRef->getLibraryHandle(), &x, &y);

            return glm::vec2(x, y);

        #endif
        }

        return glm::vec2(0, 0);
    }

    //////////////////////////////////////////////

    void Mouse::setMouseMode(const Mode mode)
    {
        if (validateWindowRef())
            ns_windowRef->setMouseMode(mode);
    }

    //////////////////////////////////////////////

    void Mouse::setClipping()
    {
    #ifdef JOP_OS_WINDOWS

        if (validateWindowRef())
        {
            glm::ivec2 winPos;
            glfwGetWindowPos(ns_windowRef->getLibraryHandle(), &winPos.x, &winPos.y);

            if (!isClipping())
            {
                glm::ivec2 winSize;
                glfwGetWindowSize(ns_windowRef->getLibraryHandle(), &winSize.x, &winSize.y);

                ns_restrictions = glm::ivec4(0, winSize.x, 0, winSize.y);
            }

            RECT res{winPos.x + ns_restrictions.x, winPos.y + ns_restrictions.z, winPos.x + ns_restrictions.y, winPos.y + ns_restrictions.w};
            ClipCursor(&res);
        }

    #endif
    }

    //////////////////////////////////////////////

    void Mouse::setClipping(const int x, const int y, const int size)
    {
        const int s = size / 2;

        setClipping(glm::ivec4(x - s, x + s, y - s, y + s));
    }

    //////////////////////////////////////////////

    void Mouse::setClipping(const int minX, const int maxX, const int minY, const int maxY)
    {
        setClipping(glm::ivec4(minX, maxX, minY, maxY));
    }

    //////////////////////////////////////////////

    void Mouse::setClipping(const glm::ivec2 x, const glm::ivec2 y)
    {
        setClipping(glm::ivec4(x.x, x.y, y.x, y.y));
    }

    //////////////////////////////////////////////

    void Mouse::setClipping(const glm::ivec4 clipping)
    {
    #ifdef JOP_OS_WINDOWS

        if (validateWindowRef())
        {
            glm::ivec2 winSize = glm::ivec2(0);
            glfwGetWindowSize(ns_windowRef->getLibraryHandle(), &winSize.x, &winSize.y);

            ns_restrictions = clipping;

            if (ns_restrictions.y > winSize.x)
                ns_restrictions.y = winSize.x;
            if (ns_restrictions.x > winSize.x)
                ns_restrictions.x = winSize.x;
            if (ns_restrictions.w > winSize.y)
                ns_restrictions.w = winSize.y;
            if (ns_restrictions.z > winSize.y)
                ns_restrictions.z = winSize.y;

            setClipping();
        }

    #endif
    }

    //////////////////////////////////////////////

    const glm::ivec4& Mouse::getClipping()
    {
        return ns_restrictions;
    }

    //////////////////////////////////////////////

    bool Mouse::isClipping()
    {
    #ifdef JOP_OS_WINDOWS

        if (ns_restrictions.x != -1 && ns_restrictions.y != -1 && ns_restrictions.z != -1 && ns_restrictions.w != -1)
            return true;

    #endif

        return false;
    }

    //////////////////////////////////////////////

    void Mouse::releaseClipping()
    {
    #ifdef JOP_OS_WINDOWS

        ClipCursor(NULL);
        ns_restrictions = glm::ivec4(-1, -1, -1, -1);

    #endif
    }
}