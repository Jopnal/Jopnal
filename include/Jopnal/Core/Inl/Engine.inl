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


namespace detail
{
    template<typename T, bool Threaded, bool WaitSignal>
    struct SceneCreator
    {
        template<typename ... Args>
        inline static void create(Args&&... args)
        {
            auto newPtr = new T(std::forward<Args>(args)...);

            if (!Engine::hasCurrentScene())
            {
                Engine::m_engineObject->m_currentScene.reset(newPtr);
                return;
            }

            auto& scenePtr = Engine::m_engineObject->m_newScene;
            delete scenePtr.load();

            scenePtr.store(newPtr);
            Engine::signalNewScene();
        }
    };

    inline Window::Settings getWindowSettings()
    {
        Window::Settings s(false);
        s.size.x = 1; s.size.y = 1;
        s.visible = false;
        s.displayMode = Window::DisplayMode::Borderless;
        s.vSync = false;

        return s;
    }

    template<bool Wait>
    inline void waitSignal(std::atomic<bool>&, const std::string&)
    {
        Engine::signalNewScene();
    }
    template<>
    inline void waitSignal<true>(std::atomic<bool>& signal, const std::string& id)
    {
    #if JOP_CONSOLE_VERBOSITY < 2
        id;
    #endif

        JOP_DEBUG_INFO("Scene \"" << id << "\" loaded, waiting for signal...");

        while (!signal.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    template<typename T, bool WaitSignal>
    struct SceneCreator<T, true, WaitSignal>
    {
        template<typename ... Args>
        static void wait(Args&&... args)
        {
            Thread::attachJavaThread(nullptr, nullptr);

            Window win(getWindowSettings());

            auto newPtr = new T(std::forward<Args>(args)...);

            auto& scenePtr = Engine::m_engineObject->m_newScene;
            delete scenePtr.load();

            scenePtr.store(newPtr);
            GlState::flush();

            waitSignal<WaitSignal>(Engine::m_engineObject->m_newSceneSignal, newPtr->getID());

            Thread::detachJavaThread(nullptr);
        }

        template<typename ... Args>
        static void create(Args&&... args)
        {
            Thread t(&wait<Args...>, std::forward<Args>(args)...);

            t.setPriority(Thread::Priority::Lowest);
            t.detach();
        }
    };
}

template<typename T, bool Threaded, bool WaitSignal, typename ... Args>
void Engine::createScene(Args&&... args)
{
    static_assert(std::is_base_of<Scene, T>::value, "jop::Engine::createScene(): Attempted to create a scene which is not derived from jop::Scene");

    JOP_ASSERT(m_engineObject != nullptr, "Tried to create a scene while the engine wasn't loaded!");
    
    detail::SceneCreator<T, Threaded, WaitSignal>::create(std::forward<Args>(args)...);
}

//////////////////////////////////////////////

template<typename T, typename ... Args>
T& Engine::createSubsystem(Args&&... args)
{
    static_assert(std::is_base_of<Subsystem, T>::value, "jop::Engine::createSubsystem(): Attempted to create a subsystem which is not derived from jop::Subsystem");

    JOP_ASSERT(m_engineObject != nullptr, "Tried to create a sub system while the engine wasn't loaded!");

#pragma warning(suppress: 6011)
    m_engineObject->m_subsystems.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));

    JOP_DEBUG_INFO("Subsystem \"" << typeid(T).name() << "\" added");

    return static_cast<T&>(*m_engineObject->m_subsystems.back());
}

//////////////////////////////////////////////

template<typename T>
T* Engine::getSubsystem()
{
    static_assert(std::is_base_of<Subsystem, T>::value, "jop::Engine::getSubsystem<T>(): Attempted to get a subsystem which is not derived from jop::Subsystem");

    if (m_engineObject)
    {
        const std::type_info& ti = typeid(T);

        for (auto& i : m_engineObject->m_subsystems)
        {
            if (typeid(*i) == ti)
                return static_cast<T*>(i.get());
        }
    }

    return nullptr;
}

//////////////////////////////////////////////

template<typename T>
T* Engine::getSubsystem(const uint32 ID)
{
    static_assert(std::is_base_of<Subsystem, T>::value, "jop::Engine::getSubsystem<T>(): Attempted to get a subsystem which is not derived from jop::Subsystem");

    if (m_engineObject)
    {
        const std::type_info& ti = typeid(T);

        for (auto& i : m_engineObject->m_subsystems)
        {
            if (typeid(*i) == ti && i->getID() == ID)
                return static_cast<T*>(i.get());
        }
    }

    return nullptr;
}

//////////////////////////////////////////////

template<typename T>
bool Engine::removeSubsystem(const uint32 ID)
{
    if (m_engineObject)
    {
        for (auto itr = m_engineObject->m_subsystems.begin(); itr != m_engineObject->m_subsystems.end(); ++itr)
        {
            if (typeid(*(*itr)) == typeid(T) && (*itr)->getID() == ID)
            {
                JOP_DEBUG_INFO("Subsystem \"" << typeid(*(*itr)).name() << "\" removed");
                m_engineObject->m_subsystems.erase(itr);
                return true;
            }
        }
    }

    return false;
}

//////////////////////////////////////////////

template<typename T, typename ... Args>
T& Engine::setSharedScene(Args&&... args)
{
    JOP_ASSERT(m_engineObject != nullptr, "Tried to set the shared scene when it or the engine wasn't loaded!");

    m_engineObject->m_sharedScene = std::make_unique<T>(std::forward<Args>(args)...);
    return static_cast<T&>(*m_engineObject->m_sharedScene);
}