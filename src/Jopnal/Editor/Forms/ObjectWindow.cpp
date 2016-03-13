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

    void createObjTree(const jop::Object& parent, nana::treebox::item_proxy itemRoot)
    {
        for (auto& i : parent.getChildren())
            createObjTree(i, itemRoot.append(i.getID(), i.getID()).value(i.getReference()));
    }
}

namespace jope
{
    ObjectWindow::ObjectWindow(nana::window parent, PropertyWindow& propWindow)
        : nana::nested_form(parent, ns_objWindowAppearance),
          m_layout(*this),
          m_newObjButton(*this, true),
          m_delObjButton(*this, true),
          m_objTree(*this, true),
          m_sceneItem(m_objTree.insert("Scene", "New Scene")),
          m_propWindowRef(propWindow)
    {
        m_instance = this;

        HWND hwnd = reinterpret_cast<HWND>(this->native_handle());

        LONG style = GetWindowLongPtr(hwnd, GWL_STYLE);
        SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_CHILD | WS_CLIPSIBLINGS);

        this->z_order(nullptr, nana::z_order_action::top);
        this->bgcolor(nana::colors::white);
        this->caption("Object Browser");

        // Object tree
        m_objTree.bgcolor(this->bgcolor());
        m_objTree.auto_draw(true);
        m_objTree.events().selected([this](nana::arg_treebox arg)
        {
            if (!jop::Engine::hasCurrentScene())
                return;

            if (arg.item == m_sceneItem)
                m_propWindowRef.updateComponents(jop::Engine::getCurrentScene(), m_sceneItem);
            else
                m_propWindowRef.updateComponents(*arg.item.value<jop::WeakReference<jop::Object>>(), arg.item);
        });
        
        // New object button
        m_newObjButton.caption("New");
        m_newObjButton.events().click([this](nana::arg_click arg)
        {
            if (!arg.mouse_args->is_left_button() || !jop::Engine::hasCurrentScene())
                return;

            typedef jop::WeakReference<jop::Object> ObjRef;

            auto countSubnodes = [](nana::treebox::item_proxy item) -> std::size_t
            {
                std::size_t num = 0;
                for (auto& i : item)
                {
                    i;
                    ++num;
                }

                return num;
            };

            if (m_objTree.selected().empty())
                m_sceneItem.select(true);

            if (m_objTree.selected() == m_sceneItem)
            {
                const std::string name("Object " + std::to_string(countSubnodes(m_sceneItem)));

                auto newItem = m_sceneItem.append(std::to_string(countSubnodes(m_sceneItem)), name, ObjRef());
                m_sceneItem.expand(true);

                CommandBuffer::pushCommand(new CreateObjectCommand(ObjRef(), newItem.value<ObjRef>(), name));
            }
            else
            {
                const std::string name("Child " + std::to_string(countSubnodes(m_objTree.selected())));

                auto parent = m_objTree.selected().value<ObjRef>();
                auto newItem = m_objTree.selected().append(std::to_string(countSubnodes(m_objTree.selected())), name, ObjRef());
                m_objTree.selected().expand(true);

                CommandBuffer::pushCommand(new CreateObjectCommand(parent, newItem.value<ObjRef>(), name));
            }
        });

        // Delete button
        m_delObjButton.caption("Delete");

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
                createObjTree(i, m_sceneItem.append(i.getID(), i.getID()).value(i.getReference()));
        }
    }

    //////////////////////////////////////////////

    void ObjectWindow::refresh()
    {
        nana::API::refresh_window(m_instance->m_objTree);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////

    ObjectWindow* ObjectWindow::m_instance = nullptr;
}