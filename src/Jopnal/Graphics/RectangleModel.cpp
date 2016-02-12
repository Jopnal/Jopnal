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

//////////////////////////////////////////////


namespace jop
{
    RectangleModel::RectangleModel(const std::string& name)
        : Model(name)
    {}

    RectangleModel::RectangleModel(const std::string& name, const float size)
        : Model(name)
    {
        load(size);
    }

    //////////////////////////////////////////////

    bool RectangleModel::load(const float size)
    {
        const float half = 0.5f * size;

        const std::vector<Vertex> vertexarray
        ({
            Vertex(glm::vec3(-half, -half, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, 1.f)), // 0, Left, Bottom, Front   
            Vertex(glm::vec3(half, -half, 0.f), glm::vec2(1.f, 0.f), glm::vec3(0.f, 0.f, 1.f)),  // 1, Right, Bottom, Front  
            Vertex(glm::vec3(half, half, 0.f), glm::vec2(1.f, 1.f), glm::vec3(0.f, 0.f, 1.f)),  // 2, Left, Top, Front      
            Vertex(glm::vec3(-half, half, 0.f), glm::vec2(0.f, 1.f), glm::vec3(0.f, 0.f, 1.f)),   // 3, Right, Top, Front     
        });
        static const std::vector<unsigned int> indices
        ({
            0, 1, 2,
            2, 3, 0,
        });
        return Model::load(vertexarray, indices);
    }
}