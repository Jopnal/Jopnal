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

    #include <Jopnal/Core/DebugHandler.hpp>
    #include <Jopnal/Core/SettingManager.hpp>
    #include <Jopnal/Core/Android/ActivityState.hpp>
    #include <Jopnal/Core/Engine.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/EglCheck.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>
    #include <Jopnal/Window/Keyboard.hpp>
    #include <Jopnal/Window/InputEnumsImpl.hpp>
    #include <android/native_window.h>
    #include <android/window.h>
    #include <jni.h>
    #include <EGL/eglext.h>
    #include <unordered_map>
    #include <glm/vec2.hpp>

#endif

#define JOP_AXIS_TOLERANCE 0.005

//////////////////////////////////////////////


namespace
{
    std::unordered_map<EGLContext, jop::Window*> ns_windowRefs;
    EGLContext ns_shared = EGL_NO_CONTEXT;
    EGLSurface ns_sharedSurface = EGL_NO_SURFACE;

    const int ns_touchAxes[] =
    {
        AMOTION_EVENT_AXIS_PRESSURE, AMOTION_EVENT_AXIS_SIZE,
        AMOTION_EVENT_AXIS_TOUCH_MAJOR, AMOTION_EVENT_AXIS_TOOL_MAJOR
    };

    const int ns_joystickAxes[] =
    {
        AMOTION_EVENT_AXIS_X, AMOTION_EVENT_AXIS_Y, AMOTION_EVENT_AXIS_Z,
        AMOTION_EVENT_AXIS_RX, AMOTION_EVENT_AXIS_RY, AMOTION_EVENT_AXIS_RZ,
        AMOTION_EVENT_AXIS_HAT_X, AMOTION_EVENT_AXIS_HAT_Y, AMOTION_EVENT_AXIS_LTRIGGER,
        AMOTION_EVENT_AXIS_RTRIGGER, AMOTION_EVENT_AXIS_THROTTLE, AMOTION_EVENT_AXIS_RUDDER,
        AMOTION_EVENT_AXIS_WHEEL, AMOTION_EVENT_AXIS_GAS, AMOTION_EVENT_AXIS_BRAKE,
    };

    EGLDisplay getDisplay()
    {
        static EGLDisplay display = EGL_NO_DISPLAY;

        if (display == EGL_NO_DISPLAY)
        {
            display = eglCheck(eglGetDisplay(EGL_DEFAULT_DISPLAY));
        }

        return display;
    }

    //////////////////////////////////////////////

    EGLContext createContext(EGLConfig config)
    {
        EGLint version[] =
        {
            EGL_CONTEXT_CLIENT_VERSION, 3, 
            EGL_NONE
        };

        EGLContext context = EGL_NO_CONTEXT;

        while (context == EGL_NO_CONTEXT && version[1] >= 2)
        {
            context = eglCreateContext(getDisplay(), config, ns_shared, version);
            --version[1];
        }

        jop::detail::checkEGLError("eglCreateContext", __FILE__, __LINE__);

        return context;
    }

    //////////////////////////////////////////////

    void initialize()
    {
        if (ns_windowRefs.empty())
        {
            eglCheck(eglInitialize(getDisplay(), NULL, NULL));

            if (jop::SettingManager::get<bool>("engine@Debug|bPrintEGLExtensions", false))
                JOP_DEBUG_INFO("Available EGL extensions:\n\n" << eglQueryString(getDisplay(), EGL_EXTENSIONS));

            const EGLint attribs[] =
            {
                EGL_WIDTH,  1,
                EGL_HEIGHT, 1,
                JOP_CHECK_EGL_EXTENSION(EGL_KHR_gl_colorspace) ? EGL_VG_COLORSPACE : EGL_NONE, EGL_VG_COLORSPACE_sRGB,
                EGL_NONE
            };

            const EGLint configAttribs[] =
            {
                EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
                EGL_NONE
            };

            EGLConfig config;
            EGLint numConfigs = 0;

            eglCheck(eglChooseConfig(getDisplay(), configAttribs, &config, 1, &numConfigs));

            ns_sharedSurface = eglCheck(eglCreatePbufferSurface(getDisplay(), config, attribs));
            JOP_ASSERT(ns_sharedSurface != EGL_NO_SURFACE, "Failed to create shared context surface!");

            ns_shared = createContext(config);
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

            ns_shared = EGL_NO_CONTEXT;
        }
    }

