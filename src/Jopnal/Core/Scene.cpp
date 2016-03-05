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
        JOP_BIND_MEMBER_COMMAND(&Scene::setActive, "setActive");
        JOP_BIND_MEMBER_COMMAND(&Scene::setID, "setID");

    JOP_END_COMMAND_HANDLER(Scene)

    JOP_REGISTER_LOADABLE(jop, Scene) [](std::unique_ptr<Scene>& scene, const json::Value& val) -> bool
    {
        const char* id = val.HasMember("id") && val["id"].IsString() ? val["id"].GetString() : ""; 
        const bool active = val.HasMember("active") && val["active"].IsBool() ? val["active"].GetBool() : true;

        scene = std::make_unique<Scene>(id);
        scene->setActive(active);

        return true;
    }
    JOP_END_LOADABLE_REGISTRATION(Scene)

    JOP_REGISTER_SAVEABLE(jop, Scene) [](const Scene& scene, json::Value& obj, json::Value::AllocatorType& alloc) -> bool
    {
        obj.AddMember(json::StringRef("id"), json::StringRef(scene.getID().c_str()), alloc)
           .AddMember(json::StringRef("active"), scene.isActive(), alloc);

        return true;
    }
    JOP_END_SAVEABLE_REGISTRATION(Scene)
}

namespace jop
{
    Scene::Scene(const std::string& ID)
        : Activateable      (true),
          m_objects         (),
          m_layers          (),
          m_ID              (ID)
    {}

    Scene::~Scene()
    {}

    //////////////////////////////////////////////

    WeakReference<Object> Scene::getObject(const std::string& ID)
    {
        for (auto& i : m_objects)
        {
            if (i.getID() == ID)
                return i.getReference();
        }

        return WeakReference<Object>();
    }

    //////////////////////////////////////////////

    WeakReference<Object> Scene::createObject(const std::string& ID)
    {
        m_objects.emplace_back(ID);
        return m_objects.back().getReference();
    }

    //////////////////////////////////////////////

    WeakReference<Object> Scene::cloneObject(const std::string& ID, const std::string& clonedID)
    {
        auto ptr = getObject(ID);

        if (!ptr.expired())
        {
            m_objects.push_back(Object(*ptr, clonedID));
            return m_objects.back().getReference();
        }

        return WeakReference<Object>();
    }

    //////////////////////////////////////////////

    void Scene::deleteObject(const std::string& ID)
    {
        for (auto itr = m_objects.begin(); itr != m_objects.end(); ++itr)
        {
            if (itr->getID() == ID)
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

    WeakReference<Layer> Scene::getLayer(const std::string& ID) const
    {
        for (auto& i : m_layers)
        {
            if (i->getID() == ID)
                return static_ref_cast<Layer>(i->getReference());
        }

        return WeakReference<Layer>();
    }

    //////////////////////////////////////////////

    void Scene::deleteLayer(const std::string& ID)
    {
        for (auto itr = m_layers.begin() + 1; itr != m_layers.end(); ++itr)
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
        if (!m_layers.empty())
            m_layers.erase(m_layers.begin() + 1, m_layers.end());
    }

    //////////////////////////////////////////////

    WeakReference<Layer> Scene::getDefaultLayer() const
    {
        if (m_layers.empty())
            m_layers.emplace_back(std::make_unique<Layer>("jop_default_layer"));

        return static_ref_cast<Layer>(m_layers.front()->getReference());
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

    Message::Result Scene::sendMessage(const std::string& message)
    {
        Any wrap;
        return sendMessage(message, wrap);
    }

    //////////////////////////////////////////////

    Message::Result Scene::sendMessage(const std::string& message, Any& returnWrap)
    {
        const Message msg(message, returnWrap);
        return sendMessage(msg);
    }

    //////////////////////////////////////////////

    Message::Result Scene::sendMessage(const Message& message)
    {
        if (message.passFilter(getID()))
        {
            if ((message.passFilter(Message::Scene) || (this == &Engine::getSharedScene() && message.passFilter(Message::SharedScene)) && message.passFilter(Message::Command)))
            {
                Any instance(this);
                if (JOP_EXECUTE_COMMAND(Scene, message.getString(), instance, message.getReturnWrapper()) == Message::Result::Escape)
                    return Message::Result::Escape;
            }

            if (message.passFilter(Message::Custom) && sendMessageImpl(message) == Message::Result::Escape)
                return Message::Result::Escape;
        }

        static const unsigned short objectField = Message::Object |
                                                  Message::Component;

        if (message.passFilter(objectField))
        {
            for (auto& i : m_objects)
            {
                if (i.sendMessage(message) == Message::Result::Escape)
                    return Message::Result::Escape;
            }
        }

        if (message.passFilter(Message::Layer))
        {
            for (auto& i : m_layers)
            {
                if (i->sendMessage(message) == Message::Result::Escape)
                    return Message::Result::Escape;
            }
        }

        return Message::Result::Continue;
    }

    //////////////////////////////////////////////

    void Scene::updateBase(const float deltaTime)
    {
        if (isActive())
        {
            preUpdate(deltaTime);

            for (auto& i : m_layers)
                i->preUpdate(deltaTime);

            for (auto& i : m_objects)
            {
                i.update(deltaTime);
                i.updateTransformTree(nullptr, false);
            }

            for (auto& i : m_layers)
                i->postUpdate(deltaTime);

            postUpdate(deltaTime);
        }
    }

    //////////////////////////////////////////////

    void Scene::updateTransformTree()
    {
        for (auto& i : m_objects)
            i.updateTransformTree(nullptr, false);
    }

    //////////////////////////////////////////////

    void Scene::fixedUpdateBase(const float timeStep)
    {
        if (isActive())
        {
            preFixedUpdate(timeStep);

            for (auto& i : m_layers)
                i->preFixedUpdate(timeStep);

            for (auto& i : m_objects)
                i.fixedUpdate(timeStep);

            for (auto& i : m_layers)
                i->postFixedUpdate(timeStep);

            postFixedUpdate(timeStep);
        }
    }

    //////////////////////////////////////////////

    void Scene::drawBase()
    {
        if (isActive())
        {
            preDraw();

            for (auto& i : m_layers)
                i->drawBase();

            postDraw();
        }
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

    Message::Result Scene::sendMessageImpl(const Message&)
    {
        return Message::Result::Continue;
    }
}