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
#include <nana/gui/widgets/listbox.hpp>

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

    void createObjTree(const jop::Object& parent, nana::treebox::item_proxy itemRoot)
    {
        for (auto& i : parent.getChildren())
            createObjTree(i, itemRoot.append(i.getID(), i.getID()).value(&i));
    }
}

namespace jope
{
    ObjectWindow::ObjectWindow(nana::window parent)
        : nana::nested_form(parent, nana::rectangle(10, 200, 200, 600), ns_objWindowAppearance),
          m_newObjButton(*this, true),
          m_delObjButton(*this, true),
          m_objTree(*this, true),
          m_layout(*this),
          m_rootObjects(0)
    {
        HWND hwnd = reinterpret_cast<HWND>(this->native_handle());

        LONG style = GetWindowLongPtr(hwnd, GWL_STYLE);
        SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_CHILD | WS_CLIPSIBLINGS);

        this->z_order(nullptr, nana::z_order_action::top);
        this->bgcolor(nana::colors::white);
        
        m_newObjButton.caption("New");
        m_newObjButton.events().click([this](nana::arg_click a)
        {
            if (!a.mouse_args->is_left_button() || !jop::Engine::hasCurrentScene())
                return;

            typedef jop::WeakReference<jop::Object> ObjRef;
            const char* const emptyStr = "Empty Object";

            if (m_objTree.selected().empty())
            {
                auto& newItem = m_objTree.insert(std::to_string(m_rootObjects++), emptyStr).value(ObjRef());
                CommandBuffer::pushCommand(new CreateObjectCommand(ObjRef(), newItem.value<ObjRef>(), emptyStr));
            }
            else
            {
                auto parent = m_objTree.selected().value<ObjRef>();
                auto newItem = m_objTree.selected().append(std::to_string(m_objTree.selected().size() + 1), emptyStr, ObjRef());

                CommandBuffer::pushCommand(new CreateObjectCommand(parent, newItem.value<ObjRef>(), emptyStr));
            }
        });

        m_delObjButton.caption("Delete");
        
        // Fill the object tree
        buildObjectTree();

        // Set the layout
        m_layout.div("<vertical <button margin=10 weight=8% max=50 gap=5><tree>>");
        m_layout.field("button") << m_newObjButton << m_delObjButton;
        m_layout.field("tree") << m_objTree;
        m_layout.collocate();

        // Disable the close button
        EnableMenuItem(GetSystemMenu(reinterpret_cast<HWND>(this->native_handle()), FALSE), SC_CLOSE, MF_GRAYED);
    }

    //////////////////////////////////////////////

    void ObjectWindow::buildObjectTree()
    {
        
        if (jop::Engine::hasCurrentScene())
        {
            for (auto& i : jop::Engine::getCurrentScene().getChildren())
                createObjTree(i, m_objTree.insert(i.getID(), i.getID()).value(&i));
        }
    }
}