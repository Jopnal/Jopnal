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
    SphereMesh::SphereMesh(const std::string& name)
        : Mesh(name)
    {}

    SphereMesh::SphereMesh(const std::string& name, const float radius, const unsigned int rings, const unsigned int sectors, const bool normalizedTexCoords)
        : Mesh(name)
    {
        load(radius, rings, sectors, normalizedTexCoords);
    }

    //////////////////////////////////////////////

    bool SphereMesh::load(const float radius, const unsigned int rings, const unsigned int sectors, const bool normalizedTexCoords)
    {
        const float R = 1.0f / static_cast<float>(rings - 1);
        const float S = 1.0f / static_cast<float>(sectors - 1);

        const unsigned int texCoordMultS = normalizedTexCoords ? 1u : sectors;
        const unsigned int texCoordMultR = normalizedTexCoords ? 1u : rings;

        std::vector<Vertex> vertexArray(rings * sectors);
        auto itr = vertexArray.begin();

        for (std::size_t s = 0; s < sectors; ++s)
        {
            for (std::size_t r = 0; r < rings; ++r)
            {
                static const float pi1 = glm::pi<float>();
                static const float pi2 = glm::half_pi<float>();

                const float y = sin(-pi2 + pi1 * r * R);
                const float x = cos(2 * pi1 * s * S) * sin(pi1 * r * R);
                const float z = sin(2 * pi1 * s * S) * sin(pi1 * r * R);

                itr->position.x = x * radius;
                itr->position.y = y * radius;
                itr->position.z = z * radius;

                itr->texCoords.x = texCoordMultS * s * S;
                itr->texCoords.y = texCoordMultR * r * R;

                itr->normalVector.x = x;
                itr->normalVector.y = y;
                itr->normalVector.z = z;

                ++itr;
            }
        }

        std::vector<unsigned int> indices(rings * sectors * 6);
        std::vector<unsigned int>::iterator i = indices.begin();
        for (std::size_t r = 0; r < rings - 1; ++r)
        {
            for (std::size_t s = 0; s < sectors - 1; ++s)
            {
                // First triangle
                *i++ = r * sectors + s;             // 0, 0          
                *i++ = r * sectors + (s + 1);       // 1, 0
                *i++ = (r + 1) * sectors + (s + 1); // 1, 1

                // Second triangle
                *i++ = r * sectors + s;             // 0, 0    
                *i++ = (r + 1) * sectors + (s + 1); // 1, 1
                *i++ = (r + 1) * sectors + s;       // 0, 1
            }
        }

        return Mesh::load(vertexArray, indices);
    }
}