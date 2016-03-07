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

//Headers
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_COMMAND_HANDLER(Object)

        // Transform
        JOP_BIND_MEMBER_COMMAND_NORETURN((Transform& (Object::*)(const float, const float, const float))&Object::setRotation, "setRotation");
        JOP_BIND_MEMBER_COMMAND_NORETURN((Transform& (Object::*)(const float, const float, const float))&Object::setScale, "setScale");
        JOP_BIND_MEMBER_COMMAND_NORETURN((Transform& (Object::*)(const float, const float, const float))&Object::setPosition, "setPosition");

        JOP_BIND_MEMBER_COMMAND_NORETURN((Transform& (Object::*)(const float, const float, const float))&Object::lookAt, "lookAt");

        JOP_BIND_MEMBER_COMMAND_NORETURN((Transform& (Object::*)(const float, const float, const float))&Object::move, "move");
        JOP_BIND_MEMBER_COMMAND_NORETURN((Transform& (Object::*)(const float, const float, const float))&Object::scale, "scale");
        JOP_BIND_MEMBER_COMMAND_NORETURN((Transform& (Object::*)(const float, const float, const float))&Object::rotate, "rotate");

        // Object
        JOP_BIND_MEMBER_COMMAND(&Object::setActive, "setActive");
        JOP_BIND_MEMBER_COMMAND(&Object::removeComponents, "removeComponents");
        JOP_BIND_MEMBER_COMMAND(&Object::cloneChild, "cloneChild");
        JOP_BIND_MEMBER_COMMAND(&Object::removeChildren, "removeChildren");
        JOP_BIND_MEMBER_COMMAND(&Object::clearChildren, "clearChildren");
        JOP_BIND_MEMBER_COMMAND(&Object::setID, "setID");

    JOP_END_COMMAND_HANDLER(Object)
}

namespace jop
{
    Object::Object()
        : Transform                 (),
          Activateable              (true),
          SafeReferenceable<Object> (this),
          m_children                (),
          m_components              (),
          m_ID                      ()
    {}

    Object::Object(const std::string& ID)
        : Transform                 (),
          Activateable              (true),
          SafeReferenceable<Object> (this),
          m_children                (),
          m_components              (),
          m_ID                      (ID)
    {}

    Object::Object(const Object& other, const std::string& newName)
        : Transform                 (other),
          Activateable              (other.isActive()),
          SafeReferenceable<Object> (this),
          m_children                (),
          m_components              (),
          m_ID                      (newName)
    {
        m_components.reserve(other.m_components.size());
        for (auto& i : other.m_components)
        {
            m_components.emplace_back(std::unique_ptr<Component>(i->clone()));
            m_components.back()->m_objectRef = getReference();
        }

        m_children.reserve(other.m_children.size());
        for (auto& i : other.m_children)
            m_children.emplace_back(i, i.getID());
    }

    Object::Object(Object&& other)
        : Transform                 (other),
          Activateable              (other.isActive()),
          SafeReferenceable<Object> (std::move(other)),
          m_children                (std::move(other.m_children)),
          m_components              (std::move(other.m_components)),
          m_ID                      (std::move(other.m_ID))
    {}

    Object& Object::operator=(Object&& other)
    {
        Transform::operator =(other);
        SafeReferenceable<Object>::operator=(std::move(other));

        m_children      = std::move(other.m_children);
        m_components    = std::move(other.m_components);
        m_ID            = std::move(other.m_ID);

        setActive(other.isActive());

        return *this;
    }

    //////////////////////////////////////////////

    WeakReference<Component> Object::getComponent(const std::string& ID)
    {
        for (auto& i : m_components)
        {
            if (i->getID() == ID)
                return i->getReference();
        }

        return WeakReference<Component>();
    }

    //////////////////////////////////////////////

    const std::vector<std::unique_ptr<Component>>& Object::getCmponents() const
    {
        return m_components;
    }

    /////////////////////////////////////////////

    void Object::removeComponents(const std::string& ID)
    {
        m_components.erase(std::remove_if(m_components.begin(), m_components.end(), [&ID](const std::unique_ptr<Component>& comp)
        {
            return comp->getID() == ID;

        }), m_components.end());
    }

    //////////////////////////////////////////////

