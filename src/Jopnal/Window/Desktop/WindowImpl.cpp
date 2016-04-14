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
#include <Jopnal/Precompiled.hpp>

#if defined(JOP_OS_WINDOWS)

#include <Jopnal/Window/Desktop/WindowImpl.hpp>

//////////////////////////////////////////////


namespace
{
    unsigned int ns_windowCount = 0;
    GLFWwindow* ns_shared;

    void errorCallback(int code, const char* message)
    {
        const std::string msgStr(message);

        switch (code)
        {
            case GLFW_NOT_INITIALIZED:
                JOP_ASSERT(false, "GLFW not initialized!\n\nDetails: " + msgStr);
                break;
            case GLFW_API_UNAVAILABLE:
                JOP_ASSERT(false, "The requested OpenGL API is not available!\n\nDetails: " + msgStr);
                break;
            case GLFW_VERSION_UNAVAILABLE:
                JOP_ASSERT(false, "The requested OpenGL version is not available on this system!\n\nDetails: " + msgStr);
                break;
            case GLFW_PLATFORM_ERROR:
                JOP_ASSERT(false, "GLFW reported a platform error!\n\nDetails: " + msgStr);
        }

        JOP_DEBUG_ERROR(msgStr);
    }

    void initExtensions()
    {
        static bool init = false;

        if (!init)
        {
            auto result = gl::sys::LoadFunctions();

            JOP_ASSERT(result, "Failed to load OpenGL extensions!");

            if (result.GetNumMissing() > 0)
                JOP_DEBUG_WARNING("Some requested OpenGL extensions failed to load. Missing extensions: " << result.GetNumMissing());

            init = true;
        }
    }

    void initialize()
    {
        if (ns_windowCount++ == 0)
        {
            glfwSetErrorCallback(errorCallback);
            glfwInit();

            glfwWindowHint(GLFW_RED_BITS, 0);
            glfwWindowHint(GLFW_GREEN_BITS, 0);
            glfwWindowHint(GLFW_BLUE_BITS, 0);
            glfwWindowHint(GLFW_ALPHA_BITS, 0);

            glfwWindowHint(GLFW_DEPTH_BITS, 0);
            glfwWindowHint(GLFW_STENCIL_BITS, 0);

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, JOP_OPENGL_VERSION_MAJOR);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, JOP_OPENGL_VERSION_MINOR);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            glfwWindowHint(GLFW_VISIBLE, gl::FALSE_);
            glfwWindowHint(GLFW_DECORATED, gl::FALSE_);

            ns_shared = glfwCreateWindow(1, 1, "", NULL, NULL);
            glfwMakeContextCurrent(ns_shared);

            initExtensions();

            glfwMakeContextCurrent(NULL);
        }
    }

    void deInitialize()
    {
        if (--ns_windowCount == 0)
        {
            glfwDestroyWindow(ns_shared);
            glfwTerminate();
        }
    }
}

namespace jop { namespace detail
{
    WindowImpl::WindowImpl(const Window::Settings& settings)
        : m_window      (nullptr),
          m_vertexArray (0)
    {
        initialize();

        glfwWindowHint(GLFW_RESIZABLE, gl::FALSE_);
        glfwWindowHint(GLFW_VISIBLE, settings.visible);
        glfwWindowHint(GLFW_SAMPLES, settings.samples);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, settings.debug);

        // Pixel format
        glfwWindowHint(GLFW_RED_BITS, 8);
        glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8);
        glfwWindowHint(GLFW_ALPHA_BITS, 8);

        // Depth & stencil exist in the renderer frame buffer
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        glfwWindowHint(GLFW_STENCIL_BITS, 8);

        // Frame rate limit
        glfwWindowHint(GLFW_REFRESH_RATE, settings.maxFrameRate == 0 ? GLFW_DONT_CARE : settings.maxFrameRate);
        
        // Decorated window
        glfwWindowHint(GLFW_DECORATED, settings.displayMode == Window::DisplayMode::Windowed);
        
        m_window = glfwCreateWindow(settings.size.x, settings.size.y, settings.title.c_str(), settings.displayMode != Window::DisplayMode::Fullscreen ? NULL : glfwGetPrimaryMonitor(), ns_shared);

        JOP_ASSERT(m_window != nullptr, "Failed to create window! Title: " + settings.title);

        if (settings.displayMode != Window::DisplayMode::Fullscreen)
        {
            auto vm = glfwGetVideoMode(glfwGetPrimaryMonitor());
            
            int width = 0, height = 0;
            glfwGetWindowSize(m_window, &width, &height);
            glfwSetWindowPos(m_window, vm->width / 2 - width / 2, vm->height / 2 - height / 2);
        }

        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(static_cast<int>(settings.vSync));
        
        glCheck(gl::GenVertexArrays(1, &m_vertexArray));
        glCheck(gl::BindVertexArray(m_vertexArray));
    }

    WindowImpl::~WindowImpl()
    {
        glCheck(gl::BindVertexArray(0));
        glCheck(gl::DeleteVertexArrays(1, &m_vertexArray));

        // There should always be a valid window
        glfwDestroyWindow(m_window);
        deInitialize();
    }

    //////////////////////////////////////////////

    void WindowImpl::swapBuffers()
    {
        glfwSwapBuffers(m_window);
    }

    //////////////////////////////////////////////

    GLFWwindow* WindowImpl::getLibraryHandle()
    {
        return m_window;
    }

    //////////////////////////////////////////////

    WindowHandle WindowImpl::getNativeHandle()
    {
    #ifdef JOP_OS_WINDOWS
        return glfwGetWin32Window(m_window);
    #endif
    }

    //////////////////////////////////////////////

    void WindowImpl::pollEvents()
    {
        glfwPollEvents();
    }

    void WindowImpl::setMouseMode(const Mouse::Mode mode)
    {
        static const int modes[] = 
        {
            GLFW_CURSOR_NORMAL,
            GLFW_CURSOR_HIDDEN,
            GLFW_CURSOR_DISABLED
        };

        glfwSetInputMode(m_window, GLFW_CURSOR, modes[static_cast<int>(mode)]);
    }

    //////////////////////////////////////////////

    void WindowImpl::setPosition(const int x, const int y)
    {
        glfwSetWindowPos(m_window, x, y);
    }

    //////////////////////////////////////////////

    glm::ivec2 WindowImpl::getPosition() const
    {
        glm::ivec2 p;
        glfwGetWindowPos(m_window, &p.x, &p.y);

        return p;
    }

    //////////////////////////////////////////////

    void WindowImpl::setSize(const int width, const int height)
    {
        glfwSetWindowSize(m_window, width, height);
    }

    //////////////////////////////////////////////

    glm::ivec2 WindowImpl::getSize() const
    {
        glm::ivec2 s;
        glfwGetFramebufferSize(m_window, &s.x, &s.y);

        return s;
    }
}}

#endif