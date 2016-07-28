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

#include <Jopnal/Graphics/Mesh/CapsuleMesh.hpp>

#include <glm/gtc/constants.hpp>
#include <cmath>

#endif

//////////////////////////////////////////////


namespace jop
{
	CapsuleMesh::CapsuleMesh(const std::string& name)
		: Mesh(name),
		m_radius(0.f),
        m_height(0.f),
		m_rings(0),
		m_sectors(0),
		m_normTexCoords(true)
	{}

	CapsuleMesh::CapsuleMesh(const CapsuleMesh& other, const std::string& newName)
		: Mesh(newName),
        m_radius(0.f),
        m_height(0.f),
		m_rings(0),
		m_sectors(0),
		m_normTexCoords(true)
	{
		load(other.m_radius, other.m_height, other.m_rings, other.m_sectors, other.m_normTexCoords);
	}

	//////////////////////////////////////////////

	bool CapsuleMesh::load(const float radius, const float height, const unsigned int rings, const unsigned int sectors, const bool normalizedTexCoords)
	{
		m_radius = radius;
        m_height = height;
		m_rings = rings;
		m_sectors = sectors;
		m_normTexCoords = normalizedTexCoords;

		const float half = height*0.5f;
		const float stretch = radius / (static_cast<float>(rings)+1.f);
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

				float y = sin(-pi2 + pi1 * r * R);
				const float x = cos(2 * pi1 * s * S) * sin(pi1 * r * R);
				const float z = sin(2 * pi1 * s * S) * sin(pi1 * r * R);

				if (y >= stretch)
					y += half;
				else
					y -= half;

				itr->position.x = x * radius;
				itr->position.y = y * radius;
				itr->position.z = z * radius;

				itr->texCoords.x = texCoordMultS * s * -S;
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

	//////////////////////////////////////////////

	float CapsuleMesh::getRadius() const
	{
		return m_radius;
	}

    //////////////////////////////////////////////

    float CapsuleMesh::getHeight() const
    {
        return m_height;
    }

	//////////////////////////////////////////////

	unsigned int CapsuleMesh::getRings() const
	{
		return m_rings;
	}

	//////////////////////////////////////////////

	unsigned int CapsuleMesh::getSectors() const
	{
		return m_sectors;
	}

	//////////////////////////////////////////////

	bool CapsuleMesh::normalizedTexCoords() const
	{
		return m_normTexCoords;
	}
}