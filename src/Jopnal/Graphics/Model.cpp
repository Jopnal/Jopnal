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
    Model::LoadOptions::LoadOptions(const bool centerOrigin_, const bool flipV_, const bool generateNormals_)
        : transform         (),
          centerOrigin      (centerOrigin_),
          flipV             (flipV_),
          generateNormals   (generateNormals_)
    {}

    const Model::LoadOptions Model::DefaultOptions(false, false, false);

    //////////////////////////////////////////////

    Model::Model(const std::string& name)
        : Resource      (name),
          m_mesh        (),
          m_material    (Material::getDefault())
    {}

    //////////////////////////////////////////////

    bool Model::load(const std::string& filepath, const LoadOptions& options)
    {
        std::string buffer;
        if (!FileLoader::read(filepath, buffer))
            return false;

        struct MatRead : tinyobj::MaterialReader
        {
            MatRead(const std::string& rootPath)
                : tinyobj::MaterialReader   (),
                  m_rootPath                (rootPath)
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
            // #TODO When drawing is indexed, will this work?

            for (auto itr = vertexArray.begin(); itr != vertexArray.end(); itr += 3)
            {
                const glm::vec3 u((itr + 1)->position - itr->position);
                const glm::vec3 v((itr + 2)->position - itr->position);

                itr->normalVector = glm::cross(u, v);
                (itr + 1)->normalVector = itr->normalVector;
                (itr + 2)->normalVector = itr->normalVector;
            }

            normalsGenerated = true;
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

            m_material.setReflection(Color(mat.ambient[0], mat.ambient[1], mat.ambient[3], mat.dissolve),
                                     Color(mat.diffuse[0], mat.diffuse[1], mat.diffuse[3], mat.dissolve),
                                     Color(mat.specular[0], mat.specular[1], mat.specular[3], mat.dissolve))
                      .setShininess(mat.shininess);

            // The default texture will be used in case of failure
            if (!mat.diffuse_texname.empty())
                m_material.setMap(Material::Map::Diffuse, *ResourceManager::getResource<Texture>(rootPath + mat.diffuse_texname).lock());
        }

        m_mesh = ResourceManager::getNamedResource<Mesh>(filepath + "_buf", vertexArray, mesh.indices);

        return !m_mesh.expired();
    }

    //////////////////////////////////////////////

    bool Model::load(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    {
        return load(vertices, indices, Material::getDefault());
    }

    //////////////////////////////////////////////

    bool Model::load(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Material& material)
    {
        m_material = material;
        m_mesh = ResourceManager::getNamedResource<Mesh>(getName() + "_buf", vertices, indices);

        return !m_mesh.expired();
    }

    //////////////////////////////////////////////

    std::weak_ptr<const Mesh> Model::getMesh() const
    {
        return m_mesh;
    }

    //////////////////////////////////////////////

    const Material& Model::getMaterial() const
    {
        return m_material;
    }

    //////////////////////////////////////////////

    std::size_t Model::getElementAmount() const
    {
        return (m_mesh.expired() ? 0 : m_mesh.lock()->getIndexBuffer().getAllocatedSize() / sizeof(unsigned int));
    }

    //////////////////////////////////////////////

    std::weak_ptr<Model> Model::getDefault()
    {
        auto defModel = ResourceManager::getNamedResource<BoxModel>("Default Model", 1.f);

        JOP_ASSERT(!defModel.expired(), "Couldn't load default model!");

        return defModel;
    }
}