    void goFullscreen()
    {
        ANativeActivity_setWindowFlags(jop::detail::ActivityState::get()->nativeActivity, AWINDOW_FLAG_FULLSCREEN, AWINDOW_FLAG_FULLSCREEN);
    }

    void updateSize(glm::uvec2& size, EGLSurface surface)
    {
        eglCheck(eglQuerySurface(getDisplay(), surface, EGL_WIDTH, reinterpret_cast<EGLint*>(&size.x)));
        eglCheck(eglQuerySurface(getDisplay(), surface, EGL_HEIGHT, reinterpret_cast<EGLint*>(&size.y)));
    }
}

namespace jop { namespace detail
{
    WindowImpl::WindowImpl(const Window::Settings& settings, Window& windowPtr)
        : m_config      (NULL),
          m_surface     (EGL_NO_SURFACE),
          m_context     (EGL_NO_CONTEXT),
          m_windowRef   (windowPtr),
          m_size        (0),
          m_fullScreen  (false)
    {
        initialize();

        auto state = detail::ActivityState::get();

        if (!state->window)
        {
            const EGLint configAttribs[] =
            {
                EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,

                EGL_RED_SIZE,       static_cast<EGLint>(settings.colorBits.r),
                EGL_GREEN_SIZE,     static_cast<EGLint>(settings.colorBits.g),
                EGL_BLUE_SIZE,      static_cast<EGLint>(settings.colorBits.b),
                EGL_ALPHA_SIZE,     static_cast<EGLint>(settings.colorBits.a),

                EGL_DEPTH_SIZE,     static_cast<EGLint>(settings.depthBits),
                EGL_STENCIL_SIZE,   static_cast<EGLint>(settings.stencilBits),

                EGL_NONE
            };

            EGLint numConfigs = 0;

            eglCheck(eglChooseConfig(getDisplay(), configAttribs, &m_config, 1, &numConfigs));

            JOP_ASSERT(numConfigs > 0, "Failed to choose EGL config!");

            state->window = this;
            state->handleSurfaceCreation    = &handleSurfaceCreation;
            state->handleSurfaceDestruction = &handleSurfaceDestruction;

            handleSurfaceCreation();

            m_context = createContext(m_config);
            JOP_ASSERT(m_context != EGL_NO_CONTEXT, "Failed to create context!");

            EGLBoolean success = eglCheck(eglMakeCurrent(getDisplay(), m_surface, m_surface, m_context));
            JOP_ASSERT(success == EGL_TRUE, "Failed to make context current!");

            updateSize(m_size, m_surface);

            if (settings.displayMode != Window::DisplayMode::Windowed)
            {
                m_fullScreen = true;
                goFullscreen();
            }
        }
        else
        {
            const EGLint configAttribs[] =
            {
                EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
                EGL_NONE
            };

            const EGLint surfaceAttribs[] =
            {
                EGL_WIDTH,  1,
                EGL_HEIGHT, 1,
                JOP_CHECK_EGL_EXTENSION(EGL_KHR_gl_colorspace) ? EGL_VG_COLORSPACE : EGL_NONE, EGL_VG_COLORSPACE_sRGB,
                EGL_NONE
            };

            EGLConfig config;
            EGLint numConfigs = 0;

            eglCheck(eglChooseConfig(getDisplay(), configAttribs, &config, 1, &numConfigs));

            m_surface = eglCheck(eglCreatePbufferSurface(getDisplay(), config, surfaceAttribs));
            JOP_ASSERT(m_surface != EGL_NO_SURFACE, "Failed to create pbuffer surface!");

            m_context = createContext(config);
            JOP_ASSERT(m_context != EGL_NO_CONTEXT, "Failed to create context!");

            EGLBoolean success = eglCheck(eglMakeCurrent(getDisplay(), m_surface, m_surface, m_context));
            JOP_ASSERT(success == EGL_TRUE, "Failed to make context current!");
        }

        ns_windowRefs[m_context] = &windowPtr;

        if (JOP_CHECK_GL_EXTENSION(GL_EXT_sRGB_write_control))
        {
            static const bool srgbConversion = SettingManager::get<bool>("engine@DefaultWindow|bSRGBConversion", false);

            if (srgbConversion)
            {
                glCheck(glEnable(GL_FRAMEBUFFER_SRGB_EXT));
            }
        }
    }

