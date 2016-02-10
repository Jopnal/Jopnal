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
    Model::Model()
        : Resource      (),
          m_mesh        (),
          m_material    (Material::getDefault())
    {

    }

    //////////////////////////////////////////////

    bool Model::load(const std::string& filepath)
    {
        std::string buffer;
        if (!FileLoader::read(filepath, buffer))
            return false;

        struct MatRead : tinyobj::MaterialReader
        {
            JOP_DISALLOW_COPY_MOVE(MatRead);

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

            const std::string& m_rootPath;
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
        
        auto posItr = mesh.positions.cbegin();
        auto tcItr = mesh.texcoords.empty() ? mesh.texcoords.cend() : mesh.texcoords.cbegin();
        auto nItr = mesh.normals.empty() ? mesh.normals.cend() : mesh.normals.cbegin();
        
        std::vector<Vertex> vertexArray;
        vertexArray.reserve(mesh.positions.size() / 3);

        for (std::size_t i = 0; i < vertexArray.capacity(); ++i)
        {
            vertexArray.emplace_back();
            auto& v = vertexArray.back();

            v.position.x = *posItr;
            v.position.y = *(++posItr);
            v.position.z = *(++posItr);

            if (tcItr != mesh.texcoords.cend())
            {
                v.texCoords.x = *tcItr;
                v.texCoords.y = 1.f - *(++tcItr);
                ++tcItr;
            }
            if (nItr != mesh.normals.cend())
            {
                v.normalVector.x = *nItr;
                v.normalVector.y = *(++nItr);
                v.normalVector.z = *(++nItr);
                ++nItr;
            }

            ++posItr;
        }

        if (!materials.empty())
        {
            const auto& mat = materials.front();
            const uint8 dissolve = static_cast<uint8>(mat.dissolve * 255.f);

            m_material.setReflection(Color(mat.ambient[0], mat.ambient[1], mat.ambient[3], dissolve),
                                     Color(mat.diffuse[0], mat.diffuse[1], mat.diffuse[3], dissolve),
                                     Color(mat.specular[0], mat.specular[1], mat.specular[3], dissolve))
                      .setShininess(mat.shininess);

            // The default texture will be used in case of failure
            if (!mat.diffuse_texname.empty())
                m_material.setMap(Material::Map::Diffuse, *ResourceManager::getResource<Texture>(rootPath + mat.diffuse_texname).lock());
        }

        m_mesh = ResourceManager::getNamedResource<Mesh>(filepath + "_buf", vertexArray, mesh.indices);

        return !m_mesh.expired();
    }

    //////////////////////////////////////////////

    bool Model::load(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    {
        return load(name, vertices, indices, Material::getDefault());
    }

    //////////////////////////////////////////////

    bool Model::load(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Material& material)
    {
        m_material = material;
        
        m_mesh = ResourceManager::getNamedResource<Mesh>(name, vertices, indices);
        // #TODO Fix
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