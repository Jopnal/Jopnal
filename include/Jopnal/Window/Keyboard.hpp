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

#ifndef JOP_KEYBOARD_HPP
#define JOP_KEYBOARD_HPP

// Headers
#include <Jopnal/Header.hpp>

//////////////////////////////////////////////


namespace jop
{
    class Keyboard
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
            RAlth,
            RSuper,
            Menu
        };

        struct Modifier
        {
            /// Keyboard modifiers
            ///
            enum
            {
                Shift = 1,
                Control = 1 << 1,
                Alt = 1 << 2,
                Super = 1 << 3
            };
        };
    };
}

#endif

/// \class Keyboard
/// \ingroup window
///
/// #TODO Detailed description