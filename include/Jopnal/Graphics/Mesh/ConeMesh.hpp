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

#ifndef JOP_CONEMESH_HPP
#define JOP_CONEMESH_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Mesh/Mesh.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API ConeMesh : public Mesh
    {
    public:

        /// \copydoc Mesh::Mesh()
        ///
        ConeMesh(const std::string& name);

        /// \copydoc Mesh::Mesh(const Mesh&, const std::string&)
        ///
        ConeMesh(const ConeMesh& other, const std::string& newName);


        /// \brief Load a Cone
        ///
        /// \param radius The radius of the Cone's lowest ring
        /// \param height How tall will the Cone be
        /// \param sectors How many blocks will form the exterior surface
        /// \param dividedTexCoords If true side will use coordinates 0.25-0.1 from texture and rest is used for bottom
        ///
        /// \return True if successful
        ///
        bool load(const float radius, const float height, const unsigned int sectors, const bool dividedTexCoords = false);

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

        /// \brief Check if this cone uses normalized texture coordinates
        ///
        /// \return True if normalized
        ///
        bool dividedTexCoords() const;

    private:

        float m_radius;             ///< The radius
        float m_height;                ///< The height
        unsigned int m_sectors;     ///< Sectors
        bool m_dividedTexCoords;    ///< Normalized texture coordinates
    };
}

/// \class jop::ConeMesh
/// \ingroup graphics

#endif