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

/////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_COMMAND_HANDLER(Scene)

        JOP_BIND_MEMBER_COMMAND(&Scene::cloneObject, "cloneObject");
        JOP_BIND_MEMBER_COMMAND(&Scene::deleteObject, "deleteObject");
        JOP_BIND_MEMBER_COMMAND(&Scene::clearObjects, "clearObjects");
        JOP_BIND_MEMBER_COMMAND(&Scene::deleteLayer, "deleteLayer");
        JOP_BIND_MEMBER_COMMAND(&Scene::clearLayers, "clearLayers");
        JOP_BIND_MEMBER_COMMAND(&Scene::setID, "setID");

    JOP_END_COMMAND_HANDLER(Scene)
}

namespace jop
{
    Scene::Scene(const std::string& ID)
        : m_objects         (),
          m_layers          (),
          m_defaultLayer    (std::make_shared<Layer>("DefaultLayer")),
          m_ID              (ID)
    {}

    Scene::~Scene()
    {}

    //////////////////////////////////////////////

    std::weak_ptr<Object> Scene::getObject(const std::string& ID)
    {
        for (auto& i : m_objects)
        {
            if (i->getID() == ID)
                return std::weak_ptr<Object>(i);
        }

        return std::weak_ptr<Object>();
    }

    //////////////////////////////////////////////

    Object& Scene::createObject(const std::string& ID)
    {
        m_objects.emplace_back(std::make_unique<Object>(ID));
        return *m_objects.back();
    }

    //////////////////////////////////////////////

    std::weak_ptr<Object> Scene::cloneObject(const std::string& ID)
    {
        auto ptr = getObject(ID);

        if (!ptr.expired())
        {
            m_objects.emplace_back(std::make_unique<Object>(*ptr.lock()));
            return std::weak_ptr<Object>(m_objects.back());
        }

        return std::weak_ptr<Object>();
    }

    //////////////////////////////////////////////

    void Scene::deleteObject(const std::string& ID)
    {
        for (auto itr = m_objects.begin(); itr != m_objects.end(); ++itr)
        {
            if ((*itr)->getID() == ID)
            {
                m_objects.erase(itr);
                return;
            }
        }
    }

    //////////////////////////////////////////////

    void Scene::clearObjects()
    {
        m_objects.clear();
    }

    //////////////////////////////////////////////

    unsigned int Scene::objectCount() const
    {
        return m_objects.size();
    }

    //////////////////////////////////////////////

    std::weak_ptr<Layer> Scene::getLayer(const std::string& ID)
    {
        for (auto& i : m_layers)
        {
            if (i->getID() == ID)
                return std::weak_ptr<Layer>(i);
        }

        return std::weak_ptr<Layer>();
    }

    //////////////////////////////////////////////

    void Scene::deleteLayer(const std::string& ID)
    {
        for (auto itr = m_layers.begin(); itr != m_layers.end(); ++itr)
        {
            if ((*itr)->getID() == ID)
            {
                m_layers.erase(itr);
                return;
            }
        }
    }

    //////////////////////////////////////////////

    void Scene::clearLayers()
    {
        m_layers.clear();
    }

    //////////////////////////////////////////////

    Layer& Scene::getDefaultLayer()
    {
        return *m_defaultLayer;
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

    MessageResult Scene::sendMessage(const std::string& message)
    {
        Any wrap;
        return sendMessage(message, wrap);
    }

    //////////////////////////////////////////////

    MessageResult Scene::sendMessage(const std::string& message, Any& returnWrap)
    {
        const Message msg(message, returnWrap);
        return sendMessage(msg);
    }

    //////////////////////////////////////////////

    MessageResult Scene::sendMessage(const Message& message)
    {
        if (message.passFilter(getID()))
        {
            if ((message.passFilter(Message::Scene) || (this == &Engine::getSharedScene() && message.passFilter(Message::SharedScene)) && message.passFilter(Message::Command)))
            {
                Any instance(this);
                JOP_EXECUTE_COMMAND(Scene, message.getString(), instance, message.getReturnWrapper());
            }

            if (message.passFilter(Message::Custom) && sendMessageImpl(message) == MessageResult::Escape)
                return MessageResult::Escape;
        }

        static const unsigned short objectField = Message::Object |
                                                  Message::Component;

        if (message.passFilter(objectField))
        {
            for (auto& i : m_objects)
            {
                if (i->sendMessage(message) == MessageResult::Escape)
                    return MessageResult::Escape;
            }
        }

        if (message.passFilter(Message::Layer))
        {
            for (auto& i : m_layers)
            {
                if (i->sendMessage(message) == MessageResult::Escape)
                    return MessageResult::Escape;
            }
        }

        return MessageResult::Continue;
    }
    //////////////////////////////////////////////

    void Scene::setActive(const bool active)
    {
        m_active = active;
    }
    //////////////////////////////////////////////

    bool Scene::isActive()
    {
        return m_active;
    }

    //////////////////////////////////////////////

    void Scene::updateBase(const float deltaTime)
    {
        preUpdate(deltaTime);

        for (auto& i : m_layers)
            i->preUpdate(deltaTime);

        for (auto& i : m_objects)
        {
            i->update(deltaTime);
            i->updateTransformTree(nullptr, false);
        }

        for (auto& i : m_layers)
            i->postUpdate(deltaTime);

        postUpdate(deltaTime);
    }

    //////////////////////////////////////////////

    void Scene::fixedUpdateBase(const float timeStep)
    {
        preFixedUpdate(timeStep);

        for (auto& i : m_layers)
            i->preFixedUpdate(timeStep);

        for (auto& i : m_objects)
            i->fixedUpdate(timeStep);

        for (auto& i : m_layers)
            i->postFixedUpdate(timeStep);

        postFixedUpdate(timeStep);
    }

    //////////////////////////////////////////////

    void Scene::drawBase()
    {
        preDraw();

        m_defaultLayer->drawBase();

        for (auto& i : m_layers)
            i->drawBase();

        postDraw();
    }

    //////////////////////////////////////////////

    void Scene::initialize()
    {}

    //////////////////////////////////////////////

    void Scene::preUpdate(const float)
    {}

    //////////////////////////////////////////////

    void Scene::postUpdate(const float)
    {}

    //////////////////////////////////////////////

    void Scene::preFixedUpdate(const float)
    {}

    //////////////////////////////////////////////

    void Scene::postFixedUpdate(const float)
    {}

    //////////////////////////////////////////////

    void Scene::preDraw()
    {}

    //////////////////////////////////////////////

    void Scene::postDraw()
    {}

    //////////////////////////////////////////////

    MessageResult Scene::sendMessageImpl(const Message&)
    {
        return MessageResult::Continue;
    }
}