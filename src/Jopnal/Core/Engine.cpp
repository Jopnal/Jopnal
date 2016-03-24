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

//////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_COMMAND_HANDLER(Engine)
    
        JOP_BIND_COMMAND(&Engine::removeSubsystem, "removeSubsystem");

    JOP_END_COMMAND_HANDLER(Engine)
}

namespace
{
    std::string ns_projectName;

    int ns_argc;
    char** ns_argv;
}

namespace jop
{
    Engine::Engine(const std::string& name, int argc, char* argv[])
        : m_sharedScene     (),
          m_totalTime       (0.0),
          m_subsystems      (),
          m_currentScene    (),
          m_exit            (false),
          m_state           (State::Running),
          m_deltaScale      (1.f),
          m_advanceFrame    (false)
    {
        JOP_ASSERT(m_engineObject == nullptr, "Only one jop::Engine object may exist at a time!");
        JOP_ASSERT(!name.empty(), "Project name mustn't be empty!");

        ns_projectName = name;
        m_engineObject = this;
        
        ns_argc = argc;
        ns_argv = argv;
    }

    Engine::~Engine()
    {
        m_currentScene.reset();
        m_sharedScene.reset();
        m_subsystems.clear();

        ns_projectName = std::string();
        m_engineObject = nullptr;
    }

    //////////////////////////////////////////////

    void Engine::loadDefaultConfiguration()
    {
        // Setting manager
        createSubsystem<SettingManager>();

        // File loader
        createSubsystem<FileLoader>(ns_argv[0]);

        // Resource manager
        createSubsystem<ResourceManager>();

        // Main window
        createSubsystem<Window>(Window::Settings(true));

        // Shader manager
        createSubsystem<ShaderManager>();

        // Shared scene
        m_sharedScene = std::make_unique<Scene>("sharedscene");

        JOP_DEBUG_INFO("Default engine configuration loaded");
    }

    //////////////////////////////////////////////

    int Engine::runMainLoop()
    {
        JOP_ASSERT(m_engineObject != nullptr, "Tried to run main loop without a valid jop::Engine instance!");

#pragma warning(suppress: 6011)
        auto& eng = *m_engineObject;

        if (!eng.m_currentScene)
            JOP_DEBUG_WARNING("No scene was loaded before entering main loop. Only the shared scene will be used.");

        Clock frameClock;

        while (!eng.m_exit)
        {
            float frameTime = static_cast<float>(frameClock.reset().asSeconds());
            eng.m_totalTime += frameTime;

            frameTime *= (eng.m_deltaScale * (getState() != State::ZeroDelta || eng.m_advanceFrame));

            // Update
            {
                for (auto& i : eng.m_subsystems)
                {
                    if (i->isActive())
                        i->preUpdate(frameTime);
                }

                if (getState() <= State::ZeroDelta || eng.m_advanceFrame)
                {
                    if (eng.m_currentScene)
                        eng.m_currentScene->updateBase(frameTime);

                    if (eng.m_sharedScene)
                        eng.m_sharedScene->updateBase(frameTime);
                }

                for (auto& i : eng.m_subsystems)
                {
                    if (i->isActive())
                        i->postUpdate(frameTime);
                }
            }

            eng.m_advanceFrame = false;

            // Draw
            {
                if (getState() != State::Frozen)
                {
                    if (eng.m_currentScene)
                        eng.m_currentScene->drawBase();

                    if (eng.m_sharedScene)
                        eng.m_sharedScene->drawBase();
                }

                for (auto& i : eng.m_subsystems)
                {
                    if (i->isActive())
                        i->draw();
                }
            }
        }

        // #TODO Threaded event loop

        return EXIT_SUCCESS;
    }

    //////////////////////////////////////////////

    Scene& Engine::getCurrentScene()
    {
        JOP_ASSERT(hasCurrentScene(), "Tried to get the current scene when it didn't exist!");
        return *m_engineObject->m_currentScene;
    }

    //////////////////////////////////////////////

    Subsystem* Engine::getSubsystem(const std::string& ID)
    {
        if (m_engineObject)
        {
            for (auto& i : m_engineObject->m_subsystems)
            {
                if (i->getID() == ID)
                    return i.get();
            }
        }

        return nullptr;
    }

    //////////////////////////////////////////////

    bool Engine::removeSubsystem(const std::string& ID)
    {
        if (m_engineObject)
        {
            for (auto itr = m_engineObject->m_subsystems.begin(); itr != m_engineObject->m_subsystems.end(); ++itr)
            {
                if ((*itr)->getID() == ID)
                {
                    JOP_DEBUG_INFO("Sub system with id \"" << (*itr)->getID() << "\" removed");
                    m_engineObject->m_subsystems.erase(itr);
                    return true;
                }
            }
        }

        return false;
    }

