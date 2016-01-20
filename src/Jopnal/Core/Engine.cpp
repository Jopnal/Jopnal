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
}

namespace jop
{
    Engine::Engine(const std::string& name, int, char**)
        : m_running(true)
    {
        JOP_ASSERT(ns_engineObject == nullptr, "Only one jop::Engine object may exist at a time!");
        JOP_ASSERT(!name.empty(), "Project name mustn't be empty!");

        ns_projectName = name;
        ns_engineObject = this;

        SettingManager::initialize();
    }

    Engine::~Engine()
    {
        m_subsystems.clear();

        SettingManager::save();

        ns_projectName = std::string();
        ns_engineObject = nullptr;
    }

    //////////////////////////////////////////////

    void Engine::loadDefaultSubsystems()
    {
        Window::Settings s;
        s.displayMode = static_cast<Window::DisplayMode>(std::min(2u, SettingManager::getUint("uDefaultWindowMode", 0)));
        s.size.x = SettingManager::getUint("uDefaultWindowSizeX", 1024); s.size.y = SettingManager::getUint("uDefaultWindowSizeY", 600);
        s.title = SettingManager::getString("sDefaultWindowTitle", getProjectName());
        s.visible = true;

        createSubsystem<Window>(s);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////

    int Engine::runMainLoop()
    {
        const double timeStep = 1.0 / SettingManager::getUint("uFixedUpdateFrequency", 30);
        float64 accumulator = 0.0;

        Clock frameClock;

        while (m_running)
        {
            float64 frameTime = frameClock.reset().asSeconds();

            if (frameTime > 0.1)
                frameTime = 0.1;

            accumulator += frameTime;

            while (accumulator >= timeStep)
            {
                for (auto& i : m_subsystems)
                    i->preFixedUpdate(timeStep);

                // Scene fixed update here

                for (auto& i : m_subsystems)
                    i->postFixedUpdate(timeStep);

                accumulator -= timeStep;
            }

            for (auto& i : m_subsystems)
                i->preUpdate(frameTime);

            // Scene update here

            for (auto& i : m_subsystems)
                i->postUpdate(frameTime);
            
            // Scene draw here

            for (auto& i : m_subsystems)
                i->postDraw();
        }

        // #TODO Threaded event loop

        return EXIT_SUCCESS;
    }

    //////////////////////////////////////////////

    Subsystem* Engine::getSubsystem(const std::string& name)
    {
        for (auto& i : m_subsystems)
        {
            if (i->getName() == name)
                return i.get();
        }

        return nullptr;
    }

    //////////////////////////////////////////////

    bool Engine::removeSubsystem(const std::string& name)
    {
        for (auto itr = m_subsystems.begin(); itr != m_subsystems.end(); ++itr)
        {
            if ((*itr)->getName() == name)
            {
                m_subsystems.erase(itr);
                return true;
            }
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////

    void Engine::exit()
    {
        if (ns_engineObject)
            ns_engineObject->m_running = false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////

    const std::string& getProjectName()
    {
        return ns_projectName;
    }
}