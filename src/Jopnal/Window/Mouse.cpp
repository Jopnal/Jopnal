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
    #include <Jopnal/Core/Win32/Win32.hpp>
    #include <Jopnal/Window/InputEnumsImpl.hpp>

    #ifdef JOP_OS_DESKTOP
        #include <GLFW/glfw3.h>
    #endif

#endif

//////////////////////////////////////////////

namespace
{
    jop::Window* ns_windowRef = nullptr;
    std::pair<glm::ivec2, glm::ivec2> ns_restrictions(glm::ivec2(0), glm::ivec2(0));
    bool ns_clipped = false;

    bool validateWindowRef()
    {
        if (!ns_windowRef)
            ns_windowRef = &jop::Engine::getMainWindow();

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
    #ifdef JOP_OS_DESKTOP

        if (validateWindowRef())
        {
            double x, y; 
            glfwGetCursorPos(ns_windowRef->getLibraryHandle(), &x, &y);

            return glm::vec2(x, y);

        }

    #endif

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
            glm::ivec2 winSize;
            glfwGetWindowSize(ns_windowRef->getLibraryHandle(), &winSize.x, &winSize.y);

            setClipping(glm::ivec2(0), winSize);
        }

    #endif
    }

    //////////////////////////////////////////////

    void Mouse::setClipping(const glm::ivec2& min, const glm::ivec2& max)
    {
    #ifdef JOP_OS_WINDOWS

        if (validateWindowRef())
        {
            glm::ivec2 winPos, winSize;
            glfwGetWindowPos(ns_windowRef->getLibraryHandle(), &winPos.x, &winPos.y);
            glfwGetWindowSize(ns_windowRef->getLibraryHandle(), &winSize.x, &winSize.y);

            RECT res
            {
                /* Left   */ glm::clamp(winPos.x + min.x, winPos.x, winPos.x + winSize.x),
                /* Top    */ glm::clamp(winPos.y + min.y, winPos.y, winPos.y + winSize.y),
                /* Right  */ glm::clamp(winPos.x + max.x, winPos.x, winPos.x + winSize.x),
                /* Bottom */ glm::clamp(winPos.y + max.y, winPos.y, winPos.y + winSize.y)
            };

            ClipCursor(&res);
        }

    #endif
    }

    //////////////////////////////////////////////

    const std::pair<glm::ivec2, glm::ivec2>& Mouse::getClipping()
    {
        return ns_restrictions;
    }

    //////////////////////////////////////////////

    bool Mouse::isClipping()
    {
        return ns_clipped;
    }

    //////////////////////////////////////////////

    void Mouse::releaseClipping()
    {
    #ifdef JOP_OS_WINDOWS

        ClipCursor(NULL);
        ns_restrictions.first = ns_restrictions.second = glm::ivec2(0);
        ns_clipped = false;

    #endif
    }
}