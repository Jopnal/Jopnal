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

#ifndef JOP_KEYBOARD_HPP
#define JOP_KEYBOARD_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <glm/vec2.hpp>
#include <string>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Keyboard
    {
    public:

        /// Keyboard keys
        ///
        enum Key
        {
            Unknown,
            Space,
            Apostrophe,
            Comma,
            Minus,
            Period,
            Slash,
            Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
            Semicolon,
            Equal,
            A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
            LBracket,
            Backslash,
            RBracket,
            GraveAccent,
            World1,
            World2,
            Escape,
            Enter,
            Tab,
            Backspace,
            Insert,
            Delete,
            Right,
            Left,
            Down,
            Up,
            PgUp,
            PgDown,
            Home,
            End,
            CapsLock,
            ScrollLock,
            NumLock,
            PrintScreen,
            Pause,
            F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
            KeypadZero,
            KeypadOne,
            KeypadTwo,
            KeypadThree,
            KeypadFour,
            KeypadFive,
            KeypadSix,
            KeypadSeven,
            KeypadEight,
            KeypadNine,
            KeypadDecimal,
            KeypadDivide,
            KeypadMultiply,
            KeypadSubtract,
            KeypadAdd,
            KeypadEnter,
            KeypadEqual,
            LShift,
            LControl,
            LAlt,
            LSuper,
            RShift,
            RControl,
            RAlt,
            RSuper,
            Menu
        };

        struct Modifier
        {
            /// Keyboard modifiers
            ///
            enum
            {
                Shift   = 1,
                Control = 1 << 1,
                Alt     = 1 << 2,
                Super   = 1 << 3
            };
        };

    public:

        /// \brief Get the key name in the current locale
        ///
        /// \param scanCode The scan code
        ///
        /// \return String with the name
        ///
        static std::string getKeyName(const int scanCode);

        /// \brief Get boolean is key down
        ///
        /// \param Keycode
        ///
        /// \return True if down
        ///
        static bool isKeyDown(const Key key);

        /// \brief Shows or hides virtual keyboard. Currently only for android
        ///
        /// \param Boolean true to show keyboard and false to hide it
        ///
        static void showVirtualKeyboard(const bool show);   
    };
}

#endif

/// \class Keyboard
/// \ingroup window
///
/// #TODO Detailed description