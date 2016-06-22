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

//Headers
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
	CircleMesh::CircleMesh(const std::string& name)
		: Mesh(name),
		m_radius(0.f)
	{}

	CircleMesh::CircleMesh(const CircleMesh& other, const std::string& newName)
		: Mesh(newName),
		m_radius(0.f)
	{
		load(other.m_radius, other.m_points);
	}

	//////////////////////////////////////////////

	bool CircleMesh::load(const float radius, const int pointCount)
	{
		m_radius = radius * 0.5;

		//float position[3]({ 2.f, 3.f, 0.f }); //X, Y, Z; middle point


		if (pointCount > 2)
		{
			float degrees = 360 / pointCount;
			float totaldegrees = degrees;

			struct pos
			{
				float x, y, z;

				pos(float x, float y, float z) : x(x), y(y), z(z){}
			};

			std::vector<pos> points;

			points.emplace_back(0.f, m_radius, 0.f);

			for (int i = 0; i <= pointCount; ++i) // start 0, equal to pointCount because first point already done
			{
				points.emplace_back((points[i].x * cos(totaldegrees) - points[i].y * sin(totaldegrees)), (points[i].x * sin(totaldegrees) - points[i].y * cos(totaldegrees)), 0);
				totaldegrees += degrees;
			}

			//FIX degrees =/= radians


			std::vector<Vertex> vertexarray;

			for (int i = 0; i < points.size(); ++i)
			{
				//push 3 vertices? 1 for each point?
				//if so, what about the texture position
				vertexarray.emplace_back(Vertex{
					glm::vec3(0, 0, 0),
					glm::vec2((((points[i].x / m_radius) + 1) / 2), (((points[i].y / m_radius) + 1) / 2)),
					glm::vec3(0.f, 0.f, 1.f) });

				vertexarray.emplace_back(Vertex{
					glm::vec3(points[i].x, points[i].y, points[i].z),
					glm::vec2((((points[i].x / m_radius) + 1) / 2), (((points[i].y / m_radius) + 1) / 2)),
					glm::vec3(0.f, 0.f, 1.f) });

				vertexarray.emplace_back(Vertex{
					glm::vec3(points[i].x, points[i].y, points[i].z),
					glm::vec2((((points[i].x / m_radius) + 1) / 2), (((points[i].y / m_radius) + 1) / 2)),
					glm::vec3(0.f, 0.f, 1.f) });
			}


			//iterate through indices backwards

			std::vector<unsigned int> indices;

			const unsigned int origo = 0;
			unsigned int point = 1;


			for (auto itr = vertexarray.rbegin(); itr != vertexarray.rend(); ++itr)
			{
				indices.push_back(origo);
				indices.push_back(point);
				++point;
				if (itr->position.y != vertexarray[0].position.y)
				{
					indices.push_back(point);
				}
			}
			indices.push_back(1);
			return Mesh::load(vertexarray, indices);
		}
		return false;
	}

	//////////////////////////////////////////////

	float CircleMesh::getSize() const
	{
		return m_radius;
	}
}