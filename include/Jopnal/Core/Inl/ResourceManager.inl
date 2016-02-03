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


template<typename T> 
std::weak_ptr<T> ResourceManager::getResource(const std::string& path)
{
    static_assert(std::is_base_of<Resource, T>::value, "Tried to load a resource that doesn't inherit from jop::Resource");

    auto it = m_resources.find(path);

    if (it == m_resources.end())
    {
        auto res = std::make_shared<T>();

        if (res->load(path))
        {
            m_resources[path] = std::move(res);
            return std::weak_ptr<T>(std::static_pointer_cast<T>(m_resources[path]));
        }
        else
            JOP_DEBUG_ERROR("Couldn't load resource: " << path);
    }
    else
    {
        if (typeid(T) == typeid(*it->second.get()))
            return std::weak_ptr<T>(std::static_pointer_cast<T>(it->second));
        else
            JOP_DEBUG_ERROR("Resource is not of type " << typeid(T).name() << ": " << path);
    }

    return std::weak_ptr<T>();
}
