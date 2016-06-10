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
    JOP_REGISTER_COMMAND_HANDLER(Window)

        JOP_BIND_MEMBER_COMMAND(&Window::setMouseMode, "setMouseMode");

    JOP_END_COMMAND_HANDLER(Window)
}

namespace
{
    bool ns_eventsPolled = false;

    struct jop_DefaultEventHandler : jop::WindowEventHandler
    {
        explicit jop_DefaultEventHandler(jop::Window& w) : jop::WindowEventHandler(w){}
        void closed() override {jop::Engine::exit();}
    };

    const char* const ns_settingStr[] =
    {
        /* 0 */ "engine@DefaultWindow|uSizeX",
        /* 1 */ "engine@DefaultWindow|uSizeY",
        /* 2 */ "engine@DefaultWindow|bVerticalSync"
    };
}

namespace jop
{
    BufferSwapper::BufferSwapper(Window& window)
        : Subsystem     (0),
          m_windowRef   (window)
    {}

    //////////////////////////////////////////////

    void BufferSwapper::draw()
    {
        if (m_windowRef.isOpen() && Engine::getState() != Engine::State::Frozen)
            m_windowRef.m_impl->swapBuffers();
    }


    //////////////////////////////////////////////


    Window::Settings::Settings(const bool loadSettings)
        : size          (1u, 1u),
          title         ("Window Title"),
          displayMode   (DisplayMode::Windowed),
          samples       (0),
          maxFrameRate  (0),
          visible       (false),
          vSync         (true),
          debug         (false)
    {
        if (loadSettings)
        {
            typedef SettingManager SM;

            static const glm::uvec2 deskTop = VideoInfo::getDesktopResolution();

            size.x = SM::get<unsigned int>(ns_settingStr[0], deskTop.x); size.y = SM::get<unsigned int>(ns_settingStr[1], deskTop.y);
            title = SM::get<std::string>("engine@DefaultWindow|sTitle", getProjectName());
            displayMode = static_cast<Window::DisplayMode>(std::min(2u, SM::get<unsigned int>("engine@DefaultWindow|uMode", 2)));
            samples = SM::get<unsigned int>("engine@DefaultWindow|uMultisampling", 0);
            maxFrameRate = SM::get<unsigned int>("engine@DefaultWindow|uFrameLimit", 0);
            visible = SM::get<bool>("engine@DefaultWindow|bVisible", true);
            vSync = SM::get<bool>(ns_settingStr[2], true);
            debug = SM::get<bool>("engine@DefaultWindow|bDebugContext", false);
        }
    }

    //////////////////////////////////////////////

    Window::Window()
        : RenderTarget      (0),
          m_impl            (),
          m_eventHandler    ()
    {}

    Window::Window(const Settings& settings)
        : RenderTarget      (0),
          m_impl            (),
          m_eventHandler    ()
    {
        open(settings);
        setDefaultEventHandler();

    #ifdef JOP_DEBUG_MODE

        if (ogl_ext_KHR_debug != ogl_LOAD_FAILED && settings.debug)
        {
            glDebugMessageCallback([](GLenum, GLenum, GLuint, GLenum severity, GLsizei, const GLchar* msg, const void*)
            {
                if (severity == GL_DEBUG_SEVERITY_HIGH)
                    JOP_DEBUG_ERROR(msg)

                else if (severity == GL_DEBUG_SEVERITY_MEDIUM)
                    JOP_DEBUG_WARNING(msg)

                else if (severity == GL_DEBUG_SEVERITY_LOW)
                    JOP_DEBUG_INFO(msg)

                else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
                    JOP_DEBUG_DIAG(msg);

            }, NULL);
        }

    #endif
    }

    Window::~Window()
    {}

    //////////////////////////////////////////////

    void Window::preUpdate(const float)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        ns_eventsPolled = false;
    }

    //////////////////////////////////////////////

    void Window::postUpdate(const float deltaTime)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (isOpen() && Engine::getState() != Engine::State::Frozen)
            RenderTarget::postUpdate(deltaTime);
    }

    //////////////////////////////////////////////

    void Window::draw()
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        // Only poll the events if they haven't yet been during this frame.
        // We care about this because we don't want to invoke controller
        // callbacks multiple times.
        if (!ns_eventsPolled)
        {
            static const bool controllers = SettingManager::get<unsigned int>("engine@Input|Controller|uMaxControllers", 1) > 0;

            pollEvents();

            if (controllers && m_eventHandler)
                m_eventHandler->handleControllerInput();

            ns_eventsPolled = true;
        }
    }

    //////////////////////////////////////////////

    bool Window::bind() const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (isOpen())
            RenderTexture::unbind();

        return isOpen();
    }

    //////////////////////////////////////////////

    void Window::open(const Settings& settings)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        m_impl = std::make_unique<detail::WindowImpl>(settings, *this);
        auto s = getSize();

        static const Color defColor(SettingManager::get<std::string>("engine@DefaultWindow|sClearColor", "000000FF"));
        setClearColor(defColor);
    }

    //////////////////////////////////////////////

    void Window::close()
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        m_impl.reset();
    }

    //////////////////////////////////////////////

    bool Window::isOpen() const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        return m_impl.operator bool();
    }

    //////////////////////////////////////////////

    void Window::setDefaultEventHandler()
    {
        setEventHandler<jop_DefaultEventHandler>();
    }

    //////////////////////////////////////////////

    WindowEventHandler* Window::getEventHandler()
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        return m_eventHandler.get();
    }

    //////////////////////////////////////////////

    void Window::removeEventHandler()
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        m_eventHandler.reset();
    }

    //////////////////////////////////////////////

    GLFWwindow* Window::getLibraryHandle()
    {
    #ifdef JOP_OS_DESKTOP
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (isOpen())
            return m_impl->getLibraryHandle();
    #endif

        return nullptr;
    }

    //////////////////////////////////////////////

    WindowHandle Window::getNativeHandle()
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

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
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (isOpen())
            m_impl->setMouseMode(mode);
    }

    //////////////////////////////////////////////

    void Window::setPosition(const int x, const int y)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (isOpen())
            m_impl->setPosition(x, y);
    }

    //////////////////////////////////////////////

    glm::ivec2 Window::getPosition() const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (isOpen())
            return m_impl->getPosition();

        return glm::ivec2();
    }

    //////////////////////////////////////////////

    void Window::setSize(const int width, const int height)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (isOpen())
            m_impl->setSize(width, height);
    }

    //////////////////////////////////////////////

    glm::uvec2 Window::getSize() const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (isOpen())
            return m_impl->getSize();

        return glm::uvec2();
    }

    //////////////////////////////////////////////

    Window* Window::getCurrentContextWindow()
    {
        return detail::WindowImpl::getCurrentContextWindow();
    }

    //////////////////////////////////////////////

    Message::Result Window::receiveMessage(const Message& message)
    {
        if (JOP_EXECUTE_COMMAND(Window, message.getString(), this) == Message::Result::Escape)
            return Message::Result::Escape;

        return Subsystem::receiveMessage(message);
    }
}