    unsigned int Object::componentCount() const
    {
        return m_components.size();
    }

    //////////////////////////////////////////////

    WeakReference<Object> Object::createChild(const std::string& ID)
    {
        m_children.emplace_back(ID);
        return m_children.back().getReference();
    }

    //////////////////////////////////////////////

    WeakReference<Object> Object::getChild(const std::string& ID)
    {
        for (auto& i : m_children)
        {
            if (i.getID() == ID)
                return i.getReference();
        }

        return WeakReference<Object>();
    }

    //////////////////////////////////////////////

    const std::vector<Object>& Object::getChildren() const
    {
        return m_children;
    }

    //////////////////////////////////////////////

    WeakReference<Object> Object::cloneChild(const std::string& ID, const std::string& clonedID)
    {
        auto ptr = getChild(ID);

        if (!ptr.expired())
        {
            m_children.emplace_back(*ptr, clonedID);
            return m_children.back().getReference();
        }

        return WeakReference<Object>();
    }

    //////////////////////////////////////////////

    void Object::removeChildren(const std::string& ID)
    {
        m_children.erase(std::remove_if(m_children.begin(), m_children.end(), [&ID](const Object& obj)
        {
            return obj.getID() == ID;

        }), m_children.end());
    }

    //////////////////////////////////////////////

    void Object::clearChildren()
    {
        m_children.clear();
    }

    /////////////////////////////////////////////

    unsigned int Object::childCount() const
    {
        return m_children.size();
    }

    /////////////////////////////////////////////

    unsigned int Object::childCountRecursive() const
    {
        unsigned int count = childCount();;

        for (auto& i : m_children)
            count += i.childCountRecursive();

        return count;
    }

    //////////////////////////////////////////////

    Message::Result Object::sendMessage(const std::string& message)
    {
        Any wrap;
        return sendMessage(message, wrap);
    }

    /////////////////////////////////////////////

    Message::Result Object::sendMessage(const std::string& message, Any& returnWrap)
    {
        const Message msg(message, returnWrap);
        return sendMessage(msg);
    }

    /////////////////////////////////////////////

    Message::Result Object::sendMessage(const Message& message)
    {
        if (message.passFilter(Message::Object, getID()) && message.passFilter(Message::Command))
        {
            Any instance(this);
            if (JOP_EXECUTE_COMMAND(Object, message.getString(), instance, message.getReturnWrapper()) == Message::Result::Escape)
                return Message::Result::Escape;
        }

        if (message.passFilter(Message::Component))
        {
            for (auto& i : m_components)
            {
                if (i->sendMessage(message) == Message::Result::Escape)
                    return Message::Result::Escape;
            }
        }

        if (message.passFilter(Message::Object))
        {
            for (auto& i : m_children)
            {
                if (i.sendMessage(message) == Message::Result::Escape)
                    return Message::Result::Escape;
            }
        }

        return Message::Result::Continue;
    }

    /////////////////////////////////////////////

    void Object::update(const float deltaTime)
    {
        if (isActive())
        {
            for (auto& i : m_components)
                i->update(deltaTime);

            for (auto& i : m_children)
                i.update(deltaTime);
        }
    }

    /////////////////////////////////////////////

    void Object::fixedUpdate(const float timeStep)
    {
        if (isActive())
        {
            for (auto& i : m_components)
                i->fixedUpdate(timeStep);

            for (auto& i : m_children)
                i.fixedUpdate(timeStep);
        }
    }

    /////////////////////////////////////////////

    const std::string& Object::getID() const
    {
        return m_ID;
    }

    /////////////////////////////////////////////

    void Object::setID(const std::string& ID)
    {
        m_ID = ID;
    }

    //////////////////////////////////////////////

    void Object::updateTransformTree(const Object* parent, bool parentUpdated)
    {
        if (parent)
        {
            if (m_transformNeedUpdate || ((m_transformNeedUpdate = parentUpdated) == true))
            {
                parentUpdated = true;
                m_transform = parent->getMatrix() * this->getMatrix();
            }
        }
        else if (m_transformNeedUpdate)
            parentUpdated = true;

        for (auto& i : m_children)
            i.updateTransformTree(this, parentUpdated);
    }
}