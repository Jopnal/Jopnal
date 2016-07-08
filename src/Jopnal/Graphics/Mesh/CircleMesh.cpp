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

// Headers
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Graphics/Mesh/CircleMesh.hpp>

    #include <glm/vec3.hpp>
    #include <glm/gtc/constants.hpp>
    #include <cmath>

#endif

//////////////////////////////////////////////


namespace jop
{
    CircleMesh::CircleMesh(const std::string& name)
        : Mesh        (name),
          m_radius    (0.f)
    {}

    CircleMesh::CircleMesh(const CircleMesh& other, const std::string& newName)
        : Mesh        (newName),
          m_radius    (0.f)
    {
        load(other.m_radius, other.m_points);
    }

    //////////////////////////////////////////////

    bool CircleMesh::load(const float radius, int pointCount)
    {
        pointCount = std::max(3, pointCount);

        const float radians = glm::two_pi<float>() / pointCount;
        float theta = glm::half_pi<float>();

        std::vector<Vertex> vertexarray;
        vertexarray.reserve(pointCount + 1);

        vertexarray.emplace_back(glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.5f, 0.5f), glm::vec3(0.f, 0.f, 1.f));

        for (int i = 0; i < pointCount; ++i)
        {
            vertexarray.emplace_back(glm::vec3(std::cos(theta) * radius, std::sin(theta) * radius, 0.f), glm::vec2(0.f), glm::vec3(0.f, 0.f, 1.f));

            vertexarray.back().texCoords.x = (vertexarray.back().position.x / radius + 1.f) * 0.5f;
            vertexarray.back().texCoords.y = (vertexarray.back().position.y / radius + 1.f) * 0.5f;

            theta += radians;
        }

        std::vector<unsigned int> indices;

        for (unsigned int i = 2; i < vertexarray.size(); ++i)
        {
            indices.push_back(0);
            indices.push_back(i - 1);
            indices.push_back(i);
        }
        indices.push_back(0);
        indices.push_back(vertexarray.size() - 1);
        indices.push_back(1);

        m_radius = radius;

        return Mesh::load(vertexarray, indices);
    }

    //////////////////////////////////////////////

    float CircleMesh::getRadius() const
    {
        return m_radius;
    }
}