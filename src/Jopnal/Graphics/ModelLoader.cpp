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

//////////////////////////////////////////////


namespace jop
{
    ModelLoader::Options::Options()
        : forceDiffuseAlpha(false),
        collapseTree(true),
        fixInfacingNormals(true)
    {}

    //////////////////////////////////////////////

    ModelLoader::ModelLoader(Object& obj)
        : Component(obj, 0),
        m_path()
    {}

    ModelLoader::ModelLoader(const ModelLoader& other, Object& obj)
        : Component(other, obj),
        m_path(other.m_path)
    {}

    //////////////////////////////////////////////

    namespace detail
    {
        std::string getHex()
        {
#ifdef JOP_OS_WINDOWS
            const __int64 timePoint = __rdtsc();
#endif

            std::ostringstream ss;
            ss << std::hex << timePoint;
            return ss.str();
        }


        /*
        bool Converter::binaryReader(std::string fileOut)
        {
        std::ifstream readFile(fileOut, std::ios::binary | std::ios::in);
        if (readFile)
        {
        while (!EOF)
        {
        //	if (char )
        }
        readFile.seekg(jop::m_startPos); //find the last } in the file


        std::cout << "startpos: " << jop::m_startPos << ", size of jsonbin: " << jopmodel::m_sizeofjsonbin << std::endl;
        int n;
        int count = 0;
        for (size_t i = 0; i < jop::sizeofjsonbin; i++)
        {
        readFile.read((char*)&n, sizeof(int));	//read
        std::cout << n;							//print
        count++;
        if (count % 25 == 0)
        {
        std::cout << std::endl;
        }
        }
        readFile.close();
        }
        return true;
        }
        */

        std::vector<const Material*> getMaterials(const rapidjson::Document& doc)
        {
            auto& m = ResourceManager::getEmptyResource<Material>("jop_material_" + getHex(), true);

            std::vector<const Material*> mats;

            if (doc.HasMember("materials"))
            {
                mats.reserve(doc["materials"].Size());

                for (auto& mat : doc["materials"])
                {
                    if (mat.HasMember("reflection"))
                    {
                        float r, g, b, a;
                        for (int ref = 0; ref < mat["reflection"].Size() / 4; ++ref)
                        {
                            r = mat["reflection"][ref * 4 + 0].GetDouble();
                            g = mat["reflection"][ref * 4 + 1].GetDouble();
                            b = mat["reflection"][ref * 4 + 2].GetDouble();
                            a = mat["reflection"][ref * 4 + 3].GetDouble();
                            m.setReflection(static_cast<Material::Reflection>(ref), Color(r, g, b, a));
                        }
                    }


                    bool hadShininess = false;
                    if (mat.HasMember("shininess"))
                    {
                        hadShininess = true;
                        m.setShininess(mat["shininess"].GetDouble());
                    }

                    if (mat.HasMember("reflectivity"))
                    {
                        m.setReflectivity(mat["reflectivity"].GetDouble());
                    }


                    if (mat.HasMember("textures"))
                    {
                        if (mat["textures"].IsObject())
                        {
                            auto& texObject = mat["textures"];
                            for (auto itr = texObject.MemberBegin(); itr != texObject.MemberEnd(); ++itr)
                            {
                                //Diffuse
                                if (itr->value.HasMember("type"))
                                {
                                    auto& tex = ResourceManager::getResource<Texture2D>(itr->name.GetString(), true, true);
                                    m.setMap(static_cast<Material::Map>(itr->value["type"].GetInt()), tex);

                                    //wrapping
                                    if (itr->value.HasMember("wrapmode"))
                                    {
                                        tex.getSampler().setRepeatMode(static_cast<TextureSampler::Repeat>(itr->value["wrapmode"].GetInt()));
                                    }
                                }
                            }
                        }
                    }
                    mats.push_back(&m);
                }
            }
            return mats;
        }

