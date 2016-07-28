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
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Core/Engine.hpp>

    #include <Jopnal/Audio/AudioDevice.hpp>
    #include <Jopnal/Core/Scene.hpp>
    #include <Jopnal/Core/FileLoader.hpp>
    #include <Jopnal/Core/SettingManager.hpp>
    #include <Jopnal/Core/ResourceManager.hpp>
    #include <Jopnal/Graphics/MainRenderTarget.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>
    #include <Jopnal/Graphics/ShaderAssembler.hpp>
    #include <Jopnal/Graphics/PostProcessor.hpp>
    #include <Jopnal/Graphics/RenderPass.hpp>
    #include <Jopnal/Utility/CommandHandler.hpp>
    #include <Jopnal/Window/Window.hpp>
    #include <Jopnal/STL.hpp>
    #include <Jopnal/Core/Win32/Win32.hpp>
    #include <Jopnal/Core/Android/ActivityState.hpp>

    #ifndef JOP_OS_WINDOWS
        #include <unistd.h>
    #endif

#endif

//////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_COMMAND_HANDLER(Engine)
    
        JOP_BIND_COMMAND(&Engine::exit, "exit");
        JOP_BIND_COMMAND(&Engine::setState, "setState");
        JOP_BIND_COMMAND(&Engine::advanceFrame, "advanceFrame");

    JOP_END_COMMAND_HANDLER(Engine)
}

namespace
{
    std::string ns_projectName;

    int ns_argc;
    char** ns_argv;

    void printOpenGLInfo()
    {
        using namespace jop;

        auto getStr = [](const GLenum e) -> const char*
        {
            return reinterpret_cast<const char*>(glGetString(e));
        };

        std::lock_guard<std::recursive_mutex> lock(jop::DebugHandler::getInstance().getMutex());
        auto& deb = jop::DebugHandler::getInstance();

        deb << DebugHandler::Severity::__Always
            << "\t\tOpenGL initialized, adapter info:\n\n"
            << "    Vendor:       " << getStr(GL_VENDOR)                    << "\n"
            << "    Renderer:     " << getStr(GL_RENDERER)                  << "\n"
            << "    Version:      " << getStr(GL_VERSION)                   << "\n"
            << "    GLSL version: " << getStr(GL_SHADING_LANGUAGE_VERSION)  << "\n";

        if (SettingManager::get<bool>("engine@Debug|bPrintOpenGLCapabilities", true))
        {
            int unifVert = 0, unifFrag = 0, attr = 0, var = 0;

            glCheck(glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &unifVert));
            glCheck(glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &unifFrag));
            glCheck(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attr));
            glCheck(glGetIntegerv(GL_MAX_VARYING_VECTORS, &var));

            deb << "\n    Texture units: " << Texture::getMaxTextureUnits()               << "\n"
                <<   "    Maximum: \n"
                <<   "      Texture size:                  " << Texture::getMaximumSize() << "\n"
                <<   "      GLSL vertex uniform vectors:   " << unifVert                  << "\n"
                <<   "      GLSL fragment uniform vectors: " << unifFrag                  << "\n"
                <<   "      GLSL attribute vectors:        " << attr                      << "\n"
                <<   "      GLSL varying vectors:          " << var                       << "\n";
        }

        if (SettingManager::get<bool>("engine@Debug|bPrintOpenGLExtensions", false))
        {
            deb << "\n\n    Available extensions:\n\n";

        #ifdef JOP_OS_DESKTOP

            GLint extensions;
            glCheck(glGetIntegerv(GL_NUM_EXTENSIONS, &extensions));

            const int alignBase = 40;
            int lastAlign = 0;

            for (int i = 0; i < extensions; ++i)
            {
                const char* str = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));

                for (int j = 0; j < lastAlign; ++j)
                    deb << " ";

                deb << "    " << str << (i % 4 == 3 ? "\n" : "");

                lastAlign = (i % 4 == 3 ? 0 : alignBase - std::strlen(str));
            }

        #else

            deb << reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));

        #endif
        }

        JOP_DEBUG_INFO(jop::DebugHandler::Severity::__Always);

    }
}

namespace jop
{
    Engine::Engine(const std::string& name, int argc, char* argv[])
        : m_sharedScene     (),
          m_totalTime       (0.0),
          m_subsystems      (),
          m_currentScene    (),
          m_newScene        (nullptr),
          m_newSceneSignal  (false),
          m_exit            (false),
          m_state           (State::Running),
          m_advanceFrame    (false),
          m_mainTarget      (nullptr),
          m_mainWindow      (nullptr)
    {
        JOP_ASSERT(m_engineObject == nullptr, "Only one jop::Engine object may exist at a time!");
        JOP_ASSERT(!name.empty(), "Project name mustn't be empty!");

        ns_projectName = name;
        m_engineObject = this;
        
        ns_argc = argc;
        ns_argv = argv;

        JOP_DEBUG_INFO(DebugHandler::Severity::__Always << "\t\tJopnal Engine v. " << JOP_VERSION_STRING);
    }

    Engine::~Engine()
    {
        m_currentScene.reset();
        m_sharedScene.reset();

        // Sub systems need to be cleared in the reverse creation order. This is not
        // guaranteed to happen by the standard.
        while (!m_subsystems.empty())
        {
            JOP_DEBUG_INFO("Subsystem \"" << typeid(*(*(m_subsystems.end() - 1))).name() << "\" removed");
            m_subsystems.erase(m_subsystems.end() - 1);
        }

        ns_projectName = std::string();
        m_engineObject = nullptr;

        JOP_DEBUG_INFO(DebugHandler::Severity::__Always << "\t\tDestroying jop::Engine, goodbye!");
    }

    //////////////////////////////////////////////

