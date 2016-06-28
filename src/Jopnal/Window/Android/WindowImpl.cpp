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
#include <android/looper.h>
#include <unordered_map>

#endif

//////////////////////////////////////////////


namespace
{
    std::unordered_map<jop::WindowHandle, jop::Window*> ns_windows;
    jop::detail::WindowImpl* ns_instance = nullptr;

    void createSurface(EGLSurface* surface, EGLDisplay display, EGLConfig config, EGLNativeWindowType window)
    {
        *surface = eglCheck(eglCreateWindowSurface(display, config, window, NULL));
    }

    void destroySurface(EGLSurface* surface, EGLDisplay display)
    {
        eglCheck(eglDestroySurface(display, *surface));
        *surface = EGL_NO_SURFACE;
    }
}

namespace jop { namespace detail
{
    WindowImpl::WindowImpl(const Window::Settings& settings, Window& windowPtr)
    {
        if (ns_instance)
        {

            ns_instance = this;
        }

        ns_windows[m_context] = &windowPtr;
    }

    WindowImpl::~WindowImpl()
    {


        ns_instance = nullptr;
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
        {
            ns_instance->m_focusLost = true;
        }
    }
}}

#endif