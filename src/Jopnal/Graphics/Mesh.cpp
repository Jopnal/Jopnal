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
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

//////////////////////////////////////////////

// Headers
#include <Jopnal/Precompiled.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#pragma warning(push)
#pragma warning(disable: 4706)
#include <Jopnal/Graphics/tinyobjloader/tiny_obj_loader.h>
#pragma warning(pop)

//////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_LOADABLE(jop, Mesh)[](const void*, const json::Value& val)
    {
        if (!val.HasMember("name") || !val["name"].IsString())
        {
            JOP_DEBUG_ERROR("Couldn't load Mesh, no name found");
            return false;
        }

        Mesh::LoadOptions opts(Mesh::DefaultOptions);

        if (val.HasMember("options") && val["options"].IsObject())
        {
            auto& optVal = val["options"];

            if (optVal.HasMember("transform") && optVal["transform"].IsArray() && optVal["transform"].Size() >= 10)
            {
                Transform& trans = opts.transform;
                auto& trs = optVal["transform"];

                trans.setPosition(static_cast<float>(trs[0u].GetDouble()),
                                  static_cast<float>(trs[1u].GetDouble()),
                                  static_cast<float>(trs[2u].GetDouble()));

                trans.setScale(static_cast<float>(trs[3u].GetDouble()),
                               static_cast<float>(trs[4u].GetDouble()),
                               static_cast<float>(trs[5u].GetDouble()));

                trans.setRotation(glm::quat(static_cast<float>(trs[6u].GetDouble()),
                    static_cast<float>(trs[7u].GetDouble()),
                    static_cast<float>(trs[8u].GetDouble()),
                    static_cast<float>(trs[9u].GetDouble())));
            }

            if (optVal.HasMember("centerorig") && optVal["centerorig"].IsBool())
                opts.centerOrigin = optVal["centerorig"].GetBool();

            if (optVal.HasMember("flipv") && optVal["flipv"].IsBool())
                opts.flipV = optVal["flipv"].GetBool();

            if (optVal.HasMember("gennorm") && optVal["gennorm"].IsBool())
                opts.generateNormals = optVal["gennorm"].GetBool();
        }

        ResourceManager::getResource<Mesh>(val["name"].GetString(), opts)
            .setPersistent(val.HasMember("persistent") && val["persistent"].IsBool() ? val["persistent"].GetBool() : false);

        return true;
    }
    JOP_END_LOADABLE_REGISTRATION(Mesh)

    JOP_REGISTER_SAVEABLE(jop, Mesh)[](const void* mesh, json::Value& val, json::Value::AllocatorType& alloc)
    {
        const Mesh& ref = *static_cast<const Mesh*>(mesh);
        const Mesh::LoadOptions& opts = ref.getOptions();

        val.AddMember(json::StringRef("name"), json::StringRef(ref.getName().c_str()), alloc);
        val.AddMember(json::StringRef("persistent"), ref.isPersistent(), alloc);

        if (!(opts == Mesh::DefaultOptions))
        {
            const Transform& trs = opts.transform;
            auto& obj = val.AddMember(json::StringRef("options"), json::kObjectType, alloc)["options"];

            if (trs.getMatrix() != Transform::IdentityMatrix)
            {
                obj.AddMember(json::StringRef("transform"), json::kArrayType, alloc)["transform"]
                    .PushBack(trs.getPosition().x, alloc)
                    .PushBack(trs.getPosition().y, alloc)
                    .PushBack(trs.getPosition().z, alloc)
                    .PushBack(trs.getScale().x, alloc)
                    .PushBack(trs.getScale().y, alloc)
                    .PushBack(trs.getScale().z, alloc)
                    .PushBack(trs.getRotation().w, alloc)
                    .PushBack(trs.getRotation().x, alloc)
                    .PushBack(trs.getRotation().y, alloc)
                    .PushBack(trs.getRotation().z, alloc);
            }

            obj.AddMember(json::StringRef("centerorig"), opts.centerOrigin, alloc);
            obj.AddMember(json::StringRef("flipv"), opts.flipV, alloc);
            obj.AddMember(json::StringRef("gennorm"), opts.generateNormals, alloc);
        }

        return true;
    }
    JOP_END_SAVEABLE_REGISTRATION(Mesh)
}

