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


template<typename T, typename ... Args>
T& Engine::createScene(Args&&... args)
{
    static_assert(std::is_base_of<Scene, T>::value, "jop::Engine::createScene(): Attempted to create a scene which is not derived from jop::Scene");

    JOP_ASSERT(m_engineObject != nullptr, "Tried to create a scene while the engine wasn't loaded!");

    m_engineObject->m_currentScene = std::make_unique<T>(std::forward<Args>(args)...);
    m_engineObject->m_currentScene->initialize();
    return static_cast<T&>(*m_engineObject->m_currentScene);
}

//////////////////////////////////////////////

template<typename T, typename ... Args>
WeakReference<T> Engine::createSubsystem(Args&&... args)
{
    static_assert(std::is_base_of<Subsystem, T>::value, "jop::Engine::createSubsystem(): Attempted to create a subsystem which is not derived from jop::Subsystem");

    JOP_ASSERT(m_engineObject != nullptr, "Tried to create a sub system while the engine wasn't loaded!");

    m_engineObject->m_subsystems.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));

    JOP_DEBUG_INFO("Subsystem with id \"" << m_engineObject->m_subsystems.back()->getID() << "\" (type: \"" << typeid(T).name() << "\") added");

    return static_ref_cast<T>(m_engineObject->m_subsystems.back()->getReference());
}

//////////////////////////////////////////////

template<typename T>
WeakReference<T> Engine::getSubsystem()
{
    static_assert(std::is_base_of<Subsystem, T>::value, "jop::Engine::getSubsystem<T>(): Attempted to get a subsystem which is not derived from jop::Subsystem");

    if (m_engineObject)
    {
        const std::type_info& ti = typeid(T);

        for (auto& i : m_engineObject->m_subsystems)
        {
            if (typeid(*i) == ti)
                return static_ref_cast<T>(i->getReference());
        }
    }

    return WeakReference<T>();
}

//////////////////////////////////////////////

template<typename T, typename ... Args>
T& Engine::setSharedScene(Args&&... args)
{
    JOP_ASSERT(m_engineObject != nullptr && m_engineObject->m_currentScene, "Tried to set the shared scene when it or the engine wasn't loaded!");

    m_engineObject->m_sharedScene = std::make_unique<T>(std::forward<Args>(args)...);
    return static_cast<T&>(*m_engineObject->m_sharedScene);
}