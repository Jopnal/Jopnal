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

//////////////////////////////////////////////


namespace
{
    std::array<bool, 16> ns_controllersPresent;
    std::array<std::vector<unsigned char>, 16> ns_controllerButtonStates;
}

namespace
{
#ifdef JOP_OS_DESKTOP
    int getJopKey(const int glfwKey)
    {
        using k = jop::Keyboard::Key;

        switch (glfwKey)
        {
            case GLFW_KEY_UNKNOWN:           return k::Unknown;
            case GLFW_KEY_SPACE:             return k::Space;
            case GLFW_KEY_APOSTROPHE:        return k::Apostrophe;
            case GLFW_KEY_COMMA:             return k::Comma;
            case GLFW_KEY_MINUS:             return k::Minus;
            case GLFW_KEY_PERIOD:            return k::Period;
            case GLFW_KEY_SLASH:             return k::Slash;
            case GLFW_KEY_0:                 return k::Zero;
            case GLFW_KEY_1:                 return k::One;
            case GLFW_KEY_2:                 return k::Two;
            case GLFW_KEY_3:                 return k::Three;
            case GLFW_KEY_4:                 return k::Four;
            case GLFW_KEY_5:                 return k::Five;
            case GLFW_KEY_6:                 return k::Six;
            case GLFW_KEY_7:                 return k::Seven;
            case GLFW_KEY_8:                 return k::Eight;
            case GLFW_KEY_9:                 return k::Nine;
            case GLFW_KEY_SEMICOLON:         return k::Semicolon;
            case GLFW_KEY_EQUAL:             return k::Equal;
            case GLFW_KEY_A:                 return k::A;
            case GLFW_KEY_B:                 return k::B;
            case GLFW_KEY_C:                 return k::C;
            case GLFW_KEY_D:                 return k::D;
            case GLFW_KEY_E:                 return k::E;
            case GLFW_KEY_F:                 return k::F;
            case GLFW_KEY_G:                 return k::G;
            case GLFW_KEY_H:                 return k::H;
            case GLFW_KEY_I:                 return k::I;
            case GLFW_KEY_J:                 return k::J;
            case GLFW_KEY_K:                 return k::K;
            case GLFW_KEY_L:                 return k::L;
            case GLFW_KEY_M:                 return k::M;
            case GLFW_KEY_N:                 return k::N;
            case GLFW_KEY_O:                 return k::O;
            case GLFW_KEY_P:                 return k::P;
            case GLFW_KEY_Q:                 return k::Q;
            case GLFW_KEY_R:                 return k::R;
            case GLFW_KEY_S:                 return k::S;
            case GLFW_KEY_T:                 return k::T;
            case GLFW_KEY_U:                 return k::U;
            case GLFW_KEY_V:                 return k::V;
            case GLFW_KEY_W:                 return k::W;
            case GLFW_KEY_X:                 return k::X;
            case GLFW_KEY_Y:                 return k::Y;
            case GLFW_KEY_Z:                 return k::Z;
            case GLFW_KEY_LEFT_BRACKET:      return k::LBracket;
            case GLFW_KEY_BACKSLASH:         return k::Backslash;
            case GLFW_KEY_RIGHT_BRACKET:     return k::RBracket;
            case GLFW_KEY_GRAVE_ACCENT:      return k::GraveAccent;
            case GLFW_KEY_WORLD_1:           return k::World1;
            case GLFW_KEY_WORLD_2:           return k::World2;
            case GLFW_KEY_ESCAPE:            return k::Escape;
            case GLFW_KEY_ENTER:             return k::Enter;
            case GLFW_KEY_TAB:               return k::Tab;
            case GLFW_KEY_BACKSPACE:         return k::Backspace;
            case GLFW_KEY_INSERT:            return k::Insert;
            case GLFW_KEY_DELETE:            return k::Delete;
            case GLFW_KEY_RIGHT:             return k::Right;
            case GLFW_KEY_LEFT:              return k::Left;
            case GLFW_KEY_DOWN:              return k::Down;
            case GLFW_KEY_UP:                return k::Up;
            case GLFW_KEY_PAGE_UP:           return k::PgUp;
            case GLFW_KEY_PAGE_DOWN:         return k::PgDown;
            case GLFW_KEY_HOME:              return k::Home;
            case GLFW_KEY_END:               return k::End;
            case GLFW_KEY_CAPS_LOCK:         return k::CapsLock;
            case GLFW_KEY_SCROLL_LOCK:       return k::ScrollLock;
            case GLFW_KEY_NUM_LOCK:          return k::NumLock;
            case GLFW_KEY_PRINT_SCREEN:      return k::PrintScreen;
            case GLFW_KEY_PAUSE:             return k::Pause;
            case GLFW_KEY_F1:                return k::F1;
            case GLFW_KEY_F2:                return k::F2;
            case GLFW_KEY_F3:                return k::F3;
            case GLFW_KEY_F4:                return k::F4;
            case GLFW_KEY_F5:                return k::F5;
            case GLFW_KEY_F6:                return k::F6;
            case GLFW_KEY_F7:                return k::F7;
            case GLFW_KEY_F8:                return k::F8;
            case GLFW_KEY_F9:                return k::F9;
            case GLFW_KEY_F10:               return k::F10;
            case GLFW_KEY_F11:               return k::F11;
            case GLFW_KEY_F12:               return k::F12;
            case GLFW_KEY_KP_0:              return k::KeypadZero;
            case GLFW_KEY_KP_1:              return k::KeypadOne;
            case GLFW_KEY_KP_2:              return k::KeypadTwo;
            case GLFW_KEY_KP_3:              return k::KeypadThree;
            case GLFW_KEY_KP_4:              return k::KeypadFour;
            case GLFW_KEY_KP_5:              return k::KeypadFive;
            case GLFW_KEY_KP_6:              return k::KeypadSix;
            case GLFW_KEY_KP_7:              return k::KeypadSeven;
            case GLFW_KEY_KP_8:              return k::KeypadEight;
            case GLFW_KEY_KP_9:              return k::KeypadNine;
            case GLFW_KEY_KP_DECIMAL:        return k::KeypadDecimal;
            case GLFW_KEY_KP_DIVIDE:         return k::KeypadDivide;
            case GLFW_KEY_KP_MULTIPLY:       return k::KeypadMultiply;
            case GLFW_KEY_KP_SUBTRACT:       return k::KeypadSubtract;
            case GLFW_KEY_KP_ADD:            return k::KeypadAdd;
            case GLFW_KEY_KP_ENTER:          return k::KeypadEnter;
            case GLFW_KEY_KP_EQUAL:          return k::KeypadEqual;
            case GLFW_KEY_LEFT_SHIFT:        return k::LShift;
            case GLFW_KEY_LEFT_CONTROL:      return k::LControl;
            case GLFW_KEY_LEFT_ALT:          return k::LAlt;
            case GLFW_KEY_LEFT_SUPER:        return k::LSuper;
            case GLFW_KEY_RIGHT_SHIFT:       return k::RShift;
            case GLFW_KEY_RIGHT_CONTROL:     return k::RControl;
            case GLFW_KEY_RIGHT_ALT:         return k::RAlth;
            case GLFW_KEY_RIGHT_SUPER:       return k::RSuper;
            case GLFW_KEY_MENU:              return k::Menu;
        }

        return k::Unknown;
    }