        std::vector<std::pair<const Mesh*, unsigned int>> getMeshes(const rapidjson::Document& doc, FileLoader& fl) //no scene intake
        {

            if (doc.HasMember("meshes"))
            {
                if (!doc["meshes"].IsNull())
                {
                    std::vector<std::pair<const Mesh*, unsigned int>> meshes;
                    std::vector<uint8> meshData(fl.getSize() - fl.tell());
                    fl.read(&meshData.at(0), fl.getSize() - fl.tell());

                    meshes.reserve(doc["meshes"].Size());

                    for (auto& mes : doc["meshes"])
                    {
                        uint32 type = 0;
                        uint32 comps = 0;
                        uint32 length = 0;
                        uint32 start = 0;
                        uint32 startIndex = 0;
                        uint32 lengthIndex = 0;
                        uint32 matIndex = 0;
                        uint32 elemSize = 0;


                        if (mes.HasMember("type"))
                        {
                            type = mes["type"].GetInt();
                        }

                        if (mes.HasMember("components"))
                        {
                            comps = mes["components"].GetInt();
                        }

                        if (mes.HasMember("start"))
                        {
                            start = mes["start"].GetInt();
                        }

                        if (mes.HasMember("length"))
                        {
                            length = mes["length"].GetInt();
                        }

                        if (mes.HasMember("startIndex"))
                        {
                            startIndex = mes["startIndex"].GetInt();
                        }

                        if (mes.HasMember("lengthIndex"))
                        {
                            lengthIndex = mes["lengthIndex"].GetInt();
                        }

                        if (mes.HasMember("material"))
                        {
                            matIndex = mes["material"].GetInt();
                        }

                        if (mes.HasMember("sizeIndex"))
                        {
                            elemSize = mes["sizeIndex"].GetInt();
                        }


                        meshes.push_back(std::make_pair(&ResourceManager::getNamedResource<Mesh>("jop_mesh_" + getHex(), &meshData.at(start), length, comps, &meshData.at(startIndex), elemSize, lengthIndex / elemSize), matIndex));
                        //meshes.push_back(std::make_pair(&ResourceManager::getNamedResource<Mesh>("jop_mesh_" + getHex(), vertBuf.data(), vertBuf.size(), comps, indBuf.data(), elemSize, mesh.mNumFaces * 3), mesh.mMaterialIndex));
                    }
                    return meshes;
                }
            }
            return std::vector<std::pair<const Mesh*, unsigned int>>();
        }

        bool processNode(WeakReference<Object> object, Renderer& rend, const std::vector<std::pair<const Mesh*, unsigned int>>& meshes, const std::vector<const Material*>& mats, rapidjson::Value& val)
        {
            // Meshes (drawables)


            if (val.HasMember("scale"))
            {
                object->setScale(glm::vec3(val["scale"][0u].GetDouble(), val["scale"][1].GetDouble(), val["scale"][2].GetDouble()));
            }

            if (val.HasMember("rotation"))
            {
                object->setRotation(glm::quat(val["rotation"][0u].GetDouble(), val["rotation"][1].GetDouble(), val["rotation"][2].GetDouble(), val["rotation"][3].GetDouble()));
            }

            if (val.HasMember("position"))
            {
                object->setPosition(glm::vec3(val["position"][0u].GetDouble(), val["position"][1].GetDouble(), val["position"][2].GetDouble()));
            }

            if (val.HasMember("meshes"))
            {
                for (auto& i : val["meshes"])
                {
                    object->createComponent<GenericDrawable>(rend).setModel(Model(*meshes[i.GetUint()].first, *mats[meshes[i.GetUint()].second]));
                }
            }

            return true;
        }

        bool makeNodes(WeakReference<Object> parentObject, Renderer& rend, const std::vector<std::pair<const Mesh*, unsigned int>>& meshes, const std::vector<const Material*>& mats, rapidjson::Value& val)
        {

            if (!processNode(parentObject, rend, meshes, mats, val))
                return false;

            if (val.HasMember("children"))
            {
                for (auto itr = val["children"].MemberBegin(); itr != val["children"].MemberEnd(); ++itr)
                {
                    WeakReference<Object> thisObj = parentObject->createChild(itr->name.GetString());

                    if (!makeNodes(thisObj, rend, meshes, mats, itr->value))
                        return false;
                }
            }
            return true;

        }
    }

    bool ModelLoader::load(const std::string& path, const Options& options)
    {
        namespace rj = rapidjson;

        if (path.empty())
            return false;

        jop::FileLoader fl;
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

        unsigned int currentPos = fl.tell();
        fl.seek(0);
        std::string jsonData;
        jsonData.resize(currentPos);

        fl.read(&jsonData.at(0), currentPos);

        rj::Document doc;
        doc.Parse(jsonData.c_str());

        //doc.SetObject();
        if (doc.HasParseError())
        {
            printf("MESH PARSE ERROR"); //fix
        }


        auto materials = detail::getMaterials(doc);

        char currentChar = 0;
        do{
            fl.read(&currentChar, 1);
        } while (std::isspace(currentChar));
        fl.seek(fl.tell() - 1);
        auto meshes = detail::getMeshes(doc, fl);


        if (!doc.HasMember("rootnode"))
        {
            return false;
        }
        detail::makeNodes(getObject(), getObject()->getScene().getRenderer(), meshes, materials, doc["rootnode"]);

        return true;
    }

}