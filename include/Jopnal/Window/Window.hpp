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

#ifndef JOP_WINDOW_HPP
#define JOP_WINDOW_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/RenderTarget.hpp>
#include <Jopnal/Utility/Clock.hpp>
#include <Jopnal/Window/Mouse.hpp>
#include <Jopnal/Window/WindowHandle.hpp>
#include <Jopnal/STL.hpp>
#include <glm/vec2.hpp>
#include <memory>
#include <string>
#include <mutex>

//////////////////////////////////////////////


namespace jop
{
    class WindowEventHandler;
    class Window;

    namespace detail
    {
        class WindowImpl;

        class BufferSwapper final : public Subsystem
        {
        private:

            JOP_DISALLOW_COPY_MOVE(BufferSwapper);

        public:

            BufferSwapper(Window& window);

            void draw() override;

        private:

            Clock m_frameClock;
            Window& m_windowRef;
        };
    }

    class JOP_API Window : public RenderTarget, public Subsystem
    {
    private:

        JOP_DISALLOW_COPY_MOVE(Window);

        friend class detail::BufferSwapper;

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
        struct JOP_API Settings
        {
            /// \brief Constructor
            ///
            /// \param loadSettings Load the settings using SettingManager?
            ///
            Settings(const bool loadSettings);

            glm::uvec4 colorBits;       ///< Color bits
            glm::uvec2 size;            ///< Window size (client area)
            std::string title;          ///< Window title
            DisplayMode displayMode;    ///< Display mode
            unsigned int samples;       ///< Sample count for multi sampling
            unsigned int depthBits;     ///< Depth bits
            unsigned int stencilBits;   ///< Stencil bits
            bool visible;               ///< Is the window initially visible?
            bool vSync;                 ///< Enable vertical sync?
            bool debug;                 ///< Ask for a debug context?
        };

    public:

        /// \brief Default constructor
        ///
        /// Does not open a window.
        ///
        Window();

        /// \brief Overloaded constructor
        ///
        /// Calls open() using the given settings.
        ///
        /// \param settings The window settings
        ///
        Window(const Settings& settings);

        /// \brief Destructor
        ///
        ~Window() override;

        
        /// \copybrief Subsystem::preUpdate()
        ///
        /// This will simply set an internal boolean flag so
        /// we know when a new frame has begun.
        ///
        void preUpdate(const float deltaTime) override;

        /// \copybrief Subsystem::postUpdate()
        ///
        /// This clears the OpenGL front buffer
        ///
        void postUpdate(const float deltaTime) override;

        /// \copybrief Subsystem::draw()
        ///
        /// This simply swaps the OpenGL buffers and displays the current
        /// back buffer.
        ///
        void draw() override;

        /// \copydoc RenderTarget::bind()
        ///
        bool bind() const override;

        /// \brief Open this window
        ///
        /// This will create a new window. If one already exists, it will be replaced.
        ///
        /// \param settings The window settings
        ///
        void open(const Settings& settings);

        /// \brief Close and destroy this window
        ///
        /// If no window is open, this does nothing.
        ///
        void close();

        /// \brief Check if this window is open
        ///
        /// \return True if the window is open
        ///
        bool isOpen() const;

        /// \brief Set the event handler
        ///
        /// This function will automatically pass a reference of this
        /// window into the handler's constructor.
        ///
        /// \param args The arguments to use with construction
        ///
        /// \return Reference to the newly created event handler
        ///
        template<typename T, typename ... Args>
        T& setEventHandler(Args&&... args);

        /// \brief Set the default handler
        ///
        /// The default event handler has no functionality other than
        /// exiting the engine when the close button is clicked.
        ///
        /// On android the default handler exits the engine when the
        /// "back" button is pressed.
        ///
        void setDefaultEventHandler();

        /// \brief Get the event handler
        ///
        /// \return Reference to the event handler. nullptr if the window hasn't been opened
        ///
        WindowEventHandler* getEventHandler();

        /// \brief Remove and delete the event handler
        ///
        /// After this call, there won't be a valid event handler.
        ///
        /// \see setDefaultEventHandler()
        ///
        void removeEventHandler();

        /// \brief Get the window library handle handle
        ///
        /// On desktop operating systems this is GLFWwindow*.
        ///
        /// \return A pointer to the handle. nullptr if the window hasn't been opened
        ///
        WindowLibHandle getLibraryHandle();

        /// \brief Get the native window handle
        ///
        /// \return The native window handle. nullptr if the window hasn't been opened
        ///
        WindowHandle getNativeHandle();

        /// \brief Poll the events of all open windows
        ///
        /// This will poll all events and invoke the appropriate callbacks.
        ///
        static void pollEvents();

        /// \brief Set the mouse mode
        ///
        /// \param mode The mouse mode
        ///
        /// \see Mouse::Mode
        ///
        void setMouseMode(const Mouse::Mode mode);

        /// \brief Set the window position
        ///
        /// \param x The X coordinate
        /// \param y The Y coordinate
        ///
        void setPosition(const int x, const int y);

        /// \brief Get the window position
        ///
        /// \return The window position
        ///
        glm::ivec2 getPosition() const;

        /// \brief Set the window size
        ///
        /// This will set the size of the client area.
        ///
        /// \param width New width
        /// \param height New height
        ///
        void setSize(const int width, const int height);

        /// \brief Get the size of the window
        ///
        /// This is the size of the window's frame buffer.
        ///
        /// \return Size of the window
        ///
        glm::uvec2 getSize() const override;

    private:

        /// \copydoc Subsystem::receiveMessage()
        ///
        Message::Result receiveMessage(const Message& message) override;


        std::unique_ptr<detail::WindowImpl> m_impl;         ///< The implementation object
        std::unique_ptr<WindowEventHandler> m_eventHandler; ///< The event handler
        unsigned int m_vertexArray;                         ///< Vertex array object
    };

    // Include the template implementation file
    #include <Jopnal/Window/Inl/Window.inl>
}

/// \class jop::Window
/// \ingroup window

#endif