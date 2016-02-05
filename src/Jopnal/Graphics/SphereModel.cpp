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

#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923

//////////////////////////////////////////////

namespace jop
{
    SphereModel::SphereModel(const float radius, const unsigned int rings, const unsigned int sectors) :
        Model()
    {
        const float R = 1.0f / (float)(rings - 1.0f); // Rings
        const float S = 1.0f / (float)(sectors - 1.0f); // Sectors
        std::size_t s, r;

        std::vector<float> positions;
        std::vector<float> normals;
        std::vector<float> texcoords;
        std::vector<unsigned int> indices;

        positions.resize(rings * sectors * 3);
        normals.resize(rings * sectors * 3);
        texcoords.resize(rings * sectors * 2);

        std::vector<float>::iterator v = positions.begin();
        std::vector<float>::iterator n = normals.begin();
        std::vector<float>::iterator t = texcoords.begin();

        for (r = 0; r < rings; ++r)
        {
            for (s = 0; s < sectors; ++s)
            {
                
                float const y = sin(-M_PI_2 + M_PI * r * R);
                float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
                float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

                *t++ = s*S;
                *t++ = r*R;

                *v++ = x * radius;
                *v++ = y * radius;
                *v++ = z * radius;

                *n++ = x;
                *n++ = y;
                *n++ = z;
            }
        }

        indices.resize(rings * sectors * 4);
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

        //Different way to do this?--------------------------------------
        std::vector<Vertex> vertexArray;
        vertexArray.reserve(positions.size() / 3);

        for (std::size_t i = 0; i < vertexArray.capacity(); ++i)
        {
            vertexArray.emplace_back
                (
                glm::vec3(positions[i * 3], positions[i * 3 + 1], positions[i * 3 + 2]),
                texcoords.size() > i * 2 ? glm::vec2(texcoords[i * 2], texcoords[i * 2 + 1]) : glm::vec2(),
                normals.size() > i * 3 ? glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]) : glm::vec3()
                );
        }
        //---------------------------------------------------------
        load(vertexArray, indices);
    }

}