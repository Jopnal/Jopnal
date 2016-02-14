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

    enum
    {
        CompID,
        LayerID,
        SceneID,
        SubID,


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

    bool StateLoader::saveState(const std::string& path, const bool scene, const bool sharedScene, const bool subsystems)
    {
        return false;
    }

    //////////////////////////////////////////////

    bool StateLoader::loadState(const std::string& path, const bool scene, const bool sharedScene, const bool subsystems)
    {
        std::string buf;
        if (!FileLoader::read(path + ".jop", buf))
            return false;

        json::Document doc;
        doc.ParseInsitu<0>(&buf[0]);

        if (!json::checkParseError(doc))
            return false;

        // Handle version
        if (!doc.HasMember(ns_versionField))
        {
            JOP_DEBUG_WARNING("State file doesn't have the version defined, assuming latest (" << ns_fileVersion << ")");
        }
        else if (std::strcmp(doc[ns_versionField].GetString(), ns_fileVersion) != 0)
        {
            JOP_DEBUG_WARNING("The state file version doesn't match the current library version. Attempting to load anyway...");
        }

        // Load subsystems?
        if (subsystems)
        {
            // TODO, reminder: a complete success is required, otherwise original state must be returned
        }

        std::unique_ptr<Scene> sharedScenePtr;

        // Load the shared scene?
        if (sharedScene)
        {
            if (!doc.HasMember(ns_sharedSceneField) || !doc[ns_sharedSceneField].IsObject() || !Engine::m_engineObject || !loadScene(sharedScenePtr, doc[ns_sharedSceneField], path))
                return false;
        }

        std::unique_ptr<Scene> scenePtr;

        // Load scene?
        if (scene)
        {
            if (!doc.HasMember(ns_sceneField) || !doc[ns_sceneField].IsObject() || !Engine::m_engineObject || !loadScene(scenePtr, doc[ns_sceneField], path))
                return false;
        }

        if (subsystems)
        {

        }
        if (sharedScene)
        {
            if (sharedScenePtr)
                Engine::m_engineObject->m_sharedScene = std::move(sharedScenePtr);
            else
                return false;
        }
        if (scene)
        {
            if (scenePtr)
                Engine::m_engineObject->m_currentScene = std::move(scenePtr);
            else
                return false;
        }

        return true;
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
                json::Value v(json::kObjectType);

                // Attempt to create the scene
                if (std::get<LoadID>(itr->second)(scene, data.HasMember("data") && data["data"].IsObject() ? data["data"] : v))
                {
                    // Attempt to load layers
                    if (data.HasMember("layers") && data["layers"].IsArray() && !data["layers"].Empty())
                    {
                        if (!loadLayers(scene, data["layers"], path))
                        {
                            JOP_DEBUG_ERROR("Couldn't load scene state, a layer reported loading failure: " << path);
                            return false;
                        }
                    }

                    // Attempt to load objects
                    if (data.HasMember("objects") && data["objects"].IsArray() && !data["objects"].Empty())
                    {
                        if (!loadObjects(scene, data["objects"], path))
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
                JOP_DEBUG_ERROR("Couldn't load scene state, scene type not registered: " << path);
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

    bool StateLoader::loadLayers(std::unique_ptr<Scene>& scene, const json::Value& data, const std::string& path)
    {
        // load layers

        // link layers
    }

    //////////////////////////////////////////////

    bool StateLoader::loadObjects(std::unique_ptr<Scene>& scene, const json::Value& data, const std::string& path)
    {

    }
}

//{
//    "version" : "0.1",
//    "subsystems" : [
//        subsystem datas here
//    ],
//    "sharedScene" : {
//        shared scene data here, same as "scene"
//    },
//    "scene" : {
//        "type" : "jop::Scene",
//        "data" : {
//            this object is passed to the registered scene load function
//        },
//        "layers" : [
//            Layer datas here, first is the default layer
//        ],
//        "objects" : [
//            {
//                "id" : "someObject",
//                transform data
//                active
//                children array
//                component array
//            }
//        ]
//    }
//}