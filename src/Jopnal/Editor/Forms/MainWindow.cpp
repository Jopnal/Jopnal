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
        : nana::form(nana::API::make_center(1280, 720), ns_mainAppearance)
    {
        this->caption("Jopnal Editor");
        this->bgcolor(nana::color(nana::color_rgb(0x222222)));
        
        this->events().unload([this](const nana::arg_unload& u)
        {
            bool close = false;

            nana::nested_form dialog(*this, nana::API::make_center(*this, 200, 40), nana::appearance(true, false, true, false, false, false, false));
            dialog.caption("Really exit?");
            nana::place pl(dialog);
            pl.div("<ab>");

            nana::button yes(dialog, "Yes");
            yes.events().click([&dialog, &close]
            {
                close = true;
                dialog.close();
            });

            nana::button no(dialog, "No");
            no.events().click([&dialog]
            {
                dialog.close();
            });

            pl.field("ab") << yes << no;

            pl.collocate();
            dialog.show();
            
            nana::API::wait_for(dialog);
            
            if (close)
                jop::Engine::exit();
            else
                u.cancel = true;
        });

        // Keep last
        this->zoom(true);
        this->show();
    }
}