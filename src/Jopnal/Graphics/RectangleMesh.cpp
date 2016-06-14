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

//headers
#include <Jopnal/Precompiled.hpp>

#ifndef JOP_PRECOMPILED_HEADER

	#include <Jopnal/Graphics/Mesh/RectangleMesh.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    /*JOP_REGISTER_LOADABLE(jop, RectangleMesh)[](const void*, const json::Value& val)
    {
        if (!val.HasMember("name") || !val["name"].IsString())
        {
            JOP_DEBUG_ERROR("Couldn't load RectangleMesh, no name found");
            return false;
        }

        float size = 1.f;
        if (val.HasMember("size") && val["size"].IsDouble())
            size = static_cast<float>(val["size"].GetDouble());

        auto& res = ResourceManager::getNamedResource<RectangleMesh>(val["name"].GetString(), size);
        
        if (val.HasMember("persistence") && val["persistence"].IsUint())
            res.setPersistence(static_cast<uint16>(val["persistence"].GetUint()));

        return true;
    }
    JOP_END_LOADABLE_REGISTRATION(RectangleMesh)

    JOP_REGISTER_SAVEABLE(jop, RectangleMesh)[](const void* box, json::Value& val, json::Value::AllocatorType& alloc)
    {
        const RectangleMesh& ref = *static_cast<const RectangleMesh*>(box);

        val.AddMember(json::StringRef("name"), json::StringRef(ref.getName().c_str()), alloc);
        val.AddMember(json::StringRef("size"), ref.getSize(), alloc);
        val.AddMember(json::StringRef("persistence"), ref.getPersistence(), alloc);

        return true;
    }
    JOP_END_SAVEABLE_REGISTRATION(RectangleMesh)*/
}

namespace jop
{
    RectangleMesh::RectangleMesh(const std::string& name)
        : Mesh      (name),
          m_size    (0.f)
    {}

    RectangleMesh::RectangleMesh(const RectangleMesh& other, const std::string& newName)
        : Mesh      (newName),
          m_size    (0.f)
    {
        load(other.m_size);
    }

    //////////////////////////////////////////////

    bool RectangleMesh::load(const float size)
    {
        m_size = size;
        const float half = 0.5f * size;

        const std::vector<Vertex> vertexarray
        ({
            Vertex(glm::vec3(-half, -half, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, 1.f)), // 0, Left, Bottom, Front   
            Vertex(glm::vec3( half, -half, 0.f), glm::vec2(1.f, 0.f), glm::vec3(0.f, 0.f, 1.f)), // 1, Right, Bottom, Front  
            Vertex(glm::vec3( half,  half, 0.f), glm::vec2(1.f, 1.f), glm::vec3(0.f, 0.f, 1.f)), // 2, Left, Top, Front      
            Vertex(glm::vec3(-half,  half, 0.f), glm::vec2(0.f, 1.f), glm::vec3(0.f, 0.f, 1.f)), // 3, Right, Top, Front     
        });
        static const std::vector<unsigned int> indices
        ({
            0, 1, 2,
            2, 3, 0,
        });

        return Mesh::load(vertexarray, indices);
    }

    //////////////////////////////////////////////

    float RectangleMesh::getSize() const
    {
        return m_size;
    }
}