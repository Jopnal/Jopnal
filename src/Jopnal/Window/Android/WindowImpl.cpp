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
#include <Jopnal/Core/DebugHandler.hpp>
#ifdef JOP_OS_ANDROID

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Core/DebugHandler.hpp>
    #include <Jopnal/Core/Android/ActivityState.hpp>
    #include <Jopnal/Core/Engine.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/EglCheck.hpp>
    #include <Jopnal/Window/InputEnumsImpl.hpp>
    #include <android/native_window.h>
    #include <jni.h>
    #include <EGL/eglext.h>
    #include <unordered_map>
    #include<glm/vec2.hpp>

#endif

//////////////////////////////////////////////

namespace
{
    std::unordered_map<EGLContext, jop::Window*> windowRefs;
    EGLContext ns_shared = EGL_NO_CONTEXT;
    EGLSurface ns_sharedSurface = EGL_NO_SURFACE;

    int ns_touchAxes[4]=
    {
        AMOTION_EVENT_AXIS_PRESSURE, AMOTION_EVENT_AXIS_SIZE,
        AMOTION_EVENT_AXIS_TOUCH_MAJOR, AMOTION_EVENT_AXIS_TOOL_MAJOR
    };

    int ns_joystickAxes[15]=
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

        jop::detail::esglCheck("eglCreateContext", __FILE__, __LINE__);

