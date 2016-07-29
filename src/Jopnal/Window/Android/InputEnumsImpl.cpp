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
#include JOP_PRECOMPILED_HEADER_FILE
#ifndef JOP_PRECOMPILED_HEADER
#include <Jopnal/Window/InputEnumsImpl.hpp>
#endif

#ifdef JOP_OS_ANDROID
#include <android/input.h>

//////////////////////////////////////////////
namespace jop
{
    namespace Input
    {
        int getJopKey(const int key)
        {
            using k = jop::Keyboard::Key;

            switch (key)
            {
            case AKEYCODE_UNKNOWN:            return k::Unknown;
            case AKEYCODE_SOFT_LEFT:          return k::Unknown;
            case AKEYCODE_SOFT_RIGHT:         return k::Unknown;
            case AKEYCODE_HOME:               return k::Home;
            case AKEYCODE_BACK:               return k::Escape;
            case AKEYCODE_CALL:               return k::Unknown;
            case AKEYCODE_ENDCALL:            return k::Unknown;
            case AKEYCODE_0:                  return k::KeypadZero;
            case AKEYCODE_1:                  return k::KeypadOne;
            case AKEYCODE_2:                  return k::KeypadTwo;
            case AKEYCODE_3:                  return k::KeypadThree;
            case AKEYCODE_4:                  return k::KeypadFour;
            case AKEYCODE_5:                  return k::KeypadFive;
            case AKEYCODE_6:                  return k::KeypadSix;
            case AKEYCODE_7:                  return k::KeypadSeven;
            case AKEYCODE_8:                  return k::KeypadEight;
            case AKEYCODE_9:                  return k::KeypadNine;
            case AKEYCODE_STAR:               return k::Unknown;
            case AKEYCODE_POUND:              return k::Unknown;
            case AKEYCODE_DPAD_UP:            return k::Unknown;
            case AKEYCODE_DPAD_DOWN:          return k::Unknown;
            case AKEYCODE_DPAD_LEFT:          return k::Unknown;
            case AKEYCODE_DPAD_RIGHT:         return k::Unknown;
            case AKEYCODE_DPAD_CENTER:        return k::Unknown;
            case AKEYCODE_VOLUME_UP:          return k::Unknown;
            case AKEYCODE_VOLUME_DOWN:        return k::Unknown;
            case AKEYCODE_POWER:              return k::Unknown;
            case AKEYCODE_CAMERA:             return k::Unknown;
            case AKEYCODE_CLEAR:              return k::Unknown;
            case AKEYCODE_A:                  return k::A;
            case AKEYCODE_B:                  return k::B;
            case AKEYCODE_C:                  return k::C;
            case AKEYCODE_D:                  return k::D;
            case AKEYCODE_E:                  return k::E;
            case AKEYCODE_F:                  return k::F;
            case AKEYCODE_G:                  return k::G;
            case AKEYCODE_H:                  return k::H;
            case AKEYCODE_I:                  return k::I;
            case AKEYCODE_J:                  return k::J;
            case AKEYCODE_K:                  return k::K;
            case AKEYCODE_L:                  return k::L;
            case AKEYCODE_M:                  return k::M;
            case AKEYCODE_N:                  return k::N;
            case AKEYCODE_O:                  return k::O;
            case AKEYCODE_P:                  return k::P;
            case AKEYCODE_Q:                  return k::Q;
            case AKEYCODE_R:                  return k::R;
            case AKEYCODE_S:                  return k::S;
            case AKEYCODE_T:                  return k::T;
            case AKEYCODE_U:                  return k::U;
            case AKEYCODE_V:                  return k::V;
            case AKEYCODE_W:                  return k::W;
            case AKEYCODE_X:                  return k::X;
            case AKEYCODE_Y:                  return k::Y;
            case AKEYCODE_Z:                  return k::Z;
            case AKEYCODE_COMMA:              return k::Comma;
            case AKEYCODE_PERIOD:             return k::Period;
            case AKEYCODE_ALT_LEFT:           return k::LAlt;
            case AKEYCODE_ALT_RIGHT:          return k::RAlt;
            case AKEYCODE_SHIFT_LEFT:         return k::LShift;
            case AKEYCODE_SHIFT_RIGHT:        return k::RShift;
            case AKEYCODE_TAB:                return k::Tab;
            case AKEYCODE_SPACE:              return k::Space;
            case AKEYCODE_SYM:                return k::Unknown;
            case AKEYCODE_EXPLORER:           return k::Unknown;
            case AKEYCODE_ENVELOPE:           return k::Unknown;
            case AKEYCODE_ENTER:              return k::Enter;
            case AKEYCODE_DEL:                return k::Delete;
            case AKEYCODE_GRAVE:              return k::GraveAccent;
            case AKEYCODE_MINUS:              return k::Minus;
            case AKEYCODE_EQUALS:             return k::Equal;
            case AKEYCODE_LEFT_BRACKET:       return k::LBracket;
            case AKEYCODE_RIGHT_BRACKET:      return k::RBracket;
            case AKEYCODE_BACKSLASH:          return k::Backslash;
            case AKEYCODE_SEMICOLON:          return k::Semicolon;
            case AKEYCODE_APOSTROPHE:         return k::Apostrophe;
            case AKEYCODE_SLASH:              return k::Slash;
            case AKEYCODE_AT:                 return k::Unknown;
            case AKEYCODE_NUM:                return k::Unknown;
            case AKEYCODE_HEADSETHOOK:        return k::Unknown;
            case AKEYCODE_FOCUS:              return k::Unknown;
            case AKEYCODE_PLUS:               return k::Unknown;
            case AKEYCODE_MENU:               return k::Unknown;
            case AKEYCODE_NOTIFICATION:       return k::Unknown;
            case AKEYCODE_SEARCH:             return k::Unknown;
            case AKEYCODE_MEDIA_PLAY_PAUSE:   return k::Unknown;
            case AKEYCODE_MEDIA_STOP:         return k::Unknown;
            case AKEYCODE_MEDIA_NEXT:         return k::Unknown;
            case AKEYCODE_MEDIA_PREVIOUS:     return k::Unknown;
            case AKEYCODE_MEDIA_REWIND:       return k::Unknown;
            case AKEYCODE_MEDIA_FAST_FORWARD: return k::Unknown;
            case AKEYCODE_MUTE:               return k::Unknown;
            case AKEYCODE_PAGE_UP:            return k::PgUp;
            case AKEYCODE_PAGE_DOWN:          return k::PgDown;
            case AKEYCODE_PICTSYMBOLS:        return k::Unknown;
            case AKEYCODE_SWITCH_CHARSET:     return k::Unknown;
            case AKEYCODE_BUTTON_A:           return k::Unknown;
            case AKEYCODE_BUTTON_B:           return k::Unknown;
            case AKEYCODE_BUTTON_C:           return k::Unknown;
            case AKEYCODE_BUTTON_X:           return k::Unknown;
            case AKEYCODE_BUTTON_Y:           return k::Unknown;
            case AKEYCODE_BUTTON_Z:           return k::Unknown;
            case AKEYCODE_BUTTON_L1:          return k::Unknown;
            case AKEYCODE_BUTTON_R1:          return k::Unknown;
            case AKEYCODE_BUTTON_L2:          return k::Unknown;
            case AKEYCODE_BUTTON_R2:          return k::Unknown;
            case AKEYCODE_BUTTON_THUMBL:      return k::Unknown;
            case AKEYCODE_BUTTON_THUMBR:      return k::Unknown;
            case AKEYCODE_BUTTON_START:       return k::Unknown;
            case AKEYCODE_BUTTON_SELECT:      return k::Unknown;
            case AKEYCODE_BUTTON_MODE:        return k::Unknown;

                return k::Unknown;
            }
        }

