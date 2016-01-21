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

#if defined(JOP_OS_DESKTOP)
    #include <Jopnal/Window/Desktop/WindowImpl.hpp>
#endif

//////////////////////////////////////////////


namespace jop
{
    Window::Window()
        : Subsystem         ("Window"),
          m_impl            (),
          m_eventHandler    ()
    {}

    Window::Window(const Settings& settings)
        : Subsystem         ("Window"),
          m_impl            (),
          m_eventHandler    ()
    {
        open(settings);
    }

    Window::Window(Window&& other)
        : Subsystem ("Window"),
          m_impl    (std::move(other.m_impl))
    {}

    Window& Window::operator =(Window&& other)
    {
        m_impl = std::move(other.m_impl);
        return *this;
    }

    Window::~Window()
    {
        // The event handler needs to be reset before the window itself
        m_eventHandler.reset();
    }

    //////////////////////////////////////////////

    void Window::postUpdate(const double)
    {
        if (isOpen())
        {
            auto c = m_clearColor.asFloatVector();

            glCheck(gl::ClearColor(c.r, c.g, c.b, c.a));
            glCheck(gl::ClearDepth(static_cast<GLdouble>(1.f)));
            glCheck(gl::ClearStencil(static_cast<GLint>(0)));

            glCheck(gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT | gl::STENCIL_BUFFER_BIT));
        }
    }

    //////////////////////////////////////////////

    void Window::postDraw()
    {
        if (isOpen())
            m_impl->swapBuffers();

        // Poll window events here instead of the main loop.
        // This causes the events to be polled multiple times if there
        // are multiple windows but we don't really need to care about that.
        pollEvents();
    }

    //////////////////////////////////////////////

    void Window::open(const Settings& settings)
    {
        m_impl = std::make_unique<detail::WindowImpl>(settings);
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

    void Window::setClearColor(const Color& color)
    {
        m_clearColor = color;
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

    void Window::pollEvents()
    {
        detail::WindowImpl::pollEvents();
    }
}