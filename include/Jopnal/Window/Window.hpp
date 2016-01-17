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
#include <glm/vec2.hpp>
#include <memory>
#include <string>

//////////////////////////////////////////////


struct GLFWwindow;

namespace jop
{
    namespace detail
    {
        class WindowImpl;
    }

    class JOP_API Window
    {
    private:

        JOP_DISALLOW_COPY(Window);

        friend class Engine;

        static void initialize();

        static void deInitialize();

    public:

        /// Display modes
        enum class DisplayMode
        {
            Windowed,   ///< Windowed with decorations
            Borderless, ///< Windowed without decorations
            Fullscreen  ///< Full screen
        };

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
        ~Window();


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

        std::unique_ptr<detail::WindowImpl> m_impl; ///< The implementation object

    };
}

#endif

/// \class Window
/// \brief Class for housing & managing a window
/// \addtogroup Window
///
/// #TODO Detailed description