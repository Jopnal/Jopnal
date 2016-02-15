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

//////////////////////////////////////////////


namespace jop
{
    BoxMesh::BoxMesh(const std::string& name)
        : Mesh(name)
    {}

    BoxMesh::BoxMesh(const std::string& name, const float size)
        : Mesh(name)
    {
        load(size);
    }

    //////////////////////////////////////////////

    bool BoxMesh::load(const float size)
    {
        const float half = 0.5f * size;

        const std::vector<Vertex> vertexArray
        ({
            // Front
            Vertex(glm::vec3(-half, -half, half), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, 1.f)), // 0, Left, Bottom, Front   
            Vertex(glm::vec3(half, -half, half), glm::vec2(1.f, 0.f), glm::vec3(0.f, 0.f, 1.f)),  // 1, Right, Bottom, Front  
            Vertex(glm::vec3(-half, half, half), glm::vec2(0.f, 1.f), glm::vec3(0.f, 0.f, 1.f)),  // 2, Left, Top, Front      
            Vertex(glm::vec3(half, half, half), glm::vec2(1.f, 1.f), glm::vec3(0.f, 0.f, 1.f)),   // 3, Right, Top, Front     

            // Left
            Vertex(glm::vec3(-half, -half, -half), glm::vec2(0.f, 0.f), glm::vec3(-1.f, 0.f, 0.f)), // 4, Left, Bottom, Rear  
            Vertex(glm::vec3(-half, -half, half), glm::vec2(1.f, 0.f), glm::vec3(-1.f, 0.f, 0.f)),  // 5, Left, Bottom, Front 
            Vertex(glm::vec3(-half, half, -half), glm::vec2(0.f, 1.f), glm::vec3(-1.f, 0.f, 0.f)),  // 6, Left, Top, Rear     
            Vertex(glm::vec3(-half, half, half), glm::vec2(1.f, 1.f), glm::vec3(-1.f, 0.f, 0.f)),   // 7, Left, Top, Front    

            // Rear
            Vertex(glm::vec3(half, -half, -half), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, -1.f)),  // 8, Right, Bottom, Rear 
            Vertex(glm::vec3(-half, -half, -half), glm::vec2(1.f, 0.f), glm::vec3(0.f, 0.f, -1.f)), // 9, Left, Bottom, Rear  
            Vertex(glm::vec3(half, half, -half), glm::vec2(0.f, 1.f), glm::vec3(0.f, 0.f, -1.f)),   // 10, Right, Top, Rear   
            Vertex(glm::vec3(-half, half, -half), glm::vec2(1.f, 1.f), glm::vec3(0.f, 0.f, -1.f)),  // 11, Left, Top, Rear    

            // Right
            Vertex(glm::vec3(half, -half, half), glm::vec2(0.f, 0.f), glm::vec3(1.f, 0.f, 0.f)),    // 12, Right, Bottom, Front
            Vertex(glm::vec3(half, -half, -half), glm::vec2(1.f, 0.f), glm::vec3(1.f, 0.f, 0.f)),   // 13, Right, Bottom, Rear
            Vertex(glm::vec3(half, half, half), glm::vec2(0.f, 1.f), glm::vec3(1.f, 0.f, 0.f)),     // 14, Right, Top, Front  
            Vertex(glm::vec3(half, half, -half), glm::vec2(1.f, 1.f), glm::vec3(1.f, 0.f, 0.f)),    // 15, Right, Top, Rear   

            // Top
            Vertex(glm::vec3(-half, half, half), glm::vec2(0.f, 0.f), glm::vec3(0.f, 1.f, 0.f)),    // 16, Left, Top, Front   
            Vertex(glm::vec3(half, half, half), glm::vec2(1.f, 0.f), glm::vec3(0.f, 1.f, 0.f)),     // 17, Right, Top, Front
            Vertex(glm::vec3(-half, half, -half), glm::vec2(0.f, 1.f), glm::vec3(0.f, 1.f, 0.f)),   // 18, Left, Top, Rear
            Vertex(glm::vec3(half, half, -half), glm::vec2(1.f, 1.f), glm::vec3(0.f, 1.f, 0.f)),    // 19, Right, Top, Rear

            // Bottom
            Vertex(glm::vec3(-half, -half, -half), glm::vec2(0.f, 0.f), glm::vec3(0.f, -1.f, 0.f)), // 20, Left, Bottom, Rear
            Vertex(glm::vec3(half, -half, -half), glm::vec2(1.f, 0.f), glm::vec3(0.f, -1.f, 0.f)),  // 21, Right, Bottom, Rear
            Vertex(glm::vec3(-half, -half, half), glm::vec2(0.f, 1.f), glm::vec3(0.f, -1.f, 0.f)),  // 22, Left, Bottom, Front
            Vertex(glm::vec3(half, -half, half), glm::vec2(1.f, 1.f), glm::vec3(0.f, -1.f, 0.f))    // 23, Right, Bottom, Front
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

        return Mesh::load(vertexArray, indices);
    }
}