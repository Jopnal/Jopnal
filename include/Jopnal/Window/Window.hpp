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

#ifndef JOP_WINDOW_HPP
#define JOP_WINDOW_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Subsystem.hpp>
#include <glm/vec2.hpp>
#include <memory>
#include <string>

//////////////////////////////////////////////


struct GLFWwindow;

namespace jop
{
    class WindowEventHandler;

    namespace detail
    {
        class WindowImpl;
    }

    class JOP_API Window final : public Subsystem
    {
    private:

        JOP_DISALLOW_COPY(Window);

    public:

        /// Display modes
        ///
        enum class DisplayMode
        {
            Windowed,   ///< Windowed with decorations
            Borderless, ///< Windowed without decorations
            Fullscreen  ///< Full screen
        };

        /// Window settings
        ///
        struct Settings
        {
            glm::uvec2 size;
            std::string title;
            DisplayMode displayMode;
            bool visible;
        };

    public:

        /// \brief Default constructor
        ///
        /// Does not open a window.
        ///
        Window();

        /// \brief Overloaded constructor
        ///
        /// Calls open() using the given settings
        ///
        /// \param settings The window settings
        ///
        Window(const Settings& settings);

        /// \brief Move constructor
        Window(Window&& other);

        /// \brief Move assignment operator
        ///
        Window& operator =(Window&& other);

        /// \brief Destructor
        ///
        ~Window() override;


        /// \brief The post-update function
        ///
        /// This clears the OpenGL front buffer
        ///
        void postUpdate(const double dt) override;

        /// \brief The post-draw function
        ///
        /// This swaps the OpenGL buffers
        ///
        void postDraw() override;

        /// \brief Open this window
        ///
        /// This will create a new window. If one already exists, it will be replaced.
        ///
        /// \param settings The window settings
        ///
        void open(const Settings& settings);

        /// \brief Destroy this window
        ///
        void close();

        /// \brief Check if this window is open
        ///
        /// \return True if window is open
        ///
        bool isOpen() const;

        /// \brief Set the clear color
        ///
        /// \param color The new color
        ///
        void setClearColor(const Color& color);

        /// \brief Set the event handler
        ///
        /// This function will automatically pass a reference of this
        /// window into the handler's constructor.
        ///
        /// \param args The arguments to use with construction
        ///
        template<typename T, typename ... Args>
        T& setEventHandler(Args&... args);

        /// \brief Get the event handler
        ///
        /// \return Pointer to the event handler. NUllptr if none exists
        ///
        WindowEventHandler* getEventHandler();

        /// \brief Remove the event handler
        ///
        void removeEventHandler();

        /// \brief Get the GLFW library window handle
        ///
        /// \return A pointer to the handle. Nullptr if window hasn't been created
        ///
        GLFWwindow* getLibraryHandle();

        /// \brief Poll the events of all open windows
        ///
        /// This will poll all events and invoke the appropriate callbacks.
        ///
        static void pollEvents();

    private:

        Color m_clearColor;                                 ///< The color to use when clearing the buffer
        std::unique_ptr<detail::WindowImpl> m_impl;         ///< The implementation object
        std::unique_ptr<WindowEventHandler> m_eventHandler; ///< The event handler

    };

    // Include the template implementation file
    #include <Jopnal/Window/Inl/Window.inl>
}

#endif

/// \class Window
/// \ingroup window
///
/// #TODO Detailed description