        int getGlKey(const int key)
        {
            using k = jop::Keyboard::Key;

            switch (key)
            {

            case k::Unknown:                   return AKEYCODE_UNKNOWN;
            case k::Home:                      return AKEYCODE_HOME;
            case k::Escape:                    return AKEYCODE_BACK;
            case k::KeypadZero:                return AKEYCODE_0;
            case k::KeypadOne:                 return AKEYCODE_1;
            case k::KeypadTwo:                 return AKEYCODE_2;
            case k::KeypadThree:               return AKEYCODE_3;
            case k::KeypadFour:                return AKEYCODE_4;
            case k::KeypadFive:                return AKEYCODE_5;
            case k::KeypadSix:                 return AKEYCODE_6;
            case k::KeypadSeven:               return AKEYCODE_7;
            case k::KeypadEight:               return AKEYCODE_8;
            case k::KeypadNine:                return AKEYCODE_9;
            case k::A:                         return AKEYCODE_A;
            case k::B:                         return AKEYCODE_B;
            case k::C:                         return AKEYCODE_C;
            case k::D:                         return AKEYCODE_D;
            case k::E:                         return AKEYCODE_E;
            case k::F:                         return AKEYCODE_F;
            case k::G:                         return AKEYCODE_G;
            case k::H:                         return AKEYCODE_H;
            case k::I:                         return AKEYCODE_I;
            case k::J:                         return AKEYCODE_J;
            case k::K:                         return AKEYCODE_K;
            case k::L:                         return AKEYCODE_L;
            case k::M:                         return AKEYCODE_M;
            case k::N:                         return AKEYCODE_N;
            case k::O:                         return AKEYCODE_O;
            case k::P:                         return AKEYCODE_P;
            case k::Q:                         return AKEYCODE_Q;
            case k::R:                         return AKEYCODE_R;
            case k::S:                         return AKEYCODE_S;
            case k::T:                         return AKEYCODE_T;
            case k::U:                         return AKEYCODE_U;
            case k::V:                         return AKEYCODE_V;
            case k::W:                         return AKEYCODE_W;
            case k::X:                         return AKEYCODE_X;
            case k::Y:                         return AKEYCODE_Y;
            case k::Z:                         return AKEYCODE_Z;
            case k::Comma:                     return AKEYCODE_COMMA;
            case k::Period:                    return AKEYCODE_PERIOD;
            case k::LAlt:                      return AKEYCODE_ALT_LEFT;
            case k::RAlt:                      return AKEYCODE_ALT_RIGHT;
            case k::LShift:                    return AKEYCODE_SHIFT_LEFT;
            case k::RShift:                    return AKEYCODE_SHIFT_RIGHT;
            case k::Tab:                       return AKEYCODE_TAB;
            case k::Space:                     return AKEYCODE_SPACE;
            case k::Enter:                     return AKEYCODE_ENTER;
            case k::Delete:                    return AKEYCODE_DEL;
            case k::GraveAccent:               return AKEYCODE_GRAVE;
            case k::Minus:                     return AKEYCODE_MINUS;
            case k::Equal:                     return AKEYCODE_EQUALS;
            case k::LBracket:                  return AKEYCODE_LEFT_BRACKET;
            case k::RBracket:                  return AKEYCODE_RIGHT_BRACKET;
            case k::Backslash:                 return AKEYCODE_BACKSLASH;
            case k::Semicolon:                 return AKEYCODE_SEMICOLON;
            case k::Apostrophe:                return AKEYCODE_APOSTROPHE;
            case k::Slash:                     return AKEYCODE_SLASH;
            case k::PgUp:                      return AKEYCODE_PAGE_UP;
            case k::PgDown:                    return AKEYCODE_PAGE_DOWN;

                return  AKEYCODE_UNKNOWN;

            }
        }

