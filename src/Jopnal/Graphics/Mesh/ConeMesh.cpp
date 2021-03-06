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
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Graphics/Mesh/ConeMesh.hpp>

    #include <glm/gtc/constants.hpp>
    #include <cmath>

#endif

//////////////////////////////////////////////


namespace jop
{
    ConeMesh::ConeMesh(const std::string& name)
        : Mesh                  (name),
          m_radius              (0.f),
          m_height              (0.f),
          m_sectors             (0),
          m_dividedTexCoords    (false)
    {}

    ConeMesh::ConeMesh(const ConeMesh& other, const std::string& newName)
        : Mesh                  (newName),
          m_radius              (0.f),
          m_height              (0.f),
          m_sectors             (0),
          m_dividedTexCoords    (false)
    {
        load(other.m_radius, other.m_height, other.m_sectors, other.m_dividedTexCoords);
    }

    //////////////////////////////////////////////

    bool ConeMesh::load(const float radius, const float height, const unsigned int sectors, const bool dividedTexCoords)
    {
        m_radius = radius;
        m_height = height;
        m_sectors = sectors;
        m_dividedTexCoords = dividedTexCoords;

        const float half = 0.5f * height;
        const float norm = 1.f;
        const float divide[2] = {dividedTexCoords * 0.75f, 1.f - dividedTexCoords * 0.25f};
        const float texMulti = dividedTexCoords ? 1.f / sectors : 1.f;
        float texCoord = dividedTexCoords ? 1.f - texMulti : 0.f;

        float width[2];
        float depth[2] = {radius, radius};

        const float alfa = glm::two_pi<float>() * radius / sectors / radius;
        width[1] = std::sqrt((radius * radius + radius * radius) - (2.f * radius * radius * std::cos(alfa)));
        width[0] = width[1];

        width[1] = width[0] * std::cos(alfa) - radius * std::sin(alfa);
        depth[1] = width[0] * std::sin(alfa) + radius * std::cos(alfa);

        std::vector<Vertex> vertexArray((6 * sectors) + 1);

        auto itr = vertexArray.begin();
        for (std::size_t s = 0; s < sectors; ++s)
        {
            float lenght[2] =
            {
                std::sqrt((width[0] * width[0]) + 1.f + (depth[0] * depth[0])),
                std::sqrt((width[1] * width[1]) + 1.f + (depth[1] * depth[1]))
            };

            *itr++ = Vertex(glm::vec3(0.f, -half, 0.f),           glm::vec2(texCoord + texMulti, 1.f),       glm::vec3(0.f, -norm, 0.f));                       // 0, middle, Bottom    
            *itr++ = Vertex(glm::vec3(width[1], -half, depth[1]), glm::vec2(texCoord, divide[0]),            glm::vec3(width[1], -norm, depth[1]) / lenght[1]); // 1, Left, Bottom   
            *itr++ = Vertex(glm::vec3(width[0], -half, depth[0]), glm::vec2(texCoord + texMulti, divide[0]), glm::vec3(width[0], -norm, depth[0]) / lenght[0]); // 2, Right, Bottom  

            *itr++ = Vertex(glm::vec3(width[1], -half, depth[1]), glm::vec2(texCoord, divide[1]),            glm::vec3(width[1], 0.f, depth[1]) / lenght[1]);   // 3, Left, Bottom   
            *itr++ = Vertex(glm::vec3(width[0], -half, depth[0]), glm::vec2(texCoord + texMulti, divide[1]), glm::vec3(width[0], 0.f, depth[0]) / lenght[0]);   // 4, Right, Bottom  
            *itr++ = Vertex(glm::vec3(0.f, half, 0.f),            glm::vec2(texCoord + texMulti, 0.f),       glm::vec3(0.f, norm, 0.f));                        // 7, middle, Top

            texCoord -= texMulti * dividedTexCoords;

            const float x = width[1];
            const float z = depth[1];

            width[1] = x * std::cos(alfa) - z * std::sin(alfa);
            depth[1] = x * std::sin(alfa) + z * std::cos(alfa);

            width[0] = x;
            depth[0] = z;
        }

        std::vector<unsigned int> indices(18 * sectors);

        auto ind = indices.begin();
        for (std::size_t s = 0; s < sectors; ++s)
        {
            *ind++ = 2 + (s * 6);
            *ind++ = 1 + (s * 6);
            *ind++ = 0 + (s * 6);

            *ind++ = 3 + (s * 6);
            *ind++ = 4 + (s * 6);
            *ind++ = 5 + (s * 6);
        }

        updateBounds(glm::vec3(-radius, -half, -radius), glm::vec3(radius, half, radius));

        return Mesh::load(vertexArray, indices);
    }

    //////////////////////////////////////////////

    float ConeMesh::getRadius() const
    {
        return m_radius;
    }

    //////////////////////////////////////////////

    float ConeMesh::getHeight() const
    {
        return m_height;
    }

    //////////////////////////////////////////////

    unsigned int ConeMesh::getSectors() const
    {
        return m_sectors;
    }

    //////////////////////////////////////////////

    bool ConeMesh::dividedTexCoords() const
    {
        return m_dividedTexCoords;
    }
}