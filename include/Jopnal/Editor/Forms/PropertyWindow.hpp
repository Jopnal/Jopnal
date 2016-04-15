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

#ifndef JOPE_PROPERTYWINDOW_HPP
#define JOPE_PROPERTYWINDOW_HPP

// Headers
#include <nana/gui/wvl.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/gui/widgets/scroll.hpp>
#include <memory>

//////////////////////////////////////////////


namespace jop
{
    class Object;
    class Scene;
}

namespace jope
{
    class PropertyWindow final : public nana::nested_form
    {
    public:

        typedef std::vector<std::unique_ptr<nana::group>> CompVec;

    public:

        PropertyWindow(nana::window parent, const nana::color color);

        void updateComponents(jop::Object& obj, nana::treebox::item_proxy item);

        void updateComponents(jop::Scene& scene, nana::treebox::item_proxy item);

        static void refresh();

    private:

        static PropertyWindow* m_instance;

        void pushComponents(jop::Object& obj);

        nana::place m_layout;
        CompVec m_components;
        jop::WeakReference<jop::Object> m_lastObject;
        jop::Scene* m_lastScene;
        nana::treebox::item_proxy m_lastItem;
        nana::scroll<true> m_scrollBar;
    };
}

#endif