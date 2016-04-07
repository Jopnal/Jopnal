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

#if defined(JOP_OS_DESKTOP)
    #include <Jopnal/Window/Desktop/WindowImpl.hpp>
#endif

//////////////////////////////////////////////


namespace jop
{
    JOP_DERIVED_COMMAND_HANDLER(Subsystem, Window)

        JOP_BIND_MEMBER_COMMAND(&Window::setMouseMode, "setMouseMode");

    JOP_END_COMMAND_HANDLER(Window)
}

namespace
{
    bool ns_eventsPolled = false;

    struct jop_DefaultEventHandler : jop::WindowEventHandler
    {
        jop_DefaultEventHandler(jop::Window& w) : jop::WindowEventHandler(w){}
        void closed() override {jop::Engine::exit();}
    };
}

namespace jop
{
    Window::Settings::Settings(const bool loadSettings)
        : size          (1u, 1u),
          title         ("Window Title"),
          displayMode   (DisplayMode::Windowed),
          samples       (0),
          visible       (false),
          vSync         (true),
          debug         (false)
    {
        if (loadSettings)
        {
            size.x = SettingManager::getUint("uDefaultWindowSizeX", 1280); size.y = SettingManager::getUint("uDefaultWindowSizeY", 720);
            title = SettingManager::getString("sDefaultWindowTitle", getProjectName());
            displayMode = static_cast<Window::DisplayMode>(std::min(2u, SettingManager::getUint("uDefaultWindowMode", 0)));
            samples = SettingManager::getUint("uDefaultWindowMultisampling", 0);
            visible = SettingManager::getBool("uDefaultWindowVisible", true);
            vSync = SettingManager::getBool("bDefaultWindowVSync", true);
            debug = SettingManager::getBool("bDefaultWindowDebugContext", false);
        }
    }

    //////////////////////////////////////////////

    Window::Window()
        : RenderTarget      ("window"),
          m_impl            (),
          m_eventHandler    ()
    {}

    Window::Window(const Settings& settings)
        : RenderTarget      ("window"),
          m_impl            (),
          m_eventHandler    ()
    {
        open(settings);
        setEventHandler<jop_DefaultEventHandler>();

    #ifdef JOP_DEBUG_MODE
        if (gl::exts::var_KHR_debug)
        {
            gl::DebugMessageCallback([](GLenum, GLenum, GLuint, GLenum severity, GLsizei, const GLchar* msg, const void*)
            {
                if (severity == gl::DEBUG_SEVERITY_HIGH)
                {
                    JOP_DEBUG_ERROR(msg);
                }
                else if (severity == gl::DEBUG_SEVERITY_MEDIUM)
                {
                    JOP_DEBUG_WARNING(msg);
                }
                else if (severity == gl::DEBUG_SEVERITY_LOW)
                {
                    JOP_DEBUG_INFO(msg);
                }

            }, NULL);
        }
    #endif
    }

    Window::~Window()
    {}

    //////////////////////////////////////////////

    void Window::preUpdate(const float)
    {
        ns_eventsPolled = false;
    }

    //////////////////////////////////////////////

    void Window::postUpdate(const float deltaTime)
    {
        if (isOpen() && Engine::getState() != Engine::State::Frozen)
            RenderTarget::postUpdate(deltaTime);
    }

    //////////////////////////////////////////////

    void Window::draw()
    {
        if (isOpen() && Engine::getState() != Engine::State::Frozen)
            m_impl->swapBuffers();

        // Only poll the events if they haven't yet been during this frame.
        // We care about this because we don't want to invoke controller
        // callbacks multiple times.
        if (!ns_eventsPolled)
        {
            static const bool controllers = SettingManager::getUint("uMaxControllers", 1) > 0;

            pollEvents();

            if (controllers && m_eventHandler)
                m_eventHandler->handleControllerInput();

            ns_eventsPolled = true;
        }
    }

    //////////////////////////////////////////////

    bool Window::bind() const
    {
        if (isOpen())
            RenderTexture::unbind();

        return isOpen();
    }

    //////////////////////////////////////////////

    void Window::open(const Settings& settings)
    {
        m_impl = std::make_unique<detail::WindowImpl>(settings);
        auto s = getSize();
        gl::Viewport(0, 0, s.x, s.y);

        static const Color defColor(SettingManager::getString("sDefaultWindowClearColor", "222222FF"));
        setClearColor(defColor);
    }

    //////////////////////////////////////////////

    void Window::close()
    {
        m_impl.reset();
    }

    //////////////////////////////////////////////

    bool Window::isOpen() const
    {
        return m_impl.operator bool();
    }

    //////////////////////////////////////////////

    WindowEventHandler* Window::getEventHandler()
    {
        return m_eventHandler.get();
    }

    //////////////////////////////////////////////

    void Window::removeEventHandler()
    {
        m_eventHandler.reset();
    }

    //////////////////////////////////////////////

    GLFWwindow* Window::getLibraryHandle()
    {
    #ifdef JOP_OS_DESKTOP
        if (isOpen())
            return m_impl->getLibraryHandle();
    #endif

        return nullptr;
    }

    //////////////////////////////////////////////

    WindowHandle Window::getNativeHandle()
    {
        if (isOpen())
            return m_impl->getNativeHandle();

        return nullptr;
    }

    //////////////////////////////////////////////

    void Window::pollEvents()
    {
        detail::WindowImpl::pollEvents();
    }

    //////////////////////////////////////////////

    void Window::setMouseMode(const Mouse::Mode mode)
    {
        if (isOpen())
            m_impl->setMouseMode(mode);
    }

    //////////////////////////////////////////////

    void Window::setPosition(const int x, const int y)
    {
        if (isOpen())
            m_impl->setPosition(x, y);
    }

    //////////////////////////////////////////////

    glm::ivec2 Window::getPosition() const
    {
        if (isOpen())
            return m_impl->getPosition();

        return glm::ivec2();
    }

    //////////////////////////////////////////////

    void Window::setSize(const int width, const int height)
    {
        if (isOpen())
            m_impl->setSize(width, height);
    }

    //////////////////////////////////////////////

    glm::uvec2 Window::getSize() const
    {
        if (isOpen())
            return m_impl->getSize();

        return glm::uvec2();
    }
}