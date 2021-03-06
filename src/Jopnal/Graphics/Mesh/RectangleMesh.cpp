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

//Headers
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Graphics/Mesh/RectangleMesh.hpp>

#endif

//////////////////////////////////////////////


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
        return load(size, glm::vec2(0.f), glm::vec2(1.f));
    }

    //////////////////////////////////////////////
    
    bool RectangleMesh::load(const glm::vec2& size)
    {
        return load(size, glm::vec2(0.f), glm::vec2(1.f));
    }

    //////////////////////////////////////////////

    bool RectangleMesh::load(const float size, const glm::vec2& min, const glm::vec2& max)
    {
        return load(glm::vec2(size), min, max);
    }

    //////////////////////////////////////////////

    bool RectangleMesh::load(const glm::vec2& size, const glm::vec2& min, const glm::vec2& max)
    {
        m_size = size;
        const glm::vec2 half(size * 0.5f);

        const std::vector<Vertex> vertexarray
        ({
            Vertex(glm::vec3(-half.x, -half.y, 0.f), glm::vec2(min.x, max.y), glm::vec3(0.f, 0.f, 1.f)), // 0, Left, Bottom, Front   
            Vertex(glm::vec3( half.x, -half.y, 0.f), glm::vec2(max.x, max.y), glm::vec3(0.f, 0.f, 1.f)), // 1, Right, Bottom, Front  
            Vertex(glm::vec3( half.x,  half.y, 0.f), glm::vec2(max.x, min.y), glm::vec3(0.f, 0.f, 1.f)), // 2, Left, Top, Front      
            Vertex(glm::vec3(-half.x,  half.y, 0.f), glm::vec2(min.x, min.y), glm::vec3(0.f, 0.f, 1.f)), // 3, Right, Top, Front     
        });
        static const std::vector<unsigned int> indices
        ({
            0, 1, 2,
            2, 3, 0,
        });

        updateBounds(glm::vec3(-half, 0.f), glm::vec3(half, 0.f));

        return Mesh::load(vertexarray, indices);
    }

    //////////////////////////////////////////////

    const glm::vec2& RectangleMesh::getSize() const
    {
        return m_size;
    }
}