    //////////////////////////////////////////////

    bool Engine::exiting()
    {
        return (m_engineObject == nullptr ? true : m_engineObject->m_exit.load());
    }

    //////////////////////////////////////////////

    void Engine::exit()
    {
        if (m_engineObject)
        {
            JOP_DEBUG_INFO("Exit signal received, exiting...");
            m_engineObject->m_exit = true;
        }
    }

    //////////////////////////////////////////////

    void Engine::setState(const State state)
    {
        if (!exiting() && getState() != state)
        {
        #if JOP_CONSOLE_VERBOSITY >= 2
            static const char* const stateStr[] =
            {
                "Running",
                "ZeroDelta",
                "RenderOnly",
                "Frozen"
            };
        #endif

            JOP_DEBUG_INFO("Engine state changed: " << stateStr[static_cast<int>(state)]);

            m_engineObject->m_state = state;
        }
    }

    //////////////////////////////////////////////

    Engine::State Engine::getState()
    {
        return exiting() ? State::Frozen : m_engineObject->m_state;
    }

    //////////////////////////////////////////////

    Message::Result Engine::sendMessage(const std::string& message)
    {
        Any wrap;
        return sendMessage(message, wrap);
    }

    //////////////////////////////////////////////

    Message::Result Engine::sendMessage(const std::string& message, Any& returnWrap)
    {
        const Message msg(message, returnWrap);
        return sendMessage(msg);
    }

    //////////////////////////////////////////////

    Message::Result Engine::sendMessage(const Message& message)
    {
        if (m_engineObject)
        {
            if (message.passFilter(Message::Engine) && message.passFilter(Message::Command))
            {
                Any engPtr(m_engineObject);
                if (JOP_EXECUTE_COMMAND(Engine, message.getString(), engPtr, message.getReturnWrapper()) == Message::Result::Escape)
                    return Message::Result::Escape;
            }

            static const unsigned short sceneField = Message::SharedScene   |
                                                     Message::Scene         |
                                                     Message::Object        |
                                                     Message::Component;

            if (message.passFilter(sceneField) && m_engineObject->m_sharedScene->sendMessage(message) == Message::Result::Escape)
                return Message::Result::Escape;

            auto& s = m_engineObject->m_currentScene;

            if (s && message.passFilter(sceneField) && s->sendMessage(message) == Message::Result::Escape)
                return Message::Result::Escape;

            if (message.passFilter(Message::Subsystem))
            {
                for (auto& i : m_engineObject->m_subsystems)
                {
                    if (i->sendMessage(message) == Message::Result::Escape)
                        return Message::Result::Escape;
                }
            }
        }

        return Message::Result::Continue;
    }

    //////////////////////////////////////////////

    Scene& Engine::getSharedScene()
    {
        JOP_ASSERT(hasSharedScene(), "Tried to get the shared scene when it didn't exist!");
        return *m_engineObject->m_sharedScene;
    }

    //////////////////////////////////////////////

    bool Engine::hasSharedScene()
    {
        JOP_ASSERT(!exiting(), "There must be a valid jop::Engine object in order to call jop::Engine::has/getSharedScene()!");
        return m_engineObject->m_sharedScene.operator bool();
    }

    //////////////////////////////////////////////

    double Engine::getTotalTime()
    {
        if (m_engineObject)
            return m_engineObject->m_totalTime;

        return 0.0;
    }

    //////////////////////////////////////////////

    bool Engine::hasCurrentScene()
    {
        JOP_ASSERT(!exiting(), "There must be a valid jop::Engine object in order to call jop::Engine::has/getCurrentScene()!");
        return m_engineObject->m_currentScene.operator bool();
    }

    //////////////////////////////////////////////

    void Engine::advanceFrame()
    {
        if (!exiting())
            m_engineObject->m_advanceFrame = true;
    }

    //////////////////////////////////////////////

    void Engine::setDeltaScale(const float scale)
    {
        if (m_engineObject)
            m_engineObject->m_deltaScale = scale;
    }

    //////////////////////////////////////////////

    float Engine::getDeltaScale()
    {
        return m_engineObject != nullptr ? m_engineObject->m_deltaScale : 1.f;
    }

    //////////////////////////////////////////////

    Engine* Engine::m_engineObject = nullptr;

    //////////////////////////////////////////////

    const std::string& getProjectName()
    {
        return ns_projectName;
    }

    //////////////////////////////////////////////

    Message::Result broadcast(const std::string& message)
    {
        return Engine::sendMessage(message);
    }

    Message::Result broadcast(const std::string& message, Any& returnWrap)
    {
        const Message msg(message, returnWrap);
        return Engine::sendMessage(msg);
    }

    Message::Result broadcast(const Message& message)
    {
        return Engine::sendMessage(message);
    }
}