    void Engine::loadDefaultConfiguration()
    {
        // File system
        createSubsystem<FileSystemInitializer>(ns_argv[0]);

        // Setting manager
        createSubsystem<SettingManager>();

        // Audio output
        createSubsystem<AudioDevice>();

        // Main window
        m_mainWindow = &createSubsystem<Window>(Window::Settings(true));
        printOpenGLInfo();
        m_mainTarget = m_mainWindow;
        // Resource manager
        createSubsystem<ResourceManager>();

        // Shader manager
        createSubsystem<ShaderAssembler>();

        // Main render target
        //m_mainTarget = &createSubsystem<MainRenderTarget>(*m_mainWindow);

        // Post processor
        //createSubsystem<PostProcessor>(*m_mainTarget);

        // Post-pass render proxy
        createSubsystem<detail::RenderPassProxy>(RenderPass::Pass::Post);

        // Buffer swapper
        createSubsystem<BufferSwapper>(*m_mainWindow);

        // Set process priority
        if (SettingManager::get<bool>("engine@bForceProcessHighPriority", true))

        #if defined(JOP_OS_WINDOWS)
            SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
        #else
            nice(-10);
        #endif

        JOP_DEBUG_INFO("Default engine sub system configuration loaded");
    }

    //////////////////////////////////////////////

    int Engine::runMainLoop()
    {
        JOP_ASSERT(m_engineObject != nullptr, "Tried to run main loop before initializing the engine!");

        auto& eng = *m_engineObject;

        if (!eng.m_currentScene)
            JOP_DEBUG_WARNING("No scene was loaded before entering main loop. Only the shared scene will be used");

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

            frameTime *= (getState() != State::ZeroDelta || eng.m_advanceFrame.load());
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
                    std::lock_guard<std::recursive_mutex> lock(eng.m_mutex);

                    if (hasCurrentScene())
                        eng.m_currentScene->updateBase(frameTime);

                    if (hasSharedScene())
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
                    std::lock_guard<std::recursive_mutex> lock(eng.m_mutex);

                    if (hasCurrentScene())
                        eng.m_currentScene->drawBase();

                    if (hasSharedScene())
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

	Window& Engine::getCurrentWindow()
	{
		std::lock_guard<std::recursive_mutex> lock(m_engineObject->m_mutex);

		JOP_ASSERT(hasCurrentWindow(), "Tried to get the current window when it didn't exist!");
		return *m_engineObject->m_mainWindow;
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

    Message::Result Engine::sendMessage(const Message& message)
    {
        if (m_engineObject)
        {
            if (message.passFilter(Message::Engine))
            {
                if (JOP_EXECUTE_COMMAND(Engine, message.getString(), m_engineObject) == Message::Result::Escape)
                    return Message::Result::Escape;
            }

            const unsigned short sceneField = Message::SharedScene | Message::Scene | Message::Object | Message::Component;

            if (hasSharedScene() && message.passFilter(sceneField) && m_engineObject->m_sharedScene->sendMessage(message) == Message::Result::Escape)
                return Message::Result::Escape;

            if (hasCurrentScene())
            {
                auto& s = m_engineObject->m_currentScene;

                if (s && message.passFilter(sceneField) && s->sendMessage(message) == Message::Result::Escape)
                    return Message::Result::Escape;
            }

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

        std::lock_guard<std::recursive_mutex> lock(m_engineObject->m_mutex);
        return *m_engineObject->m_sharedScene;
    }

    //////////////////////////////////////////////

    bool Engine::hasSharedScene()
    {
        if (m_engineObject)
        {
            std::lock_guard<std::recursive_mutex> lock(m_engineObject->m_mutex);
            return m_engineObject->m_sharedScene.operator bool();
        }

        return false;
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
        if (m_engineObject)
        {
            std::lock_guard<std::recursive_mutex> lock(m_engineObject->m_mutex);
            return m_engineObject->m_currentScene.operator bool();
        }

        return false;
    }

	//////////////////////////////////////////////

	bool Engine::hasCurrentWindow()
	{
		if (m_engineObject)
		{
			std::lock_guard<std::recursive_mutex> lock(m_engineObject->m_mutex);
			return m_engineObject->m_mainWindow!=nullptr;
		}

		return false;
	}

    //////////////////////////////////////////////

    void Engine::advanceFrame()
    {
        if (!exiting())
            m_engineObject->m_advanceFrame.store(true);
    }

    //////////////////////////////////////////////

    bool Engine::signalNewScene()
    {
        if (newSceneReady())
        {
            m_engineObject->m_newSceneSignal.store(true);
            return true;
        }

        return false;
    }

    //////////////////////////////////////////////

    bool Engine::newSceneReady()
    {
        if (m_engineObject)
            return m_engineObject->m_newScene.load() != nullptr;

        return false;
    }

    //////////////////////////////////////////////

    const RenderTarget& Engine::getMainRenderTarget()
    {
        JOP_ASSERT(m_engineObject != nullptr && m_engineObject->m_mainTarget != nullptr, "Tried to get the main render target when it didn't exist!");

        return *m_engineObject->m_mainTarget;
    }

    //////////////////////////////////////////////

    const Window& Engine::getMainWindow()
    {
        JOP_ASSERT(m_engineObject != nullptr && m_engineObject->m_mainWindow != nullptr, "Tried to get the main window ehn it didn't exist!");

        return *m_engineObject->m_mainWindow;
    }

    //////////////////////////////////////////////

    Engine* Engine::m_engineObject = nullptr;

    //////////////////////////////////////////////

    const std::string& getProjectName()
    {
        return ns_projectName;
    }

    //////////////////////////////////////////////

    Message::Result broadcast(const Message& message)
    {
        return Engine::sendMessage(message);
    }
}