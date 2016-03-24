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
        JOP_BIND_MEMBER_COMMAND((WeakReference<Object> (Object::*)(const std::string&, const std::string&))&Object::cloneChild, "cloneChild");
        JOP_BIND_MEMBER_COMMAND(&Object::removeChildren, "removeChildren");
        JOP_BIND_MEMBER_COMMAND(&Object::clearChildren, "clearChildren");
        JOP_BIND_MEMBER_COMMAND(&Object::setID, "setID");

    JOP_END_COMMAND_HANDLER(Object)
}

namespace jop
{
    Object::Object(const std::string& ID)
        : Transform                 (),
          SafeReferenceable<Object> (this),
          m_children                (),
          m_components              (),
          m_ID                      (ID),
          m_ignoreParent            (false),
          m_active                  (true)
    {}

    Object::Object(const Object& other, const std::string& newID)
        : Transform                 (other),
          SafeReferenceable<Object> (this),
          m_children                (),
          m_components              (),
          m_ID                      (newID),
          m_ignoreParent            (other.m_ignoreParent),
          m_active                  (other.m_active)
    {
        m_components.reserve(other.m_components.size());
        for (auto& i : other.m_components)
            m_components.emplace_back(std::unique_ptr<Component>(i->clone(*this)));

        m_children.reserve(other.m_children.size());
        for (auto& i : other.m_children)
            m_children.emplace_back(i, i.getID());
    }

    Object::Object(Object&& other)
        : Transform                 (other),
          SafeReferenceable<Object> (std::move(other)),
          m_children                (std::move(other.m_children)),
          m_components              (std::move(other.m_components)),
          m_ID                      (std::move(other.m_ID)),
          m_ignoreParent            (other.m_ignoreParent),
          m_active                  (other.m_active)
    {}

    Object::Object(const Object& other, const std::string& newID, const Transform& newTransform)
        : Transform                 (newTransform),
          SafeReferenceable<Object> (this),
          m_children                (),
          m_components              (),
          m_ID                      (newID),
          m_ignoreParent            (other.m_ignoreParent),
          m_active                  (other.m_active)
    {
        m_components.reserve(other.m_components.size());
        for (auto& i : other.m_components)
            m_components.emplace_back(std::unique_ptr<Component>(i->clone(*this)));

        m_children.reserve(other.m_children.size());
        for (auto& i : other.m_children)
            m_children.emplace_back(i, i.getID());
    }

    Object& Object::operator=(Object&& other)
    {
        Transform::operator =(other);
        SafeReferenceable<Object>::operator=(std::move(other));

        m_children      = std::move(other.m_children);
        m_components    = std::move(other.m_components);
        m_ID            = std::move(other.m_ID);
        m_ignoreParent  = other.m_ignoreParent;
        m_active        = other.m_active;

        return *this;
    }

    //////////////////////////////////////////////

    Component* Object::getComponent(const std::string& ID)
    {
        for (auto& i : m_components)
        {
            if (i->getID() == ID)
                return i.get();
        }

        return nullptr;
    }

    const Component* Object::getComponent(const std::string& ID) const
    {
        for (auto& i : m_components)
        {
            if (i->getID() == ID)
                return i.get();
        }

        return nullptr;
    }

    //////////////////////////////////////////////

    const std::vector<std::unique_ptr<Component>>& Object::getComponents() const
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

    void Object::clearComponents()
    {
        m_components.clear();
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
            if (m_children.size() == m_children.capacity())
                m_children.reserve(m_children.size() + 1);

            m_children.emplace_back(*ptr, clonedID);
            return m_children.back().getReference();
        }

        return WeakReference<Object>();
    }

    //////////////////////////////////////////////

    WeakReference<Object> Object::cloneChild(const std::string& ID, const std::string& clonedID, const Transform& newTransform)
    {
        auto ptr = getChild(ID);

        if (!ptr.expired())
        {
            if (m_children.size() == m_children.capacity())
                m_children.reserve(m_children.size() + 1);

            m_children.emplace_back(*ptr, clonedID, newTransform);
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

    void Object::setIgnoreParent(const bool ignore)
    {
        m_ignoreParent = ignore;
    }

    //////////////////////////////////////////////

    bool Object::ignoresParent() const
    {
        return m_ignoreParent;
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

    //////////////////////////////////////////////

    void Object::setActive(const bool active)
    {
        if (active != m_active)
        {
            m_active = active;

            for (auto& i : m_components)
                i->setActive(active);

            for (auto& i : m_children)
                i.setActive(active);
        }
    }

    //////////////////////////////////////////////

    bool Object::isActive() const
    {
        return m_active;
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
        if (!isActive())
            return;

        if (parent && !ignoresParent())
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

    /////////////////////////////////////////////

    Object& Object::cloneComponent(const WeakReference<Object> other, const std::string& ID)
    {
        auto& i = *other->getComponent(ID);     
        if (&i != NULL)
        m_components.emplace_back(std::unique_ptr<Component>(i.clone(*this)));
        else
            JOP_DEBUG_ERROR("Error in cloning component " << ID<<" from object "<<other->getID()<<" to object "<<getID());

        return *this;
    }

    /////////////////////////////////////////////

    Object& Object::cloneComponent(const std::string& ID, const std::string& newID)
    {
        auto& i = *getComponent(ID);
        if (&i != NULL)
        {
            m_components.emplace_back(std::unique_ptr<Component>(i.clone(*this)));
            m_components.back()->setID(newID);
        }
        else
            JOP_DEBUG_ERROR("Error in duplicating component " << ID<<" at object "<<getID());

        return *this;
    }
}