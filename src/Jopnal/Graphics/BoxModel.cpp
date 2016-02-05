// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//////////////////////////////////////////////

// Headers
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////

namespace jop
{

    BoxModel::BoxModel(const float size) :
        Model()  
    {
        const float half = 0.5f * size;

        const std::vector<Vertex> vertexArray = {
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
            Vertex(glm::vec3(half, -half, half), glm::vec2(0.f, 0.f), glm::vec3(1.f, 0.f, 0.f)),    // 12, Right, Bottom, Fron
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
        };
        const std::vector<unsigned int> m_indices
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

        load(vertexArray, m_indices);
    }

}