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
}

namespace jope
{
    MainWindow::MainWindow()
        : nana::form(nana::API::make_center(1280, 720), ns_mainAppearance),
          m_layout(*this),
          m_oglWindow(*this),
          m_objWindow(*this)
    {
        this->caption("Jopnal Editor");
        this->bgcolor(nana::color(nana::color_rgb(0x222222)));

        this->events().unload([]
        {
            jop::Engine::exit();
        });
        
        m_layout.div("<obj weight=10% margin=[0,0,48]><ogl margin=[0, 18, 48, 20]>");
        m_layout.field("obj") << m_objWindow;
        m_layout.field("ogl") << m_oglWindow;
        
        // Keep last
        this->zoom(true);
        this->show();

        m_oglWindow.show();
        m_objWindow.show();
    }
}