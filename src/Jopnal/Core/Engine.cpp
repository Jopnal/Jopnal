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
    Engine::Engine(const std::string& name)
        : m_running(true)
    {
        JOP_ASSERT(ns_engineObject == nullptr, "Only one jop::Engine object may exist at a time!");
        JOP_ASSERT(!name.empty(), "Project name mustn't be empty!");

        ns_projectName = name;
        ns_engineObject = this;

        SettingManager::initialize();
        DebugHandler::getInstance(); // Getting the instance the first time initializes it
        Window::initialize();
    }

    Engine::~Engine()
    {
        Window::deInitialize();
        SettingManager::save();

        ns_projectName = std::string();
        ns_engineObject = nullptr;
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
                // Scene fixed update here
                accumulator -= timeStep;
            }

            // Scene update here
            
            // Scene draw here
        }

        // #TODO Threaded event loop

        return EXIT_SUCCESS;
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