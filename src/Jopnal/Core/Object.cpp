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
        JOP_BIND_MEMBER_COMMAND_NORETURN((Object& (Object::*)(const float, const float, const float))&Object::setRotation, "setRotation");
        JOP_BIND_MEMBER_COMMAND_NORETURN((Object& (Object::*)(const float, const float, const float))&Object::setScale, "setScale");
        JOP_BIND_MEMBER_COMMAND_NORETURN((Object& (Object::*)(const float, const float, const float))&Object::setPosition, "setPosition");

        JOP_BIND_MEMBER_COMMAND_NORETURN((Object& (Object::*)(const float, const float, const float))&Object::lookAt, "lookAt");

        JOP_BIND_MEMBER_COMMAND_NORETURN((Object& (Object::*)(const float, const float, const float))&Object::move, "move");
        JOP_BIND_MEMBER_COMMAND_NORETURN((Object& (Object::*)(const float, const float, const float))&Object::scale, "scale");
        JOP_BIND_MEMBER_COMMAND_NORETURN((Object& (Object::*)(const float, const float, const float))&Object::rotate, "rotate");

        // Object

        // Component
        JOP_BIND_MEMBER_COMMAND_NORETURN(&Object::removeComponents, "removeComponents");
        JOP_BIND_MEMBER_COMMAND_NORETURN(&Object::clearComponents, "clearComponents");

        // Activity
        JOP_BIND_MEMBER_COMMAND_NORETURN(&Object::setActive, "setActive");

        // Children
        JOP_BIND_MEMBER_COMMAND(&Object::createChild, "createChild");
        JOP_BIND_MEMBER_COMMAND_ESCAPE(&Object::adoptChild, "adoptChild");
        JOP_BIND_MEMBER_COMMAND((WeakReference<Object> (Object::*)(const std::string&, const std::string&))&Object::cloneChild, "cloneChild");
        JOP_BIND_MEMBER_COMMAND_NORETURN(&Object::removeChildren, "removeChildren");
        JOP_BIND_MEMBER_COMMAND_NORETURN(&Object::clearChildren, "clearChildren");
        JOP_BIND_MEMBER_COMMAND(&Object::setParent, "setParent");

        // Tags
        JOP_BIND_MEMBER_COMMAND_NORETURN(&Object::addTag, "addTag");
        JOP_BIND_MEMBER_COMMAND_NORETURN(&Object::removeTag, "removeTag");
        JOP_BIND_MEMBER_COMMAND_NORETURN(&Object::clearTags, "clearTags");

        // Other
        JOP_BIND_MEMBER_COMMAND(&Object::removeSelf, "removeSelf");
        JOP_BIND_MEMBER_COMMAND_NORETURN(&Object::setID, "setID");
        JOP_BIND_MEMBER_COMMAND_NORETURN(&Object::setIgnoreParent, "setIgnoreParent");

    JOP_END_COMMAND_HANDLER(Object)
}

namespace jop
{
    Object::Object(const std::string& ID)
        : SafeReferenceable<Object> (this),
          m_transform               (),
          m_inverseTransform        (),
          m_locals                  (),
          m_globals                 (),
          m_children                (),
          m_components              (),
          m_tags                    (),
          m_ID                      (ID),
          m_parent                  (),
          m_flags                   (ActiveFlag | MatrixDirty | InverseMatrixDirty | GlobalPositionDirty | GlobalRotationDirty | GlobalScaleDirty)
    {
        m_locals.position = glm::vec3(0.f);
        m_locals.scale = glm::vec3(1.f);
        m_locals.rotation = glm::quat(1.f, 0.f, 0.f, 0.f);
    }

    Object::Object(const Object& other, const std::string& newID, const Transform::Variables& newTransform)
        : SafeReferenceable<Object> (this),
          m_transform               (),
          m_inverseTransform        (),
          m_locals                  (newTransform),
          m_globals                 (),
          m_children                (),
          m_components              (),
          m_tags                    (other.m_tags),
          m_ID                      (newID),
          m_parent                  (other.m_parent),
          m_flags                   (other.m_flags | MatrixDirty | InverseMatrixDirty | GlobalPositionDirty | GlobalRotationDirty | GlobalScaleDirty)
    {
        m_components.reserve(other.m_components.size());
        for (auto& i : other.m_components)
            m_components.emplace_back(std::unique_ptr<Component>(i->clone(*this)));

        m_children.reserve(other.m_children.size());
        for (auto& i : other.m_children)
        {
            m_children.emplace_back(i, i.getID(), i.m_locals);
            m_children.back().m_parent = *this;
        }
    }

