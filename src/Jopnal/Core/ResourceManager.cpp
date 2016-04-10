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

//////////////////////////////////////////////

namespace jop
{
    JOP_REGISTER_LOADABLE(jop, ResourceManager)[](const json::Value& val) -> bool
    {
        return ResourceManager::loadBase(val);
    }
    JOP_END_LOADABLE_REGISTRATION(ResourceManager)

    JOP_REGISTER_SAVEABLE(jop, ResourceManager)[](const Subsystem& subsys, json::Value& val, json::Value::AllocatorType& alloc)
    {
        return ResourceManager::saveBase(subsys, val, alloc);
    }
    JOP_END_SAVEABLE_REGISTRATION(ResourceManager)
}

namespace jop
{
    ResourceManager::ResourceManager()
        : Subsystem("Resource Manager")
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
            auto& inst = *m_instance;

            auto itr = inst.m_resources.begin();
            while (itr != inst.m_resources.end())
            {
                if (itr->first.first == name)
                {
                    if (itr->second->getPersistence() != 0)
                        inst.m_resources.erase(itr);
                    
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
            auto& res = m_instance->m_resources;

            for (auto itr = res.begin(); itr != res.end();)
            {
                if (itr->second->getPersistence() == 0 || (descending ? itr->second->getPersistence() < persistence : itr->second->getPersistence() != persistence))
                    ++itr;
                else
                    itr = res.erase(itr);
            }
        }
    }

    //////////////////////////////////////////////

    bool ResourceManager::loadBase(const json::Value& val)
    {
        if (!m_instance)
        {
            JOP_DEBUG_ERROR("Couldn't load resource manager state, no instance exists");
            return false;
        }

        m_instance->setID(val.HasMember("id") && val["id"].IsString() ? val["id"].GetString() : "Resource Manager");

        if (val.HasMember("resources") && val["resources"].IsArray())
        {
            auto& custCont = StateLoader::getInstance().getFunctionContainer<Resource>();

            for (auto& i : val["resources"])
            {
                if (i.HasMember("type") && i["type"].IsString())
                {
                    auto itr = custCont.find(i["type"].GetString());

                    if (itr != custCont.end())
                    {
                        const json::Value v(json::kObjectType);
                        if (!std::get<0>(itr->second)(nullptr, i.HasMember("data") && i["data"].IsObject() ? i["data"] : v))
                        {
                            JOP_DEBUG_ERROR("Failed to load resource, registered load function reported failure");
                            return false;
                        }
                    }
                    else
                    {
                        JOP_DEBUG_WARNING("Couldn't load resource with type \"" << i["type"].GetString() << "\", load function not registered. Attempting to load the rest");
                        continue;
                    }
                }
            }
        }

        return true;
    }

    //////////////////////////////////////////////

    bool ResourceManager::saveBase(const Subsystem&, json::Value& val, json::Value::AllocatorType& alloc)
    {
        if (!m_instance)
        {
            JOP_DEBUG_ERROR("Couldn't load resource manager state, no instance exists");
            return false;
        }

        val.AddMember(json::StringRef("id"), json::StringRef(m_instance->getID().c_str()), alloc);

        if (!m_instance->m_resources.empty())
        {
            const auto& resCont = StateLoader::getInstance().getFunctionContainer<Resource>();
            const auto& nameMap = StateLoader::getInstance().getSavenameContainer();

            auto& resArr = val.AddMember(json::StringRef("resources"), json::kArrayType, alloc)["resources"];

            for (auto& i : m_instance->m_resources)
            {
                auto nameItr = nameMap.find(std::type_index(typeid(*i.second)));
                auto itr = resCont.end();

                if (nameItr == nameMap.end() || (itr = resCont.find(nameItr->second)) == resCont.end())
                {
                    JOP_DEBUG_WARNING("Couldn't save resource, type \"" << typeid(*i.second).name() << "\" not registered. Attempting to save the rest");
                    continue;
                }

                resArr.PushBack(json::kObjectType, alloc);
                auto& curr = resArr[resArr.Size() - 1];

                curr.AddMember(json::StringRef("type"), json::StringRef(nameItr->second.c_str()), alloc);

                if (!std::get<1>(itr->second)(i.second.get(), curr.AddMember(json::StringRef("data"), json::kObjectType, alloc)["data"], alloc))
                {
                    JOP_DEBUG_ERROR("Couldn't save resource with name \"" << i.second->getName() << "\", registered save function reported failure");
                    return false;
                }
            }
        }

        return true;
    }

    //////////////////////////////////////////////

    ResourceManager* ResourceManager::m_instance = nullptr;
}