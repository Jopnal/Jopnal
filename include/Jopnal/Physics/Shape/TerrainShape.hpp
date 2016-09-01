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

#ifndef JOP_TERRAINSHAPE_HPP
#define JOP_TERRAINSHAPE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics/Shape/CollisionShape.hpp>
#include <glm/vec3.hpp>
#include <vector>

//////////////////////////////////////////////


struct btIndexedMesh;
class btStridingMeshInterface;

namespace jop
{
    class JOP_API TerrainShape : public CollisionShape
    {
    public:

        /// Terrain ray cast info
        ///
        struct RayInfo
        {
            /// Constructor
            ///
            RayInfo();

            unsigned int triangleIndex; ///< Triangle index
            glm::vec3 triangle[3];      ///< Triangle points
            bool hit;                   ///< Was any triangle hit?
        };

    public:

        /// \brief Constructor
        ///
        /// \param name Name of the resource
        ///
        TerrainShape(const std::string& name);

        /// \brief Destructor
        ///
        ~TerrainShape() override;


        /// \brief Load this shape
        ///
        /// \param points Terrain points, must be triangles
        ///
        /// \return True if successful
        ///
        bool load(const std::vector<glm::vec3>& points);

        /// \brief Load this shape
        ///
        /// \param points Terrain mesh points
        /// \param indices The indices
        ///
        /// \return True if successful
        ///
        bool load(const std::vector<glm::vec3>& points, const std::vector<unsigned int>& indices);

        /// \brief Perform a ray cast on this terrain shape
        ///
        /// \param start Start ray position
        /// \param ray Ray to cast from start
        ///
        /// \return Ray cast info
        ///
        RayInfo checkRay(const glm::vec3& start, const glm::vec3& ray) const;

    private:

        std::unique_ptr<btStridingMeshInterface> m_mesh;    ///< Mesh interface
        std::unique_ptr<btIndexedMesh> m_indMesh;           ///< Indexed mesh descriptor
        std::vector<glm::vec3> m_indMeshPoints;             ///< Indexed mesh vertices
        std::vector<unsigned int> m_indMeshIndices;         ///< Indexed mesh indices
    };
}

/// \class jop::TerrainShape
/// \ingroup physics

#endif