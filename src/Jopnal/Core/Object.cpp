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
    Object::Object()
        : Transform     (),
          m_children    (),
          m_components  (),
          m_ID          ()
    {}

    Object::Object(const Object& other)
        : Transform     (other),
          m_children    (other.m_children),
          m_components  (),
          m_ID          (other.m_ID)
    {
        m_components.reserve(other.m_components.size());
        for (auto& i : other.m_components)
            m_components.emplace_back(std::unique_ptr<Component>(i->clone()));
    }

    Object::Object(const std::string& ID)
        : Transform     (),
          m_children    (),
          m_components  (),
          m_ID          (ID)
    {}

    //////////////////////////////////////////////

    bool Object::hasComponent(const std::string& ID) const
    {
        for (auto& i : m_components)
        {
            if (i->getID() == ID)
                return true;
        }

        return false;
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

    Object& Object::createChild(const std::string& ID)
    {
        m_children.emplace_back(std::make_unique<Object>(ID));
        return *m_children.back();
    }

    //////////////////////////////////////////////

    Object* Object::getChild(const std::string& ID)
    {
        for (auto& i : m_children)
        {
            if (i->getID() == ID)
                return i.get();
        }

        return nullptr;
    }

    //////////////////////////////////////////////

    Object* Object::cloneChild(const std::string& ID)
    {
        auto ptr = getChild(ID);

        if (ptr)
        {
            m_children.emplace_back(std::make_unique<Object>(*ptr));
            return m_children.back().get();
        }

        return nullptr;
    }

    //////////////////////////////////////////////

    void Object::removeChildren(const std::string& ID)
    {
        m_children.erase(std::remove_if(m_children.begin(), m_children.end(), [&ID](const std::unique_ptr<Object>& obj)
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

    void Object::sendMessage(const std::string& message, void* ptr)
    {
        for (auto& i : m_components)
            i->sendMessage(message, ptr);

        for (auto& i : m_children)
            i->sendMessage(message, ptr);
    }

    /////////////////////////////////////////////

    void Object::update(const double deltaTime)
    {
        for (auto& i : m_components)
            i->update(deltaTime);

        for (auto& i : m_children)
            i->update(deltaTime);
    }

    /////////////////////////////////////////////

    void Object::fixedUpdate(const double timeStep)
    {
        for (auto& i : m_components)
            i->fixedUpdate(timeStep);

        for (auto& i : m_children)
            i->fixedUpdate(timeStep);
    }

    /////////////////////////////////////////////

    void Object::draw()
    {
        for (auto& i : m_components)
            i->draw();

        for (auto& i : m_children)
            i->draw();
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
            i->updateTransformTree(this, parentUpdated);
    }
}