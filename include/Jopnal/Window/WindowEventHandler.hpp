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

#ifndef JOP_WINDOWEVENTHANDLER_HPP
#define JOP_WINDOWEVENTHANDLER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <string>

//////////////////////////////////////////////


namespace jop
{
    class Window;

    class JOP_API WindowEventHandler
    {
    private:

        JOP_DISALLOW_COPY_MOVE(WindowEventHandler);

        friend class Window;

    public:
    
        /// \brief Constructor
        ///
        /// \param windowRef Reference to the window this handler will be bound to
        ///
        WindowEventHandler(Window& windowRef);

        /// \brief Virtual destructor
        ///
        virtual ~WindowEventHandler() = 0;


        /// \brief Close callback
        ///
        /// This will be called if the window was requested to be closed.
        ///
        virtual void closed();

        /// \brief Focus lost callback
        ///
        /// This will be called when the window loses focus or is minimized.
        ///
        virtual void lostFocus();

        /// \brief Gained focus callback
        ///
        /// This will be called when the window gains focus or is restored.
        ///
        virtual void gainedFocus();

        /// \brief Key pressed callback
        ///
        /// This will be called when a keyboard key is pressed.
        ///
        /// \param key The key that was pressed
        /// \param scanCode The scancode
        /// \param mods Modifiers
        ///
        virtual void keyPressed(const int key, const int scanCode, const int mods);

        /// \brief Key released callback
        ///
        /// This will be called when a keyboard key is released.
        ///
        /// \param key The key that was released
        /// \param scanCode The scancode
        /// \param mods Modifiers
        ///
        virtual void keyReleased(const int key, const int scanCode, const int mods);

        /// \brief Text entered callback
        ///
        /// This will produce an unicode code point whenever text is entered.
        ///
        /// \param code The character code point
        ///
        virtual void textEntered(const unsigned int code);

        /// \brief Mouse moved callback
        ///
        /// This will be called when the mouse is moved.
        ///
        /// \param x The vertical position/offset
        /// \param y The horizontal position/offset
        ///
        virtual void mouseMoved(const float x, const float y);

        /// \brief Mouse button pressed callback
        ///
        /// This will be called when one of the mouse buttons was pressed.
        ///
        /// \param button The button that was pressed
        /// \param mods Modifiers
        ///
        virtual void mouseButtonPressed(const int button, const int mods);

        /// \brief Mouse button released callback
        ///
        /// This will be called when one of the mouse buttons was released.
        ///
        /// \param button The button that was released
        /// \param mods Modifiers
        ///
        virtual void mouseButtonReleased(const int button, const int mods);

        /// \brief Mouse left callback
        ///
        /// This will be called when the mouse cursor leaves the window.
        ///
        virtual void mouseLeft();

        /// \brief Mouse entered callback
        ///
        /// This will be called when the mouse cursor enters the window.
        ///
        virtual void mouseEntered();

        /// \brief Mouse scrolled callback
        ///
        /// This will be called when the mouse wheel is scrolled.
        ///
        /// \param x The vertical offset
        /// \param y The horizontal offset
        ///
        virtual void mouseScrolled(const float x, const float y);

        /// \brief Controller connected callback
        ///
        /// This will be called when a controller or a joystick is connected.
        ///
        /// \param index Index of the new controller
        /// \param name Name of the controller
        ///
        virtual void controllerConnected(const int index, const std::string& name);

        /// \brief Controller disconnected callback
        ///
        /// \param index Index of the controller
        ///
        virtual void controllerDisconnected(const int index);

        /// \brief Controller axis callback
        ///
        /// \param index Index of the controller
        /// \param shift Shift of the axis
        ///
        virtual void controllerAxisShifted(const int index, const int axisIndex, const float shift);

        /// \brief Controller button pressed callback
        ///
        /// \param index Index of the controller
        /// \param button The button that was pressed
        ///
        virtual void controllerButtonPressed(const int index, const int button);

        /// \brief Controller button released callback
        ///
        /// \param index Index of the controller
        /// \param button The button that was released
        ///
        virtual void controllerButtonReleased(const int index, const int button);

    private:

        /// \brief Internal function to invoke controller callbacks
        ///
        void handleControllerInput();

    protected:

        Window& m_windowRef; ///< Reference to the window
        
    };
}

#endif

/// \class WindowEventHandler
/// \ingroup window
///
/// #TODO Detailed description