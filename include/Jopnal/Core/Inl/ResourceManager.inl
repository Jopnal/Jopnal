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
    template<typename Str, typename ... Rest>
    std::string getStringArg(const Str& str, Rest&&...)
    {
        return std::string(str);
    }

    //////////////////////////////////////////////

    template<typename T>
    struct HasErrorGetter
    {
        template<typename U, T&(*)()> struct SFINAE{};
        template<typename U> static char Test(SFINAE<U, U::getError>*);
        template<typename U> static int Test(...);
        static const bool value = sizeof(Test<T>(0)) == sizeof(char);
    };

    template<typename T>
    struct HasDefaultGetter
    {
        template<typename U, T&(*)()> struct SFINAE{};
        template<typename U> static char Test(SFINAE<U, U::getDefault>*);
        template<typename U> static int Test(...);
        static const bool value = sizeof(Test<T>(0)) == sizeof(char);
    };

    //////////////////////////////////////////////

    template<typename T, bool HasError = HasErrorGetter<T>::value, bool HasDefault = HasDefaultGetter<T>::value>
    struct LoadFallback
    {
        static T& load(const std::string& name)
        {
            JOP_ASSERT(false, "Couldn't load resource and there's not error or default resource available: " + name);
            name; // Remove warning when not using assertions

            static int dummy = 0;

            return reinterpret_cast<T&>(dummy);
        }
    };
    template<typename T>
    struct LoadFallback<T, true, false>
    {
        static T& load(const std::string& name)
        {
            JOP_DEBUG_WARNING("Couldn't load resource \"" << name << "\" (" << typeid(T).name() << "), resorting to error resource");
            name; // Remove warning in release mode
            return T::getError();
        }
    };
    template<typename T>
    struct LoadFallback<T, false, true>
    {
        static T& load(const std::string& name)
        {
            JOP_DEBUG_WARNING("Couldn't load resource \"" << name << "\" (" << typeid(T).name() << "), resorting to default resource");
            name; // Remove warning in release mode
            return T::getDefault();
        }
    };
    template<typename T>
    struct LoadFallback<T, true, true>
    {
        static T& load(const std::string& name)
        {
            return LoadFallback<T, true, false>::load(name);
        }
    };

    //////////////////////////////////////////////

    template<typename T>
    void basicErrorCheck(const ResourceManager* instance)
    {
        static_assert(std::is_base_of<Resource, T>::value, "Tried to load a resource that doesn't inherit from jop::Resource");

    #ifndef JOP_ENABLE_ASSERTS
        instance;
    #endif

        JOP_ASSERT(instance != nullptr, "Tried to load a resource without there being a valid ResourceManager instance!");
    }
}

//////////////////////////////////////////////

template<typename T, typename ... Args>
T& ResourceManager::get(Args&&... args)
{
    return getNamed<T>(detail::getStringArg(args...), std::forward<Args>(args)...);
}

//////////////////////////////////////////////

template<typename T, typename ... Args> 
T& ResourceManager::getNamed(const std::string& name, Args&&... args)
{
    if (exists<T>(name))
        return getExisting<T>(name);

    else
    {
    #if JOP_CONSOLE_VERBOSITY >= 3
        Clock clk;
    #endif

        auto res = std::make_unique<T>(name);

        if (res->load(std::forward<Args>(args)...))
        {
            T& ptr = *res;

            {
                std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);

                m_instance->m_resources[std::make_pair(name, std::type_index(typeid(T)))] = std::move(res);

                if (m_instance->m_loadPhase.load())
                    m_instance->m_loadPhaseResources.emplace(name, std::type_index(typeid(T)));
            }

            JOP_DEBUG_DIAG("\"" << name << "\" (" << typeid(T).name() << ") loaded, took " << clk.getElapsedTime().asSeconds() << "s");

            return ptr;
        }
    }

    return detail::LoadFallback<T>::load(name);
}

//////////////////////////////////////////////

template<typename T, typename ... Args>
T& ResourceManager::getEmpty(Args&&... args)
{
    const std::string name = detail::getStringArg(args...);

    if (exists<T>(name))
        return getExisting<T>(name);

    else
    {
        auto res = std::make_unique<T>(args...);
        T& ptr = *res;

        {
            std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);

            m_instance->m_resources[std::make_pair(name, std::type_index(typeid(T)))] = std::move(res);

            if (m_instance->m_loadPhase.load())
                m_instance->m_loadPhaseResources.emplace(name, std::type_index(typeid(T)));
        }

        JOP_DEBUG_DIAG("\"" << name << "\" (" << typeid(T).name() << ") created");

        return ptr;
    }
}

template<typename T>
T& ResourceManager::getExisting(const std::string& name)
{
    if (exists<T>(name))
    {
        std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);

        if (m_instance->m_loadPhase.load())
            m_instance->m_loadPhaseResources.emplace(name, std::type_index(typeid(T)));

        return static_cast<T&>(*m_instance->m_resources.find(std::make_pair(name, std::type_index(typeid(T))))->second);
    }

    return detail::LoadFallback<T>::load(name);
}

//////////////////////////////////////////////

template<typename T>
bool ResourceManager::exists(const std::string& name)
{
    std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);

    detail::basicErrorCheck<T>(m_instance);

    return m_instance->m_resources.find(std::make_pair(name, std::type_index(typeid(T)))) != m_instance->m_resources.end();
}

//////////////////////////////////////////////

template<typename T>
T& ResourceManager::copy(const std::string& name, const std::string& newName)
{
    if (exists<T>(name))
    {
    #if JOP_CONSOLE_VERBOSITY >= 3
        Clock clk;
    #endif

        auto& oldRes = getExisting<T>(name);

        auto res = std::make_unique<T>(oldRes, newName);
        T& ptr = *res;

        {
            std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);

            m_instance->m_resources[std::make_pair(newName, std::type_index(typeid(T)))] = std::move(res);
        }

        JOP_DEBUG_DIAG("\"" << name << "\" (" << typeid(T).name() << ") copied, took " << clk.getElapsedTime().asSeconds() << "s");

        return ptr;
    }

    JOP_DEBUG_WARNING("Couldn't copy resource \"" << name << "\", not found or unmatched type");

    return detail::LoadFallback<T>::load(name);
}

//////////////////////////////////////////////

template<typename T>
void ResourceManager::unload(const std::string& name)
{
    detail::basicErrorCheck<T>(m_instance);

    auto& res = m_instance->m_resources;

    {
        std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);

        auto itr = res.find(std::make_pair(name, std::type_index(typeid(T))));

        if (itr != res.end() && itr->second->getPersistence())
        {
            JOP_DEBUG_DIAG("\"" << itr->first.first << "\" (" << typeid(T).name() << ") unloaded");
            res.erase(itr);
        }
    }
}

//////////////////////////////////////////////

template<typename T>
bool ResourceManager::isDefault(const T& resource)
{
    return &resource == &T::getDefault();
}

//////////////////////////////////////////////

template<typename T>
bool ResourceManager::isError(const T& resource)
{
    return &resource == &T::getError();
}