namespace jop
{
    Mesh::LoadOptions::LoadOptions(const bool centerOrigin_, const bool flipV_, const bool generateNormals_)
        : transform         (),
          centerOrigin      (centerOrigin_),
          flipV             (flipV_),
          generateNormals   (generateNormals_)
    {}

    bool Mesh::LoadOptions::operator==(const LoadOptions& right) const
    {
        return transform.getMatrix() == right.transform.getMatrix() &&
               centerOrigin          == right.centerOrigin          &&
               flipV                 == right.flipV                 &&
               generateNormals       == right.generateNormals;
    }

    const Mesh::LoadOptions Mesh::DefaultOptions(false, false, false);

    //////////////////////////////////////////////

    Mesh::Mesh(const std::string& name)
        : Resource          (name),
          m_options         (DefaultOptions),
          m_vertexbuffer    (Buffer::BufferType::ArrayBuffer),
          m_indexbuffer     (Buffer::BufferType::ElementArrayBuffer)
    {}

    //////////////////////////////////////////////

    bool Mesh::load(const std::string& filePath, const LoadOptions& options)
    {
        Material mat("");
        return load(filePath, mat, options);
    }

    //////////////////////////////////////////////

    bool Mesh::load(const std::string& filepath, Material& material, const LoadOptions& options)
    {
        std::string buffer;
        if (!FileLoader::read(filepath, buffer))
            return false;

        struct MatRead : tinyobj::MaterialReader
        {
            MatRead(const std::string& rootPath)
                : tinyobj::MaterialReader(),
                m_rootPath(rootPath)
            {}

            bool operator()(const std::string& matId,
                            std::vector<tinyobj::material_t>& materials,
                            std::map<std::string, int>& matMap,
                            std::string&err)
            {
                std::string matBuf;
                if (!FileLoader::read(m_rootPath + matId, matBuf))
                {
                    err = "Material file not found: " + m_rootPath + matId;
                    return false;
                }

                std::istringstream ss(matBuf);
                if (!ss)
                {
                    err = "Couldn't initialize material file stream: " + m_rootPath + matId;
                    return false;
                }

                tinyobj::LoadMtl(matMap, materials, ss);

                return true;
            }

        private:

            std::string m_rootPath;
        };

        std::string err;
        std::istringstream sstream(buffer);
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;

        const auto slashPos = filepath.find_last_of("/\\");
        const std::string rootPath = slashPos == std::string::npos ? filepath : filepath.substr(0, slashPos + 1);

        MatRead matReader(rootPath);

        if (!tinyobj::LoadObj(shapes, materials, err, sstream, matReader))
        {
            JOP_DEBUG_ERROR("Couldn't load .obj model: " << err);
            return false;
        }

        const auto& mesh = shapes.front().mesh;

        if (options.centerOrigin)
        {
            float minLeft = 0.f, maxRight = 0.f, minBottom = 0.f, maxTop = 0.f, minNegZ = 0.f, maxPosZ = 0.f;
            auto itr = mesh.positions.cbegin();

            while (itr != mesh.positions.cend())
            {
                minLeft = std::min(minLeft, *itr); maxRight = std::max(maxRight, *itr);
                minBottom = std::min(minBottom, *(itr + 1)); maxTop = std::max(maxTop, *(itr + 1));
                minNegZ = std::min(minNegZ, *(itr + 2)); maxPosZ = std::max(maxPosZ, *(itr + 2));
                itr += 3;
            }

            const glm::vec3 centerPoint(glm::AABB(glm::vec3(minLeft, minBottom, minNegZ), glm::vec3(maxRight, maxTop, maxPosZ)).getCenter());

            if (centerPoint.length() > 0.001f)
            {
                auto itr2 = shapes.front().mesh.positions.begin();

                while (itr2 != shapes.front().mesh.positions.end())
                {
                    *(itr2++) -= centerPoint.x;
                    *(itr2++) -= centerPoint.y;
                    *(itr2++) -= centerPoint.z;
                }
            }
        }

        auto posItr = mesh.positions.cbegin();
        auto tcItr = mesh.texcoords.empty() ? mesh.texcoords.cend() : mesh.texcoords.cbegin();
        auto nItr = mesh.normals.empty() ? mesh.normals.cend() : mesh.normals.cbegin();

        std::vector<Vertex> vertexArray;
        vertexArray.reserve(mesh.positions.size() / 3);

        for (std::size_t i = 0; i < mesh.positions.size() / 3; ++i)
        {
            vertexArray.emplace_back(glm::vec3(*posItr, *(posItr + 1), *(posItr + 2)),
                                     tcItr != mesh.texcoords.cend() ? glm::vec2(*tcItr, (options.flipV ? 1.f - *(tcItr + 1) : *(tcItr + 1))) : glm::vec2(),
                                     nItr != mesh.normals.cend() ? glm::vec3(*nItr, *(nItr + 1), *(nItr + 2)) : glm::vec3());

            posItr += 3;
            tcItr += (tcItr != mesh.texcoords.cend()) * 2;
            nItr += (nItr != mesh.normals.cend()) * 3;
        }

        bool normalsGenerated = false;
        if (options.generateNormals && mesh.normals.empty() && mesh.indices.size() % 3 == 0)
        {
            // #TODO This won't work with indices
            /*
            for (auto itr = vertexArray.begin(); itr != vertexArray.end(); itr += 3)
            {
                const glm::vec3 u((itr + 1)->position - itr->position);
                const glm::vec3 v((itr + 2)->position - itr->position);

                itr->normalVector = glm::cross(u, v);
                (itr + 1)->normalVector = itr->normalVector;
                (itr + 2)->normalVector = itr->normalVector;
            }

            normalsGenerated = true;*/
        }

        if (options.transform.getMatrix() != Transform::IdentityMatrix)
        {
            const auto& m = options.transform.getMatrix();
            const glm::mat3 m3(m);
            const bool norm = normalsGenerated || !mesh.normals.empty();

            for (auto& i : vertexArray)
            {
                glm::vec4 transformed(m * glm::vec4(i.position, 0.f));
                i.position.x = transformed.x;
                i.position.y = transformed.y;
                i.position.z = transformed.z;

                if (norm)
                    i.normalVector = m3 * i.normalVector;
            }
        }

        if (!materials.empty())
        {
            const auto& mat = materials.front();

            material.setReflection(Color(mat.ambient[0], mat.ambient[1], mat.ambient[3], mat.dissolve),
                                   Color(mat.diffuse[0], mat.diffuse[1], mat.diffuse[3], mat.dissolve),
                                   Color(mat.specular[0], mat.specular[1], mat.specular[3], mat.dissolve),
                                   Color(mat.emission[0], mat.emission[1], mat.emission[2], mat.dissolve))
                                   .setShininess(mat.shininess);
            
            // The default texture will be used in case of failure
            if (!mat.diffuse_texname.empty())
                material.setMap(Material::Map::Diffuse, ResourceManager::getResource<Texture>(rootPath + mat.diffuse_texname));
        }

        m_options = options;

        return load(vertexArray, mesh.indices);
    }

