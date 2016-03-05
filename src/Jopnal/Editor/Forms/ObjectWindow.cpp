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
    static const nana::appearance ns_objWindowAppearance
    (
        /* Decoration  */ true,
        /* Task bar    */ true,
        /* Floating    */ false,
        /* No activate */ false,
        /* Minimize    */ false,
        /* Maximize    */ false,
        /* Resizeable  */ true
    );
}

namespace jope
{
    ObjectWindow::ObjectWindow(nana::form& parent)
        : nana::nested_form(parent, nana::rectangle(10, 200, 200, 600), ns_objWindowAppearance),
          m_newObjButton(*this, true),
          m_objTree(*this, true),
          m_layout(*this)
    {
        this->bgcolor(parent.bgcolor());

        EnableMenuItem(GetSystemMenu(reinterpret_cast<HWND>(this->native_handle()), FALSE), SC_CLOSE, MF_GRAYED);

        m_newObjButton.caption("New");
        

        m_layout.div("<vertical <button margin=5 weight=10%><tree>>");
        m_layout.field("button") << m_newObjButton;
        m_layout.field("tree") << m_objTree;
        m_layout.collocate();


        // Keep last
        this->show();
        nana::API::refresh_window(*this);
    }
}