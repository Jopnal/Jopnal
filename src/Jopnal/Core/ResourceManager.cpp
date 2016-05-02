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
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace std
{
    size_t hash<std::pair<std::string, std::type_index>>::operator ()(const std::pair<std::string, std::type_index>& pair) const
    {
        const size_t first = hash<string>()(pair.first);
        const size_t second = hash<type_index>()(pair.second);

        return first ^ second + 0x9e3779b9 + (first << 6) + (first >> 2);
    }
}

namespace jop
{
    ResourceManager::ResourceManager()
        : Subsystem("resourcemanager")
    {
        JOP_ASSERT(m_instance == nullptr, "Only one jop::ResourceManager object must exist at a time!");
    
        m_instance = this;
    }
    
    ResourceManager::~ResourceManager()
    {
        m_instance = nullptr;
    }

    //////////////////////////////////////////////

    void ResourceManager::unloadResource(const std::string& name)
    {
        if (m_instance)
        {
            std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);

            auto& inst = *m_instance;

            auto itr = inst.m_resources.begin();
            while (itr != inst.m_resources.end())
            {
                if (itr->first.first == name)
                {
                    if (itr->second->getPersistence() != 0)
                    {
                        JOP_DEBUG_INFO("\"" << itr->first.first << "\" (" << typeid(*itr->second).name() << ") unloaded");
                        inst.m_resources.erase(itr);
                    }
                    
                    break;
                }

                ++itr;
            }
        }
    }

    //////////////////////////////////////////////

    void ResourceManager::unloadResources(const unsigned short persistence, const bool descending)
    {
        if (m_instance)
        {
            std::lock_guard<std::recursive_mutex> lock(m_instance->m_mutex);

            auto& res = m_instance->m_resources;

            for (auto itr = res.begin(); itr != res.end();)
            {
                if (itr->second->getPersistence() == 0 || (descending ? itr->second->getPersistence() < persistence : itr->second->getPersistence() != persistence))
                    ++itr;
                else
                {
                    JOP_DEBUG_INFO("\"" << itr->first.first << "\" (" << typeid(*itr->second).name() << ") unloaded");
                    itr = res.erase(itr);
                }
            }
        }
    }

    //////////////////////////////////////////////

    void ResourceManager::beginLoadPhase()
    {
        if (m_instance)
            m_instance->m_loadPhase.store(true);
    }

    //////////////////////////////////////////////

    void ResourceManager::endLoadPhase(const uint16 persistence)
    {
        if (m_instance && m_instance->m_loadPhase.load())
        {
            auto& inst = *m_instance;

            std::size_t count = 0;
            for (auto itr = inst.m_resources.begin(); itr != inst.m_resources.end();)
            {
                if (inst.m_loadPhaseResources.find(itr->first) == inst.m_loadPhaseResources.end() && itr->second->getPersistence() >= persistence)
                {
                    itr = inst.m_resources.erase(itr);
                    ++count;
                    continue;
                }

                ++itr;
            }

            inst.m_loadPhaseResources.clear();
            inst.m_loadPhase.store(false);

            JOP_DEBUG_INFO("Resource load phase ended, " << count << " resources removed");
        }
    }

    //////////////////////////////////////////////

    ResourceManager* ResourceManager::m_instance = nullptr;
}