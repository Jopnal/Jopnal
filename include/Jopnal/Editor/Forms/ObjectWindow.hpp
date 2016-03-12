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

#ifndef JOPE_OBJECTWINDOW_HPP
#define JOPE_OBJECTWINDOW_HPP

// Headers
#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/place.hpp>

//////////////////////////////////////////////


namespace jope
{
    class ObjectWindow final : public nana::nested_form
    {
    public:

        ObjectWindow(nana::window parent);

        void buildObjectTree();

    private:

        nana::place m_layout;
        nana::button m_newObjButton;
        nana::button m_delObjButton;
        nana::treebox m_objTree;
        nana::treebox::item_proxy m_sceneItem;

    };
}

#endif