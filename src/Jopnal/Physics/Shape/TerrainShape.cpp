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
    TerrainShape::TerrainShape(const std::string& name)
        : CollisionShape(name)
    {}

    //////////////////////////////////////////////

    bool TerrainShape::load(const std::vector<glm::vec3>& points, const std::vector<unsigned int>& indices)
    {
        if (points.empty())
            return false;

        btTriangleMesh mesh(true, false);
        mesh.preallocateVertices(points.size());

        for (auto& i : points)
            mesh.findOrAddVertex(btVector3(i.x, i.y, i.z), indices.empty());
        
        if (!indices.empty())
        {
            mesh.preallocateIndices(indices.size());

            for (auto i : indices)
                mesh.addIndex(i);
        }

        m_shape = std::make_unique<btBvhTriangleMeshShape>(&mesh, true);
        m_shape->setUserPointer(this);

        return true;
    }

    //////////////////////////////////////////////

    TerrainShape::RayInfo TerrainShape::checkRay(const glm::vec3& start, const glm::vec3& ray) const
    {
        struct Callback : btTriangleCallback
        {
            TerrainShape::RayInfo info;

            void processTriangle(btVector3* triangle, int /*partId*/, int triangleIndex) override
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