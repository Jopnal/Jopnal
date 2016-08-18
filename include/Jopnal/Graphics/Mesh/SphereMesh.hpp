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

#ifndef JOP_SPHEREMESH_HPP
#define JOP_SPHEREMESH_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Mesh/Mesh.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API SphereMesh : public Mesh
    {
    public:

        /// \copydoc Mesh::Mesh()
        ///
        SphereMesh(const std::string& name);

        /// \copydoc Mesh::Mesh(const Mesh&, const std::string&)
        ///
        SphereMesh(const SphereMesh& other, const std::string& newName);


        /// \brief Load this sphere
        ///
        /// \param radius The radius of the sphere
        /// \param rings How many rings will the sphere have
        /// \param normalizedTexCoords Normalize the texture coordinates?
        ///
        /// \return True if successful
        ///
        bool load(const float radius, const unsigned int rings, const bool normalizedTexCoords = true);

        /// \brief Get the radius
        ///
        /// \return The radius
        ///
        float getRadius() const;

        /// \brief Get the amount of rings
        ///
        /// \return The ring amount
        ///
        unsigned int getRings() const;

        /// \brief Get the amount of sectors
        ///
        /// \return The sector amount
        ///
        unsigned int getSectors() const;

        /// \brief Check if this sphere uses normalized texture coordinates
        ///
        /// \return True if normalized
        ///
        bool normalizedTexCoords() const;

    private:

        float m_radius;         ///< The radius
        unsigned int m_rings;   ///< Rings
        bool m_normTexCoords;   ///< Normalized texture coordinates
    };
}

/// \class jop::SphereMesh
/// \ingroup graphics

#endif