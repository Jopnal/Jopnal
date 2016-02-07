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
        : Resource          (),
          m_vertexbuffer    (Buffer::BufferType::ArrayBuffer),
          m_indexbuffer     (Buffer::BufferType::ElementArrayBuffer)
    {}

    //////////////////////////////////////////////

    bool Model::load(const std::string& filepath)
    {
        std::string buffer;
        FileLoader::read(filepath, buffer);

        class MatRead : public tinyobj::MaterialReader
        {
            bool operator()(const std::string&,
                            std::vector<tinyobj::material_t>&,
                            std::map<std::string, int>&,
                            std::string&)
            {
                return true;
            }
        };

        std::string err;
        std::istringstream sstream(buffer);
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        MatRead matReader;
        
        if (!tinyobj::LoadObj(shapes, materials, err, sstream, matReader))
        {
            JOP_DEBUG_ERROR("Couldn't load .obj model: " << err);
            return false;
        }
        
        const auto& mesh = shapes.front().mesh;

        auto posItr = mesh.positions.cbegin();
        auto tcItr = mesh.texcoords.cbegin();
        auto nItr = mesh.normals.cbegin();

        std::vector<Vertex> vertexArray;
        vertexArray.reserve(shapes.front().mesh.positions.size() / 3);

        for (std::size_t i = 0; i < vertexArray.capacity(); ++i)
        {
            vertexArray.emplace_back
            (
                glm::vec3(*posItr, *(++posItr), *(++posItr)),
                tcItr != mesh.texcoords.cend() ? glm::vec2(*tcItr, *(++tcItr)) : glm::vec2(),
                nItr != mesh.normals.cend() ? glm::vec3(*nItr, *(++nItr), *(++nItr)) : glm::vec3()
            );

            ++posItr; ++tcItr; ++nItr;
        }

        m_vertexbuffer.setData(&vertexArray[0], sizeof(Vertex) * vertexArray.size());

        if (!mesh.indices.empty())
            m_indexbuffer.setData(mesh.indices.data(), sizeof(unsigned int) * mesh.indices.size());

        return true;
    }

    //////////////////////////////////////////////

    bool Model::load(const std::vector<Vertex>& vertexArray, const std::vector<unsigned int>& indexArray)
    {
        m_vertexbuffer.setData(vertexArray.data(), sizeof(Vertex) * vertexArray.size());

        if (!indexArray.empty())
            m_indexbuffer.setData(indexArray.data(), sizeof(unsigned int) * indexArray.size());

        return true;
    }

    //////////////////////////////////////////////

    const VertexBuffer& Model::getIndexBuffer() const
    {
        return m_indexbuffer;
    }

    //////////////////////////////////////////////

    const VertexBuffer& Model::getVertexBuffer() const
    {
        return m_vertexbuffer;
    }

    //////////////////////////////////////////////

    const Model& Model::getDefault()
    {
        auto defModel = ResourceManager::getNamedResource<BoxModel>("DefaultModel", 1.f);

        JOP_ASSERT(!defModel.expired(), "Couldn't load default model!");

        return *defModel.lock();
    }
}