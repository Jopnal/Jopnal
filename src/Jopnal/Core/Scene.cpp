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

        JOP_BIND_MEMBER_COMMAND(&Scene::setDeltaScale, "setDeltaScale");

    JOP_END_COMMAND_HANDLER(Scene)
}

namespace jop
{
    namespace detail
    {
        class CullingWorld : public World
        {
        public:

            CullingWorld(Object& obj, Renderer& renderer)
                : World(obj, renderer)
            {
                m_worldData->world->setGravity(btVector3(0.f, 0.f, 0.f));
                m_worldData->world->getPairCache()->setInternalGhostPairCallback(nullptr);
            }
        };
    }

    Scene::Scene(const std::string& ID)
        : Object            (ID),
          m_renderer        (std::make_unique<Renderer>(Engine::getMainRenderTarget())),
          m_worlds          (nullptr, nullptr),
          m_deltaScale      (1.f)
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

    Message::Result Scene::sendMessage(const Message& message)
    {
        if (message.passFilter(getID()))
        {
            if ((message.passFilter(Message::Scene) || (this == &Engine::getSharedScene() && message.passFilter(Message::SharedScene))))
            {
                if (receiveMessage(message) == Message::Result::Escape)
                    return Message::Result::Escape;
            }
        }

        const unsigned short objectField = Message::Object | Message::Component;

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
            Object::update(dt);
            postUpdate(dt);
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

    Message::Result Scene::receiveMessage(const Message& message)
    {
        return JOP_EXECUTE_COMMAND(Scene, message.getString(), this);
    }
}