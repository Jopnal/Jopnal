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
    #include <Jopnal/Core/DebugHandler.hpp>
    #include <Jopnal/Graphics/OpenGL/EglCheck.hpp>
    #include <android/looper.h>
    #include <unordered_map>

#endif

//////////////////////////////////////////////


namespace
{
    std::unordered_map<EGLContext, jop::Window*> ns_windows;
    jop::detail::WindowImpl* ns_instance = nullptr;
    EGLSurface ns_sharedSurface = EGL_NO_SURFACE;
    EGLContext ns_shared = EGL_NO_CONTEXT;

    void createSurface(EGLSurface* surface, EGLDisplay display, EGLConfig config, EGLNativeWindowType window)
    {
        *surface = eglCheck(eglCreateWindowSurface(display, config, window, NULL));

        JOP_DEBUG_INFO("Window surface created");
    }

    void destroySurface(EGLSurface* surface, EGLDisplay display)
    {
        eglCheck(eglDestroySurface(display, *surface));
        *surface = EGL_NO_SURFACE;

        JOP_DEBUG_INFO("Window surface destroyed");
    }

    EGLDisplay getDisplay()
    {
        auto state = jop::detail::ActivityState::get();
        std::lock_guard<decltype(state->mutex)> lock(state->mutex);

        return state->display;
    }

    void initialize(EGLConfig config, const EGLint* version, const EGLint* attribs)
    {
        if (!ns_shared)
        {
            ns_sharedSurface = eglCheck(eglCreatePbufferSurface(getDisplay(), config, attribs));
            JOP_ASSERT(ns_sharedSurface != EGL_NO_SURFACE, "Failed to create shared context surface!");

            ns_shared = eglCheck(eglCreateContext(getDisplay(), config, EGL_NO_CONTEXT, version));
            JOP_ASSERT(ns_shared != EGL_NO_CONTEXT, "Failed to create shared context!");
        }
    }

    void deInitialize()
    {
        if (ns_windows.empty())
        {
            eglCheck(eglDestroyContext(getDisplay(), ns_shared));
            ns_shared = EGL_NO_CONTEXT;

            eglCheck(eglDestroySurface(getDisplay(), ns_sharedSurface));
            ns_sharedSurface = EGL_NO_SURFACE;
        }
    }
}

namespace jop { namespace detail
{
    WindowImpl::WindowImpl(const Window::Settings& settings, Window& windowPtr)
        : m_display         (getDisplay()),
          m_context         (EGL_NO_CONTEXT),
          m_surface         (EGL_NO_SURFACE),
          m_config          (getConfig(settings)),
          m_size            (0),
          m_focusRestored   (false),
          m_focusLost       (false)
    {
        const EGLint version[] =
        {
            EGL_CONTEXT_CLIENT_VERSION, 3,
            EGL_NONE
        };
        const EGLint attribs[] =
        {
            EGL_WIDTH,  1,
            EGL_HEIGHT, 1,
            EGL_NONE
        };

        initialize(m_config, version, attribs);

        if (!ns_instance)
        {
            auto state = detail::ActivityState::get();
            std::lock_guard<decltype(state->mutex)> lock(state->mutex);

            state->fullscreen = settings.displayMode == Window::DisplayMode::Fullscreen;


            //JOP_DEBUG_INFO("Creating surface");
            //createSurface(&m_surface, m_display, m_config, reinterpret_cast<EGLNativeWindowType>(getNativeHandle()));

            

            ns_instance = this;
        }
        else
        {
            m_surface = eglCheck(eglCreatePbufferSurface(m_display, m_config, attribs));
            JOP_ASSERT(m_surface != EGL_NO_SURFACE, "Failed to create context surface!");
        }

        m_context = eglCheck(eglCreateContext(m_display, m_config, ns_shared, version));
        JOP_ASSERT(m_context != EGL_NO_CONTEXT, "Failed to create context!");

        //EGLBoolean success = eglCheck(eglMakeCurrent(m_display, m_surface, m_surface, m_context));
        //JOP_ASSERT(success == EGL_TRUE, "Failed to set context current!");

        ns_windows[m_context] = &windowPtr;
    }

    WindowImpl::~WindowImpl()
    {
        if (eglGetCurrentContext() == m_context)
        {
            eglCheck(eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));
        }

        if (m_context != EGL_NO_CONTEXT)
        {
            ns_windows.erase(m_context);

            eglCheck(eglDestroyContext(m_display, m_context));
        }

        if (m_surface != EGL_NO_SURFACE)
        {
            eglCheck(eglDestroySurface(m_display, m_surface));
        }

        if (ns_instance == this)
            ns_instance = nullptr;

        deInitialize();
    }

    //////////////////////////////////////////////

    void WindowImpl::swapBuffers()
    {
        if (m_surface != EGL_NO_SURFACE)
        {
            eglCheck(eglSwapBuffers(m_display, m_surface));
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

        auto state = ActivityState::get();

        auto& inst = *ns_instance;

        if (inst.m_focusRestored)
        {
            createSurface(&inst.m_surface, inst.m_display, inst.m_config, state->nativeWindow);
            eglCheck(eglMakeCurrent(inst.m_display, inst.m_surface, inst.m_surface, inst.m_context));

            inst.m_focusRestored = false;
        }
        if (inst.m_focusLost)
        {
            destroySurface(&inst.m_surface, inst.m_display);
            eglCheck(eglMakeCurrent(inst.m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));
            inst.m_focusLost = false;
        }

        state->updated.store(true);
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
        auto itr = ns_windows.find(eglGetCurrentContext());

        if (itr != ns_windows.end())
            return itr->second;

        return nullptr;
    }

    //////////////////////////////////////////////

    int WindowImpl::handleEvent(int fd, int events, void* data)
    {
        auto state = detail::ActivityState::get();
        std::lock_guard<decltype(state->mutex)> lock(state->mutex);

        AInputEvent* _event = NULL;

        if (AInputQueue_getEvent(state->inputQueue, &_event) >= 0)
        {
            if (AInputQueue_preDispatchEvent(state->inputQueue, _event))
                return 1;

            int handled = 0;

            AInputQueue_finishEvent(state->inputQueue, _event, handled);
        }

        return 1;
    }

    //////////////////////////////////////////////

    void WindowImpl::updateFocus(const bool focus)
    {
        auto state = ActivityState::get();

        if (focus)
        {
            ns_instance->m_size.x = ANativeWindow_getWidth(state->nativeWindow);
            ns_instance->m_size.y = ANativeWindow_getHeight(state->nativeWindow);
            ns_instance->m_focusRestored = true;
        }
        else
            ns_instance->m_focusLost = true;
    }

    //////////////////////////////////////////////

    EGLConfig WindowImpl::getConfig(const Window::Settings& settings)
    {
        const EGLint attribs[] =
        {
            EGL_RED_SIZE,           8,
            EGL_GREEN_SIZE,         8,
            EGL_BLUE_SIZE,          8,
            EGL_ALPHA_SIZE,         0,

            EGL_DEPTH_SIZE,         0,
            EGL_STENCIL_SIZE,       0,

            EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE,       EGL_WINDOW_BIT | EGL_PBUFFER_BIT,
            EGL_SAMPLE_BUFFERS,     static_cast<EGLint>(settings.samples),

            EGL_NONE
        };

        EGLint count;
        EGLConfig configs[1];

        eglCheck(eglChooseConfig(m_display, attribs, configs, sizeof(configs) / sizeof(EGLConfig), &count));

        return configs[0];
    }

}}

#endif