    int getGlfwKey(const int jopKey)
    {
        using k = jop::Keyboard::Key;

        switch (jopKey)
        {
            case k::Unknown:             return GLFW_KEY_UNKNOWN;
            case k::Space:               return GLFW_KEY_SPACE;
            case k::Apostrophe:          return GLFW_KEY_APOSTROPHE;
            case k::Comma:               return GLFW_KEY_COMMA;
            case k::Minus:               return GLFW_KEY_MINUS;
            case k::Period:              return GLFW_KEY_PERIOD;
            case k::Slash:               return GLFW_KEY_SLASH;
            case k::Zero:                return GLFW_KEY_0;
            case k::One:                 return GLFW_KEY_1;
            case k::Two:                 return GLFW_KEY_2;
            case k::Three:               return GLFW_KEY_3;
            case k::Four:                return GLFW_KEY_4;
            case k::Five:                return GLFW_KEY_5;
            case k::Six:                 return GLFW_KEY_6;
            case k::Seven:               return GLFW_KEY_7;
            case k::Eight:               return GLFW_KEY_8;
            case k::Nine:                return GLFW_KEY_9;
            case k::Semicolon:           return GLFW_KEY_SEMICOLON;
            case k::Equal:               return GLFW_KEY_EQUAL;
            case k::A:                   return GLFW_KEY_A;
            case k::B:                   return GLFW_KEY_B;
            case k::C:                   return GLFW_KEY_C;
            case k::D:                   return GLFW_KEY_D;
            case k::E:                   return GLFW_KEY_E;
            case k::F:                   return GLFW_KEY_F;
            case k::G:                   return GLFW_KEY_G;
            case k::H:                   return GLFW_KEY_H;
            case k::I:                   return GLFW_KEY_I;
            case k::J:                   return GLFW_KEY_J;
            case k::K:                   return GLFW_KEY_K;
            case k::L:                   return GLFW_KEY_L;
            case k::M:                   return GLFW_KEY_M;
            case k::N:                   return GLFW_KEY_N;
            case k::O:                   return GLFW_KEY_O;
            case k::P:                   return GLFW_KEY_P;
            case k::Q:                   return GLFW_KEY_Q;
            case k::R:                   return GLFW_KEY_R;
            case k::S:                   return GLFW_KEY_S;
            case k::T:                   return GLFW_KEY_T;
            case k::U:                   return GLFW_KEY_U;
            case k::V:                   return GLFW_KEY_V;
            case k::W:                   return GLFW_KEY_W;
            case k::X:                   return GLFW_KEY_X;
            case k::Y:                   return GLFW_KEY_Y;
            case k::Z:                   return GLFW_KEY_Z;
            case k::LBracket:            return GLFW_KEY_LEFT_BRACKET;
            case k::Backslash:           return GLFW_KEY_BACKSLASH;
            case k::RBracket:            return GLFW_KEY_RIGHT_BRACKET;
            case k::GraveAccent:         return GLFW_KEY_GRAVE_ACCENT;
            case k::World1:              return GLFW_KEY_WORLD_1;
            case k::World2:              return GLFW_KEY_WORLD_2;
            case k::Escape:              return GLFW_KEY_ESCAPE;
            case k::Enter:               return GLFW_KEY_ENTER;
            case k::Tab:                 return GLFW_KEY_TAB;
            case k::Backspace:           return GLFW_KEY_BACKSPACE;
            case k::Insert:              return GLFW_KEY_INSERT;
            case k::Delete:              return GLFW_KEY_DELETE;
            case k::Right:               return GLFW_KEY_RIGHT;
            case k::Left:                return GLFW_KEY_LEFT;
            case k::Down:                return GLFW_KEY_DOWN;
            case k::Up:                  return GLFW_KEY_UP;
            case k::PgUp:                return GLFW_KEY_PAGE_UP;
            case k::PgDown:              return GLFW_KEY_PAGE_DOWN;
            case k::Home:                return GLFW_KEY_HOME;
            case k::End:                 return GLFW_KEY_END;
            case k::CapsLock:            return GLFW_KEY_CAPS_LOCK;
            case k::ScrollLock:          return GLFW_KEY_SCROLL_LOCK;
            case k::NumLock:             return GLFW_KEY_NUM_LOCK;
            case k::PrintScreen:         return GLFW_KEY_PRINT_SCREEN;
            case k::Pause:               return GLFW_KEY_PAUSE;
            case k::F1:                  return GLFW_KEY_F1;
            case k::F2:                  return GLFW_KEY_F2;
            case k::F3:                  return GLFW_KEY_F3;
            case k::F4:                  return GLFW_KEY_F4;
            case k::F5:                  return GLFW_KEY_F5;
            case k::F6:                  return GLFW_KEY_F6;
            case k::F7:                  return GLFW_KEY_F7;
            case k::F8:                  return GLFW_KEY_F8;
            case k::F9:                  return GLFW_KEY_F9;
            case k::F10:                 return GLFW_KEY_F10;
            case k::F11:                 return GLFW_KEY_F11;
            case k::F12:                 return GLFW_KEY_F12;
            case k::KeypadZero:          return GLFW_KEY_KP_0;
            case k::KeypadOne:           return GLFW_KEY_KP_1;
            case k::KeypadTwo:           return GLFW_KEY_KP_2;
            case k::KeypadThree:         return GLFW_KEY_KP_3;
            case k::KeypadFour:          return GLFW_KEY_KP_4;
            case k::KeypadFive:          return GLFW_KEY_KP_5;
            case k::KeypadSix:           return GLFW_KEY_KP_6;
            case k::KeypadSeven:         return GLFW_KEY_KP_7;
            case k::KeypadEight:         return GLFW_KEY_KP_8;
            case k::KeypadNine:          return GLFW_KEY_KP_9;
            case k::KeypadDecimal:       return GLFW_KEY_KP_DECIMAL;
            case k::KeypadDivide:        return GLFW_KEY_KP_DIVIDE;
            case k::KeypadMultiply:      return GLFW_KEY_KP_MULTIPLY;
            case k::KeypadSubtract:      return GLFW_KEY_KP_SUBTRACT;
            case k::KeypadAdd:           return GLFW_KEY_KP_ADD;
            case k::KeypadEnter:         return GLFW_KEY_KP_ENTER;
            case k::KeypadEqual:         return GLFW_KEY_KP_EQUAL;
            case k::LShift:              return GLFW_KEY_LEFT_SHIFT;
            case k::LControl:            return GLFW_KEY_LEFT_CONTROL;
            case k::LAlt:                return GLFW_KEY_LEFT_ALT;
            case k::LSuper:              return GLFW_KEY_LEFT_SUPER;
            case k::RShift:              return GLFW_KEY_RIGHT_SHIFT;
            case k::RControl:            return GLFW_KEY_RIGHT_CONTROL;
            case k::RAlth:               return GLFW_KEY_RIGHT_ALT;
            case k::RSuper:              return GLFW_KEY_RIGHT_SUPER;
            case k::Menu:                return GLFW_KEY_MENU;
        }

        return GLFW_KEY_UNKNOWN;
    }

