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

#ifndef JOP_PRECOMPILED_HEADER

	#include <Jopnal/Graphics/Mesh/BoxMesh.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    /*JOP_REGISTER_LOADABLE(jop, BoxMesh)[](const void*, const json::Value& val)
    {
        if (!val.HasMember("name") || !val["name"].IsString())
        {
            JOP_DEBUG_ERROR("Couldn't load BoxMesh, no name found");
            return false;
        }

        float size = 1.f;
        if (val.HasMember("size") && val["size"].IsDouble())
            size = static_cast<float>(val["size"].GetDouble());

        bool inv = false;
        if (val.HasMember("inverted") && val["inverted"].IsBool())
            inv = val["inverted"].GetBool();

        auto& res = ResourceManager::getNamedResource<BoxMesh>(val["name"].GetString(), size);

        if (val.HasMember("persistence") && val["persistence"].IsUint())
            res.setPersistence(static_cast<uint16>(val["persistence"].GetUint()));

        return true;
    }
    JOP_END_LOADABLE_REGISTRATION(BoxMesh)

    JOP_REGISTER_SAVEABLE(jop, BoxMesh)[](const void* box, json::Value& val, json::Value::AllocatorType& alloc)
    {
        const BoxMesh& ref = *static_cast<const BoxMesh*>(box);

        val.AddMember(json::StringRef("name"), json::StringRef(ref.getName().c_str()), alloc);
        val.AddMember(json::StringRef("size"), ref.getSize(), alloc);
        val.AddMember(json::StringRef("persistence"), ref.getPersistence(), alloc);
        val.AddMember(json::StringRef("inverted"), ref.isInverted(), alloc);

        return true;
    }
    JOP_END_SAVEABLE_REGISTRATION(BoxMesh)*/
}

namespace jop
{
    BoxMesh::BoxMesh(const std::string& name)
        : Mesh          (name),
          m_size        (0.f),
          m_inverted    (false)
    {}

    BoxMesh::BoxMesh(const BoxMesh& other, const std::string& newName)
        : Mesh          (newName),
          m_size        (0.f),
          m_inverted    (false)
    {
        load(other.getSize(), other.isInverted());
    }

    //////////////////////////////////////////////

    bool BoxMesh::load(const float size)
    {
        return load(size, false);
    }

    //////////////////////////////////////////////

