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

#include <Jopnal/Graphics/Mesh/CylinderMesh.hpp>

#include <glm/gtc/constants.hpp>
#include <cmath>

#endif

//////////////////////////////////////////////


namespace jop
{
    CylinderMesh::CylinderMesh(const std::string& name)
        : Mesh(name),
        m_radius(0.f),
        m_sectors(0.f),
        m_size(0.f),
        m_normTexCoords(true)
    {}

    CylinderMesh::CylinderMesh(const CylinderMesh& other, const std::string& newName)
        : Mesh(newName),
        m_radius(0.f),
        m_sectors(0.f),
        m_size(0.f),
        m_normTexCoords(true)
    {
        load(other.m_radius, other.m_size, other.m_sectors, other.m_normTexCoords);
    }

    //////////////////////////////////////////////

    bool CylinderMesh::load(const float radius, const float size, const unsigned int sectors, const bool normalizedTexCoords)
    {
        m_radius = radius;
        m_sectors = sectors;
        m_size = size;
        m_normTexCoords = normalizedTexCoords;

        const float height = 0.5f * size;
        const float norm = 1.f;
        float rdepth = radius;
        float ldepth = rdepth;
        float rwidth = (height)*(glm::two_pi<float>() * radius) / sectors+0.005f;
        float lwidth = -rwidth;
        const float alfa = glm::two_pi<float>() / sectors;
        float texMulti = 1.f/ sectors;
        float texCoord = 0;

        std::vector<Vertex> vertexArray((8 * sectors)+1);
        auto itr = vertexArray.begin();

        for (std::size_t s = 0; s < sectors; ++s)
        {
            float rlenght = glm::sqrt((rwidth * rwidth) + 1.f + (rdepth * rdepth));
            float llenght = glm::sqrt((lwidth * lwidth) + 1.f + (ldepth * ldepth));
           
            *itr++ = Vertex(glm::vec3(0.f, -height, 0.f), glm::vec2(0.f, 1.f), glm::vec3(0.f, -norm, 0.f));                         // 0, middle, Bottom    
            *itr++ = Vertex(glm::vec3(lwidth, -height, ldepth), glm::vec2(texCoord, 0.f), glm::vec3(lwidth, -norm, ldepth)/llenght);              // 1, Left, Bottom   
            *itr++ = Vertex(glm::vec3(rwidth, -height, rdepth), glm::vec2(texCoord + texMulti, 0.f), glm::vec3(rwidth, -norm, rdepth)/rlenght);     // 2, Right, Bottom  

            *itr++ = Vertex(glm::vec3(lwidth, -height, ldepth), glm::vec2(texCoord, 0.f), glm::vec3(lwidth, 0.f, ldepth)/llenght);               // 3, Left, Bottom   
            *itr++ = Vertex(glm::vec3(rwidth, -height, rdepth), glm::vec2(texCoord + texMulti, 0.f), glm::vec3(rwidth, 0.f, rdepth)/rlenght);    // 4, Right, Bottom  
            *itr++ = Vertex(glm::vec3(lwidth, height, ldepth), glm::vec2(texCoord, 1.f), glm::vec3(lwidth, 0.f, ldepth)/llenght);                // 5, Left, Top    
            *itr++ = Vertex(glm::vec3(rwidth, height, rdepth), glm::vec2(texCoord + texMulti, 1.f), glm::vec3(rwidth, 0.f, rdepth)/rlenght);     // 6, Right, Top    
            *itr++ = Vertex(glm::vec3(0.f, height, 0.f), glm::vec2(1.f, 1.f), glm::vec3(0.f, norm, 0.f));                           // 7, middle, Top  

            texCoord += texMulti;

            float x = rwidth;
            float z = rdepth;
            rwidth = (x*glm::cos(alfa)) - (z*glm::sin(alfa));
            rdepth = (x*glm::sin(alfa)) + (z*glm::cos(alfa));

            x = lwidth;
            z = ldepth;
            lwidth = (x*glm::cos(alfa)) - (z*glm::sin(alfa));
            ldepth = (x*glm::sin(alfa)) + (z*glm::cos(alfa));
        }
        std::vector<unsigned int> indices(24 * sectors);
        auto ind = indices.begin();
        for (std::size_t s = 0; s < sectors; ++s)
        {
            *ind++ = 2 + (s * 8);
            *ind++ = 1 + (s * 8);
            *ind++ = 0 + (s * 8);

            *ind++ = 3 + (s * 8);
            *ind++ = 4 + (s * 8);
            *ind++ = 5 + (s * 8);
            *ind++ = 5 + (s * 8);
            *ind++ = 4 + (s * 8);
            *ind++ = 6 + (s * 8);

            *ind++ = 5 + (s * 8);
            *ind++ = 6 + (s * 8);
            *ind++ = 7 + (s * 8);
        }

        return Mesh::load(vertexArray, indices);
    }

    //////////////////////////////////////////////

    float CylinderMesh::getRadius() const
    {
        return m_radius;
    }

    //////////////////////////////////////////////

    unsigned int CylinderMesh::getSectors() const
    {
        return m_sectors;
    }


    //////////////////////////////////////////////

    float CylinderMesh::getSize() const
    {
        return m_size;
    }

    //////////////////////////////////////////////

    bool CylinderMesh::normalizedTexCoords() const
    {
        return m_normTexCoords;
    }
}