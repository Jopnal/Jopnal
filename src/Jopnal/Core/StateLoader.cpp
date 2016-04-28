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


namespace
{
    using namespace jop;

    const char* const ns_fileVersion = "0.1";
    const char* const ns_sceneField = "scene";
    const char* const ns_versionField = "version";
    const char* const ns_typeField = "type";
    const char* const ns_sharedSceneField = "sharedscene";
    const char* const ns_objectField = "objects";
    const char* const ns_dataField = "data";
    const char* const ns_subsystemField = "subsystems";

    enum
    {
        CompID,
        SceneID,
        SubID,
        CustomID,

        LoadID = 0,
        SaveID
    };
}

namespace jop
{
    StateLoader& StateLoader::getInstance()
    {
        static StateLoader instance;
        return instance;
    }

    //////////////////////////////////////////////

    bool StateLoader::save(const std::string& /*descPath*/)
    {
        return false;
    }

    //////////////////////////////////////////////

    bool StateLoader::load(const std::string& /*descPath*/)
    {
        return false;
    }

    //////////////////////////////////////////////

    const std::unordered_map<std::type_index, std::string>& StateLoader::getSavenameContainer()
    {
        return std::get<std::tuple_size<decltype(getInstance().m_loaderSavers)>::value - 1>(getInstance().m_loaderSavers);
    }

    //////////////////////////////////////////////

    bool StateLoader::currentlyLoading()
    {
        return getInstance().m_loading;
    }

    //////////////////////////////////////////////

    bool StateLoader::loadScene(std::unique_ptr<Scene>& scene, const json::Value& data, const std::string& path)
    {
        const auto& sceneCont = std::get<SceneID>(m_loaderSavers);

        // Does the scene object have a type?
        if (data.HasMember(ns_typeField) && data[ns_typeField].IsString())
        {
            auto itr = sceneCont.find(data[ns_typeField].GetString());

            // Has the load function been registered for this scene type?
            if (itr != sceneCont.end())
            {
                const json::Value v(json::kObjectType);

                // Attempt to create the scene
                if (std::get<LoadID>(itr->second)(scene, data.HasMember(ns_dataField) && data[ns_dataField].IsObject() ? data[ns_dataField] : v))
                {
                    // Attempt to load objects
                    if (data.HasMember(ns_objectField) && data[ns_objectField].IsArray() && !data[ns_objectField].Empty())
                    {
                        if (!loadObjects(scene, data[ns_objectField], path))
                        {
                            JOP_DEBUG_ERROR("Couldn't load scene state, an object reported loading failure: " << path);
                            return false;
                        }
                    }
                }
                else
                {
                    JOP_DEBUG_ERROR("Couldn't load scene state, the registered load function reported failure: " << path);
                    return false;
                }
            }
            else
            {
                JOP_DEBUG_ERROR("Couldn't load scene state, scene type (\"" << data[ns_typeField].GetString() << "\") not registered: " << path);
                return false;
            }
        }
        else
        {
            JOP_DEBUG_ERROR("Couldn't load scene state, missing type info: " << path);
            return false;
        }

        return true;
    }

    //////////////////////////////////////////////

    bool StateLoader::loadObjects(std::unique_ptr<Scene>& /*scene*/, const json::Value& /*data*/, const std::string& /*path*/)
    {
        /*for (auto& i : data)
        {
            if (!i.IsObject())
                continue;

            const char* id = (i.HasMember("id") && i["id"].IsString() ? i["id"].GetString() : "");

            scene->createObject(id);
            if (!loadObject(scene->m_objects.back(), *scene, i, path))
            {
                JOP_DEBUG_ERROR("Failed to load object with id \"" << id << "\": " << path);
                return false;
            }
        }*/

        return true;
    }

    //////////////////////////////////////////////

