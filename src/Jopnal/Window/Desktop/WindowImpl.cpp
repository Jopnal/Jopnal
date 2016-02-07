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

    void initialize()
    {
        if (ns_windowCount++ == 0)
        {
            glfwSetErrorCallback(errorCallback);
            glfwInit();
        }
    }

    void deInitialize()
    {
        if (--ns_windowCount == 0)
            glfwTerminate();
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
}

namespace jop { namespace detail
{
    WindowImpl::WindowImpl(const Window::Settings& settings)
        : m_window      (nullptr),
          m_vertexArray (0)
    {
        initialize();

        glfwWindowHint(GLFW_RESIZABLE, 0);
        glfwWindowHint(GLFW_VISIBLE, settings.visible);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, JOP_OPENGL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, JOP_OPENGL_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Decorated window
        glfwWindowHint(GLFW_DECORATED, settings.displayMode == Window::DisplayMode::Windowed);
        
        m_window = glfwCreateWindow(settings.size.x, settings.size.y, settings.title.c_str(), settings.displayMode == Window::DisplayMode::Windowed ? NULL : glfwGetPrimaryMonitor(), NULL);

        JOP_ASSERT(m_window != nullptr, "Failed to create window! Title: " + settings.title);

        glfwMakeContextCurrent(m_window);

        initExtensions();

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

    void WindowImpl::pollEvents()
    {
        glfwPollEvents();
    }
}}

#endif