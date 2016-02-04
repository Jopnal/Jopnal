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
    std::string getStringArg(const Str& str, const Rest&...)
    {
        static_assert(std::is_convertible<Str, std::string>::value, "First argument passed to getResource must be convertible to \"std::string\". If the resource's load() function you're trying to use doesn't take a string as its first argument, you should use getNamedResource()");
        
        return std::string(str);
    }
}

template<typename T, typename ... Args>
std::weak_ptr<T> ResourceManager::getResource(const Args&... args)
{
    static_assert(std::is_base_of<Resource, T>::value, "Tried to load a resource that doesn't inherit from jop::Resource");

    if (!m_instance)
    {
        JOP_DEBUG_ERROR("Couldn't load resource. ResourceManager instance doesn't exist");
        return std::weak_ptr<T>();
    }

    // ResourceManager instance
    auto& inst = *m_instance;

    // Extract the first argument. Should be string
    const std::string str = detail::getStringArg(args...);

    auto it = inst.m_resources.find(str);

    if (it == inst.m_resources.end())
    {
        auto res = std::make_shared<T>();

        if (res->load(args...))
        {
            inst.m_resources[str] = res;
            return std::weak_ptr<T>(res);
        }
        
        // No need for error messages here. Those should be handled by the resources themselves
    }
    else
    {
        if (typeid(T) == typeid(*it->second.get()))
            return std::weak_ptr<T>(std::static_pointer_cast<T>(it->second));
        else
            JOP_DEBUG_ERROR("Resource is not of type " << typeid(T).name() << ": " << str);
    }

    return std::weak_ptr<T>();
}

template<typename T, typename ... Args> 
std::weak_ptr<T> ResourceManager::getNamedResource(const std::string& name, const Args&... args)
{
    static_assert(std::is_base_of<Resource, T>::value, "Tried to load a resource that doesn't inherit from jop::Resource");

    if (!m_instance)
    {
        JOP_DEBUG_ERROR("Couldn't load resource. ResourceManager instance doesn't exist");
        return std::weak_ptr<T>();
    }
    
    auto& inst = *m_instance;
    auto it = inst.m_resources.find(name);
   
    if (it == inst.m_resources.end())
    {
        auto res = std::make_shared<T>();

        if (res->load(args...))
        {
            inst.m_resources[name] = res;
            return std::weak_ptr<T>(res);
        }
    }
    else
    {
        if (typeid(T) == typeid(*it->second.get()))
            return std::weak_ptr<T>(std::static_pointer_cast<T>(it->second));
        else
            JOP_DEBUG_ERROR("Resource is not of type " << typeid(T).name() << ": " << name);
    }

    return std::weak_ptr<T>();
}