        int getJopControllerButton(const int button)
        {
            using m = jop::Controller::XBox::Button;

            switch (button)
            {
            case AKEYCODE_BUTTON_A:             return m::A;
            case AKEYCODE_BUTTON_B:             return m::B;
            case AKEYCODE_X:                    return m::X;
            case AKEYCODE_BUTTON_X:             return m::X;
            case AKEYCODE_Y:                    return m::Y;
            case AKEYCODE_BUTTON_Y:             return m::Y;
            case AKEYCODE_BUTTON_L1:            return m::LShoulder;
            case AKEYCODE_BUTTON_R1:            return m::RShoulder;
            case AKEYCODE_BACK:                 return m::Back;
            case AKEYCODE_BUTTON_START:         return m::Start;
            case AKEYCODE_BUTTON_THUMBL:        return m::LStick;
            case AKEYCODE_BUTTON_THUMBR:        return m::RStick;
            case -AMOTION_EVENT_AXIS_HAT_Y :    return m::PadUp;
            case AMOTION_EVENT_AXIS_HAT_X:      return m::PadRight;
            case AMOTION_EVENT_AXIS_HAT_Y :     return m::PadDown;
            case -AMOTION_EVENT_AXIS_HAT_X:     return m::PadLeft;
            }

            return button;
        }
        int getJopControllerAxis(const int button)
        {
            using a = jop::Controller::XBox::Axis;

            switch (button)
            {
            case AMOTION_EVENT_AXIS_X:          return a::LeftStickX;
            case AMOTION_EVENT_AXIS_Y:          return a::LeftStickY;
            case AMOTION_EVENT_AXIS_LTRIGGER:   return a::LTrigger;
            case AMOTION_EVENT_AXIS_RTRIGGER:   return a::RTrigger;
            case AMOTION_EVENT_AXIS_RZ:         return a::RightStickY;
            case AMOTION_EVENT_AXIS_Z:          return a::RightStickX;
            }

            return button;
        }

