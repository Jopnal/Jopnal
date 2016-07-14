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
    #include <Jopnal/Core/Engine.hpp>
    #include <Jopnal/Graphics/OpenGL/EglCheck.hpp>
    #include <Jopnal/Window/InputEnumsImpl.hpp>
    #include <android/native_window.h>
    #include <EGL/eglext.h>
    #include <unordered_map>
    #include<glm/vec2.hpp>

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
        auto state = ActivityState::get();
        std::lock_guard<decltype(state->mutex)> lock(state->mutex);

        state->pollFunc();
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

    //////////////////////////////////////////////

    int32_t motion(AInputEvent* event)
    {  
        Window* windowRef = &Engine::getCurrentWindow();
        if (windowRef != nullptr)
        {
            int32_t device = AInputEvent_getSource(event);
            int pointerCount = AMotionEvent_getPointerCount(event);

            for (int p = 0; p < pointerCount; p++)
            {
                int id = AMotionEvent_getPointerId(event, p);

                if(id<10)
                {
                    float x = AMotionEvent_getX(event, p);
                    float y = AMotionEvent_getY(event, p);

                    if (device & AINPUT_SOURCE_TOUCHSCREEN)
                    {
                        windowRef->getEventHandler()->touchMoved(id, x-ActivityState::get()->lastTouchPosition[id].x, y-ActivityState::get()->lastTouchPosition[id].y);
                        windowRef->getEventHandler()->touchMovedApsolute(id, x, y);
                    }
                    else if (device & AINPUT_SOURCE_MOUSE)
                    {
                        windowRef->getEventHandler()->mouseMoved(x-ActivityState::get()->lastTouchPosition[id].x, y-ActivityState::get()->lastTouchPosition[id].y);
                        windowRef->getEventHandler()->mouseMovedApsolute(x, y);
                    }
                    else
                        return 0;

                    ActivityState::get()->lastTouchPosition[id]=glm::vec2(x,y);
                }
                return 1;
            }
        }
        return 0;
    }

    //////////////////////////////////////////////

    int32_t scroll(AInputEvent* event)
    {
        Window* windowRef = &Engine::getCurrentWindow();
        if (windowRef != nullptr)
        {
            int32_t device = AInputEvent_getSource(event);

            float x = AMotionEvent_getX(event, 0);
            float y = AMotionEvent_getY(event, 0);

            if (device & AINPUT_SOURCE_TOUCHSCREEN)
                windowRef->getEventHandler()->touchScrolled(x, y);
            else if (device & AINPUT_SOURCE_MOUSE)
                windowRef->getEventHandler()->mouseScrolled(x, y);
            else
                return 0;

            return 1;
        }
        return 0;
    }

    //////////////////////////////////////////////

    int32_t touch(bool down, int32_t action, AInputEvent* event)
    {

        Window* windowRef = &Engine::getCurrentWindow();
        if (windowRef != nullptr)
        {
            int32_t device = AInputEvent_getSource(event);
            if (device & AINPUT_SOURCE_TOUCHSCREEN)
            {
                int index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
                int id = AMotionEvent_getPointerId(event, index);
                if(id<10)
                {
                    float x = AMotionEvent_getX(event, index);
                    float y = AMotionEvent_getY(event, index);

                    if (down)
                    {
                        windowRef->getEventHandler()->touchEvent(id, x, y);
                        ActivityState::get()->lastTouchPosition[id]=glm::vec2(x,y);
                    }
                    else
                    {
                        windowRef->getEventHandler()->touchReleased(id, x, y);
                        ActivityState::get()->lastTouchPosition[id]=glm::vec2(-1.f,-1.f);
                    }

                    return 1;
                }
            }
        }
        return 0;
    }

    //////////////////////////////////////////////

    int32_t onKey(const int32_t& action, const int32_t& meta, const int32_t& key)
    {
        int jopKey = input::getJopKey(key);

        int mod = 0x0000;
        if (meta & AMETA_ALT_ON)
            mod = 0x0001;
        else if (meta & AMETA_SHIFT_ON)
            mod = 0x0004;

        switch (action)
        {
        case AKEY_EVENT_ACTION_DOWN:
        {
            Window* windowRef = &Engine::getCurrentWindow();
            if (windowRef != nullptr)
            {
                windowRef->getEventHandler()->keyPressed(jopKey, key, mod);
                return 1;
            }
            return 0;
        }
        case AKEY_EVENT_ACTION_UP:
        {
            Window* windowRef = &Engine::getCurrentWindow();
            if (windowRef != nullptr)
            {
                windowRef->getEventHandler()->keyReleased(jopKey, key, mod);
                return 1;
            }
            return 0;
        }
        case AKEY_EVENT_ACTION_MULTIPLE:
        {
            Window* windowRef = &Engine::getCurrentWindow();
            if (windowRef != nullptr)
            {
                windowRef->getEventHandler()->keyPressed(jopKey, key, mod);
                windowRef->getEventHandler()->keyReleased(jopKey, key, mod);
                return 1;
            }
            return 0;
        }
        default:
            break;
        }
        return 0;
    }

    //////////////////////////////////////////////

    int32_t onMotion(const int32_t& action, void* data)
    {
        auto event= static_cast<AInputEvent*>(data);

        switch (action & AMOTION_EVENT_ACTION_MASK)
        {
        case AMOTION_EVENT_ACTION_SCROLL:
        {
            return scroll(event);
            break;
        }
        case AMOTION_EVENT_ACTION_MOVE:
        {
            return motion(event);
            break;
        }
        case AMOTION_EVENT_ACTION_POINTER_DOWN:
        case AMOTION_EVENT_ACTION_DOWN:
        {
            return touch(true, action, event);
            break;
        }

        case AMOTION_EVENT_ACTION_POINTER_UP:
        case AMOTION_EVENT_ACTION_UP:
        case AMOTION_EVENT_ACTION_CANCEL:
        {
            return touch(false, action, event);
            break;
        }
        }
    }
}}

#endif