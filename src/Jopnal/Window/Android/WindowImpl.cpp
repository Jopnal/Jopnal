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

#include <Jopnal/Window/Android/WindowImpl.hpp>
#ifdef JOP_OS_ANDROID

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Core/Android/ActivityState.hpp>
    #include <Jopnal/Graphics/OpenGL/EglCheck.hpp>
    #include <android/native_window.h>
    #include <EGL/eglext.h>
    #include <unordered_map>

#endif

//////////////////////////////////////////////


namespace
{
    std::unordered_map<EGLContext, jop::Window*> ns_windowRefs;
    EGLContext ns_shared;
    EGLSurface ns_sharedSurface;

    EGLDisplay getDisplay()
    {
        static EGLDisplay display = EGL_NO_DISPLAY;

        if (display == EGL_NO_DISPLAY)
        {
            display = eglCheck(eglGetDisplay(EGL_DEFAULT_DISPLAY));
        }

        return display;
    }

    void initialize()
    {
        if (ns_windowRefs.empty())
        {
            const EGLint attribs[] =
            {
                EGL_WIDTH,  1,
                EGL_HEIGHT, 1,
                EGL_NONE
            };

            const EGLint configAttribs[] =
            {
                EGL_SURFACE_TYPE,       EGL_PBUFFER_BIT,
                EGL_NONE
            };

            const EGLint version[] =
            {
                EGL_CONTEXT_CLIENT_VERSION, 3,
                EGL_NONE
            };

            eglCheck(eglInitialize(getDisplay(), NULL, NULL));


            EGLConfig config;
            EGLint numConfigs = 0;

            eglCheck(eglChooseConfig(getDisplay(), configAttribs, &config, 1, &numConfigs));


            ns_sharedSurface = eglCheck(eglCreatePbufferSurface(getDisplay(), config, attribs));
            JOP_ASSERT(ns_sharedSurface != EGL_NO_SURFACE, "Failed to create shared context surface!");

            ns_shared = eglCheck(eglCreateContext(getDisplay(), config, nullptr, version));
            JOP_ASSERT(ns_shared != EGL_NO_CONTEXT, "Failed to create shared context!");
        }
    }

    void deInitialize()
    {
        if (ns_windowRefs.empty())
        {
            eglCheck(eglDestroyContext(getDisplay(), ns_shared));
            eglCheck(eglDestroySurface(getDisplay(), ns_sharedSurface));

            eglCheck(eglTerminate(getDisplay()));
        }
    }
}

