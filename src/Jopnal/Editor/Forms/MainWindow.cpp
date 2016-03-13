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
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/place.hpp>

//////////////////////////////////////////////


namespace
{
    static const nana::appearance ns_mainAppearance
    (
        /* Decoration  */ true,  
        /* Task bar    */ true,
        /* Floating    */ false,
        /* No activate */ false,
        /* Minimize    */ true,
        /* Maximize    */ true,
        /* Resizeable  */ true
    );

    static const nana::color ns_commonColor(nana::color_rgb(0x222222));
}

namespace jope
{
    MainWindow::MainWindow()
        : nana::form(nana::API::make_center(1280, 720), ns_mainAppearance),
          m_layout(*this),
          m_propertyWindow(*this, ns_commonColor),
          m_objWindow(*this, m_propertyWindow),
          m_oglWindow(*this),
          m_toolWindow(*this, ns_commonColor)
    {
        this->caption("Jopnal Editor");
        this->bgcolor(ns_commonColor);
        
        this->events().unload([]
        {
            jop::Engine::exit();
        });
        this->events().resized([this](nana::arg_resized)
        {
            // Prevent corrupting the layout
            nana::API::refresh_window_tree(*this);
        });
        this->events().key_press([](nana::arg_keyboard arg)
        {
            if (arg.ctrl)
            {
                switch (arg.key)
                {
                    case L'Z':
                        CommandBuffer::undoLast();
                        break;
                    case L'R':
                        CommandBuffer::redoCurrent();
                        break;
                }
            }
        });
        
        // margin=[top,right,bottom,left]

        m_layout.div("<vertical <tool weight=14%><<obj weight=10% margin=[0,0,40,2]><ogl margin=[0,18,40,18]><prop weight=18% margin=[0,18,40]>>>");
        m_layout.field("tool") << m_toolWindow;
        m_layout.field("obj") << m_objWindow;
        m_layout.field("ogl") << m_oglWindow;
        m_layout.field("prop") << m_propertyWindow;
        
        // Keep last
        this->zoom(true);
        this->show();

        // Hot keys need to be registered after show()
        // Undo
        this->register_shortkey(L'Z');
        // Redo
        this->register_shortkey(L'R');

        m_toolWindow.show();
        m_oglWindow.show();
        m_objWindow.show();
        m_propertyWindow.show();

        // Prevents the tool window turning gray upon start
        nana::API::refresh_window(m_toolWindow);
    }
}