    Object::Object(Object&& other)
        : SafeReferenceable<Object> (std::move(other)),
          m_transform               (),
          m_inverseTransform        (),
          m_locals                  (other.m_locals),
          m_globals                 (),
          m_children                (std::move(other.m_children)),
          m_components              (std::move(other.m_components)),
          m_tags                    (std::move(other.m_tags)),
          m_ID                      (std::move(other.m_ID)),
          m_parent                  (other.m_parent),
          m_flags                   (other.m_flags | MatrixDirty | InverseMatrixDirty | GlobalPositionDirty | GlobalRotationDirty | GlobalScaleDirty)
    {}

    Object& Object::operator=(Object&& other)
    {
        SafeReferenceable<Object>::operator =(std::move(other));
        
        m_locals        = other.m_locals;
        m_children      = std::move(other.m_children);
        m_components    = std::move(other.m_components);
        m_tags          = std::move(other.m_tags);
        m_ID            = std::move(other.m_ID);
        m_parent        = other.m_parent;
        m_flags         = other.m_flags | MatrixDirty | InverseMatrixDirty | GlobalPositionDirty | GlobalRotationDirty | GlobalScaleDirty;

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

    Object& Object::removeComponents(const std::string& ID)
    {
        m_components.erase(std::remove_if(m_components.begin(), m_components.end(), [&ID](const std::unique_ptr<Component>& comp)
        {
            return comp->getID() == ID;

        }), m_components.end());

        return *this;
    }

    //////////////////////////////////////////////

    Object& Object::clearComponents()
    {
        m_components.clear();
        return *this;
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

            m_children.emplace_back(*ptr, clonedID, ptr->m_locals);
            return m_children.back().getReference();
        }

        return WeakReference<Object>();
    }

    //////////////////////////////////////////////

    WeakReference<Object> Object::cloneChild(const std::string& ID, const std::string& clonedID, const Transform::Variables& newTransform)
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

    Object& Object::removeChildren(const std::string& ID)
    {
        for (auto& i : m_children)
        {
            if (i.getID() == ID)
                i.removeSelf();
        }

        return *this;
    }

    //////////////////////////////////////////////

    Object& Object::removeChildrenWithTag(const std::string& tag, const bool recursive)
    {
        auto children = findChildrenWithTag(tag, recursive);

        for (auto& i : children)
            i->removeSelf();

        return *this;
    }

    //////////////////////////////////////////////

    Object& Object::clearChildren()
    {
        m_children.clear();
        return *this;
    }

    //////////////////////////////////////////////

    void Object::removeSelf()
    {
        setFlags(RemoveFlag);

        if (!m_parent.expired())
            m_parent->m_flags |= ChildrenRemovedFlag;
    }

    //////////////////////////////////////////////

    WeakReference<Object> Object::cloneSelf()
    {
        return cloneSelf(getID(), m_locals);
    }

    //////////////////////////////////////////////

    WeakReference<Object> Object::cloneSelf(const std::string& newID)
    {
        return cloneSelf(newID, m_locals);
    }

    //////////////////////////////////////////////

    WeakReference<Object> Object::cloneSelf(const std::string& newID, const Transform::Variables& newTransform)
    {
        JOP_ASSERT(!getParent().expired(), "Cannot clone a scene!");

        auto& vec = getParent()->m_children;
        if (vec.size() == vec.capacity())
            vec.reserve(vec.size() + 1);

        vec.emplace_back(*this, newID, newTransform);
        return vec.back().getReference();
    }

    //////////////////////////////////////////////

