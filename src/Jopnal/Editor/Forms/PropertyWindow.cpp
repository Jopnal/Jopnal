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
            m_idBox.bgcolor(nana::API::bgcolor(parent));
            m_idBox.fgcolor(nana::colors::white);
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

                    //jope::CommandBuffer::pushCommand(new jope::ChangeObjectIDCommand(m_scene.getAsObject(), id));
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
            m_idBox.bgcolor(nana::API::bgcolor(parent));
            m_idBox.fgcolor(nana::colors::white);
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
              m_posGroup(*create_child<nana::group>("grp")),

              // Scale
              m_sclGroup(*create_child<nana::group>("grp")),

              // Rotation
              m_rotGroup(*create_child<nana::group>("grp")),
              
              m_obj(obj)
        {
            this->enable_format_caption(true).caption("<white>Transform</>");
            this->div("<vertical margin=5 "
                    
                      // Groups
                      "<vertical grp margin=5 gap=4 arrange=[33%,repeated]>"

                      ">");

            std::function<bool(wchar_t)> acceptFunc = [](wchar_t chr) -> bool
            {
                return (chr <= L'9' && chr >= L'0') || chr == L'.' || chr == '\b' || chr == L'-';
            };

            // Position
            {
                auto makeBox = [acceptFunc, this](nana::group& parent, const float value, const std::size_t index) -> void
                {
                    auto& box = *parent.create_child<nana::textbox>("p");

                    box.bgcolor(nana::API::bgcolor(parent));
                    box.fgcolor(nana::colors::white);
                    box.editable(true).multi_lines(false).reset(std::to_string(value))
                       .set_accept(acceptFunc);

                    std::function<void(nana::textbox&)> applyFunc = [index, this](nana::textbox& tb)
                    {
                        std::string id;
                        tb.getline(0, id);
                        tb.edited_reset();
                        tb.select(false);

                        glm::vec3 newPos(m_obj->getPosition());
                        newPos[index] = static_cast<float>(tb.to_double());
                        jope::CommandBuffer::pushCommand(new jope::SetObjectObjectPositionCommand(*m_obj, newPos));
                    };

                    box.events().key_press([&parent, &box, applyFunc](nana::arg_keyboard arg)
                    {
                        if (arg.key == nana::keyboard::enter && box.edited())
                        {
                            std::string id;
                            box.getline(0, id);
                            box.edited_reset();
                            box.select(false);
                            parent.focus();
                        }
                    });
                    box.events().focus([&box, applyFunc](nana::arg_focus arg)
                    {
                        if (!arg.getting)
                            applyFunc(box);
                    });
                    box.events().dbl_click([&box]()
                    {
                        box.focus();
                        box.select(true);
                    });
                    box.events().click([&box](nana::arg_click arg)
                    {
                        box.focus();
                        box.caret_pos(nana::upoint(arg.mouse_args->pos.x, arg.mouse_args->pos.y));
                    });
                    box.events().mouse_wheel([index, this, &box](nana::arg_wheel arg)
                    {
                        glm::vec3 tempPos(m_obj->getPosition());
                        tempPos[index] += (arg.upwards ? 1.f : -1.f) * 0.25f;



                        box.reset(std::to_string(tempPos[index]));
                        jope::CommandBuffer::pushCommand(new jope::SetObjectObjectPositionCommand(*m_obj, tempPos));
                    });
                };

                m_posGroup.enable_format_caption(true).caption("<white>Position [x,y,z]</>");
                m_posGroup.bgcolor(nana::API::bgcolor(parent));
                m_posGroup.div("<vertical p gap=4 margin=[2,5,0,5]>");

                for (std::size_t i = 0; i < 3; ++i)
                    makeBox(m_posGroup, obj.getPosition()[i], i);
            }

            //////////////////////////////////////////////

            // Scale
            {
                auto makeBox = [acceptFunc, this](nana::group& parent, const float value, const std::size_t index) -> void
                {
                    auto& box = *parent.create_child<nana::textbox>("p");

                    box.bgcolor(nana::API::bgcolor(parent));
                    box.fgcolor(nana::colors::white);
                    box.editable(true).multi_lines(false).reset(std::to_string(value))
                       .set_accept(acceptFunc);

                    std::function<void(nana::textbox&)> applyFunc = [index, this](nana::textbox& tb)
                    {
                        std::string id;
                        tb.getline(0, id);
                        tb.edited_reset();
                        tb.select(false);

                        glm::vec3 newScl(m_obj->getScale());
                        newScl[index] = static_cast<float>(tb.to_double());
                        jope::CommandBuffer::pushCommand(new jope::SetObjectScaleCommand(*m_obj, newScl));
                    };

                    box.events().key_press([&parent, &box, applyFunc](nana::arg_keyboard arg)
                    {
                        if (arg.key == nana::keyboard::enter && box.edited())
                        {
                            std::string id;
                            box.getline(0, id);
                            box.edited_reset();
                            box.select(false);
                            parent.focus();
                        }
                    });
                    box.events().focus([&box, applyFunc](nana::arg_focus arg)
                    {
                        if (!arg.getting)
                            applyFunc(box);
                    });
                    box.events().dbl_click([&box]()
                    {
                        box.focus();
                        box.select(true);
                    });
                    box.events().click([&box](nana::arg_click arg)
                    {
                        box.focus();
                        box.caret_pos(nana::upoint(arg.mouse_args->pos.x, arg.mouse_args->pos.y));
                    });
                    box.events().mouse_wheel([index, this, &box](nana::arg_wheel arg)
                    {
                        glm::vec3 tempScale(m_obj->getScale());
                        tempScale[index] += (arg.upwards ? 1.f : -1.f) * 0.05f;
                        box.reset(std::to_string(tempScale[index]));
                        jope::CommandBuffer::pushCommand(new jope::SetObjectScaleCommand(*m_obj, tempScale));
                    });
                };

                m_sclGroup.enable_format_caption(true).caption("<white>Scale [x,y,z]</>");
                m_sclGroup.bgcolor(nana::API::bgcolor(parent));
                m_sclGroup.div("<vertical p gap=4 margin=[2,5,0,5]>");

                for (std::size_t i = 0; i < 3; ++i)
                    makeBox(m_sclGroup, obj.getScale()[i], i);
            }

            //////////////////////////////////////////////

            // Rotation
            {
                auto makeBox = [acceptFunc, this](nana::group& parent, const float value, const std::size_t index) -> void
                {
                    auto& box = *parent.create_child<nana::textbox>("p");

                    box.bgcolor(nana::API::bgcolor(parent));
                    box.fgcolor(nana::colors::white);
                    box.editable(true).multi_lines(false).reset(std::to_string(glm::degrees(value)))
                       .set_accept(acceptFunc);

                    std::function<void(nana::textbox&)> applyFunc = [index, this](nana::textbox& tb)
                    {
                        std::string id;
                        tb.getline(0, id);
                        tb.edited_reset();
                        tb.select(false);

                        glm::vec3 newRot(glm::eulerAngles(m_obj->getRotation()));
                        newRot[index] = glm::radians(static_cast<float>(tb.to_double()));
                        jope::CommandBuffer::pushCommand(new jope::SetObjectRotCommand(*m_obj, newRot));
                    };

                    box.events().key_press([&parent, &box, applyFunc](nana::arg_keyboard arg)
                    {
                        if (arg.key == nana::keyboard::enter && box.edited())
                        {
                            std::string id;
                            box.getline(0, id);
                            box.edited_reset();
                            box.select(false);
                            parent.focus();
                        }
                    });
                    box.events().focus([&box, applyFunc](nana::arg_focus arg)
                    {
                        if (!arg.getting)
                            applyFunc(box);
                    });
                    box.events().dbl_click([&box]()
                    {
                        box.focus();
                        box.select(true);
                    });
                    box.events().click([&box](nana::arg_click arg)
                    {
                        box.focus();
                        box.caret_pos(nana::upoint(arg.mouse_args->pos.x, arg.mouse_args->pos.y));
                    });
                    box.events().mouse_wheel([index, this, &box](nana::arg_wheel arg)
                    {
                        glm::vec3 tempRot(glm::eulerAngles(m_obj->getRotation()));
                        tempRot[index] += (arg.upwards ? 1.f : -1.f) * glm::radians(5.f);
                        box.reset(std::to_string(glm::degrees(tempRot[index])));

                        jope::CommandBuffer::pushCommand(new jope::SetObjectRotCommand(*m_obj, tempRot));
                    });
                };

                m_rotGroup.enable_format_caption(true).caption("<white>Rotation [x,y,z]</>");
                m_rotGroup.bgcolor(nana::API::bgcolor(parent));
                m_rotGroup.div("<vertical p gap=4 margin=[2,5,0,5]>");

                const glm::vec3 rot(glm::eulerAngles(m_obj->getRotation()));
                for (std::size_t i = 0; i < 3; ++i)
                    makeBox(m_rotGroup, rot[i], i);
            }

            this->collocate();
        }

    private:

        // Position
        nana::group& m_posGroup;

        // Scale
        nana::group& m_sclGroup;
        
        // Rotation
        nana::group& m_rotGroup;


        glm::vec3 m_rotation;
        jop::WeakReference<::jop::Object> m_obj;
    };

    //////////////////////////////////////////////

    const std::unordered_map<std::type_index, void(*)(jope::PropertyWindow::CompVec&)> factories
    ({
        // Camera
        std::make_pair(std::type_index(typeid(jop::Camera)), [](jope::PropertyWindow::CompVec& /*comps*/)
        {

        }),

        // Light source
        std::make_pair(std::type_index(typeid(jop::LightSource)), [](jope::PropertyWindow::CompVec& /*comps*/)
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
          m_lastScene(nullptr),
          m_scrollBar(*this, true)
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

        m_components.reserve(obj.componentCount() + 2);

        // margin=[top,right,bottom,left]
        m_layout.div("<vertical comps margin=[2,4,0,4] arrange=[80,300,variable,repeated]><scrollbar weight=5%>");
        m_layout.field("scrollbar") << m_scrollBar;

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

        m_components.reserve(scene.componentCount() + 1);

        // margin=[top,right,bottom,left]
        m_layout.div("<vertical comps margin=[5,5,0,5] arrange=[50,variable,repeated]>");

        // Scene properties
        m_components.emplace_back(std::make_unique<comp::Scene>(*this, item, scene));
        m_layout.field("comps") << *m_components.back();

        // Components
        //pushComponents(scene.getAsObject());


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