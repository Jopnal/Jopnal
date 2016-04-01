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
        // Component
        JOP_BIND_MEMBER_COMMAND(&Object::removeComponents, "removeComponents");
        JOP_BIND_MEMBER_COMMAND(&Object::clearComponents, "clearComponents");

        // Activity
        JOP_BIND_MEMBER_COMMAND(&Object::setActive, "setActive");

        // Children
        JOP_BIND_MEMBER_COMMAND(&Object::createChild, "createChild");
        JOP_BIND_MEMBER_COMMAND_ESCAPE(&Object::adoptChild, "adoptChild");
        JOP_BIND_MEMBER_COMMAND((WeakReference<Object> (Object::*)(const std::string&, const std::string&))&Object::cloneChild, "cloneChild");
        JOP_BIND_MEMBER_COMMAND(&Object::removeChildren, "removeChildren");
        JOP_BIND_MEMBER_COMMAND(&Object::clearChildren, "clearChildren");
        JOP_BIND_MEMBER_COMMAND(&Object::setParent, "setParent");

        // Tags
        JOP_BIND_MEMBER_COMMAND(&Object::addTag, "addTag");
        JOP_BIND_MEMBER_COMMAND(&Object::removeTag, "removeTag");
        JOP_BIND_MEMBER_COMMAND(&Object::clearTags, "clearTags");

        // Other
        JOP_BIND_MEMBER_COMMAND(&Object::removeSelf, "removeSelf");
        JOP_BIND_MEMBER_COMMAND(&Object::setID, "setID");
        JOP_BIND_MEMBER_COMMAND(&Object::setIgnoreParent, "setIgnoreParent");

    JOP_END_COMMAND_HANDLER(Object)
}

namespace jop
{
    Object::Object(const std::string& ID)
        : Transform                 (),
          SafeReferenceable<Object> (this),
          m_children                (),
          m_components              (),
          m_tags                    (),
          m_ID                      (ID),
          m_parent                  (),
          m_flags                   (ActiveFlag)
    {}

    Object::Object(const Object& other, const std::string& newID, const Transform& newTransform)
        : Transform                 (newTransform),
          SafeReferenceable<Object> (this),
          m_children                (),
          m_components              (),
          m_tags                    (other.m_tags),
          m_ID                      (newID),
          m_parent                  (other.m_parent),
          m_flags                   (other.m_flags)
    {
        m_components.reserve(other.m_components.size());
        for (auto& i : other.m_components)
            m_components.emplace_back(std::unique_ptr<Component>(i->clone(*this)));

        m_children.reserve(other.m_children.size());
        for (auto& i : other.m_children)
        {
            m_children.emplace_back(i, i.getID(), i);
            m_children.back().m_parent = *this;
        }
    }

    Object::Object(Object&& other)
        : Transform                 (other),
          SafeReferenceable<Object> (std::move(other)),
          m_children                (std::move(other.m_children)),
          m_components              (std::move(other.m_components)),
          m_tags                    (std::move(other.m_tags)),
          m_ID                      (std::move(other.m_ID)),
          m_parent                  (other.m_parent),
          m_flags                   (other.m_flags)
    {}

    Object& Object::operator=(Object&& other)
    {
        Transform::operator =(other);
        SafeReferenceable<Object>::operator =(std::move(other));

        m_children      = std::move(other.m_children);
        m_components    = std::move(other.m_components);
        m_tags          = std::move(other.m_tags);
        m_ID            = std::move(other.m_ID);
        m_parent        = other.m_parent;
        m_flags         = other.m_flags;

        return *this;
    }