    //////////////////////////////////////////////

    bool Mesh::load(const std::vector<Vertex>& vertexArray, const std::vector<unsigned int>& indexArray)
    {
        m_vertexbuffer.setData(vertexArray.data(), sizeof(Vertex) * vertexArray.size());

        if (!indexArray.empty())
            m_indexbuffer.setData(indexArray.data(), sizeof(unsigned int) * indexArray.size());

        return true;
    }

    //////////////////////////////////////////////

    unsigned int Mesh::getVertexAmount() const
    {
        return m_vertexbuffer.getAllocatedSize() / sizeof(Vertex);
    }

    //////////////////////////////////////////////

    unsigned int Mesh::getElementAmount() const
    {
        return m_indexbuffer.getAllocatedSize() / sizeof(unsigned int);
    }

    //////////////////////////////////////////////

    const VertexBuffer& Mesh::getIndexBuffer() const
    {
        return m_indexbuffer;
    }

    //////////////////////////////////////////////

    const VertexBuffer& Mesh::getVertexBuffer() const
    {
        return m_vertexbuffer;
    }

    //////////////////////////////////////////////

    const Mesh::LoadOptions& Mesh::getOptions() const
    {
        return m_options;
    }

    //////////////////////////////////////////////

    Mesh& Mesh::getDefault()
    {
        static WeakReference<BoxMesh> defMesh;

        if (defMesh.expired())
        {
            defMesh = static_ref_cast<BoxMesh>(ResourceManager::getEmptyResource<BoxMesh>("jop_default_mesh").getReference());

            JOP_ASSERT_EVAL(defMesh->load(1.f), "Couldn't load default model!");

            defMesh->setPersistent(true);
            defMesh->setManaged(true);
        }

        return *defMesh;
    }
}