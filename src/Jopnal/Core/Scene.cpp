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
    #pragma warning(push)
    #pragma warning(disable: 4189)
    JOP_DERIVED_COMMAND_HANDLER(Object, Scene)
    JOP_END_COMMAND_HANDLER(Scene)
    #pragma warning(pop)

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
        : Object        (ID),
          m_renderer    (std::make_unique<Renderer>(*Engine::getSubsystem<Window>())),
          m_world       (createComponent<World>(*m_renderer)),
          m_deltaScale  (1.f)
    {}

    Scene::Scene(const std::string& ID, const RenderTarget& mainTarget)
        : Object        (ID),
          m_renderer    (std::make_unique<Renderer>(mainTarget)),
          m_world       (createComponent<World>(*m_renderer)),
          m_deltaScale  (1.f)
    {}

    Scene::~Scene()
    {
        // Child objects need to be deinitialized before the renderer
        Object::clearChildren();
        Object::clearComponents();
    }

    //////////////////////////////////////////////

    Renderer& Scene::getRenderer() const
    {
        return *m_renderer;
    }

    //////////////////////////////////////////////

    World& Scene::getWorld() const
    {
        return m_world;
    }

    //////////////////////////////////////////////

    void Scene::setDeltaScale(const float scale)
    {
        m_deltaScale = scale;
    }

    //////////////////////////////////////////////

    float Scene::getDeltaScale() const
    {
        return m_deltaScale;
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
                if (JOP_EXECUTE_COMMAND(Object, message.getString(), instance, message.getReturnWrapper()) == Message::Result::Escape)
                    return Message::Result::Escape;
            }

            if (message.passFilter(Message::Custom) && sendMessageImpl(message) == Message::Result::Escape)
                return Message::Result::Escape;
        }

        static const unsigned short objectField = Message::Object |
                                                  Message::Component;

        if (message.passFilter(objectField) && Object::sendMessage(message) == Message::Result::Escape)
            return Message::Result::Escape;

        return Message::Result::Continue;
    }

    //////////////////////////////////////////////

    void Scene::updateBase(const float deltaTime)
    {
        if (isActive())
        {
            const float dt = deltaTime * m_deltaScale;

            preUpdate(dt);

            // Need to update transforms twice to ensure correct global state at all times.
            // TODO: Redesign the transformation system to eliminate this requirement.
            Object::updateTransformTree(nullptr, false);
            Object::update(dt);

            postUpdate(dt);
            Object::updateTransformTree(nullptr, false);
        }
    }

    //////////////////////////////////////////////

    void Scene::drawBase()
    {
        if (isActive())
        {
            preDraw();

            m_renderer->draw();

            postDraw();
        }
    }

    //////////////////////////////////////////////

    void Scene::preUpdate(const float)
    {}

    //////////////////////////////////////////////

    void Scene::postUpdate(const float)
    {}

    //////////////////////////////////////////////

    void Scene::preDraw()
    {}

    //////////////////////////////////////////////

    void Scene::postDraw()
    {}

    //////////////////////////////////////////////

    Object& Scene::getAsObject()
    {
        return *this;
    }

    //////////////////////////////////////////////

    const Object& Scene::getAsObject() const
    {
        return *this;
    }

    //////////////////////////////////////////////

    Message::Result Scene::sendMessageImpl(const Message&)
    {
        return Message::Result::Continue;
    }
}