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
#include <Jopnal/Precompiled/Precompiled.hpp>

#ifndef JOP_PRECOMPILED_HEADER

#include <Jopnal/Graphics/ModelLoader.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    ModelLoader::ModelLoader(Object& obj)
        : Component (obj, 0),
          m_path    ()
    {}

    ModelLoader::ModelLoader(const ModelLoader& other, Object& obj)
        : Component (other, obj),
          m_path    (other.m_path)
    {}

    //////////////////////////////////////////////

    namespace detail
    {
        std::string getHex()
        {
            static unsigned long long count = 0;

            std::ostringstream ss;
            ss << std::hex << ++count;
            return ss.str();
        }

        void getTextures(const json::Document& doc, const std::vector<uint8>& data, const std::string& root)
        {
            if (!doc.HasMember("textures") || !doc["textures"].IsObject())
                return;

            for (auto itr = doc["textures"].MemberBegin(); itr < doc["textures"].MemberEnd(); ++itr)
            {
                Texture2D* tex = nullptr;

                if (itr->value.HasMember("path") && itr->value["path"].IsString())
                    tex = &ResourceManager::getResource<Texture2D>
                    (
                        root.substr(0, root.find_last_of('/') + 1) + itr->name.GetString(),
                        itr->value.HasMember("srgb") && itr->value["srgb"].IsBool() ? itr->value["srgb"].GetBool() : false,
                        itr->value.HasMember("genmipmaps") && itr->value["genmipmaps"].IsBool() ? itr->value["genmipmaps"].GetBool() : true
                    );

                else if (itr->value.HasMember("start") && itr->value["start"].IsUint() &&
                         itr->value.HasMember("length") && itr->value["length"].IsUint())
                    tex = &ResourceManager::getNamedResource<Texture2D>
                    (
                        root.substr(0, root.find_last_of('/') + 1) + itr->name.GetString(), &data.at(itr->value["start"].GetUint()), itr->value["length"].GetUint(),
                        itr->value.HasMember("srgb") && itr->value["srgb"].IsBool() ? itr->value["srgb"].GetBool() : false,
                        itr->value.HasMember("genmipmaps") && itr->value["genmipmaps"].IsBool() ? itr->value["genmipmaps"].GetBool() : true
                    );

                else
                {
                    JOP_DEBUG_ERROR("Failed load texture \"" << itr->name.GetString() << "\", invalid data");
                    continue;
                }

                if (tex == &Texture2D::getError())
                    continue;

                if (itr->value.HasMember("wrapmode") && itr->value["wrapmode"].IsInt())
                    tex->getSampler().setRepeatMode(static_cast<TextureSampler::Repeat>(itr->value["wrapmode"].GetInt()));
            }
        }

        std::vector<const Material*> getMaterials(const json::Document& doc, const std::string& root)
        {
            std::vector<const Material*> mats;

            if (doc.HasMember("materials") && doc["materials"].IsArray())
            {
                mats.reserve(doc["materials"].Size());

                for (auto& mat : doc["materials"])
                {
                    auto& m = ResourceManager::getEmptyResource<Material>("jop_material_" + getHex(), true);

                    if (mat.HasMember("reflection") && mat["reflection"].IsArray() && mat["reflection"].Size() >= 16)
                    {
                        auto getFloatColor = [](const json::Value& val, const unsigned int index) -> float
                        {
                            return val[index].IsUint() ? static_cast<float>(val[index].GetDouble()) : 1.f;
                        };

                        const auto& refVal = mat["reflection"];

                        for (unsigned int i = 0; i < refVal.Size() / 4; ++i)
                        {
                            m.setReflection(static_cast<Material::Reflection>(i), Color
                            (
                                getFloatColor(refVal, i * 4 + 0),
                                getFloatColor(refVal, i * 4 + 1),
                                getFloatColor(refVal, i * 4 + 2),
                                getFloatColor(refVal, i * 4 + 3)
                            ));
                        }
                    }

                    if (mat.HasMember("shininess"))
                        m.setShininess(static_cast<float>(mat["shininess"].GetDouble()));

                    if (mat.HasMember("reflectivity"))
                        m.setReflectivity(static_cast<float>(mat["reflectivity"].GetDouble()));

                    if (mat.HasMember("textures") && mat["textures"].IsObject())
                    {
                        auto& texObject = mat["textures"];

                        for (auto itr = texObject.MemberBegin(); itr != texObject.MemberEnd(); ++itr)
                        {
                            if (!itr->value.HasMember("type") || !itr->value["type"].IsUint())
                                continue;
                                
                            m.setMap(static_cast<Material::Map>(itr->value["type"].GetUint()),
                                     ResourceManager::getExistingResource<Texture2D>(root.substr(0, root.find_last_of('/') + 1) + itr->name.GetString()));
                        }
                    }

                    mats.push_back(&m);
                }
            }

            return mats;
        }

        std::vector<std::pair<const Mesh*, unsigned int>> getMeshes(const json::Document& doc, const std::vector<uint8>& data)
        {
            std::vector<std::pair<const Mesh*, unsigned int>> meshes;

            if (doc.HasMember("meshes") && doc["meshes"].IsArray())
            {
                const auto& meshVal = doc["meshes"];

                meshes.reserve(meshVal.Size());

                for (auto& mes : meshVal)
                {
                    uint32 info[8];
                    const char* const dataKeys[] =
                    {
                        "type",         // 0
                        "components",   // 1
                        "start",        // 2
                        "length",       // 3
                        "startIndex",   // 4
                        "lengthIndex",  // 5
                        "material",     // 6
                        "sizeIndex"     // 7
                    };

                    bool error = false;
                    for (unsigned int i = 0; i < sizeof(info) / sizeof(uint32); ++i)
                    {
                        if (!mes.HasMember(dataKeys[i]) || !mes[dataKeys[i]].IsUint())
                        {
                            JOP_DEBUG_ERROR("Failed to load mesh from model, missing data member \"" << dataKeys[i] << "\"");

                            error = true;
                            break;
                        }

                        info[i] = mes[dataKeys[i]].GetUint();
                    }

                    if (error)
                        continue;

                    meshes.push_back(std::make_pair
                    (
                        &ResourceManager::getNamedResource<Mesh>("jop_mesh_" + getHex(), &data.at(info[2]), data[3], data[1], &data.at(data[4]), data[7], data[5] / data[7]),
                        data[6]
                    ));
                }
            }

            return meshes;
        }

        bool processNode(WeakReference<Object> object, Renderer& rend, const std::vector<std::pair<const Mesh*, unsigned int>>& meshes, const std::vector<const Material*> mats, const json::Value& val)
        {
            auto getFloatVal = [](const json::Value& val, const unsigned int index, const float def) -> float
            {
                return val[index].IsDouble() ? static_cast<float>(val[index].GetDouble()) : def;
            };

            if (val.HasMember("scale") && val["scale"].IsArray() && val["scale"].Size() >= 3)
            {
                auto& sclVal = val["scale"];
                object->setScale(getFloatVal(sclVal, 0, 1.f), getFloatVal(sclVal, 1, 1.f), getFloatVal(sclVal, 2, 1.f));
            }

            if (val.HasMember("rotation") && val["rotation"].IsArray() && val["rotation"].Size() >= 4)
            {
                auto& rotVal = val["rotation"];
                object->setRotation(glm::quat(getFloatVal(rotVal, 0, 1.f), getFloatVal(rotVal, 1, 0.f), getFloatVal(rotVal, 2, 0.f), getFloatVal(rotVal, 3, 0.f)));
            }

            if (val.HasMember("position") && val["position"].IsArray() && val["position"].Size() >= 3)
            {
                auto& posVal = val["position"];
                object->setPosition(getFloatVal(posVal, 0, 0.f), getFloatVal(posVal, 1, 0.f), getFloatVal(posVal, 2, 0.f));
            }

            if (val.HasMember("meshes") && val["meshes"].IsArray())
            {
                for (auto& i : val["meshes"])
                {
                    if (i.IsUint())
                        object->createComponent<GenericDrawable>(rend).setModel(Model(*meshes[i.GetUint()].first, *mats[meshes[i.GetUint()].second]));
                }
            }

            return true;
        }

        bool makeNodes(WeakReference<Object> parentObject, Renderer& rend, const std::vector<std::pair<const Mesh*, unsigned int>>& meshes, const std::vector<const Material*>& mats, json::Value& val)
        {
            if (!processNode(parentObject, rend, meshes, mats, val))
                return false;

            if (val.HasMember("children") && val["children"].IsObject())
            {
                for (auto itr = val["children"].MemberBegin(); itr != val["children"].MemberEnd(); ++itr)
                {
                    if (!makeNodes(parentObject->createChild(itr->name.GetString()), rend, meshes, mats, itr->value))
                        return false;
                }
            }

            return true;
        }
    }

    bool ModelLoader::load(const std::string& path)
    {
        FileLoader fl;

        if (!fl.open(path))
        {
            JOP_DEBUG_ERROR("Failed to open model file \"" << path << "\"");
            return false;
        }

        unsigned int scopes = 0;
        do{
            char current = 0;
            fl.read(&current, 1);

            if (current == '{')
                scopes++;
            if (current == '}')
                scopes--;

        } while (scopes != 0);

        unsigned int currentPos = static_cast<unsigned int>(fl.tell());
        fl.seek(0);
        std::string jsonData;
        jsonData.resize(currentPos);

        fl.read(&jsonData.at(0), currentPos);

        json::Document doc;
        doc.Parse(jsonData.c_str());

        if (!json::checkParseError(doc))
        {
            JOP_DEBUG_ERROR("Failed to parse model file \"" << path << "\"");
            return false;
        }

        // Skip whitespace
        char currentChar = 0;
        do{
            fl.read(&currentChar, 1);
        } while (currentChar == ' ' || currentChar == '\n');
        fl.seek(fl.tell() - 1);

        std::vector<uint8> data(static_cast<std::size_t>(fl.getSize() - fl.tell()));
        fl.read(&data.at(0), fl.getSize() - fl.tell());

        detail::getTextures(doc, data, path);

        const auto materials = detail::getMaterials(doc, path);
        const auto meshes = detail::getMeshes(doc, data);

        if (!doc.HasMember("rootnode"))
        {
            JOP_DEBUG_ERROR("Model \"" << path << "\" has no root node");
            return false;
        }

        if (!detail::makeNodes(getObject(), getObject()->getScene().getRenderer(), meshes, materials, doc["rootnode"]))
        {
            JOP_DEBUG_ERROR("Failed to load nodes from model \"" << path << "\"");
            return false;
        }

        m_path = path;

        return true;
    }
}