    bool BoxMesh::load(const float size, const bool invert)
    {
        m_size = size;
        m_inverted = invert;

        const float half = (invert ? -0.5f : 0.5f) * size;
        const float norm = 1.f;

        const std::vector<Vertex> vertices
        ({
            // Front
            Vertex(glm::vec3(-half, -half,  half), glm::vec2(0.f, 0.f), glm::vec3( 0.f,  0.f,  norm )), // 0, Left, Bottom, Front   
            Vertex(glm::vec3( half, -half,  half), glm::vec2(1.f, 0.f), glm::vec3( 0.f,  0.f,  norm )), // 1, Right, Bottom, Front  
            Vertex(glm::vec3(-half,  half,  half), glm::vec2(0.f, 1.f), glm::vec3( 0.f,  0.f,  norm )), // 2, Left, Top, Front      
            Vertex(glm::vec3( half,  half,  half), glm::vec2(1.f, 1.f), glm::vec3( 0.f,  0.f,  norm )), // 3, Right, Top, Front     
                                                                                                    
            // Left                                                                                 
            Vertex(glm::vec3(-half, -half, -half), glm::vec2(0.f, 0.f), glm::vec3(-norm, 0.f,  0.f  )), // 4, Left, Bottom, Rear  
            Vertex(glm::vec3(-half, -half,  half), glm::vec2(1.f, 0.f), glm::vec3(-norm, 0.f,  0.f  )), // 5, Left, Bottom, Front 
            Vertex(glm::vec3(-half,  half, -half), glm::vec2(0.f, 1.f), glm::vec3(-norm, 0.f,  0.f  )), // 6, Left, Top, Rear     
            Vertex(glm::vec3(-half,  half,  half), glm::vec2(1.f, 1.f), glm::vec3(-norm, 0.f,  0.f  )), // 7, Left, Top, Front    
                                                                                                    
            // Rear                                                                                 
            Vertex(glm::vec3( half, -half, -half), glm::vec2(0.f, 0.f), glm::vec3( 0.f,  0.f, -norm )), // 8, Right, Bottom, Rear 
            Vertex(glm::vec3(-half, -half, -half), glm::vec2(1.f, 0.f), glm::vec3( 0.f,  0.f, -norm )), // 9, Left, Bottom, Rear  
            Vertex(glm::vec3( half,  half, -half), glm::vec2(0.f, 1.f), glm::vec3( 0.f,  0.f, -norm )), // 10, Right, Top, Rear   
            Vertex(glm::vec3(-half,  half, -half), glm::vec2(1.f, 1.f), glm::vec3( 0.f,  0.f, -norm )), // 11, Left, Top, Rear    
                                                                                                    
            // Right                                                                                
            Vertex(glm::vec3( half, -half,  half), glm::vec2(0.f, 0.f), glm::vec3( norm, 0.f,  0.f  )), // 12, Right, Bottom, Front
            Vertex(glm::vec3( half, -half, -half), glm::vec2(1.f, 0.f), glm::vec3( norm, 0.f,  0.f  )), // 13, Right, Bottom, Rear
            Vertex(glm::vec3( half,  half,  half), glm::vec2(0.f, 1.f), glm::vec3( norm, 0.f,  0.f  )), // 14, Right, Top, Front  
            Vertex(glm::vec3( half,  half, -half), glm::vec2(1.f, 1.f), glm::vec3( norm, 0.f,  0.f  )), // 15, Right, Top, Rear   
                                                                                                    
            // Top                                                                                  
            Vertex(glm::vec3(-half,  half,  half), glm::vec2(0.f, 0.f), glm::vec3( 0.f,  norm, 0.f  )), // 16, Left, Top, Front   
            Vertex(glm::vec3( half,  half,  half), glm::vec2(1.f, 0.f), glm::vec3( 0.f,  norm, 0.f  )), // 17, Right, Top, Front
            Vertex(glm::vec3(-half,  half, -half), glm::vec2(0.f, 1.f), glm::vec3( 0.f,  norm, 0.f  )), // 18, Left, Top, Rear
            Vertex(glm::vec3( half,  half, -half), glm::vec2(1.f, 1.f), glm::vec3( 0.f,  norm, 0.f  )), // 19, Right, Top, Rear
                                                                                                    
            // Bottom                                                                               
            Vertex(glm::vec3(-half, -half, -half), glm::vec2(0.f, 0.f), glm::vec3( 0.f, -norm, 0.f  )), // 20, Left, Bottom, Rear
            Vertex(glm::vec3( half, -half, -half), glm::vec2(1.f, 0.f), glm::vec3( 0.f, -norm, 0.f  )), // 21, Right, Bottom, Rear
            Vertex(glm::vec3(-half, -half,  half), glm::vec2(0.f, 1.f), glm::vec3( 0.f, -norm, 0.f  )), // 22, Left, Bottom, Front
            Vertex(glm::vec3( half, -half,  half), glm::vec2(1.f, 1.f), glm::vec3( 0.f, -norm, 0.f  ))  // 23, Right, Bottom, Front
        });
        static const std::vector<unsigned int> indices
        ({
            // Front
            0, 1, 2,
            2, 1, 3,

            // Left
            4, 5, 6,
            6, 5, 7,

            // Rear
            8, 9, 10,
            10, 9, 11,

            // Right
            12, 13, 14,
            14, 13, 15,

            // Top
            16, 17, 18,
            18, 17, 19,

            // Bottom
            20, 21, 22,
            22, 21, 23
        });

        return Mesh::load(vertices, indices);
    }

    //////////////////////////////////////////////

    float BoxMesh::getSize() const
    {
        return m_size;
    }

    //////////////////////////////////////////////

    bool BoxMesh::isInverted() const
    {
        return m_inverted;
    }
}