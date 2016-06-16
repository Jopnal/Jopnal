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

#ifndef JOP_VERTEX_HPP
#define JOP_VERTEX_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Vertex
    {
    public:

        /// \brief Default constructor
        ///
        Vertex() = default;

        /// \brief Construct the vertex from its position
        ///
        /// \param position Vertex position
        ///
        Vertex(const glm::vec3& position);

        /// \brief Construct the vertex from its position and text coordinates
        ///
        /// \param position Vertex position
        /// \param texCoords Vertex texture coordinates
        ///
        Vertex(const glm::vec3& position, const glm::vec2& texCoords);

        /// \brief Construct the vertex from its position and normal vector 
        ///
        /// \param position Vertex position
        /// \param normalVector Vertex normal vector
        ///
        Vertex(const glm::vec3& position, const glm::vec3& normalVector);

        /// \brief Construct the vertex from its position, text coordinates and normal vector
        ///
        /// \param position Vertex position
        /// \param texCoords Vertex texture coordinates
        /// \param normalVector Vertex normal vector
        ///
        Vertex(const glm::vec3& position, const glm::vec2& texCoords, const glm::vec3& normalVector);

        glm::vec3 position;       ///< 3D position of the vertex
        glm::vec2 texCoords;      ///< Coordinates of the texture's pixel to map to vertex
        glm::vec3 normalVector;   ///< Normal vector of the vertex 
    };
}
#endif

/// \class Vertex
/// \ingroup Graphics
///
/// 
