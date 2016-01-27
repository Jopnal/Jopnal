// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//////////////////////////////////////////////

// Headers
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
    Component::Component(const Component& other)
        : std::enable_shared_from_this<Component>   (other),
          m_objectRef                               (other.m_objectRef),
          m_ID                                      (other.m_ID)
    {}

    Component::Component(Object& object, const std::string& ID)
        : std::enable_shared_from_this<Component>   (),
          m_objectRef                               (object),
          m_ID                                      (ID)
    {}

    Component::~Component()
    {}

    //////////////////////////////////////////////

    void Component::sendMessage(const std::string&, void*)
    {}

    //////////////////////////////////////////////

    void Component::update(double)
    {}

    //////////////////////////////////////////////

    void Component::fixedUpdate(const double)
    {}

    //////////////////////////////////////////////

    const std::string& Component::getID() const
    {
        return m_ID;
    }

    //////////////////////////////////////////////

    void Component::setID(const std::string& ID)
    {
        m_ID = ID;
    }

    //////////////////////////////////////////////

    Object& Component::getObject()
    {
        return m_objectRef;
    }

    //////////////////////////////////////////////

    const Object& Component::getObject() const
    {
        return m_objectRef;
    }
}