    bool StateLoader::loadObject(Object& obj, const Scene& scene, const json::Value& data, const std::string& path)
    {
        const char* const activeField = "active";
        const char* const componentsField = "components";
        const char* const childrenField = "children";
        const char* const transformField = "transform";

        if (data.HasMember(activeField) && data[activeField].IsBool())
            obj.setActive(data[activeField].GetBool());

        if (data.HasMember(transformField) && data[transformField].IsArray() && data[transformField].Size() >= 10)
        {
            auto& val = data[transformField];

            for (auto& i : val)
            {
                if (!i.IsDouble())
                {
                    JOP_DEBUG_WARNING("Encountered unexpected transform value(s) while loading object with id \"" << obj.getID() << "\": " << path);
                    goto SkipTransform;
                }
            }

            obj.setPosition(static_cast<float>(val[0u].GetDouble()),
                            static_cast<float>(val[1u].GetDouble()),
                            static_cast<float>(val[2u].GetDouble()));

            obj.setScale(static_cast<float>(val[3u].GetDouble()),
                         static_cast<float>(val[4u].GetDouble()),
                         static_cast<float>(val[5u].GetDouble()));

            obj.setRotation(glm::quat(static_cast<float>(val[6u].GetDouble()),
                                      static_cast<float>(val[7u].GetDouble()),
                                      static_cast<float>(val[8u].GetDouble()),
                                      static_cast<float>(val[9u].GetDouble())));
        }

        SkipTransform:

        if (data.HasMember(componentsField) && data[componentsField].IsArray())
        {
            const auto& compCont = std::get<CompID>(m_loaderSavers);

            for (auto& i : data[componentsField])
            {
                if (i.IsObject() && i.HasMember(ns_typeField) && i[ns_typeField].IsString())
                {
                    auto itr = compCont.find(i[ns_typeField].GetString());

                    if (itr != compCont.end())
                    {
                        if (!i.HasMember(ns_dataField) || !i[ns_dataField].IsObject() || !std::get<LoadID>(itr->second)(obj, scene, i[ns_dataField]))
                        {
                            JOP_DEBUG_ERROR("Couldn't load component state, registered load function reported failure: " << path);
                            return false;
                        }
                    }
                    else
                    {
                        JOP_DEBUG_ERROR("Couldn't load component state, component type (\"" << i[ns_typeField].GetString() << "\") not registered: " << path);
                        return false;
                    }
                }
                else
                {
                    JOP_DEBUG_ERROR("Failed to load component, no type specified: " << path);
                    return false;
                }
            }
        }

        if (data.HasMember(childrenField) && data[childrenField].IsArray())
        {
            for (auto& i : data[childrenField])
            {
                const char* id = (i.HasMember("id") && i["id"].IsString() ? i["id"].GetString() : "");

                obj.createChild(id);
                if (!loadObject(obj.m_children.back(), scene, i, path))
                {
                    JOP_DEBUG_ERROR("Failed to load child object with id \"" << id << "\": " << path);
                    return false;
                }
            }
        }

        return true;
    }

    //////////////////////////////////////////////

    bool StateLoader::saveScene(const Scene& scene, json::Value& data, json::Value::AllocatorType& alloc, const std::string& path)
    {
        const auto& sceneCont = std::get<SceneID>(m_loaderSavers);
        const auto& nameMap = getSavenameContainer();

        // Type name iterator
        auto itrRedir = nameMap.find(std::type_index(typeid(scene)));
        // Function iterator
        auto itr = sceneCont.end();

        if (itrRedir == nameMap.end() || (itr = sceneCont.find(itrRedir->second)) == sceneCont.end())
        {
            JOP_DEBUG_ERROR("Couldn't save scene, type (name) not registered: " << path);
            return false;
        }

        data.AddMember(json::StringRef(ns_typeField), json::StringRef(itrRedir->second.c_str()), alloc);

        if (std::get<SaveID>(itr->second)(scene, data.AddMember(json::StringRef(ns_dataField), json::kObjectType, alloc)[ns_dataField], alloc))
        {
            // Attempt to save objects
            /*if (!scene.m_objects.empty())
            {
                if (!saveObjects(scene, data.AddMember(json::StringRef(ns_objectField), json::kArrayType, alloc)[ns_objectField], alloc, path))
                {
                    JOP_DEBUG_ERROR("Couldn't save scene, an object failed to save: " << path);
                    return false;
                }
            }*/
        }
        else
        {
            JOP_DEBUG_ERROR("Couldn't save scene, registered save function reported failure: " << path);
            return false;
        }

        return true;
    }

