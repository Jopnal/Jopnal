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
    std::string getStringArg(const Str& str, Rest&...)
    {
        static_assert(std::is_convertible<Str, std::string>::value, "First argument passed to getResource must be convertible to \"std::string\". If the resource's load() function you're trying to use doesn't take a string as its first argument, you should use getNamedResource()");
        
        return str;
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

            #pragma warning(push)
            #pragma warning(disable: 4172)
            int dummy;
            return reinterpret_cast<T&>(dummy);
            #pragma warning(pop)
        }
    };
    template<typename T>
    struct LoadFallback<T, true, false>
    {
        static T& load(const std::string& name)
        {
            JOP_DEBUG_WARNING("Couldn't load resource, resorting to error resource: " << name);
            return T::getError();
        }
    };
    template<typename T>
    struct LoadFallback<T, false, true>
    {
        static T& load(const std::string& name)
        {
            JOP_DEBUG_WARNING("Couldn't load resource, resorting to default: " << name);
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

        JOP_ASSERT(instance != nullptr, "Tried to load a resource without there being a valid ResourceManager instance!");
    }
}

//////////////////////////////////////////////

template<typename T, typename ... Args>
T& ResourceManager::getResource(Args&&... args)
{
    return getNamedResource<T>(detail::getStringArg(args...), std::forward<Args>(args)...);
}

//////////////////////////////////////////////

template<typename T, typename ... Args> 
T& ResourceManager::getNamedResource(const std::string& name, Args&&... args)
{
    if (resourceExists<T>(name))
        return getExistingResource<T>(name);
    else
    {
        auto res = std::make_unique<T>(name);

        if (res->load(std::forward<Args>(args)...))
        {
            T& ptr = *res;
            m_instance->m_resources[name] = std::move(res);
            return ptr;
        }
    }

    return detail::LoadFallback<T>::load(name);
}

//////////////////////////////////////////////

template<typename T, typename ... Args>
static T& ResourceManager::getEmptyResource(Args&&... args)
{
    detail::basicErrorCheck<T>(m_instance);

    auto res = std::make_unique<T>(args...);
    T& ptr = *res;
    m_instance->m_resources[detail::getStringArg(args...)] = std::move(res);

    return ptr;
}

template<typename T>
T& ResourceManager::getExistingResource(const std::string& name)
{
    if (resourceExists<T>(name))
        return static_cast<T&>(*m_instance->m_resources.find(name)->second);

    return detail::LoadFallback<T>::load(name);
}

//////////////////////////////////////////////

template<typename T>
bool ResourceManager::resourceExists(const std::string& name)
{
    detail::basicErrorCheck<T>(m_instance);

    auto itr = m_instance->m_resources.find(name);
    
    return (itr != m_instance->m_resources.end() && (typeid(T) == typeid(Resource) || dynamic_cast<T*>(itr->second.get()) != nullptr));
}

//////////////////////////////////////////////

template<typename T>
T& ResourceManager::copyResource(const std::string& name, const std::string& newName)
{
    if (resourceExists<T>(name))
    {
        auto& oldRes = getExistingResource<T>(name);

        auto res = std::make_unique<T>(oldRes);
        res->m_name = newName;
        T& ptr = *res;

        m_instance->m_resources[newName] = std::move(res);

        return ptr;
    }

    return detail::LoadFallback<T>::load(name);
}