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

#ifndef JOP_WINDOWEVENTHANDLER_HPP
#define JOP_WINDOWEVENTHANDLER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <glm/vec2.hpp>
#include <string>

//////////////////////////////////////////////


namespace jop
{
    class Window;
    class Controller;

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

        /// \brief Resize callback
        ///
        /// This will be called if/when the window frame buffer size changes
        ///
        /// \param newWidth The new width
        /// \param newHeight The new height
        ///
        virtual void resized(const unsigned int newWidth, const unsigned int newHeight);

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
        /// \param scanCode The scan code
        /// \param mods Modifiers
        ///
        virtual void keyPressed(const int key, const int scanCode, const int mods);

        /// \brief Key released callback
        ///
        /// This will be called when a keyboard key is released.
        ///
        /// \param key The key that was released
        /// \param scanCode The scan code
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
        /// \param x The vertical offset
        /// \param y The horizontal offset
        ///
        virtual void mouseMoved(const float x, const float y);

        /// \brief Mouse position callback
        ///
        /// This will be called when the mouse is moved.
        ///
        /// \param x The vertical position
        /// \param y The horizontal position
        ///
        virtual void mouseMovedAbsolute(const float x, const float y);

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
        virtual    void mouseScrolled(const float x, const float y);

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
        /// \param axisIndex Index of the axis
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

        /// \brief Touch event callback
        ///
        /// This will be called when touch is received.
        ///
        /// \param touchId Id for finger in case of multiple touches
        /// \param x The vertical position
        /// \param y The horizontal position
        ///
        virtual void touchPressed(const int touchId, const float x, const float y);

        /// \brief Touch event callback
        ///
        /// This will be called when touch is ended.
        ///
        /// \param touchId Id for finger in case of multiple touches
        /// \param x The vertical position
        /// \param y The horizontal position
        ///
        virtual void touchReleased(const int touchId, const float x, const float y);

        /// \brief Touch event callback
        ///
        /// This will be called when touch isn't stationary.
        ///
        /// \param touchId Id for finger in case of multiple touches
        /// \param x The vertical position/offset
        /// \param y The horizontal position/offset
        ///
        virtual void touchMoved(const int touchId, const float x, const float y);

        /// \brief Touch event callback
        ///
        /// This will be called when touch isn't stationary.
        ///
        /// \param touchId Id for finger in case of multiple touches
        /// \param x The vertical offset
        /// \param y The horizontal offset
        ///
        virtual void touchMovedAbsolute(const int touchId, const float x, const float y);

        /// \brief Information about touch event callback.
        ///
        /// \param touchId Id for finger in case of multiple touches
        /// \param info Enum value of information
        /// \param value Pressure or radius of touch
        ///
        virtual void touchInfo(const int touchId, const int info, const float value);

        /// \brief Touch event callback
        ///
        /// This will be called when touch has momentum only on one axis.
        ///
        /// \param x The vertical offset
        /// \param y The horizontal offset
        ///
        virtual void touchScrolled(const float x, const float y);
        
   private:

        /// \brief Internal function to invoke controller callbacks
        ///
        void handleControllerInput();

    protected:

        Window& m_windowRef;    ///< Reference to the window

    public:
        
        float m_lastMouseX;     ///< For internal use. Do not touch
        float m_lastMouseY;     ///< For internal use. Do not touch
    };
}

#endif

/// \class jop::WindowEventHandler
/// \ingroup window
///
///