    //////////////////////////////////////////////

    bool StateLoader::saveObjects(const Scene& /*scene*/, json::Value& /*data*/, json::Value::AllocatorType& /*alloc*/, const std::string& /*path*/)
    {
        /*for (auto& i : scene.m_objects)
        {
            data.PushBack(json::kObjectType, alloc);
            auto& obj = data[data.Size() - 1u];

            obj.AddMember(json::StringRef("id"), json::StringRef(i.getID().c_str()), alloc);

            if (!saveObject(i, obj, alloc, path))
            {
                JOP_DEBUG_ERROR("Failed to save object with id \"" << i.getID() << "\": " << path);
                return false;
            }
        }*/

        return true;
    }

    //////////////////////////////////////////////

    bool StateLoader::saveObject(const Object& obj, json::Value& data, json::Value::AllocatorType& alloc, const std::string& path)
    {
        const char* const activeField = "active";
        const char* const componentsField = "components";
        const char* const childrenField = "children";
        //const char* const transformField = "transform";

        const auto& compCont = std::get<CompID>(m_loaderSavers);
        const auto& nameMap = getSavenameContainer();

        // Active
        data.AddMember(json::StringRef(activeField), obj.isActive(), alloc);

        // Transform
        /*data.AddMember(json::StringRef(transformField), json::kArrayType, alloc)[transformField]
            .PushBack(obj.getPosition().x, alloc)
            .PushBack(obj.getPosition().y, alloc)
            .PushBack(obj.getPosition().z, alloc)
            .PushBack(obj.getScale().x, alloc)
            .PushBack(obj.getScale().y, alloc)
            .PushBack(obj.getScale().z, alloc)
            .PushBack(obj.getRotation().w, alloc)
            .PushBack(obj.getRotation().x, alloc)
            .PushBack(obj.getRotation().y, alloc)
            .PushBack(obj.getRotation().z, alloc);*/
           
        // Components
        if (!obj.m_components.empty())
        {
            auto& comps = data.AddMember(json::StringRef(componentsField), json::kArrayType, alloc)[componentsField];

            for (auto& i : obj.m_components)
            {
                // Type name iterator
                auto itrRedir = nameMap.find(std::type_index(typeid(*i)));
                // Function iterator
                auto itr = compCont.end();

                if (itrRedir == nameMap.end() || (itr = compCont.find(itrRedir->second)) == compCont.end())
                {
                    JOP_DEBUG_WARNING("Couldn't save component with id \"" << i->getID() << "\", type (name) not registered. Attempting to save the rest: " << path);
                    continue;
                }

                comps.PushBack(json::kObjectType, alloc);
                auto& compObj = comps[comps.Size() - 1u];

                compObj.AddMember(json::StringRef(ns_typeField), json::StringRef(itrRedir->second.c_str()), alloc);

                if (!std::get<SaveID>(itr->second)(*i, compObj.AddMember(json::StringRef(ns_dataField), json::kObjectType, alloc)[ns_dataField], alloc))
                {
                    JOP_DEBUG_ERROR("Couldn't save component with id \"" << i->getID() << "\", registered save function reported failure: " << path);
                    return false;
                }
            }
        }

        if (!obj.m_children.empty())
        {
            auto& objs = data.AddMember(json::StringRef(childrenField), json::kArrayType, alloc)[childrenField];

            for (auto& i : obj.m_children)
            {
                objs.PushBack(json::kObjectType, alloc);
                auto& curr = objs[objs.Size() - 1];

                curr.AddMember(json::StringRef("id"), json::StringRef(i.getID().c_str()), alloc);

                if (!saveObject(i, curr, alloc, path))
                {
                    JOP_DEBUG_ERROR("Failed to save object with id \"" << i.getID() << "\": " << path);
                    return false;
                }
            }
        }

        return true;
    }
}