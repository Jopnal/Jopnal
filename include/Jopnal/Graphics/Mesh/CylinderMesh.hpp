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

#ifndef JOP_CYLINDERMESH_HPP
#define JOP_CYLINDERMESH_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Mesh/Mesh.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API CylinderMesh : public Mesh
    {
    public:

        /// \copydoc jop::BoxMesh::BoxMesh()
        ///
        CylinderMesh(const std::string& name);

        /// \brief Copy constructor
        ///
        /// \param other The other mesh to be copied
        /// \param newName Name of the new mesh
        ///
        CylinderMesh(const CylinderMesh& other, const std::string& newName);


        /// \brief Load a Cylinder
        ///
        /// \param radius The radius of the Cylinder
        /// \param height How tall will the Cylinder be
        /// \param sectors How many blocks will form the exterior surface
        /// \param dividedTexCoords If true side will use coordinates 0.25-0.75 from texture and rest is used for top and bottom
        ///
        /// \return True if successful
        ///
		bool load(const float radius, const float height, const unsigned int sectors, const bool dividedTexCoords = true);


        /// \brief Get the radius
        ///
        /// \return The radius
        ///
        float getRadius() const;

        /// \brief Get the amount of sectors
        ///
        /// \return The sector amount
        ///
        unsigned int getSectors() const;

        /// \brief Get the size
        ///
        /// \return The size
        ///
        float getHeight() const;

        /// \brief Check if this Cylinder uses normalized texture coordinates
        ///
        /// \return True if normalized
        ///
        bool dividedTexCoords() const;

    private:

        float m_radius;             ///< The radius
        float m_height;			    ///< The height
        unsigned int m_sectors;     ///< Sectors
		bool m_dividedTexCoords;    ///< Normalized texture coordinates
    };
}

#endif

/// \class CylinderMesh
/// \ingroup Graphics
///
/// 