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

    void clearLayout(nana::place& layout, jope::PropertyWindow::CompVec& comps)
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
        Scene(nana::window parent, nana::treebox::item_proxy item, ::jop::Scene& scene)
            : nana::group(parent),

              // ID field
              m_idLabel(*create_child<nana::label>("id")),
              m_idBox(*create_child<nana::textbox>("id")),

              m_item(item),
              m_scene(scene)
        {
            this->enable_format_caption(true).caption("<white>Scene</>");

            this->div
            (
                "<vertical "

                // ID box
                "<id gap=5 margin=5 arrange=[5%,92%]>"
                
                ">"
            );

            // ID box
            m_idLabel.format(true).caption("<white>ID</>");
            
            m_idBox.multi_lines(false).editable(true).events().key_press([this](nana::arg_keyboard arg)
            {
                if (arg.key == nana::keyboard::enter && m_idBox.edited())
                {
                    std::string id;
                    m_idBox.getline(0, id);
                    m_idBox.edited_reset();
                    m_idBox.select(false);
                    this->focus();
                    m_item.text(id);

                    jope::ObjectWindow::refresh();

                    jope::CommandBuffer::pushCommand(new jope::ChangeObjectIDCommand(m_scene.getAsObject(), id));
                }
            });
            m_idBox.events().click([this](nana::arg_click arg)
            {
                m_idBox.select(true);
            });
            m_idBox.reset(scene.getID());
            
            this->collocate();
        }

    private:

        nana::label& m_idLabel;
        nana::textbox& m_idBox;

        nana::treebox::item_proxy m_item;
        ::jop::Scene& m_scene;
    };

    //////////////////////////////////////////////

    struct Object : nana::group
    {
        Object(nana::window parent, nana::treebox::item_proxy item, ::jop::Object& obj)
            : nana::group(parent),

            // ID field
            m_idLabel(*create_child<nana::label>("id")),
            m_idBox(*create_child<nana::textbox>("id")),

            // Active
            m_activeLabel(*create_child<nana::label>("active")),
            m_activeBox(*create_child<nana::checkbox>("active")),

            m_item(item),
            m_obj(obj)
        {
            this->enable_format_caption(true).caption("<white>Object</>");
            this->div
            (
                "<vertical gap=5 "
                          
                // ID box
                "<id margin=5 arrange=[40%,60%]>"

                // Active box
                "<active margin=5 arrange=[40%,60%]>"
                
                ">"
            );

            // ID box
            m_idLabel.format(true).caption("<white>ID</>");
            m_idBox.multi_lines(false).editable(true).events().key_press([this](nana::arg_keyboard arg)
            {
                if (arg.key == nana::keyboard::enter && m_idBox.edited())
                {
                    std::string id;
                    m_idBox.getline(0, id);
                    m_idBox.edited_reset();
                    m_idBox.select(false);
                    this->focus();
                    m_item.text(id);

                    jope::ObjectWindow::refresh();

                    jope::CommandBuffer::pushCommand(new jope::ChangeObjectIDCommand(*m_obj, id));
                }
            });
            m_idBox.events().click([this](nana::arg_click arg)
            {
                m_idBox.select(true);
            });
            m_idBox.reset(obj.getID());

            // Active box
            m_activeLabel.format(true).caption("<white>Active</>");
            m_activeBox.bgcolor(nana::API::bgcolor(parent));
            m_activeBox.fgcolor(nana::colors::white);
            m_activeBox.check(obj.isActive());
            m_activeBox.radio(false);
            m_activeBox.events().checked([this](nana::arg_checkbox arg)
            {
                jope::CommandBuffer::pushCommand(new jope::SetAciveCommand(*m_obj, arg.widget->checked()));
            });

            this->collocate();
        }

    private:

        // ID
        nana::label& m_idLabel;
        nana::textbox& m_idBox;

        // Active
        nana::label& m_activeLabel;
        nana::checkbox& m_activeBox;

        nana::treebox::item_proxy m_item;
        jop::WeakReference<::jop::Object> m_obj;
    };

    //////////////////////////////////////////////

    struct Transform : nana::group
    {
        Transform(nana::window parent, ::jop::Object& obj)
            : nana::group(parent),

              // Position
              m_posLabel(*create_child<nana::label>("pos")),
              m_posBoxX(*create_child<nana::spinbox>("pos")),
              m_posBoxY(*create_child<nana::spinbox>("pos")),
              m_posBoxZ(*create_child<nana::spinbox>("pos")),

              
              m_obj(obj)
        {
            this->enable_format_caption(true).caption("<white>Transform</>");
            this->div("<vertical margin=5 "
                    
                      // Position
                      "<vertical pos margin=5 gap=4 arrange=[25%,25%,25%,25%]>"

                      // Scale
                      //"<scl>"

                      // Rotation
                      //"<rot>"

                      ">");

            std::function<void(jop::Object&, const glm::vec3&, Transform&)> handlePos =
            [](jop::Object& obj, const glm::vec3& pos, Transform& parent)
            {
                parent.focus();

                jope::CommandBuffer::pushCommand(new jope::SetObjectObjectPositionCommand(obj, obj.getPosition() + pos));
            };
            
            // Position
            m_posLabel.format(true).caption("<white>Position</>");
            // X
            m_posBoxX.editable(true);
            m_posBoxX.modifier("X ", "");
            m_posBoxX.range(-FLT_MAX, FLT_MAX, 0.1);
            m_posBoxX.value(std::to_string(obj.getPosition().x));
            m_posBoxX.events().text_changed([handlePos, this](nana::arg_spinbox arg)
            {
                handlePos(*m_obj, glm::vec3(static_cast<float>(arg.widget.to_double()), 0.f, 0.f), *this);
            });
            // Y
            m_posBoxY.editable(true);
            m_posBoxY.modifier("Y ", "");
            m_posBoxY.range(-FLT_MAX, FLT_MAX, 0.1);
            m_posBoxY.value(std::to_string(obj.getPosition().y));
            m_posBoxY.events().text_changed([handlePos, this](nana::arg_spinbox arg)
            {
                handlePos(*m_obj, glm::vec3(static_cast<float>(0.f, arg.widget.to_double(), 0.f)), *this);
            });
            //Z
            m_posBoxZ.editable(true);
            m_posBoxZ.modifier("Z ", "");
            m_posBoxZ.range(-FLT_MAX, FLT_MAX, 0.1);
            m_posBoxZ.value(std::to_string(obj.getPosition().z));
            m_posBoxZ.events().text_changed([handlePos, this](nana::arg_spinbox arg)
            {
                handlePos(*m_obj, glm::vec3(static_cast<float>(0.f, 0.f, arg.widget.to_double())), *this);
            });

            this->collocate();
        }

    private:

        // Position
        nana::label& m_posLabel;
        nana::spinbox& m_posBoxX;
        nana::spinbox& m_posBoxY;
        nana::spinbox& m_posBoxZ;

        // Scale


        // Rotation


        jop::WeakReference<::jop::Object> m_obj;
    };

    //////////////////////////////////////////////

    const std::unordered_map<std::type_index, void(*)(jope::PropertyWindow::CompVec&)> factories
    ({
        // Camera
        std::make_pair(std::type_index(typeid(jop::Camera)), [](jope::PropertyWindow::CompVec& comps)
        {

        }),

        // Light source
        std::make_pair(std::type_index(typeid(jop::LightSource)), [](jope::PropertyWindow::CompVec& comps)
        {

        })

    });
}

