// Jopnal Engine C++ Library
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
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_COMMAND_HANDLER(Component)

        JOP_BIND_MEMBER_COMMAND(&Component::setID, "setID");

    JOP_END_COMMAND_HANDLER(Component)

    bool jop__baseComponentLoadFunc(JOP_COMPONENT_LOAD_ARGS)
    {
        if (val.HasMember("id") && val["id"].IsUint())
            comp.setID(val["id"].GetUint());

        return true;
    }
    bool jop__baseComponentSaveFunc(JOP_COMPONENT_SAVE_ARGS)
    {
        val.AddMember(json::StringRef("id"), comp.getID(), alloc);

        return true;
    }

    JOP_REGISTER_SERIALIZER_NO_FACTORY(jop, Component, jop__baseComponentLoadFunc, jop__baseComponentSaveFunc);
}

namespace jop
{
    Component* Component::clone(Object&) const
    {
        JOP_DEBUG_ERROR("clone() not defined for component \"" << typeid(*this).name() << "\", was not copied");

        return nullptr;
    }

    //////////////////////////////////////////////

    Component::Component(Object& object, const uint32 ID)
        : m_objectRef   (object),
          m_ID          (ID)
    {}

    Component::Component(const Component& other, Object& newObj)
        : m_objectRef   (newObj),
          m_ID          (other.m_ID)
    {}

    Component::~Component()
    {}

    //////////////////////////////////////////////

    Message::Result Component::sendMessage(const Message& message)
    {
        if (message.passFilter(Message::Component))
            return receiveMessage(message);

        return Message::Result::Continue;
    }

    //////////////////////////////////////////////

    void Component::update(const float)
    {}

    //////////////////////////////////////////////

    uint32 Component::getID() const
    {
        return m_ID;
    }

    //////////////////////////////////////////////

    void Component::setID(const uint32 ID)
    {
        m_ID = ID;
    }

    //////////////////////////////////////////////

    WeakReference<Object> Component::getObject()
    {
        return m_objectRef;
    }

    //////////////////////////////////////////////

    WeakReference<const Object> Component::getObject() const
    {
        return static_ref_cast<const Object>(m_objectRef);
    }

    //////////////////////////////////////////////

    bool Component::isActive() const
    {
        return getObject()->isActive();
    }

    //////////////////////////////////////////////

    void Component::removeSelf()
    {
        auto& comps = getObject()->m_components;
        for (auto itr = comps.begin(); itr != comps.end(); ++itr)
        {
            if (itr->get() == this)
            {
                comps.erase(itr);
                break;
            }
        }
    }

    //////////////////////////////////////////////

    void Component::setActive(const bool)
    {}

    //////////////////////////////////////////////

    Message::Result Component::receiveMessage(const Message& message)
    {
        return JOP_EXECUTE_COMMAND(Component, message.getString(), this);
    }
}