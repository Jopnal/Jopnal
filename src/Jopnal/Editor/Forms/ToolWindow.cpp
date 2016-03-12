// Jopnal Editor C++ Application
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
#include <Jopnal/Editor/Precompiled/Precompiled.hpp>

//////////////////////////////////////////////


namespace
{
    static const nana::appearance ns_toolAppearance
    (
        /* Decoration  */ false,
        /* Task bar    */ true,
        /* Floating    */ false,
        /* No activate */ false,
        /* Minimize    */ false,
        /* Maximize    */ false,
        /* Resizeable  */ false
    );
}

namespace jope
{
    ToolWindow::ToolWindow(nana::window parent, const nana::color color)
        : nana::nested_form(parent, ns_toolAppearance),
          m_layout(*this),
          m_toolBoxes()
    {
        {
            HWND hwnd = reinterpret_cast<HWND>(this->native_handle());

            LONG style = GetWindowLongPtr(hwnd, GWL_STYLE);
            SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_CHILD | WS_CLIPSIBLINGS);

            // Disable the close button
            EnableMenuItem(GetSystemMenu(hwnd, FALSE), SC_CLOSE, MF_GRAYED);
        }

        this->caption("Tools");
        this->z_order(nullptr, nana::z_order_action::top);
        this->bgcolor(color);
    }
}