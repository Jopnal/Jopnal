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
    static const nana::appearance ns_propertyAppearance
    (
        /* Decoration  */ true,
        /* Task bar    */ true,
        /* Floating    */ false,
        /* No activate */ false,
        /* Minimize    */ true,
        /* Maximize    */ false,
        /* Resizeable  */ true
    );

    void clearLayout(nana::place& layout, std::vector<std::unique_ptr<nana::group>>& comps)
    {
        for (auto& i : comps)
            layout.erase(*i);

        comps.clear();
    }
}

namespace comp
{
    struct Scene : nana::group
    {
        Scene(nana::window parent, ::jop::Scene& scene)
            : nana::group(parent, "Scene"),
              m_scene(scene)
        {
            
        }

    private:

        ::jop::Scene& m_scene;
    };

    //////////////////////////////////////////////

    struct Object : nana::group
    {
        Object(nana::window parent, ::jop::Object& obj)
            : nana::group(parent, "Object"),
              m_obj(obj)
        {


        }

    private:

        jop::WeakReference<::jop::Object> m_obj;
    };

    //////////////////////////////////////////////

    struct Transform : nana::group
    {
        Transform(nana::window parent, ::jop::Object& obj)
            : nana::group(parent, "Transform"),
              m_obj(obj)
        {
            
        }

    private:

        jop::WeakReference<::jop::Object> m_obj;
    };
}

namespace jope
{
    PropertyWindow::PropertyWindow(nana::window parent, const nana::color color)
        : nana::nested_form(parent, ns_propertyAppearance),
          m_layout(*this),
          m_components()
    {
        {
            HWND hwnd = reinterpret_cast<HWND>(this->native_handle());

            LONG style = GetWindowLongPtr(hwnd, GWL_STYLE);
            SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_CHILD | WS_CLIPSIBLINGS);

            // Disable the close button
            EnableMenuItem(GetSystemMenu(hwnd, FALSE), SC_CLOSE, MF_GRAYED);
        }

        this->caption("Properties");
        this->z_order(nullptr, nana::z_order_action::top);
        this->bgcolor(color);

        // margin=[top,right,bottom,left]

        m_layout.div("<vertical comps margin=[5,5,0,5]>");
    }

    //////////////////////////////////////////////

    void PropertyWindow::updateComponents(jop::Object& obj)
    {
        clearLayout(m_layout, m_components);

        m_components.emplace_back(std::make_unique<comp::Transform>(*this, obj));
        m_layout.field("comps") << *m_components.back();


    }

    //////////////////////////////////////////////

    void PropertyWindow::updateComponents(jop::Scene& scene)
    {
        clearLayout(m_layout, m_components);

        m_components.emplace_back(std::make_unique<comp::Scene>(*this, scene));
        m_layout.field("comps") << *m_components.back();
    }
}