    Object::~Object()
    {
        // These need to be cleared before anything else
        m_children.clear();
        m_components.clear();
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

    Component* Object::cloneComponent(Object& object, const std::string& ID) const
    {
        auto i = getComponent(ID);

        if (i)
        {
            object.m_components.emplace_back(i->clone(object));
            return object.m_components.back().get();
        }

        return nullptr;
    }

    /////////////////////////////////////////////

    Component* Object::cloneComponent(const std::string& ID, const std::string& newID)
    {
        auto i = getComponent(ID);

        if (i)
        {
            m_components.emplace_back(i->clone(*this));
            m_components.back()->setID(newID);
        }

        return i;
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
        m_children.back().m_parent = *this;

        return m_children.back().getReference();
    }

    //////////////////////////////////////////////

    WeakReference<Object> Object::adoptChild(Object& child)
    {
        JOP_ASSERT(!child.getParent().expired(), "You must not reparent a scene!");

        if (child.getParent().get() == this)
            return child.getReference();

        if (m_children.size() == m_children.capacity())
            m_children.reserve(m_children.size() + 1);

        m_children.emplace_back(std::move(child));
        m_children.back().m_parent = *this;

        child.removeSelf();

        return m_children.back().getReference();
    }

    //////////////////////////////////////////////

    const std::vector<Object>& Object::getChildren() const
    {
        return m_children;
    }

    //////////////////////////////////////////////

    WeakReference<Object> Object::cloneChild(const std::string& ID, const std::string& clonedID)
    {
        auto ptr = findChild(ID);

        if (!ptr.expired())
        {
            if (m_children.size() == m_children.capacity())
                m_children.reserve(m_children.size() + 1);

            m_children.emplace_back(*ptr, clonedID, *ptr);
            return m_children.back().getReference();
        }

        return WeakReference<Object>();
    }

    //////////////////////////////////////////////

    WeakReference<Object> Object::cloneChild(const std::string& ID, const std::string& clonedID, const Transform& newTransform)
    {
        auto ptr = findChild(ID);

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
        for (auto& i : m_children)
        {
            if (i.getID() == ID)
                i.removeSelf();
        }
    }

    //////////////////////////////////////////////

    void Object::clearChildren()
    {
        m_children.clear();
    }

    //////////////////////////////////////////////

    void Object::removeSelf()
    {
        m_flags |= RemoveFlag;

        if (!m_parent.expired())
            m_parent->m_flags |= ChildrenRemovedFlag;
    }

    //////////////////////////////////////////////

    bool Object::isRemoved() const
    {
        return (m_flags & RemoveFlag) != 0;
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
        m_flags = (ignore ? m_flags | IgnoreParentFlag : m_flags & ~(IgnoreParentFlag));
    }

    //////////////////////////////////////////////

    bool Object::ignoresParent() const
    {
        return (m_flags & IgnoreParentFlag) != 0;
    }

    //////////////////////////////////////////////

    WeakReference<Object> Object::getParent() const
    {
        return m_parent;
    }

    //////////////////////////////////////////////

    WeakReference<Object> Object::setParent(Object& newParent)
    {
        return newParent.adoptChild(*this);
    }

    //////////////////////////////////////////////

    Scene& Object::getScene()
    {
        return getParent().expired() ? static_cast<Scene&>(*this) : getParent()->getScene();
    }

    //////////////////////////////////////////////

    const Scene& Object::getScene() const
    {
        return getParent().expired() ? static_cast<const Scene&>(*this) : getParent()->getScene();
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
        if (message.passFilter(Message::Object) && message.passFilter(getID()) && message.passFilter(m_tags) && message.passFilter(Message::Command))
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

    namespace detail
    {
        bool findChildStrict(const std::string& childID, const std::string& findID)
        {
            return childID == findID;
        }

        bool findChildLoose(const std::string& childID, const std::string& findID)
        {
            return childID.find(findID) != std::string::npos;
        }
    }

    WeakReference<Object> Object::findChild(const std::string& ID, const bool recursive, const bool strict) const
    {
        auto method = strict ? &detail::findChildStrict : &detail::findChildLoose;

        for (auto& i : m_children)
        {
            if (method(i.getID(), ID))
                return i.getReference();

            if (recursive)
            {
                auto ref = i.findChild(ID, true, strict);

                if (!ref.expired())
                    return ref;
            }
        }

        return WeakReference<Object>();
    }


    /////////////////////////////////////////////

    std::vector<WeakReference<Object>> Object::findChildren(const std::string& ID, const bool recursive, const bool strict) const
    {
        auto method = strict ? &detail::findChildStrict : &detail::findChildLoose;

        std::vector<WeakReference<Object>> vec;

        for (auto& i : m_children)
        {
            if (method(i.getID(), ID))
                vec.push_back(i.getReference());

            if (recursive)
            {
                auto ref = i.findChildren(ID, true, strict);
                vec.insert(vec.end(), ref.begin(), ref.end());
            }
        }

        return vec;
    }

    /////////////////////////////////////////////

    std::vector<WeakReference<Object>> Object::findChildrenWithTag(const std::string& tag, const bool recursive) const
    {
        std::vector<WeakReference<Object>> vec;

        for (auto& i : m_children)
        {
            if (i.hasTag(tag))
                vec.push_back(i.getReference());

            if (recursive)
            {
                auto ref = i.findChildrenWithTag(tag, true);
                vec.insert(vec.end(), ref.begin(), ref.end());
            }
        }

        return vec;
    }

    /////////////////////////////////////////////

    WeakReference<Object> Object::findChildWithPath(const std::string& path) const
    {
        if (!path.empty())
        {
            auto pos = path.find_first_of('>');

            if (pos != 0)
            {
                if (pos != std::string::npos)
                {
                    auto ref = findChild(path.substr(0, pos), false, true);

                    if (!ref.expired() && pos < path.length())
                        return ref->findChild(path.substr(pos + 1));
                }
                else
                    return findChild(path, false, true);
            }
            else
                JOP_DEBUG_ERROR("Invalid object path: " << path);
        }

        return WeakReference<Object>();
    }
        
    /////////////////////////////////////////////

    std::string Object::makeSearchPath() const
    {
        WeakReference<Object> obj(m_parent);
        std::string path((obj.expired() ? "" : ">") + getID());

        while (!obj.expired())
        {
            path.insert(0, (obj->m_parent.expired() ? "" : ">") + obj->getID());
            obj = obj->m_parent;
        }

        return path;
    }

    /////////////////////////////////////////////

    void Object::setActive(const bool active)
    {
        if (isActive() != active)
        {
            m_flags = (active ? m_flags | ActiveFlag : m_flags & ~(ActiveFlag));

            for (auto& i : m_components)
                i->setActive(active);

            for (auto& i : m_children)
                i.setActive(active);
        }
    }

    //////////////////////////////////////////////

    bool Object::isActive() const
    {
        return (m_flags & ActiveFlag) != 0 && !isRemoved();
    }

    /////////////////////////////////////////////

    void Object::update(const float deltaTime)
    {
        if (isActive())
        {
            sweepRemoved();

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

    void Object::addTag(const std::string& tag)
    {
        m_tags.insert(tag);
    }

    //////////////////////////////////////////////

    void Object::removeTag(const std::string& tag)
    {
        m_tags.erase(tag);
    }

    //////////////////////////////////////////////

    void Object::clearTags()
    {
        m_tags.clear();
    }

    //////////////////////////////////////////////

    bool Object::hasTag(const std::string& tag) const
    {
        return m_tags.find(tag) != m_tags.end();
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

    void Object::sweepRemoved()
    {
        if ((m_flags & ChildrenRemovedFlag) != 0)
        {
            m_children.erase(std::remove_if(m_children.begin(), m_children.end(), [](const Object& obj)
            {
                return obj.isRemoved();
            }), m_children.end());

            m_flags &= ~(ChildrenRemovedFlag);
        }
    }

}