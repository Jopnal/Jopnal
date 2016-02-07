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
    SphereModel::SphereModel(const float radius, const unsigned int rings, const unsigned int sectors)
        : Model()
    {
        const float R = 1.0f / static_cast<float>(rings - 1); // Rings
        const float S = 1.0f / static_cast<float>(sectors - 1); // Sectors
        std::size_t s, r;

        std::vector<Vertex> vertexArray(rings * sectors);
        auto itr = vertexArray.begin();

        for (r = 0; r < rings; ++r)
        {
            for (s = 0; s < sectors; ++s)
            {
                static const float pi1 = glm::pi<float>(); // pi
                static const float pi2 = glm::half_pi<float>(); // half pi

                const float y = sin(-pi2 + pi1 * r * R);
                const float x = cos(2 * pi1 * s * S) * sin(pi1 * r * R);
                const float z = sin(2 * pi1 * s * S) * sin(pi1 * r * R);

                itr->position.x = x * radius;
                itr->position.y = y * radius;
                itr->position.z = z * radius;

                itr->texCoords.x = s * S;
                itr->texCoords.y = r * R;

                itr->normalVector.x = x;
                itr->normalVector.y = y;
                itr->normalVector.z = z;

                ++itr;
            }
        }

        std::vector<unsigned int> indices(rings * sectors * 4);
        std::vector<unsigned int>::iterator i = indices.begin();
        for (r = 0; r < rings - 1; ++r) 
        {
            for (s = 0; s < sectors - 1; ++s)
            {
                *i++ = r * sectors + s;
                *i++ = r * sectors + (s + 1);
                *i++ = (r + 1) * sectors + (s + 1);
                *i++ = (r + 1) * sectors + s;
            }
        }

        load(vertexArray, indices);
    }
}