    int getJopMouseButton(const int glfwButton)
    {
        using m = jop::Mouse::Button;

        switch (glfwButton)
        {
            case GLFW_MOUSE_BUTTON_LEFT:    return m::Left;
            case GLFW_MOUSE_BUTTON_RIGHT:   return m::Right;
            case GLFW_MOUSE_BUTTON_MIDDLE:  return m::Middle;
            case GLFW_MOUSE_BUTTON_4:       return m::X1;
            case GLFW_MOUSE_BUTTON_5:       return m::X2;
            case GLFW_MOUSE_BUTTON_6:       return m::X3;
            case GLFW_MOUSE_BUTTON_7:       return m::X4;
            case GLFW_MOUSE_BUTTON_8:       return m::X5;
        }
        
        return m::Unknown;
    }

    int getGlfwButton(const int jopButton)
    {
        using m = jop::Mouse::Button;

        switch (jopButton)
        {
            case m::Left:   return GLFW_MOUSE_BUTTON_LEFT;
            case m::Right:  return GLFW_MOUSE_BUTTON_RIGHT;
            case m::Middle: return GLFW_MOUSE_BUTTON_MIDDLE;
            case m::X1:     return GLFW_MOUSE_BUTTON_4;
            case m::X2:     return GLFW_MOUSE_BUTTON_5;
            case m::X3:     return GLFW_MOUSE_BUTTON_6;
            case m::X4:     return GLFW_MOUSE_BUTTON_7;
            case m::X5:     return GLFW_MOUSE_BUTTON_8;
        }

        return GLFW_MOUSE_BUTTON_1;
    }
#endif
}
namespace jop
{
    WindowEventHandler::WindowEventHandler(Window& windowRef)
        : m_windowRef(windowRef)
    {
        static bool init = false;

        if (!init)
        {
            std::memset(ns_controllersPresent.data(), 0, ns_controllersPresent.size() * sizeof(bool));

            for (auto& i : ns_controllerButtonStates)
                i.resize(20, 0);

            init = true;
        }

    #ifdef JOP_OS_DESKTOP

        auto handle = windowRef.getLibraryHandle();
        glfwSetWindowUserPointer(handle, this);

        // Close callback
        glfwSetWindowCloseCallback(handle, [](GLFWwindow* w)
        {
            static_cast<WindowEventHandler*>(glfwGetWindowUserPointer(w))->closed();
        });

        // Lost/restored focus callback
        glfwSetWindowIconifyCallback(handle, [](GLFWwindow* w, int iconified)
        {
            auto handler = static_cast<WindowEventHandler*>(glfwGetWindowUserPointer(w));

            if (iconified)
                handler->lostFocus();
            else
                handler->gainedFocus();
        });

        // Keyboard callback
        glfwSetKeyCallback(handle, [](GLFWwindow* w, int key, int scancode, int action, int mods)
        {
            auto handler = static_cast<WindowEventHandler*>(glfwGetWindowUserPointer(w));

            switch (action)
            {
                case GLFW_REPEAT:
                case GLFW_PRESS:
                    handler->keyPressed(getJopKey(key), scancode, mods);
                    break;
                case GLFW_RELEASE:
                    handler->keyReleased(getJopKey(key), scancode, mods);
            }
        });

        // Text callback
        glfwSetCharCallback(handle, [](GLFWwindow* w, unsigned int codepoint)
        {
            static_cast<WindowEventHandler*>(glfwGetWindowUserPointer(w))->textEntered(codepoint);
        });

        // Mouse position callback
        // First set the initial mouse position
        double x = 0.0, y = 0.0;
        glfwGetCursorPos(handle, &x, &y);
        m_lastMouseX = static_cast<float>(x);
        m_lastMouseY = static_cast<float>(y);

        glfwSetCursorPosCallback(handle, [](GLFWwindow* w, double x, double y)
        {
            auto h = static_cast<WindowEventHandler*>(glfwGetWindowUserPointer(w));

            double realX = h->m_lastMouseX;
            double realY = h->m_lastMouseY;

            if (glfwGetInputMode(w, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
            {
                realX -= x;
                realY -= y;
            }
            else
            {
                realX = -x;
                realY = -y;
            }

            h->mouseMoved(static_cast<float>(-realX), static_cast<float>(-realY));

            h->m_lastMouseX = static_cast<float>(x);
            h->m_lastMouseY = static_cast<float>(y);
        });

        // Mouse button callback
        glfwSetMouseButtonCallback(handle, [](GLFWwindow* w, int button, int action, int mods)
        {
            auto handler = static_cast<WindowEventHandler*>(glfwGetWindowUserPointer(w));

            switch (action)
            {
                case GLFW_PRESS:
                    handler->mouseButtonPressed(getJopMouseButton(button), mods);
                    break;
                case GLFW_RELEASE:
                    handler->mouseButtonReleased(getJopMouseButton(button), mods);
            }
        });

        // Mouse left/entered callback
        glfwSetCursorEnterCallback(handle, [](GLFWwindow* w, int entered)
        {
            auto handler = static_cast<WindowEventHandler*>(glfwGetWindowUserPointer(w));

            if (entered)
                handler->mouseEntered();
            else
                handler->mouseLeft();
        });

        // Scroll callback
        glfwSetScrollCallback(handle, [](GLFWwindow* w, double x, double y)
        {
            static_cast<WindowEventHandler*>(glfwGetWindowUserPointer(w))->mouseScrolled(static_cast<const float>(x), static_cast<const float>(y));
        });

        // Frame buffer size change
        glfwSetFramebufferSizeCallback(handle, [](GLFWwindow* w, int x, int y)
        {
            static_cast<WindowEventHandler*>(glfwGetWindowUserPointer(w))->resized(x, y);
        });

    #endif
    }

    WindowEventHandler::~WindowEventHandler()
    {
    #ifdef JOP_OS_DESKTOP

        auto handle = m_windowRef.getLibraryHandle();

        glfwSetWindowCloseCallback(handle, NULL);
        glfwSetWindowIconifyCallback(handle, NULL);
        glfwSetKeyCallback(handle, NULL);
        glfwSetCharCallback(handle, NULL);
        glfwSetCursorPosCallback(handle, NULL);
        glfwSetMouseButtonCallback(handle, NULL);
        glfwSetCursorEnterCallback(handle, NULL);
        glfwSetScrollCallback(handle, NULL);
        glfwSetFramebufferSizeCallback(handle, NULL);

    #endif
    }

    //////////////////////////////////////////////

    void WindowEventHandler::closed()
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::resized(const unsigned int, const unsigned int)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::lostFocus()
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::gainedFocus()
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::keyPressed(const int, const int, const int)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::keyReleased(const int, const int, const int)
    {}

    //////////////////////////////////////////////

    bool WindowEventHandler::keyDown(const int key) const
    {
        return glfwGetKey(m_windowRef.getLibraryHandle(), getGlfwKey(key)) == GLFW_PRESS;
    }

    //////////////////////////////////////////////

    void WindowEventHandler::textEntered(const unsigned int)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::mouseMoved(const float, const float)
    {}

    //////////////////////////////////////////////

    glm::vec2 WindowEventHandler::getCursorPosition() const
    {
        double x = 0.0, y = 0.0;
        glfwGetCursorPos(m_windowRef.getLibraryHandle(), &x, &y);

        return glm::vec2(x, y);
    }

    //////////////////////////////////////////////

    void WindowEventHandler::mouseButtonPressed(const int, const int)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::mouseButtonReleased(const int, const int)
    {}

    //////////////////////////////////////////////

    bool WindowEventHandler::mouseButtonDown(const int button) const
    {
        return glfwGetMouseButton(m_windowRef.getLibraryHandle(), getGlfwButton(button)) == GLFW_PRESS;
    }

    //////////////////////////////////////////////

    void WindowEventHandler::mouseLeft()
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::mouseEntered()
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::mouseScrolled(const float, const float)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::controllerConnected(const int, const std::string&)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::controllerDisconnected(const int)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::controllerAxisShifted(const int, const int, const float)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::controllerButtonPressed(const int, const int)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::controllerButtonReleased(const int, const int)
    {}

    //////////////////////////////////////////////

    void WindowEventHandler::handleControllerInput()
    {
    #ifdef JOP_OS_DESKTOP

        static const int maxControllers = static_cast<int>(std::min(unsigned int(GLFW_JOYSTICK_LAST), SettingManager::get<unsigned int>("engine@Input|Controller|uMaxControllers", 1)));
        static const float deadzone = SettingManager::get<float>("engine@Input|Controller|fDeadzone", 0.1f);
        static unsigned int counter = 99;

        // Query the presence of controllers and update states.
        // We won't need to check the states every single frame,
        // so we can do a little optimization using a counter.
        for (int i = 0; i < maxControllers && (++counter % 100) == 0; ++i)
        {
            bool present = glfwJoystickPresent(i) == gl::TRUE_;

            if (present != ns_controllersPresent[i])
            {
                ns_controllersPresent[i] = present;
                present ? controllerConnected(i, std::string(glfwGetJoystickName(i))) : controllerDisconnected(i);
            }
        }

        // Query controller axes
        for (int i = 0; i < maxControllers; ++i)
        {
            if (ns_controllersPresent[i])
            {
                int n;
                const float* axes = glfwGetJoystickAxes(i, &n);

                for (int j = 0; j < n; ++j)
                {
                    if (axes[j] < -deadzone || axes[j] > deadzone)
                        controllerAxisShifted(i, j, axes[j]);
                }
            }
        }

        // Query controller buttons
        for (int i = 0; i < maxControllers; ++i)
        {
            if (ns_controllersPresent[i])
            {
                int n;
                const unsigned char* buttons = glfwGetJoystickButtons(i, &n);

                if (n > static_cast<int>(ns_controllerButtonStates[i].size()))
                    ns_controllerButtonStates[i].resize(n, 0);

                for (int j = 0; j < n; ++j)
                {
                    if (buttons[j] != ns_controllerButtonStates[i][j])
                    {
                        ns_controllerButtonStates[i][j] = buttons[j];
                        buttons[j] ? controllerButtonPressed(i, j) : controllerButtonReleased(i, j);
                    }
                }
            }
        }

    #endif
    }
}