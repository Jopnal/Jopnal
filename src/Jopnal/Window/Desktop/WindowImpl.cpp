// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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
    {
        initialize();

        glfwWindowHint(GLFW_RESIZABLE, 0);
        glfwWindowHint(GLFW_VISIBLE, settings.visible);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, JOP_OPENGL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, JOP_OPENGL_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
        m_window = glfwCreateWindow(settings.size.x, settings.size.y, settings.title.c_str(), settings.displayMode == Window::DisplayMode::Fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

        JOP_ASSERT(m_window != nullptr, "Failed to create window! Title: " + settings.title);

        glfwMakeContextCurrent(m_window);

        initExtensions();
    }

    WindowImpl::~WindowImpl()
    {
        // There should always be a valid window
        glfwDestroyWindow(m_window);
        deInitialize();
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