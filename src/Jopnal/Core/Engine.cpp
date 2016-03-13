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
        JOP_BIND_COMMAND(&Engine::setPaused, "setPaused");
        JOP_BIND_COMMAND(&Engine::exit, "exit");

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
          m_running         (true),
          m_paused          (false)
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
        m_sharedScene = std::make_unique<Scene>("SharedScene");

        JOP_DEBUG_INFO("Default engine configuration loaded");
    }

    //////////////////////////////////////////////

    int Engine::runMainLoop()
    {
        JOP_ASSERT(m_engineObject != nullptr, "Tried to run main loop without a valid jop::Engine instance!");

        auto& eng = *m_engineObject;

        if (!eng.m_currentScene)
            JOP_DEBUG_WARNING("No scene was loaded before entering main loop. Only the shared scene will be used.");

        Clock frameClock;

        while (eng.m_running)
        {
            // Clamp the delta time to a certain value. This is to prevent
            // a "spiral of death" if fps goes below 5.
            const float frameTime = static_cast<float>(std::min(0.2, frameClock.reset().asSeconds()));
            eng.m_totalTime += frameTime;

            // Update
            {
                for (auto& i : eng.m_subsystems)
                {
                    if (i->isActive())
                        i->preUpdate(frameTime);
                }

                if (!isPaused())
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

            // Draw
            {
                if (eng.m_currentScene)
                    eng.m_currentScene->drawBase();

                if (eng.m_sharedScene)
                    eng.m_sharedScene->drawBase();

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
        JOP_ASSERT(m_engineObject != nullptr && m_engineObject->m_currentScene, "Tried to get the current scene when it or the engine wasn't loaded!");
        return *m_engineObject->m_currentScene;
    }

    //////////////////////////////////////////////

    WeakReference<Subsystem> Engine::getSubsystem(const std::string& ID)
    {
        if (m_engineObject)
        {
            for (auto& i : m_engineObject->m_subsystems)
            {
                if (i->getID() == ID)
                    return i->getReference();
            }
        }

        return WeakReference<Subsystem>();
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

    bool Engine::isRunning()
    {
        return m_engineObject && m_engineObject->m_running;
    }

    //////////////////////////////////////////////

    void Engine::exit()
    {
        if (m_engineObject)
        {
            JOP_DEBUG_INFO("Exit signal received, exiting...");
            m_engineObject->m_running = false;
        }
    }

    //////////////////////////////////////////////

    void Engine::setPaused(const bool paused)
    {
        if (isRunning())
        {
            JOP_DEBUG_INFO("Engine " << (paused ? "paused" : "unpaused"));
            m_engineObject->m_paused = paused;
        }
    }

    //////////////////////////////////////////////

    bool Engine::isPaused()
    {
        return isRunning() ? m_engineObject->m_paused : false;
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
        JOP_ASSERT(isRunning(), "There must be a valid jop::Engine object in order to call jop::Engine::getSharedScene()!");
        JOP_ASSERT(m_engineObject->m_sharedScene.operator bool(), "Tried to get the shared scene when it didn't exist!");

        return *m_engineObject->m_sharedScene;
    }

    //////////////////////////////////////////////

    bool Engine::hasSharedScene()
    {
        JOP_ASSERT(isRunning(), "There must be a valid jop::Engine object in order to call jop::Engine::hasSharedScene()!");
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
        if (m_engineObject)
            return m_engineObject->m_currentScene.operator bool();

        return false;
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