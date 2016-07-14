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

    #include <Jopnal/Window/WindowEventHandler.hpp>
    #include <Jopnal/Window/Window.hpp>
    #include <array>
    #include <vector>

    #ifdef JOP_OS_DESKTOP
    #include <GLFW/glfw3.h>
    #endif

#endif

#include<Jopnal/Window/InputEnumsImpl.hpp>

//////////////////////////////////////////////

namespace jop
{
    WindowEventHandler::WindowEventHandler(Window& windowRef)
        : m_windowRef(windowRef)
    {
#ifdef JOP_OS_DESKTOP

        auto handle = windowRef.getLibraryHandle();
        glfwSetWindowUserPointer(handle, this);

        // Close callback
        glfwSetWindowCloseCallback(handle, [](GLFWwindow* w)
        {
            static_cast<WindowEventHandler*>(glfwGetWindowUserPointer(w))->closed();
        });

        // Lost/restored focus callback
        glfwSetWindowIconifyCallback(handle, [](GLFWwindow* w, int iconified)
        {
            auto handler = static_cast<WindowEventHandler*>(glfwGetWindowUserPointer(w));

            if (iconified)
                handler->lostFocus();
            else
                handler->gainedFocus();
        });

        // Keyboard callback
        glfwSetKeyCallback(handle, [](GLFWwindow* w, int key, int scancode, int action, int mods)
        {
            auto handler = static_cast<WindowEventHandler*>(glfwGetWindowUserPointer(w));
            using namespace input;

            switch (action)
            {
            case GLFW_REPEAT:
            case GLFW_PRESS:
                handler->keyPressed(getJopKey(key), scancode, mods);
                break;
            case GLFW_RELEASE:
                handler->keyReleased(getJopKey(key), scancode, mods);
            }
        });

        // Text callback
        glfwSetCharCallback(handle, [](GLFWwindow* w, unsigned int codepoint)
        {
            static_cast<WindowEventHandler*>(glfwGetWindowUserPointer(w))->textEntered(codepoint);
        });

        // Mouse position callback
        // First set the initial mouse position
        double x = 0.0, y = 0.0;
        glfwGetCursorPos(handle, &x, &y);
        m_lastMouseX = static_cast<float>(x);
        m_lastMouseY = static_cast<float>(y);

        glfwSetCursorPosCallback(handle, [](GLFWwindow* w, double x, double y)
        {
            auto h = static_cast<WindowEventHandler*>(glfwGetWindowUserPointer(w));

            double realX = h->m_lastMouseX;
            double realY = h->m_lastMouseY;

            if (glfwGetInputMode(w, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
            {
                realX -= x;
                realY -= y;
            }
            else
            {
                realX = -x;
                realY = -y;
            }

            h->mouseMoved(static_cast<float>(-realX), static_cast<float>(-realY));
            h->mouseMovedApsolute(static_cast<float>(x), static_cast<float>(x));

            h->m_lastMouseX = static_cast<float>(x);
            h->m_lastMouseY = static_cast<float>(y);
        });

        // Mouse button callback
        glfwSetMouseButtonCallback(handle, [](GLFWwindow* w, int button, int action, int mods)
        {
            auto handler = static_cast<WindowEventHandler*>(glfwGetWindowUserPointer(w));
            using namespace input;

            switch (action)
            {
            case GLFW_PRESS:
                handler->mouseButtonPressed(getJopMouseButton(button), mods);
                break;
            case GLFW_RELEASE:
                handler->mouseButtonReleased(getJopMouseButton(button), mods);
            }
        });

        // Mouse left/entered callback
        glfwSetCursorEnterCallback(handle, [](GLFWwindow* w, int entered)
        {
            auto handler = static_cast<WindowEventHandler*>(glfwGetWindowUserPointer(w));

            if (entered)
                handler->mouseEntered();
            else
                handler->mouseLeft();
        });

        // Scroll callback
        glfwSetScrollCallback(handle, [](GLFWwindow* w, double x, double y)
        {
            static_cast<WindowEventHandler*>(glfwGetWindowUserPointer(w))->mouseScrolled(static_cast<const float>(x), static_cast<const float>(y));
        });

        // Frame buffer size change
        glfwSetFramebufferSizeCallback(handle, [](GLFWwindow* w, int x, int y)
        {
            static_cast<WindowEventHandler*>(glfwGetWindowUserPointer(w))->resized(x, y);
        });

#endif
    }

    WindowEventHandler::~WindowEventHandler()
    {
#ifdef JOP_OS_DESKTOP

        auto handle = m_windowRef.getLibraryHandle();

        glfwSetWindowCloseCallback(handle, NULL);
        glfwSetWindowIconifyCallback(handle, NULL);
        glfwSetKeyCallback(handle, NULL);
        glfwSetCharCallback(handle, NULL);
        glfwSetCursorPosCallback(handle, NULL);
        glfwSetMouseButtonCallback(handle, NULL);
        glfwSetCursorEnterCallback(handle, NULL);
        glfwSetScrollCallback(handle, NULL);
        glfwSetFramebufferSizeCallback(handle, NULL);

#endif
    }

    //////////////////////////////////////////////

    void WindowEventHandler::closed()
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::resized(const unsigned int, const unsigned int)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::lostFocus()
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::gainedFocus()
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::keyPressed(const int, const int, const int)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::keyReleased(const int, const int, const int)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::textEntered(const unsigned int)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::mouseMoved(const float, const float)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::mouseMovedApsolute(const float x, const float y)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::mouseButtonPressed(const int, const int)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::mouseButtonReleased(const int, const int)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::mouseLeft()
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::mouseEntered()
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::mouseScrolled(const float x, const float y)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::controllerConnected(const int, const std::string&)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::controllerDisconnected(const int)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::controllerAxisShifted(const int, const int, const float)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::controllerButtonPressed(const int, const int)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::controllerButtonReleased(const int, const int)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::touchEvent(const int touchId, const float x, const float y)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::touchReleased(const int touchId, const float x, const float y)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::touchMoved(const int touchId, const float x, const float y)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::touchMovedApsolute(const int touchId, const float x, const float y)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::touchScrolled(const float x, const float y)
    {}
}