namespace jope
{
    PropertyWindow::PropertyWindow(nana::window parent, const nana::color color)
        : nana::nested_form(parent, ns_propertyAppearance),
          m_layout(*this),
          m_components(),
          m_lastObject(),
          m_lastScene(nullptr)
    {
        m_instance = this;

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
    }

    //////////////////////////////////////////////

    void PropertyWindow::updateComponents(jop::Object& obj, nana::treebox::item_proxy item)
    {
        clearLayout(m_layout, m_components);

        std::lock_guard<std::recursive_mutex> lock(CommandBuffer::acquireMutex());

        m_components.reserve(obj.componentCount() + 2);

        // margin=[top,right,bottom,left]
        m_layout.div("<vertical comps margin=[5,5,0,5] arrange=[80,300,variable,repeated]>");

        // Object properties
        m_components.emplace_back(std::make_unique<comp::Object>(*this, item, obj));
        m_layout.field("comps") << *m_components.back();

        // Transform
        m_components.emplace_back(std::make_unique<comp::Transform>(*this, obj));
        m_layout.field("comps") << *m_components.back();

        // Components
        pushComponents(obj);


        m_lastObject = obj;
        m_lastScene = nullptr;
        m_lastItem = item;
    }

    //////////////////////////////////////////////

    void PropertyWindow::updateComponents(jop::Scene& scene, nana::treebox::item_proxy item)
    {
        clearLayout(m_layout, m_components);
        
        std::lock_guard<std::recursive_mutex> lock(CommandBuffer::acquireMutex());

        m_components.reserve(scene.componentCount() + 1);

        // margin=[top,right,bottom,left]
        m_layout.div("<vertical comps margin=[5,5,0,5] arrange=[50,variable,repeated]>");

        // Scene properties
        m_components.emplace_back(std::make_unique<comp::Scene>(*this, item, scene));
        m_layout.field("comps") << *m_components.back();

        // Components
        pushComponents(scene.getAsObject());


        m_lastObject = jop::WeakReference<jop::Object>();
        m_lastScene = &scene;
        m_lastItem = item;
    }

    //////////////////////////////////////////////

    void PropertyWindow::pushComponents(jop::Object& obj)
    {
        for (auto& i : obj.getComponents())
        {
            auto itr = comp::factories.find(std::type_index(typeid(*i)));

            if (itr != comp::factories.end())
                itr->second(m_components);
            else
                JOP_DEBUG_INFO("No property group defined for component of type \"" << typeid(*i).name() << "\"");
        }

        m_layout.collocate();
    }

    //////////////////////////////////////////////

    void PropertyWindow::refresh()
    {
        auto& inst = *m_instance;

        if (!inst.m_lastObject.expired())
            inst.updateComponents(*inst.m_lastObject, inst.m_lastItem);
        else if (inst.m_lastScene)
            inst.updateComponents(*inst.m_lastScene, inst.m_lastItem);

        for (auto& i : inst.m_components)
            nana::API::refresh_window(*i);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////

    PropertyWindow* PropertyWindow::m_instance = nullptr;
}