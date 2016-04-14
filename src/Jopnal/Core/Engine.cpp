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
        JOP_BIND_COMMAND(&Engine::exit, "exit");
        JOP_BIND_COMMAND(&Engine::setState, "setState");
        JOP_BIND_COMMAND(&Engine::advanceFrame, "advanceFrame");
        JOP_BIND_COMMAND(&Engine::setDeltaScale, "setDeltaScale");

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

    #if JOP_CONSOLE_VERBOSITY >= 2
        std::cout << "Jopnal Engine v. " << JOP_VERSION_STRING << "\n\n";
    #endif
    }

    Engine::~Engine()
    {
        m_currentScene.reset();
        m_sharedScene.reset();

        // Sub systems need to be cleared in the reverse creation order. This is not
        // guaranteed to happen by the standard.
        while (!m_subsystems.empty())
        {
        #if JOP_CONSOLE_VERBOSITY >= 2
            std::cout << "Deleting sub system " << (m_subsystems.end() - 1)->get()->getID() << "\n\n";
        #endif

            m_subsystems.erase(m_subsystems.end() - 1);
        }

        ns_projectName = std::string();
        m_engineObject = nullptr;

    #if JOP_CONSOLE_VERBOSITY >= 2
        std::cout << "Destroying jop::Engine..." << "\n\n";
    #endif
    }

    //////////////////////////////////////////////

    void Engine::loadDefaultConfiguration()
    {
        // Set process priority
    #ifdef JOP_OS_WINDOWS
        SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
    #endif

        // File system
        createSubsystem<FileSystemInitializer>(ns_argv[0]);

        // Setting manager
        createSubsystem<SettingManager>();

        // Main window
        createSubsystem<Window>(Window::Settings(true));

        // Resource manager
        createSubsystem<ResourceManager>();

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
            if (eng.m_newScene.load() && eng.m_newSceneSignal.load())
            {
                eng.m_currentScene.reset(eng.m_newScene.load());

                eng.m_newSceneSignal.store(false);
                eng.m_newScene.store(nullptr);
            }

            float frameTime = static_cast<float>(frameClock.reset().asSeconds());
            eng.m_totalTime.store(eng.m_totalTime.load() + static_cast<double>(frameTime));

            frameTime *= (getDeltaScale() * (getState() != State::ZeroDelta || eng.m_advanceFrame.load()));
            frameTime = std::min(0.1f, frameTime);

            // Update
            {
                for (auto& i : eng.m_subsystems)
                {
                    if (i->isActive())
                        i->preUpdate(frameTime);
                }

                if (getState() <= State::ZeroDelta || eng.m_advanceFrame.load())
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

            eng.m_advanceFrame.store(false);
        }

        return EXIT_SUCCESS;
    }

    //////////////////////////////////////////////

    Scene& Engine::getCurrentScene()
    {
        std::lock_guard<std::recursive_mutex> lock(m_engineObject->m_mutex);

        JOP_ASSERT(hasCurrentScene(), "Tried to get the current scene when it didn't exist!");
        return *m_engineObject->m_currentScene;
    }

    //////////////////////////////////////////////

    Subsystem* Engine::getSubsystem(const std::string& ID)
    {
        if (m_engineObject)
        {
            std::lock_guard<std::recursive_mutex> lock(m_engineObject->m_mutex);

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
            std::lock_guard<std::recursive_mutex> lock(m_engineObject->m_mutex);

            for (auto itr = m_engineObject->m_subsystems.begin(); itr != m_engineObject->m_subsystems.end(); ++itr)
            {
                if ((*itr)->getID() == ID)
                {
                    JOP_DEBUG_INFO("Subsystem \"" << (*itr)->getID() << "\" (type: \"" << typeid(*(*itr)).name() << "\") removed");
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

            m_engineObject->m_state.store(state);
        }
    }

    //////////////////////////////////////////////

    Engine::State Engine::getState()
    {
        return exiting() ? State::Frozen : m_engineObject->m_state.load();
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
        std::lock_guard<std::recursive_mutex> lock(m_engineObject->m_mutex);

        JOP_ASSERT(hasSharedScene(), "Tried to get the shared scene when it didn't exist!");
        return *m_engineObject->m_sharedScene;
    }

    //////////////////////////////////////////////

    bool Engine::hasSharedScene()
    {
        std::lock_guard<std::recursive_mutex> lock(m_engineObject->m_mutex);

        JOP_ASSERT(!exiting(), "There must be a valid jop::Engine object in order to call jop::Engine::has/getSharedScene()!");
        return m_engineObject->m_sharedScene.operator bool();
    }

    //////////////////////////////////////////////

    double Engine::getTotalTime()
    {
        if (m_engineObject)
            return m_engineObject->m_totalTime.load();

        return 0.0;
    }

    //////////////////////////////////////////////

    bool Engine::hasCurrentScene()
    {
        std::lock_guard<std::recursive_mutex> lock(m_engineObject->m_mutex);

        JOP_ASSERT(!exiting(), "There must be a valid jop::Engine object in order to call jop::Engine::has/getCurrentScene()!");
        return m_engineObject->m_currentScene.operator bool();
    }

    //////////////////////////////////////////////

    void Engine::advanceFrame()
    {
        if (!exiting())
            m_engineObject->m_advanceFrame.store(true);
    }

    //////////////////////////////////////////////

    void Engine::setDeltaScale(const float scale)
    {
        if (m_engineObject)
            m_engineObject->m_deltaScale.store(scale);
    }

    //////////////////////////////////////////////

    float Engine::getDeltaScale()
    {
        return m_engineObject != nullptr ? m_engineObject->m_deltaScale.load() : 1.f;
    }

    //////////////////////////////////////////////

    void Engine::signalNewScene()
    {
        if (m_engineObject)
            m_engineObject->m_newSceneSignal.store(true);
    }

    //////////////////////////////////////////////

    bool Engine::newSceneReady()
    {
        if (m_engineObject)
            return m_engineObject->m_newScene.load() != nullptr;

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