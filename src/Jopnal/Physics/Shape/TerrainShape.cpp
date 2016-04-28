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
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
    TerrainShape::RayInfo::RayInfo()
        : triangleIndex (0),
          triangle      (0.f),
          hit           (false)
    {}

    //////////////////////////////////////////////

    TerrainShape::TerrainShape(const std::string& name)
        : CollisionShape(name)
    {}

    TerrainShape::~TerrainShape()
    {}

    //////////////////////////////////////////////

    bool TerrainShape::load(const std::vector<glm::vec3>& points)
    {
        if (points.empty())
            return false;

        auto& mesh = static_cast<btTriangleMesh&>(*(m_mesh = std::make_unique<btTriangleMesh>(true, false)));
        mesh.preallocateVertices(points.size());
        
        for (auto itr = points.begin(); itr != points.end(); itr += 3)
        {
            auto st = itr;
            auto nd = itr + 1;
            auto rd = itr + 2;
            
            mesh.addTriangle(btVector3(st->x, st->y, st->z), btVector3(nd->x, nd->y, nd->z), btVector3(rd->x, rd->y, rd->z));
        }

        m_shape = std::make_unique<btBvhTriangleMeshShape>(m_mesh.get(), true);
        m_shape->setUserPointer(this);

        return true;
    }

    //////////////////////////////////////////////

    bool TerrainShape::load(const std::vector<glm::vec3>& points, const std::vector<unsigned int>& indices)
    {
        if (points.empty() || indices.empty())
            return false;

        m_indMeshPoints = points;
        m_indMeshIndices = indices;

        auto& mesh = static_cast<btTriangleIndexVertexArray&>(*(m_mesh = std::make_unique<btTriangleIndexVertexArray>()));
        mesh.preallocateIndices(indices.size());

        m_indMesh = std::make_unique<btIndexedMesh>();
        m_indMesh->m_numVertices = points.size();
        m_indMesh->m_numTriangles = indices.size() / 3;
        m_indMesh->m_triangleIndexBase = reinterpret_cast<const unsigned char*>(m_indMeshIndices.data());
        m_indMesh->m_triangleIndexStride = sizeof(unsigned int) * 3;
        m_indMesh->m_vertexBase = reinterpret_cast<const unsigned char*>(m_indMeshPoints.data());
        m_indMesh->m_vertexStride = sizeof(glm::vec3);

        mesh.addIndexedMesh(*m_indMesh);
        
        return false;
    }

    //////////////////////////////////////////////

    TerrainShape::RayInfo TerrainShape::checkRay(const glm::vec3& start, const glm::vec3& ray) const
    {
        struct Callback : btTriangleCallback
        {
            TerrainShape::RayInfo info;

            void processTriangle(btVector3* triangle, int, int triangleIndex) override
            {
                info.triangle.x = triangle->x();
                info.triangle.y = triangle->y();
                info.triangle.z = triangle->z();

                info.triangleIndex = static_cast<unsigned int>(triangleIndex);
            }

        } cb;

        const glm::vec3 dest = start + ray;

        static_cast<btBvhTriangleMeshShape&>(*m_shape).performRaycast(&cb, btVector3(start.x, start.y, start.z), btVector3(dest.x, dest.y, dest.z));

        return cb.info;
    }
}