namespace jop { namespace detail
{
    WindowImpl::WindowImpl(const Window::Settings& settings, Window& windowPtr)
        : m_surface     (EGL_NO_SURFACE),
          m_context     (EGL_NO_CONTEXT),
          m_size        (0),
          m_windowPtr   (&windowPtr)
    {
        initialize();

        auto state = detail::ActivityState::get();
        std::lock_guard<std::mutex> lock(state->mutex);

        if (!state->window)
        {
            const EGLint configAttribs[] =
            {
                EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,

                EGL_RED_SIZE,           8,
                EGL_GREEN_SIZE,         8,
                EGL_BLUE_SIZE,          8,
                EGL_ALPHA_SIZE,         8,

                EGL_DEPTH_SIZE,         16,
                EGL_STENCIL_SIZE,       8,

                EGL_NONE
            };

            const EGLint version[] =
            {
                EGL_CONTEXT_CLIENT_VERSION, 3,
                EGL_NONE
            };

            EGLConfig config;
            EGLint numConfigs = 0;

            eglCheck(eglChooseConfig(getDisplay(), configAttribs, &config, 1, &numConfigs));

            EGLint format;
            eglCheck(eglGetConfigAttrib(getDisplay(), config, EGL_NATIVE_VISUAL_ID, &format));

            ANativeWindow_setBuffersGeometry(state->nativeWindow, 0, 0, format);


            m_surface = eglCheck(eglCreateWindowSurface(getDisplay(), config, state->nativeWindow, NULL));
            JOP_ASSERT(m_surface != EGL_NO_SURFACE, "Failed to create window surface!");

            m_context = eglCheck(eglCreateContext(getDisplay(), config, ns_shared, version));
            JOP_ASSERT(m_context != EGL_NO_CONTEXT, "Failed to create context!");

            EGLBoolean success = eglCheck(eglMakeCurrent(getDisplay(), m_surface, m_surface, m_context));
            JOP_ASSERT(success == EGL_TRUE, "Failed to make context current!");

            eglCheck(eglQuerySurface(getDisplay(), m_surface, EGL_WIDTH, reinterpret_cast<EGLint*>(&m_size.x)));
            eglCheck(eglQuerySurface(getDisplay(), m_surface, EGL_HEIGHT, reinterpret_cast<EGLint*>(&m_size.y)));

            state->window = &windowPtr;
        }
        else
        {
            const EGLint configAttribs[] =
            {
                EGL_SURFACE_TYPE,       EGL_PBUFFER_BIT,
                EGL_NONE
            };

            const EGLint version[] =
            {
                EGL_CONTEXT_CLIENT_VERSION, 3,
                EGL_NONE
            };

            const EGLint surfaceAttribs[] =
            {
                EGL_WIDTH,  1,
                EGL_HEIGHT, 1,
                EGL_NONE
            };

            EGLConfig config;
            EGLint numConfigs = 0;

            eglCheck(eglChooseConfig(getDisplay(), configAttribs, &config, 1, &numConfigs));

            m_surface = eglCheck(eglCreatePbufferSurface(getDisplay(), config, surfaceAttribs));
            JOP_ASSERT(m_surface != EGL_NO_SURFACE, "Failed to create pbuffer surface!");

            m_context = eglCheck(eglCreateContext(getDisplay(), config, ns_shared, version));
            JOP_ASSERT(m_context != EGL_NO_CONTEXT, "Failed to create context!");

            EGLBoolean success = eglCheck(eglMakeCurrent(getDisplay(), m_surface, m_surface, m_context));
            JOP_ASSERT(success == EGL_TRUE, "Failed to make context current!");
        }

        ns_windowRefs[m_context] = &windowPtr;
    }

    WindowImpl::~WindowImpl()
    {
        ns_windowRefs.erase(m_context);

        {
            auto state = detail::ActivityState::get();
            std::lock_guard<std::mutex> lock(state->mutex);

            if (state->window == m_windowPtr)
                state->window = nullptr;
        }

        eglCheck(eglMakeCurrent(getDisplay(), EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));

        if (m_context != EGL_NO_CONTEXT)
        {
            eglCheck(eglDestroyContext(getDisplay(), m_context));
        }

        if (m_surface != EGL_NO_SURFACE)
        {
            eglCheck(eglDestroySurface(getDisplay(), m_surface));
        }

        deInitialize();
    }

    //////////////////////////////////////////////

    void WindowImpl::swapBuffers()
    {
        if (m_surface != EGL_NO_SURFACE)
        {
            eglCheck(eglSwapBuffers(getDisplay(), m_surface));
        }
    }

    //////////////////////////////////////////////

    WindowLibHandle WindowImpl::getLibraryHandle()
    {
        return getNativeHandle();
    }

    //////////////////////////////////////////////

    WindowHandle WindowImpl::getNativeHandle()
    {
        auto state = ActivityState::get();
        std::lock_guard<decltype(state->mutex)> lock(state->mutex);

        return state->nativeWindow;
    }

    //////////////////////////////////////////////

    void WindowImpl::pollEvents()
    {
        ALooper_pollAll(0, NULL, NULL, NULL);
    }

    //////////////////////////////////////////////

    void WindowImpl::setMouseMode(const Mouse::Mode)
    {}

    //////////////////////////////////////////////

    void WindowImpl::setPosition(const int, const int)
    {}

    //////////////////////////////////////////////

    glm::ivec2 WindowImpl::getPosition() const
    {
        return glm::ivec2();
    }

    //////////////////////////////////////////////

    void WindowImpl::setSize(const int, const int)
    {}

    //////////////////////////////////////////////

    glm::uvec2 WindowImpl::getSize() const
    {
        return m_size;
    }

    //////////////////////////////////////////////

    Window* WindowImpl::getCurrentContextWindow()
    {
        return nullptr;
    }
}}

#endif