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
        m_sectors(0),
        m_height(0.f),
		m_dividedTexCoords(false)
    {}

    CylinderMesh::CylinderMesh(const CylinderMesh& other, const std::string& newName)
        : Mesh(newName),
        m_radius(0.f),
        m_sectors(0),
        m_height(0.f),
		m_dividedTexCoords(false)
    {
		load(other.m_radius, other.m_height, other.m_sectors, other.m_dividedTexCoords);
    }

    //////////////////////////////////////////////

	bool CylinderMesh::load(const float radius, const float height, const int sectors, const bool dividedTexCoords)
	{
		m_radius = radius;
		m_height = height;
		m_dividedTexCoords = dividedTexCoords;

		const float half = 0.5f * height;
		const float norm = 1.f;
		float divide = 0.f;

		if (dividedTexCoords)
		divide = 0.25f;

		float width[2] = { NULL };
		float depth[2] = {radius, radius};

        const float alfa = ((glm::two_pi<float>()*radius) / static_cast<float>(sectors)) / radius;
        width[1] = glm::sqrt(((radius*radius) + (radius*radius)) - ((2 * radius*radius*glm::cos((alfa)))));
        width[0] = width[1];

        width[1] = (width[0] * glm::cos(alfa)) - (radius*glm::sin(alfa));
        depth[1] = (width[0] * glm::sin(alfa)) + (radius*glm::cos(alfa));

		float texMulti = 1.f / (static_cast<float>(sectors));
        float texCoord = 1.f-texMulti;

        std::vector<Vertex> vertexArray((8 * sectors)+1);
        auto itr = vertexArray.begin();
        for (std::size_t s = 0; s < sectors; ++s)
        {
			float lenght[2] = {glm::sqrt((width[0] * width[0]) + 1.f + (depth[0] * depth[0])), glm::sqrt((width[1] * width[1]) + 1.f + (depth[1] * depth[1]))};

           
			*itr++ = Vertex(glm::vec3(0.f, -half, 0.f), glm::vec2(texCoord + texMulti, 1.f), glm::vec3(0.f, -norm, 0.f));                         // 0, middle, Bottom    
			*itr++ = Vertex(glm::vec3(width[1], -half, depth[1]), glm::vec2(texCoord, 1.f-divide), glm::vec3(width[1], -norm, depth[1]) / lenght[1]);              // 1, Left, Bottom   
			*itr++ = Vertex(glm::vec3(width[0], -half, depth[0]), glm::vec2(texCoord + texMulti, 1.f - divide), glm::vec3(width[0], -norm, depth[0]) / lenght[0]);     // 2, Right, Bottom  

			*itr++ = Vertex(glm::vec3(width[1], -half, depth[1]), glm::vec2(texCoord, 1.f - divide), glm::vec3(width[1], 0.f, depth[1]) / lenght[1]);               // 3, Left, Bottom   
			*itr++ = Vertex(glm::vec3(width[0], -half, depth[0]), glm::vec2(texCoord + texMulti, 1.f - divide), glm::vec3(width[0], 0.f, depth[0]) / lenght[0]);    // 4, Right, Bottom  
			*itr++ = Vertex(glm::vec3(width[1], half, depth[1]), glm::vec2(texCoord, 0.f + divide), glm::vec3(width[1], 0.f, depth[1]) / lenght[1]);                // 5, Left, Top    
			*itr++ = Vertex(glm::vec3(width[0], half, depth[0]), glm::vec2(texCoord + texMulti, 0.f + divide), glm::vec3(width[0], 0.f, depth[0]) / lenght[0]);     // 6, Right, Top    
			*itr++ = Vertex(glm::vec3(0.f, half, 0.f), glm::vec2(texCoord + texMulti, 0.f), glm::vec3(0.f, norm, 0.f));                           // 7, middle, Top  

            texCoord -= texMulti;

            float x = width[1];
            float z = depth[1];

            width[1] = (x*glm::cos(alfa)) - (z*glm::sin(alfa));
            depth[1] = (x*glm::sin(alfa)) + (z*glm::cos(alfa));

            width[0] = x;
            depth[0] = z;
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

    float CylinderMesh::getHeight() const
    {
        return m_height;
    }

    //////////////////////////////////////////////

	bool CylinderMesh::dividedTexCoords() const
    {
		return m_dividedTexCoords;
    }
}