    bool Object::isRemoved() const
    {
        return flagSet(RemoveFlag);
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

    Object& Object::setIgnoreParent(const bool ignore)
    {
        ignore ? setFlags(IgnoreParent) : clearFlags(IgnoreParent);
        return *this;
    }

    //////////////////////////////////////////////

    bool Object::ignoresParent() const
    {
        return flagSet(IgnoreParent);
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

    Object& Object::setActive(const bool active)
    {
        if (isActive() != active)
        {
            active ? setFlags(ActiveFlag) : clearFlags(ActiveFlag);

            for (auto& i : m_components)
                i->setActive(active);

            for (auto& i : m_children)
                i.setActive(active);
        }

        return *this;
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

            // Have to use a good old fashion loop here
            // since it's possible that objects are added
            // or removed inside it.
            for (std::size_t i = 0; i < m_children.size(); ++i)
                m_children[i].update(deltaTime);
        }
    }

    /////////////////////////////////////////////

    const std::string& Object::getID() const
    {
        return m_ID;
    }

    /////////////////////////////////////////////

    Object& Object::setID(const std::string& ID)
    {
        m_ID = ID;
        return *this;
    }

    //////////////////////////////////////////////

    Object& Object::addTag(const std::string& tag)
    {
        m_tags.insert(tag);
        return *this;
    }

    //////////////////////////////////////////////

    Object& Object::removeTag(const std::string& tag)
    {
        m_tags.erase(tag);
        return *this;
    }

    //////////////////////////////////////////////

    Object& Object::clearTags()
    {
        m_tags.clear();
        return *this;
    }

    //////////////////////////////////////////////

    bool Object::hasTag(const std::string& tag) const
    {
        return m_tags.find(tag) != m_tags.end();
    }

    /////////////////////////////////////////////

    void Object::sweepRemoved()
    {
        if (flagSet(ChildrenRemovedFlag))
        {
            m_children.erase(std::remove_if(m_children.begin(), m_children.end(), [](const Object& obj)
            {
                return obj.isRemoved();
            }), m_children.end());

            clearFlags(ChildrenRemovedFlag);
        }
    }

    /////////////////////////////////////////////

    const Transform& Object::getTransform() const
    {
        if (flagSet(MatrixDirty))
        {
            m_transform.getMatrix() = Transform::IdentityMatrix;
            m_transform.translate(getLocalPosition());
            m_transform.rotate(getLocalRotation());
            m_transform.scale(getLocalScale());

            if (!m_parent.expired() && !ignoresParent())
                m_transform = m_parent->getTransform() * m_transform;

            clearFlags(MatrixDirty);
        }

        return m_transform;
    }

    const Transform& Object::getInverseTransform() const
    {
        if (flagSet(InverseMatrixDirty))
        {
            m_inverseTransform.getMatrix() = glm::inverse(getTransform().getMatrix());
            clearFlags(InverseMatrixDirty);
        }

        return m_inverseTransform;
    }

    /////////////////////////////////////////////S
    
    Object& Object::setRotation(const float x, const float y, const float z)
    {
        return setRotation(glm::vec3(x, y, z));
    }

    //////////////////////////////////////////////

    Object& Object::setRotation(const glm::vec3& rotation)
    {
        return setRotation(glm::quat(rotation));
    }

    //////////////////////////////////////////////

    Object& Object::setRotation(const glm::quat& rotation)
    {
        m_locals.rotation = rotation;
        propagateFlags(MatrixDirty | InverseMatrixDirty | GlobalRotationDirty);

        return *this;
    }

    //////////////////////////////////////////////

    Object& Object::setRotation(const float angle, const glm::vec3& axis)
    {
        return setRotation(glm::quat(angle, axis));
    }

    //////////////////////////////////////////////

    const glm::quat& Object::getLocalRotation() const
    {
        return m_locals.rotation;
    }

    //////////////////////////////////////////////

    const glm::quat& Object::getGlobalRotation() const
    {
        if (flagSet(GlobalRotationDirty))
        {
            if (m_parent.expired() || ignoresParent())
                m_globals.rotation = getLocalRotation();
            else
                m_globals.rotation = m_parent->getGlobalRotation() * getLocalRotation();

            clearFlags(GlobalRotationDirty);
        }

        return m_globals.rotation;
    }

    //////////////////////////////////////////////

    glm::vec3 Object::getGlobalFront() const
    {
        return getGlobalRotation() * Transform::Front;
    }

    //////////////////////////////////////////////

    glm::vec3 Object::getGlobalRight() const
    {
        return getGlobalRotation() * Transform::Right;
    }

    //////////////////////////////////////////////

    glm::vec3 Object::getGlobalUp() const
    {
        return getGlobalRotation() * Transform::Up;
    }

    //////////////////////////////////////////////

    glm::vec3 Object::getLocalFront() const
    {
        return getLocalRotation() * Transform::Front;
    }

    //////////////////////////////////////////////

    glm::vec3 Object::getLocalRight() const
    {
        return getLocalRotation() * Transform::Right;
    }

    //////////////////////////////////////////////

    glm::vec3 Object::getLocalUp() const
    {
        return getLocalRotation() * Transform::Up;
    }

    //////////////////////////////////////////////

    Object& Object::setScale(const float x, const float y, const float z)
    {
        return setScale(glm::vec3(x, y, z));
    }

    //////////////////////////////////////////////

    Object& Object::setScale(const glm::vec3& scale)
    {
        m_locals.scale = scale;
        propagateFlags(MatrixDirty | InverseMatrixDirty | GlobalScaleDirty);

        return *this;
    }

    //////////////////////////////////////////////

    Object& Object::setScale(const float delta)
    {
        return setScale(delta, delta, delta);
    }

    //////////////////////////////////////////////

    const glm::vec3& Object::getLocalScale() const
    {
        return m_locals.scale;
    }

    //////////////////////////////////////////////

    const glm::vec3& Object::getGlobalScale() const
    {
        if (flagSet(GlobalScaleDirty))
        {
            if (m_parent.expired() || ignoresParent())
                m_globals.scale = getLocalScale();
            else
                m_globals.scale = m_parent->getGlobalScale() * getLocalScale();

            clearFlags(GlobalScaleDirty);
        }

        return m_globals.scale;
    }

    //////////////////////////////////////////////

    Object& Object::setPosition(const float x, const float y, const float z)
    {
        return setPosition(glm::vec3(x, y, z));
    }

    //////////////////////////////////////////////

    Object& Object::setPosition(const glm::vec3& position)
    {
        m_locals.position = position;
        propagateFlags(MatrixDirty | InverseMatrixDirty | GlobalPositionDirty);

        return *this;
    }

    //////////////////////////////////////////////

    const glm::vec3& Object::getLocalPosition() const
    {
        return m_locals.position;
    }

    //////////////////////////////////////////////

    const glm::vec3& Object::getGlobalPosition() const
    {
        if (flagSet(GlobalPositionDirty))
        {
            if (m_parent.expired() || ignoresParent())
                m_globals.position = getLocalPosition();
            else
                m_globals.position = m_parent->getGlobalPosition() + getLocalPosition();

            clearFlags(GlobalPositionDirty);
        }

        return m_globals.position;
    }

    //////////////////////////////////////////////

    Object& Object::lookAt(const glm::vec3& point)
    {
        return lookAt(point, Transform::Up);
    }

    //////////////////////////////////////////////

    Object& Object::lookAt(const float x, const float y, const float z)
    {
        return lookAt(glm::vec3(x, y, z));
    }

    //////////////////////////////////////////////

    Object& Object::lookAt(const glm::vec3& point, const glm::vec3& up)
    {
        return setRotation(glm::quat(glm::inverse(glm::lookAt(getGlobalPosition(), point, up))));
    }

    //////////////////////////////////////////////

    Object& Object::move(const float x, const float y, const float z)
    {
        return move(glm::vec3(x, y, z));
    }

    //////////////////////////////////////////////

    Object& Object::move(const glm::vec3& offset)
    {
        return setPosition(m_locals.position + offset);
    }

    //////////////////////////////////////////////

    Object& Object::rotate(const float x, const float y, const float z)
    {
        return rotate(glm::vec3(x, y, z));
    }

    //////////////////////////////////////////////

    Object& Object::rotate(const glm::quat& rotation)
    {
        return setRotation(m_locals.rotation * rotation);
    }

    //////////////////////////////////////////////

    Object& Object::rotate(const glm::vec3& rotation)
    {
        return rotate(glm::quat(rotation));
    }

    //////////////////////////////////////////////

    Object& Object::rotate(const float angle, const glm::vec3& axis)
    {
        return rotate(glm::angleAxis(angle, axis));
    }

    //////////////////////////////////////////////

    Object& Object::scale(const float x, const float y, const float z)
    {
        return scale(glm::vec3(x, y, z));
    }

    //////////////////////////////////////////////

    Object& Object::scale(const glm::vec3& scale)
    {
        return setScale(m_locals.scale * scale);
    }

    //////////////////////////////////////////////

    Object& Object::scale(const float delta)
    {
        return scale(glm::vec3(delta));
    }

    //////////////////////////////////////////////

    bool Object::flagSet(const uint16 flag) const
    {
        return (m_flags & flag) != 0;
    }

    void Object::setFlags(const uint16 flags) const
    {
        m_flags |= flags;
    }

    void Object::clearFlags(const uint16 flags) const
    {
        m_flags &= ~flags;
    }

    void Object::propagateFlags(const uint16 flags)
    {
        setFlags(flags);

        for (auto& i : m_children)
            i.propagateFlags(flags);
    }
}