        return context;
    }

    //////////////////////////////////////////////

    void initialize()
    {
        if (windowRefs.empty())
        {
            eglCheck(eglInitialize(getDisplay(), NULL, NULL));

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
        if (windowRefs.empty())
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
                EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,

                EGL_RED_SIZE,       static_cast<EGLint>(settings.colorBits.r),
                EGL_GREEN_SIZE,     static_cast<EGLint>(settings.colorBits.g),
                EGL_BLUE_SIZE,      static_cast<EGLint>(settings.colorBits.b),
                EGL_ALPHA_SIZE,     static_cast<EGLint>(settings.colorBits.a),

                EGL_DEPTH_SIZE,     static_cast<EGLint>(settings.depthBits),
                EGL_STENCIL_SIZE,   static_cast<EGLint>(settings.stencilBits),

                EGL_NONE
            };

            const EGLint surfaceAttribs[] =
            {
                JOP_CHECK_EGL_EXTENSION(EGL_KHR_gl_colorspace) ? EGL_VG_COLORSPACE : EGL_NONE, EGL_VG_COLORSPACE_sRGB,
                EGL_NONE
            };

            EGLConfig config;
            EGLint numConfigs = 0;

            eglCheck(eglChooseConfig(getDisplay(), configAttribs, &config, 1, &numConfigs));

            EGLint format;
            eglCheck(eglGetConfigAttrib(getDisplay(), config, EGL_NATIVE_VISUAL_ID, &format));

            ANativeWindow_setBuffersGeometry(state->nativeWindow, 0, 0, format);


            m_surface = eglCheck(eglCreateWindowSurface(getDisplay(), config, state->nativeWindow, surfaceAttribs));
            JOP_ASSERT(m_surface != EGL_NO_SURFACE, "Failed to create window surface!");

            m_context = createContext(config);
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

        windowRefs[m_context] = &windowPtr;
    }

    WindowImpl::~WindowImpl()
    {
        windowRefs.erase(m_context);

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
                        windowRef->getEventHandler()->touchMovedAbsolute(id, x, y);

                        for(int info = 0;info<4;++info)
                        {
                            float i = AMotionEvent_getAxisValue(event,ns_touchAxes[info],p);
                            if(i>JOP_AXIS_TOLERANCE)
                            windowRef->getEventHandler()->touchInfo(id,input::getJopTouchInfo(ns_touchAxes[info]) , i);
                        }
                    }
                    else if (device & AINPUT_SOURCE_MOUSE)
                    {
                        windowRef->getEventHandler()->mouseMoved(x-ActivityState::get()->lastTouchPosition[id].x, y-ActivityState::get()->lastTouchPosition[id].y);
                        windowRef->getEventHandler()->mouseMovedAbsolute(x, y);
                    }
                    else if (device & AINPUT_SOURCE_JOYSTICK )
                    {
                        for(int axis = 0;axis<15;++axis)
                        {
                            x = AMotionEvent_getAxisValue(event,ns_joystickAxes[axis],p);
                            if(x>JOP_AXIS_TOLERANCE||x<-JOP_AXIS_TOLERANCE)
                            {
                                if(ns_joystickAxes[axis]==AMOTION_EVENT_AXIS_HAT_Y||ns_joystickAxes[axis]==AMOTION_EVENT_AXIS_HAT_X)
                                {
                                    if(x>0)
                                        x=1;
                                    else
                                        x=-1;
                                    int jopKey=input::getJopControllerButton(x*ns_joystickAxes[axis]);
                                    ActivityState::get()->activeKey=jopKey;
                                    windowRef->getEventHandler()->controllerButtonPressed(id,jopKey);
                                }
                                else if(ns_joystickAxes[axis]==AMOTION_EVENT_AXIS_Y||ns_joystickAxes[axis]==AMOTION_EVENT_AXIS_RZ)
                                {
                                    x=-x;
                                    if(ns_joystickAxes[axis]==AMOTION_EVENT_AXIS_Y)
                                        ActivityState::get()->activeAxes[1]=x;
                                    else
                                        ActivityState::get()->activeAxes[3]=x;

                                    windowRef->getEventHandler()->controllerAxisShifted(id,input::getJopControllerAxis(ns_joystickAxes[axis]),x);
                                }
                                else
                                {
                                    if(ns_joystickAxes[axis]==AMOTION_EVENT_AXIS_X)
                                        ActivityState::get()->activeAxes[0]=x;
                                    else if(ns_joystickAxes[axis]==AMOTION_EVENT_AXIS_Z)
                                        ActivityState::get()->activeAxes[2]=x;

                                    windowRef->getEventHandler()->controllerAxisShifted(id,input::getJopControllerAxis(ns_joystickAxes[axis]),x);
                                }
                            }
                            else if(ns_joystickAxes[axis]==AMOTION_EVENT_AXIS_X)
                                ActivityState::get()->activeAxes[0]=0.f;
                            else if(ns_joystickAxes[axis]==AMOTION_EVENT_AXIS_Y)
                                ActivityState::get()->activeAxes[1]=0.f;
                            else if(ns_joystickAxes[axis]==AMOTION_EVENT_AXIS_Z)
                                ActivityState::get()->activeAxes[2]=0.f;
                            else if(ns_joystickAxes[axis]==AMOTION_EVENT_AXIS_RZ)
                                ActivityState::get()->activeAxes[3]=0.f;
                        }

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
            {
                windowRef->getEventHandler()->touchScrolled(x, y);
            }
            else if (device & AINPUT_SOURCE_MOUSE)
            {
                windowRef->getEventHandler()->mouseScrolled(x, y);
            }
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
                        ActivityState::get()->lastTouchPosition[id]=glm::vec2(x,y);
                        windowRef->getEventHandler()->touchEvent(id, x, y);
                    }
                    else
                    {
                        windowRef->getEventHandler()->touchReleased(id, x, y);
                        ActivityState::get()->lastTouchPosition[id]=glm::vec2(-1.f,-1.f);
                    }

                    for(int info = 0;info<4;++info)
                    {
                        float i = AMotionEvent_getAxisValue(event,ns_touchAxes[info],index);
                        if(i>JOP_AXIS_TOLERANCE)
                            windowRef->getEventHandler()->touchInfo(id,input::getJopTouchInfo(ns_touchAxes[info]) , i);
                    }

                    return 1;
                }
            }
        }
        return 0;
    }

    //////////////////////////////////////////////

    int32_t onKey(const int32_t& action, void* data)
    {
        auto event= static_cast<AInputEvent*>(data);

        int32_t metakey = AKeyEvent_getMetaState(event);
        int32_t key = AKeyEvent_getKeyCode(event);
        int jopKey = input::getJopKey(key);

        int mod = 0x0000;
        if (metakey & AMETA_ALT_ON)
            mod = 0x0001;
        else if (metakey & AMETA_SHIFT_ON)
            mod = 0x0004;

        int32_t device = AInputEvent_getSource(event);

        switch (action)
        {
        case AKEY_EVENT_ACTION_DOWN:
        {
            Window* windowRef = &Engine::getCurrentWindow();
            if (windowRef != nullptr)
            {          
                    if(jopKey!=0)
                    {
                        ActivityState::get()->activeKey=jopKey;
                        windowRef->getEventHandler()->keyPressed(jopKey, key, mod);
                    }
                    else
                    {
                        jopKey = input::getJopControllerButton(key);
                        ActivityState::get()->activeKey=jopKey;
                        windowRef->getEventHandler()->controllerButtonPressed(0, jopKey);
                    }
                return 1;
            }
            return 0;
        }
        case AKEY_EVENT_ACTION_UP:
        {           
            Window* windowRef = &Engine::getCurrentWindow();
            if (windowRef != nullptr)
            {
               if(jopKey!=0)
               windowRef->getEventHandler()->keyReleased(jopKey, key, mod);
               else
               {
                 jopKey = input::getJopControllerButton(key);
                 windowRef->getEventHandler()->controllerButtonReleased(0, jopKey);
               } 

                return 1;
            }
            return 0;
        }
        case AKEY_EVENT_ACTION_MULTIPLE:
        {
            Window* windowRef = &Engine::getCurrentWindow();
            if (windowRef != nullptr)
            {
                if(jopKey!=0)
                {
                    windowRef->getEventHandler()->keyPressed(jopKey, key, mod);
                    windowRef->getEventHandler()->keyReleased(jopKey, key, mod);
                } 
                else
                {
                    jopKey = input::getJopControllerButton(key);
                    windowRef->getEventHandler()->controllerButtonPressed(0, jopKey);
                    windowRef->getEventHandler()->controllerButtonReleased(0, jopKey);
                }

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

    //////////////////////////////////////////////

    void getUnicode(void* data, void* vm, void* env, void* args)
    {        
        auto event = static_cast<AInputEvent*>(data);
        auto virtualMachine = static_cast<JavaVM*>(vm);
        auto environment = static_cast<JNIEnv*>(env);
        auto vmArgs = *static_cast<JavaVMAttachArgs*>(args);

        jint error;
        jint flags = 0;
        error=virtualMachine->AttachCurrentThread(&environment, &vmArgs);

        if (error == JNI_ERR)
            JOP_DEBUG_ERROR("Failed to initialize java interface to read unicode");

        jlong downTime = AKeyEvent_getDownTime(event);
        jlong eventTime = AKeyEvent_getEventTime(event);
        jint action = AKeyEvent_getAction(event);
        jint code = AKeyEvent_getKeyCode(event);
        jint repeat = AKeyEvent_getRepeatCount(event);
        jint metaState = AKeyEvent_getMetaState(event);
        jint deviceId = AInputEvent_getDeviceId(event);
        jint scancode = AKeyEvent_getScanCode(event);
        flags = AKeyEvent_getFlags(event);
        jint source = AInputEvent_getSource(event);

        jclass ClassKeyEvent = environment->FindClass("android/view/KeyEvent");
        jmethodID KeyEventConstructor = environment->GetMethodID(ClassKeyEvent, "<init>", "(JJIIIIIIII)V");
        jobject ObjectKeyEvent = environment->NewObject(ClassKeyEvent, KeyEventConstructor, downTime, eventTime, action, code, repeat, metaState, deviceId, scancode, flags, source);

        jmethodID MethodGetUnicode = environment->GetMethodID(ClassKeyEvent, "getUnicodeChar", "(I)I");
        unsigned int utf = environment->CallIntMethod(ObjectKeyEvent, MethodGetUnicode, metaState);

        Window* windowRef = &Engine::getCurrentWindow();
        if (windowRef != nullptr)
            windowRef->getEventHandler()->textEntered(utf);

        environment->DeleteLocalRef(ClassKeyEvent);
        environment->DeleteLocalRef(ObjectKeyEvent);
        virtualMachine->DetachCurrentThread();
    }

    //////////////////////////////////////////////

    void showVirtualKeyboard(const bool show, void* vm, void* env, void* clazz, void* args)
    {
        auto virtualMachine = static_cast<JavaVM*>(vm);
        auto environment = static_cast<JNIEnv*>(env);
        auto nativeActivity = *static_cast<jobject*>(clazz);
        auto vmArgs = *static_cast<JavaVMAttachArgs*>(args);

        jint error;
        jint flags = 0;
        error = virtualMachine->AttachCurrentThread(&environment, &vmArgs);

        if (error == JNI_ERR)
            JOP_DEBUG_ERROR("Failed to initialize java interface to able opening virtual keyboard");

        jclass classNativeActivity = environment->GetObjectClass(nativeActivity);
        jclass ClassContext = environment->FindClass("android/content/Context");
        jfieldID fieldInput = environment->GetStaticFieldID(ClassContext,
            "INPUT_METHOD_SERVICE", "Ljava/lang/String;");
        jobject inputService = environment->GetStaticObjectField(ClassContext,
            fieldInput);
        environment->DeleteLocalRef(ClassContext);
        jclass classInputManager =
            environment->FindClass("android/view/inputmethod/InputMethodManager");
        jmethodID systemService = environment->GetMethodID(classNativeActivity,
            "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
        jobject inputManager = environment->CallObjectMethod(nativeActivity,
            systemService, inputService);
        environment->DeleteLocalRef(inputService);

        jmethodID javaWindow = environment->GetMethodID(classNativeActivity,
            "getWindow", "()Landroid/view/Window;");
        jobject javaWindowObject = environment->CallObjectMethod(nativeActivity, javaWindow);
        jclass classWindow = environment->FindClass("android/view/Window");
        jmethodID decorView = environment->GetMethodID(classWindow,
            "getDecorView", "()Landroid/view/View;");
        jobject decorViewObject = environment->CallObjectMethod(javaWindowObject, decorView);
        environment->DeleteLocalRef(javaWindowObject);
        environment->DeleteLocalRef(classWindow);

        if (show)
        {
            jmethodID MethodShowSoftInput = environment->GetMethodID(classInputManager,
                "showSoftInput", "(Landroid/view/View;I)Z");
            jboolean result = environment->CallBooleanMethod(inputManager,
                MethodShowSoftInput, decorViewObject, flags);
        }
        else
        {
            jclass classView = environment->FindClass("android/view/View");
            jmethodID javaWindowToken = environment->GetMethodID(classView,
                "getWindowToken", "()Landroid/os/IBinder;");
            jobject binder = environment->CallObjectMethod(decorViewObject,
                javaWindowToken);
            environment->DeleteLocalRef(classView);


            jmethodID MethodHideSoftInput = environment->GetMethodID(classInputManager,
                "hideSoftInputFromWindow", "(Landroid/os/IBinder;I)Z");
            jboolean res = environment->CallBooleanMethod(inputManager,
                MethodHideSoftInput, binder, flags);
            environment->DeleteLocalRef(binder);
        }

        environment->DeleteLocalRef(classNativeActivity);
        environment->DeleteLocalRef(classInputManager);
        environment->DeleteLocalRef(decorViewObject);
        virtualMachine->DetachCurrentThread();
    }
}}

#endif