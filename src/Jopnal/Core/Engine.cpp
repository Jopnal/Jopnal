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


namespace
{
    std::string ns_projectName;
    jop::Engine* ns_engineObject;

    int ns_argc;
    char** ns_argv;
}

namespace jop
{
    Engine::Engine(const std::string& name, int argc, char* argv[])
        : m_sharedScene     (std::make_unique<Scene>("SharedScene")),
          m_subsystems      (),
          m_currentScene    (),
          m_running         (true)
    {
        JOP_ASSERT(ns_engineObject == nullptr, "Only one jop::Engine object may exist at a time!");
        JOP_ASSERT(!name.empty(), "Project name mustn't be empty!");

        ns_projectName = name;
        ns_engineObject = this;

        ns_argc = argc;
        ns_argv = argv;
    }

    Engine::~Engine()
    {
        m_currentScene.reset();
        m_sharedScene.reset();
        m_subsystems.clear();

        ns_projectName = std::string();
        ns_engineObject = nullptr;
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
        const Window::Settings wSettings(true);
        createSubsystem<Window>(wSettings);

        // Shared scene set-up
        //m_sharedScene.createLayer<>();
    }

    //////////////////////////////////////////////

    int Engine::runMainLoop()
    {
        JOP_DEBUG_WARNING("No scene was loaded before entering main loop. Only the shared scene will be used.");

        const double timeStep = 1.0 / SettingManager::getUint("uFixedUpdateFrequency", 30);
        float64 accumulator = 0.0;

        Clock frameClock;

        while (m_running)
        {
            float64 frameTime = frameClock.reset().asSeconds();

            if (frameTime > 0.1)
                frameTime = 0.1;

            // Fixed update
            {
                accumulator += frameTime;

                while (accumulator >= timeStep)
                {
                    for (auto& i : m_subsystems)
                        i->preFixedUpdate(timeStep);

                    if (m_currentScene)
                        m_currentScene->fixedUpdateBase(timeStep);

                    m_sharedScene->fixedUpdateBase(timeStep);

                    for (auto& i : m_subsystems)
                        i->postFixedUpdate(timeStep);

                    accumulator -= timeStep;
                }
            }

            // Update
            {
                for (auto& i : m_subsystems)
                    i->preUpdate(frameTime);

                if (m_currentScene)
                    m_currentScene->updateBase(frameTime);

                m_sharedScene->updateBase(frameTime);

                for (auto& i : m_subsystems)
                    i->postUpdate(frameTime);
            }
            
            if (m_currentScene)
                m_currentScene->drawBase();

            m_sharedScene->drawBase();

            for (auto& i : m_subsystems)
                i->postDraw();
        }

        // #TODO Threaded event loop

        return EXIT_SUCCESS;
    }

    //////////////////////////////////////////////

    Subsystem* Engine::getSubsystem(const std::string& ID)
    {
        for (auto& i : m_subsystems)
        {
            if (i->getID() == ID)
                return i.get();
        }

        return nullptr;
    }

    //////////////////////////////////////////////

    bool Engine::removeSubsystem(const std::string& ID)
    {
        for (auto itr = m_subsystems.begin(); itr != m_subsystems.end(); ++itr)
        {
            if ((*itr)->getID() == ID)
            {
                m_subsystems.erase(itr);
                return true;
            }
        }

        return false;
    }

    //////////////////////////////////////////////

    bool Engine::isRunning()
    {
        return ns_engineObject && ns_engineObject->m_running;
    }

    //////////////////////////////////////////////

    void Engine::exit()
    {
        if (ns_engineObject)
            ns_engineObject->m_running = false;
    }

    //////////////////////////////////////////////

    void Engine::sendMessage(const std::string& message, void* ptr)
    {
        if (ns_engineObject)
        {
            if (ns_engineObject->m_currentScene && MessageUtil::hasFilterSymbol(message, "Sc"))
                ns_engineObject->m_currentScene->sendMessage(message, ptr);

            if (MessageUtil::hasFilterSymbol(message, "Su"))
            {
                for (auto& i : ns_engineObject->m_subsystems)
                    i->sendMessage(message, ptr);
            }
        }
    }

    //////////////////////////////////////////////

    Scene& Engine::getSharedScene()
    {
        JOP_ASSERT(isRunning(), "There must be a valid jop::Engine object in order to call jop::Engine::getSharedScene()!");
        return *ns_engineObject->m_sharedScene;
    }

    //////////////////////////////////////////////

    const std::string& getProjectName()
    {
        return ns_projectName;
    }

    //////////////////////////////////////////////

    void broadcast(const std::string& message, void* ptr)
    {
        Engine::sendMessage(message, ptr);
    }
}