        int getGlControllerButton(const int button)
        {
            using m = jop::Controller::XBox::Button;

            switch (button)
            {
            case m::A:                          return AKEYCODE_BUTTON_A;
            case m::B:                          return AKEYCODE_BUTTON_B;
            case m::X:                          return AKEYCODE_BUTTON_X;
            case m::Y:                          return AKEYCODE_BUTTON_Y;
            case m::LShoulder:                  return AKEYCODE_BUTTON_L1;
            case m::RShoulder:                  return AKEYCODE_BUTTON_R1;
            case m::Back:                       return AKEYCODE_BACK;
            case m::Start:                      return AKEYCODE_BUTTON_START;
            case m::LStick:                     return AKEYCODE_BUTTON_THUMBL;
            case m::RStick:                     return AKEYCODE_BUTTON_THUMBR;
            case m::PadUp:                      return -AMOTION_EVENT_AXIS_HAT_X;
            case m::PadRight:                   return AMOTION_EVENT_AXIS_HAT_Y;
            case m::PadDown:                    return AMOTION_EVENT_AXIS_HAT_X;
            case m::PadLeft:                    return -AMOTION_EVENT_AXIS_HAT_Y;
            }

            return button;
        }
        int getGlControllerAxis(const int button)
        {
            using a = jop::Controller::XBox::Axis;

            switch (button)
            {
            case a::LeftStickX: return AMOTION_EVENT_AXIS_X;
            case a::LeftStickY: return AMOTION_EVENT_AXIS_Y;
            case a::LTrigger: return AMOTION_EVENT_AXIS_LTRIGGER;
            case a::RTrigger: return AMOTION_EVENT_AXIS_RTRIGGER;
            case a::RightStickY: return AMOTION_EVENT_AXIS_RZ;
            case a::RightStickX: return AMOTION_EVENT_AXIS_Z;
            }

            return button;
        }

        int getJopMouseButton(const int button)
        {
            using m = jop::Mouse::Button;

            return m::Unknown;
        }

        int getGlMouseButton(const int button)
        {
            using m = jop::Mouse::Button;

            return  AKEYCODE_UNKNOWN;
        }

        int getJopTouchInfo(const int info)
        {
            using i = jop::Touch::Info;

            switch (info)
            {
            case AMOTION_EVENT_AXIS_PRESSURE:       return i::PRESSURE;
            case AMOTION_EVENT_AXIS_SIZE:           return i::SIZE;
            case AMOTION_EVENT_AXIS_TOUCH_MAJOR:    return i::TOUCH_MAJOR;
            case AMOTION_EVENT_AXIS_TOOL_MAJOR:     return i::TOOL_MAJOR;
            }

            return info;
        }

        int getGlTouchInfo(const int info)
        {
            using i = jop::Touch::Info;

            switch (info)
            {
            case i::PRESSURE:           return AMOTION_EVENT_AXIS_PRESSURE;
            case i::SIZE:               return AMOTION_EVENT_AXIS_SIZE;
            case i::TOUCH_MAJOR:        return AMOTION_EVENT_AXIS_TOUCH_MAJOR;
            case i::TOOL_MAJOR:         return AMOTION_EVENT_AXIS_TOOL_MAJOR;
            }

            return info;
        }
    }
}
#endif