    WindowImpl::~WindowImpl()
    {
        ns_windowRefs.erase(m_context);

        {
            auto state = detail::ActivityState::get();

            if (state->window == this)
            {
                state->window = nullptr;
                state->handleSurfaceCreation    = nullptr;
                state->handleSurfaceDestruction = nullptr;
            }
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
        return ActivityState::get()->nativeWindow;
    }

    //////////////////////////////////////////////

    void WindowImpl::pollEvents()
    {
        auto state = ActivityState::get();

        if (state && state->pollFunc)
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

    WindowEventHandler* WindowImpl::getEventHandler()
    {
        return m_windowRef.getEventHandler();
    }

    //////////////////////////////////////////////

    void WindowImpl::handleResize()
    {
        auto state = ActivityState::get();

        if (state->nativeWindow)
        {
            std::swap(m_size.x, m_size.y);
            getEventHandler()->resized(m_size.x, m_size.y);
        }
    }

    //////////////////////////////////////////////

    Window* WindowImpl::getCurrentContextWindow()
    {
        auto itr = ns_windowRefs.find(eglGetCurrentContext());

        if (itr != ns_windowRefs.end())
            return itr->second;

        return nullptr;
    }

    //////////////////////////////////////////////

    void WindowImpl::handleSurfaceCreation()
    {
        const EGLint surfaceAttribs[] =
        {
            JOP_CHECK_EGL_EXTENSION(EGL_KHR_gl_colorspace) ? EGL_VG_COLORSPACE : EGL_NONE, EGL_VG_COLORSPACE_sRGB,
            EGL_NONE
        };

        auto state = detail::ActivityState::get();
        auto win = state->window;

        if (win && win->m_surface == EGL_NO_SURFACE)
        {
            win->m_surface = eglCheck(eglCreateWindowSurface(getDisplay(), win->m_config, state->nativeWindow, surfaceAttribs));
            JOP_ASSERT(win->m_surface != EGL_NO_SURFACE, "Failed to create window surface!");

            if (win->m_context != EGL_NO_CONTEXT)
            {
                EGLBoolean success = eglCheck(eglMakeCurrent(getDisplay(), win->m_surface, win->m_surface, win->m_context));
                JOP_ASSERT(success == EGL_TRUE, "Failed to make context current!");
            }

            if (win->m_fullScreen)
                goFullscreen();

            updateSize(win->m_size, win->m_surface);
        }
    }

    //////////////////////////////////////////////

    void WindowImpl::handleSurfaceDestruction()
    {
        auto state = detail::ActivityState::get();
        auto win = state->window;

        if (win && win->m_surface != EGL_NO_SURFACE)
        {
            eglCheck(eglDestroySurface(getDisplay(), win->m_surface));
            win->m_surface = EGL_NO_SURFACE;
        }
    }

    //////////////////////////////////////////////

    int32_t motion(AInputEvent* event)
    {
        auto& windowRef = Engine::getMainWindow();

        const int32_t device = AInputEvent_getSource(event);

        const int pointerCount = AMotionEvent_getPointerCount(event);

        for (int p = 0; p < pointerCount; ++p)
        {
            const int id = AMotionEvent_getPointerId(event, p);

            if (id < 10)
            {
                float x = AMotionEvent_getX(event, p);
                const float y = AMotionEvent_getY(event, p);

                auto state = ActivityState::get();

                if (device & AINPUT_SOURCE_TOUCHSCREEN)
                {
                    windowRef.getEventHandler()->touchMoved(id, x - state->lastTouchPosition[id].x, y - state->lastTouchPosition[id].y);
                    windowRef.getEventHandler()->touchMovedAbsolute(id, x, y);

                    for (int info = 0; info < 4; ++info)
                    {
                        const float i = AMotionEvent_getAxisValue(event, ns_touchAxes[info], p);

                        if (i > JOP_AXIS_TOLERANCE)
                            windowRef.getEventHandler()->touchInfo(id, Input::getJopTouchInfo(ns_touchAxes[info]), i);
                    }
                }
                else if (device & AINPUT_SOURCE_MOUSE)
                {
                    windowRef.getEventHandler()->mouseMoved(x - state->lastTouchPosition[id].x, y - state->lastTouchPosition[id].y);
                    windowRef.getEventHandler()->mouseMovedAbsolute(x, y);
                }
                else if (device & AINPUT_SOURCE_JOYSTICK)
                {
                    for (int axis = 0; axis < 15; ++axis)
                    {
                        x = AMotionEvent_getAxisValue(event, ns_joystickAxes[axis], p);

                        if (x > JOP_AXIS_TOLERANCE || x < -JOP_AXIS_TOLERANCE)
                        {
                            if (ns_joystickAxes[axis] == AMOTION_EVENT_AXIS_HAT_Y || ns_joystickAxes[axis] == AMOTION_EVENT_AXIS_HAT_X)
                            {
                                if (x > 0)
                                    x = 1;
                                else
                                    x = -1;

                                const int jopKey = Input::getJopControllerButton(x * ns_joystickAxes[axis]);
                                state->activeKey = jopKey;

                                windowRef.getEventHandler()->controllerButtonPressed(id, jopKey);
                            }
                            else if (ns_joystickAxes[axis] == AMOTION_EVENT_AXIS_Y || ns_joystickAxes[axis] == AMOTION_EVENT_AXIS_RZ)
                            {
                                x = -x;

                                if (ns_joystickAxes[axis] == AMOTION_EVENT_AXIS_Y)
                                    state->activeAxes[1] = x;
                                else
                                    state->activeAxes[3] = x;

                                windowRef.getEventHandler()->controllerAxisShifted(id, Input::getJopControllerAxis(ns_joystickAxes[axis]), x);
                            }
                            else
                            {
                                if (ns_joystickAxes[axis] == AMOTION_EVENT_AXIS_X)
                                    state->activeAxes[0] = x;

                                else if(ns_joystickAxes[axis] == AMOTION_EVENT_AXIS_Z)
                                    state->activeAxes[2] = x;

                                else if(ns_joystickAxes[axis] == AMOTION_EVENT_AXIS_LTRIGGER)
                                    state->activeAxes[4] = x;

                                else if(ns_joystickAxes[axis] == AMOTION_EVENT_AXIS_RTRIGGER)
                                    state->activeAxes[5] = x;

                                windowRef.getEventHandler()->controllerAxisShifted(id, Input::getJopControllerAxis(ns_joystickAxes[axis]), x);
                            }
                        }
                        else if (ns_joystickAxes[axis] == AMOTION_EVENT_AXIS_X)
                            state->activeAxes[0] = 0.f;

                        else if (ns_joystickAxes[axis] == AMOTION_EVENT_AXIS_Y)
                            state->activeAxes[1] = 0.f;

                        else if (ns_joystickAxes[axis] == AMOTION_EVENT_AXIS_Z)
                            state->activeAxes[2] = 0.f;

                        else if (ns_joystickAxes[axis] == AMOTION_EVENT_AXIS_RZ)
                            state->activeAxes[3] = 0.f;

                        else if (ns_joystickAxes[axis] == AMOTION_EVENT_AXIS_LTRIGGER)
                            state->activeAxes[4] = 0.f;

                        else if (ns_joystickAxes[axis] == AMOTION_EVENT_AXIS_RTRIGGER)
                            state->activeAxes[5] = 0.f;

                        if (!state->controllerPresent)
                        {
                            windowRef.getEventHandler()->controllerConnected(0, "Android_Controller");
                            state->controllerPresent = true;
                        }
                    }
                }
                else
                    return 0;

                state->lastTouchPosition[id] = glm::vec2(x,y);
            }

            return 1;
        }

        return 0;
    }

    //////////////////////////////////////////////

    int32_t scroll(AInputEvent* event)
    {
        auto& windowRef = Engine::getMainWindow();

        const int32_t device = AInputEvent_getSource(event);

        const float x = AMotionEvent_getX(event, 0);
        const float y = AMotionEvent_getY(event, 0);

        if (device & AINPUT_SOURCE_TOUCHSCREEN)
            windowRef.getEventHandler()->touchScrolled(x, y);

        else if (device & AINPUT_SOURCE_MOUSE)
            windowRef.getEventHandler()->mouseScrolled(x, y);

        else
            return 0;

        return 1;
    }

    //////////////////////////////////////////////

    int32_t touch(bool down, int32_t action, AInputEvent* event)
    {
        auto& windowRef = Engine::getMainWindow();

        const int32_t device = AInputEvent_getSource(event);

        if (device & AINPUT_SOURCE_TOUCHSCREEN)
        {
            const int index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
            const int id = AMotionEvent_getPointerId(event, index);

            if (id < 10)
            {
                const float x = AMotionEvent_getX(event, index);
                const float y = AMotionEvent_getY(event, index);

                auto state = ActivityState::get();

                if (down)
                {
                    state->lastTouchPosition[id] = glm::vec2(x, y);
                    windowRef.getEventHandler()->touchPressed(id, x, y);
                }
                else
                {
                    state->lastTouchPosition[id] = glm::vec2(-1.f, -1.f);
                    windowRef.getEventHandler()->touchReleased(id, x, y);
                }

                for (int info = 0; info < 4; ++info)
                {
                    const float i = AMotionEvent_getAxisValue(event, ns_touchAxes[info], index);

                    if (i > JOP_AXIS_TOLERANCE)
                        windowRef.getEventHandler()->touchInfo(id, Input::getJopTouchInfo(ns_touchAxes[info]), i);
                }

                return 1;
            }
        }

        return 0;
    }

    //////////////////////////////////////////////

    int32_t onKey(const int32_t& action, void* data)
    {
        auto event = static_cast<AInputEvent*>(data);

        const int32_t metakey = AKeyEvent_getMetaState(event);
        const int32_t key = AKeyEvent_getKeyCode(event);
        const int32_t scanCode = AKeyEvent_getScanCode(event);

        int jopKey = Input::getJopKey(key);

        const int mod = ((metakey & AMETA_SHIFT_ON) != 0) * Keyboard::Modifier::Shift
                      | ((metakey & AMETA_ALT_ON)   != 0) * Keyboard::Modifier::Alt
                      | ((metakey & AMETA_CTRL_ON)  != 0) * Keyboard::Modifier::Control;

        int32_t device = AInputEvent_getSource(event);

        auto& windowRef = Engine::getMainWindow();

        switch (action)
        {
            case AKEY_EVENT_ACTION_DOWN:
            {      
                auto state = ActivityState::get();

                if (jopKey != Keyboard::Unknown)
                {
                    state->activeKey = jopKey;
                    windowRef.getEventHandler()->keyPressed(jopKey, scanCode, mod);
                }
                
                if (Controller::controllersPresent())
                {
                    jopKey = Input::getJopControllerButton(key);

                    if (jopKey != Controller::XBox::Unknown)
                    {
                        state->activeControllerButtons[jopKey] = true;
                        windowRef.getEventHandler()->controllerButtonPressed(0, jopKey);
                    }
                }

                return 1;
            }
            case AKEY_EVENT_ACTION_UP:
            {
                auto state = ActivityState::get();

                if (jopKey != Keyboard::Unknown)
                {
                    state->activeKey = -1;
                    windowRef.getEventHandler()->keyReleased(jopKey, scanCode, mod);
                }

                if (Controller::controllersPresent())
                {
                    jopKey = Input::getJopControllerButton(key);

                    if (jopKey != Controller::XBox::Unknown)
                    {
                        state->activeControllerButtons[jopKey] = false;
                        windowRef.getEventHandler()->controllerButtonReleased(0, jopKey);
                    }
                } 

                return 1;
            }
            case AKEY_EVENT_ACTION_MULTIPLE:
            {
                if (jopKey != 0)
                {
                    windowRef.getEventHandler()->keyPressed(jopKey, scanCode, mod);
                    windowRef.getEventHandler()->keyReleased(jopKey, scanCode, mod);
                } 

                if (Controller::controllersPresent())
                {
                    jopKey = Input::getJopControllerButton(key);

                    if (jopKey != Controller::XBox::Unknown)
                    {
                        windowRef.getEventHandler()->controllerButtonPressed(0, jopKey);
                        windowRef.getEventHandler()->controllerButtonReleased(0, jopKey);
                    }
                }

                return 1;
            }
        }

        return 0;
    }

    //////////////////////////////////////////////

    int32_t onMotion(const int32_t& action, void* data)
    {
        auto event = static_cast<AInputEvent*>(data);

        switch (action & AMOTION_EVENT_ACTION_MASK)
        {
            case AMOTION_EVENT_ACTION_SCROLL:
                return scroll(event);

            case AMOTION_EVENT_ACTION_MOVE:
                return motion(event);

            case AMOTION_EVENT_ACTION_POINTER_DOWN:
            case AMOTION_EVENT_ACTION_DOWN:
                return touch(true, action, event);

            case AMOTION_EVENT_ACTION_POINTER_UP:
            case AMOTION_EVENT_ACTION_UP:
            case AMOTION_EVENT_ACTION_CANCEL:
                return touch(false, action, event);
        }

        return 0;
    }

    //////////////////////////////////////////////

    void getUnicode(void* data)
    {
        auto env = Thread::getCurrentJavaEnv();

        if (!env)
        {
            JOP_DEBUG_ERROR("No current java environment, function \"" << __func__ << "\"");
            return;
        }

        //auto event = static_cast<AInputEvent*>(data);
        //
        //jint flags = 0;
        //
        //jlong downTime = AKeyEvent_getDownTime(event);
        //jlong eventTime = AKeyEvent_getEventTime(event);
        //jint action = AKeyEvent_getAction(event);
        //jint code = AKeyEvent_getKeyCode(event);
        //jint repeat = AKeyEvent_getRepeatCount(event);
        //jint metaState = AKeyEvent_getMetaState(event);
        //jint deviceId = AInputEvent_getDeviceId(event);
        //jint scancode = AKeyEvent_getScanCode(event);
        //flags = AKeyEvent_getFlags(event);
        //jint source = AInputEvent_getSource(event);
        //
        //jclass ClassKeyEvent = env->FindClass("android/view/KeyEvent");
        //jmethodID KeyEventConstructor = env->GetMethodID(ClassKeyEvent, "<init>", "(JJIIIIIIII)V");
        //jobject ObjectKeyEvent = env->NewObject(ClassKeyEvent, KeyEventConstructor, downTime, eventTime, action, code, repeat, metaState, deviceId, scancode, flags, source);
        //
        //jmethodID MethodGetUnicode = env->GetMethodID(ClassKeyEvent, "getUnicodeChar", "(I)I");
        //const unsigned int utf = env->CallIntMethod(ObjectKeyEvent, MethodGetUnicode, metaState);
        //
        //Engine::getMainWindow().getEventHandler()->textEntered(utf);
        //
        //env->DeleteLocalRef(ClassKeyEvent);
        //env->DeleteLocalRef(ObjectKeyEvent);
    }
}}

#endif