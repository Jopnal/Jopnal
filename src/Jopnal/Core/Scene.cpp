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

/////////////////////////////////////////////


namespace jop
{
    Scene::Scene(const std::string& ID)
        : m_objects (),
          m_ID      (ID)
    {}

    Scene::~Scene()
    {}

    //////////////////////////////////////////////

    Object* Scene::getObject(const std::string& ID)
    {
        for (auto& i : m_objects)
        {
            if (i->getID() == ID)
                return i.get();
        }

        return nullptr;
    }

    //////////////////////////////////////////////

    Object& Scene::createObject(const std::string& ID)
    {
        m_objects.emplace_back(std::make_unique<Object>(ID));
        return *m_objects.back();
    }

    //////////////////////////////////////////////

    Object* Scene::cloneObject(const std::string& ID)
    {
        auto ptr = getObject(ID);

        if (ptr)
        {
            m_objects.emplace_back(std::make_unique<Object>(*ptr));
            return m_objects.back().get();
        }

        return nullptr;
    }

    //////////////////////////////////////////////

    void Scene::deleteObject(const std::string& ID)
    {
        for (auto itr = m_objects.begin(); itr != m_objects.end(); ++itr)
        {
            if ((*itr)->getID() == ID)
                m_objects.erase(itr);
        }
    }

    //////////////////////////////////////////////

    void Scene::clearObjects()
    {
        m_objects.clear();
    }

    //////////////////////////////////////////////

    void Scene::setID(const std::string& ID)
    {
        m_ID = ID;
    }

    //////////////////////////////////////////////

    const std::string& Scene::getID() const
    {
        return m_ID;
    }

    //////////////////////////////////////////////

    void Scene::sendMessage(const std::string& message, void* ptr)
    {
        sendMessageImpl(message, ptr);

        for (auto& i : m_objects)
            i->sendMessage(message, ptr);
    }

    //////////////////////////////////////////////

    void Scene::updateBase(const double deltaTime)
    {
        preUpdate(deltaTime);

        for (auto& i : m_objects)
        {
            i->update(deltaTime);
            i->updateTransformTree(nullptr, false);
        }

        postUpdate(deltaTime);
    }

    //////////////////////////////////////////////

    void Scene::fixedUpdateBase(const double timeStep)
    {
        preFixedUpdate(timeStep);

        for (auto& i : m_objects)
            i->fixedUpdate(timeStep);

        postFixedUpdate(timeStep);
    }

    //////////////////////////////////////////////

    void Scene::drawBase()
    {
        preDraw();

        for (auto& i : m_objects)
            i->draw();

        postDraw();
    }

    //////////////////////////////////////////////

    void Scene::preUpdate(const double)
    {}

    //////////////////////////////////////////////

    void Scene::postUpdate(const double)
    {}

    //////////////////////////////////////////////

    void Scene::preFixedUpdate(const double)
    {}

    //////////////////////////////////////////////

    void Scene::postFixedUpdate(const double)
    {}

    //////////////////////////////////////////////

    void Scene::preDraw()
    {}

    //////////////////////////////////////////////

    void Scene::postDraw()
    {}

    //////////////////////////////////////////////

    void Scene::sendMessageImpl(const std::string&, void*)
    {}
}