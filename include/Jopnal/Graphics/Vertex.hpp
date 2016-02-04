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

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Vertex
    {
    public:

        ///\brief default constructor
        ///
        Vertex();

        ///\brief default deconstructor
        ///
        ~Vertex();

        ///\brief construct the vertex from its position
        ///
        ///\param position Vertex position
        ///
        Vertex(const glm::vec3& position);

        ///\brief construct the vertex from its position and text coordinates
        ///
        ///\param position vertex position
        ///\param texCoords vertex texture coordinates
        ///
        Vertex(const glm::vec3& position, const glm::vec2& texCoords);

        ///\brief construct the vertex from its position and normal vector 
        ///
        ///\param position vertex position
        ///\param normalVector vertex normal vector
        ///
        Vertex(const glm::vec3& position, const glm::vec3& normalVector);

        ///\brief construct the vertex from its position, text coordinates and normal vector
        ///
        ///\param position vertex position
        ///\param texCoords vertex texture coordinates
        ///\param normalVector vertex normal vector
        ///
        Vertex(const glm::vec3& position, const glm::vec2& texCoords, const glm::vec3 normalVector);
                
    private:

        glm::vec3 m_position;       ///< 2D position of the vertex
        glm::vec2 m_texCoords;      ///< coordinates of the texture's pixel to map to vertex
        glm::vec3 m_normalVector;   ///< normal vector of the vertex 

    };
}
#endif

/// \class Vertex
/// \ingroup Graphics
///
/// Do explanation about the class