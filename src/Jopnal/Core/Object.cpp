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
#include <Jopnal\Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
    Object::Object()
        : m_components  (),
          m_ID          ()
    {}

    Object::Object(const std::string& ID)
        : m_components  (),
          m_ID          (ID)
    {}
    
    Object::~Object()
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

    void Object::update(const double deltaTime)
    {
        for (auto& i : m_components)
            i->update(deltaTime);
    } 

    /////////////////////////////////////////////

    void Object::removeComponents(const std::string& ID)
    {
        m_components.erase(std::remove_if(m_components.begin(), m_components.end(), [&ID](const std::unique_ptr<Component>& comp)
        {
            return comp->getID() == ID;

        }), m_components.end());
    }

    /////////////////////////////////////////////

    void Object::sendMessage(const std::string& message, void* ptr)
    {
        for (auto& i: m_components)
            i->sendMessage(message, ptr);
    }

    /////////////////////////////////////////////

    void Object::fixedUpdate(const double timeStep)
    {
        for (auto& i : m_components)
            i->fixedUpdate(timeStep);
    }

    /////////////////////////////////////////////

    void Object::draw()
    {
        for (auto& i : m_components)
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
}