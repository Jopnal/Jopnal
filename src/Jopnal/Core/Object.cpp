//Jopnal Engine C++ Library
//Copyright(c) 2016 Team Jopnal
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

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
        : Transform                             (),
          std::enable_shared_from_this<Object>  (),
          m_children                            (),
          m_components                          (),
          m_ID                                  (),
          m_active                              (true)
    {}

    Object::Object(const Object& other)
        : Transform                             (other),
          std::enable_shared_from_this<Object>  (other),
          m_children                            (),
          m_components                          (),
          m_ID                                  (other.m_ID),
          m_active                              (other.m_active)
    {
        m_components.reserve(other.m_components.size());
        for (auto& i : other.m_components)
            m_components.emplace_back(std::shared_ptr<Component>(i->clone()));

        m_children.reserve(other.m_children.size());
        for (auto& i : other.m_children)
            m_children.emplace_back(std::make_shared<Object>(*i));
    }

    Object::Object(const std::string& ID)
        : Transform                             (),
          std::enable_shared_from_this<Object>  (),
          m_children                            (),
          m_components                          (),
          m_ID                                  (ID),
          m_active                              (true)
    {}

    //////////////////////////////////////////////

    std::weak_ptr<Component> Object::getComponent(const std::string& ID)
    {
        for (auto& i : m_components)
        {
            if (i->getID() == ID)
                return std::weak_ptr<Component>(i);
        }

        return std::weak_ptr<Component>();
    }

    /////////////////////////////////////////////

    void Object::removeComponents(const std::string& ID)
    {
        m_components.erase(std::remove_if(m_components.begin(), m_components.end(), [&ID](const std::shared_ptr<Component>& comp)
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

    Object& Object::createChild(const std::string& ID)
    {
        m_children.emplace_back(std::make_unique<Object>(ID));
        return *m_children.back();
    }

    //////////////////////////////////////////////

    std::weak_ptr<Object> Object::getChild(const std::string& ID)
    {
        for (auto& i : m_children)
        {
            if (i->getID() == ID)
                return std::weak_ptr<Object>(i);
        }

        return std::weak_ptr<Object>();
    }

    //////////////////////////////////////////////

    std::weak_ptr<Object> Object::cloneChild(const std::string& ID)
    {
        auto ptr = getChild(ID);

        if (!ptr.expired())
        {
            m_children.emplace_back(std::make_shared<Object>(*ptr.lock()));
            return std::weak_ptr<Object>(m_children.back());
        }

        return std::weak_ptr<Object>();
    }

    //////////////////////////////////////////////

    void Object::removeChildren(const std::string& ID)
    {
        m_children.erase(std::remove_if(m_children.begin(), m_children.end(), [&ID](const std::shared_ptr<Object>& obj)
        {
            return obj->getID() == ID;

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
        {
            count += i->childCountRecursive();
        }
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
                if (i->sendMessage(message) == Message::Result::Escape)
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
                i->update(deltaTime);
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
                i->fixedUpdate(timeStep);
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

    void Object::setActive(const bool active)
    {
        m_active = active;
    }

    //////////////////////////////////////////////

    bool Object::isActive() const
    {
        return m_active;
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
            i->updateTransformTree(this, parentUpdated);
    }
}