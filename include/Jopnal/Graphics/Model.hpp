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

#ifndef JOP_MODEL_HPP
#define JOP_MODEL_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Resource.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Model : public Resource
    {
    public:

        /// \brief Default constructor
        ///
        Model();


        /// \brief Loads a .obj model from file
        ///
        /// Loads .obj and copies data to their containers (positions, normals, texcoords, indices)
        /// Assigns data to index and vertex buffers
        ///
        /// \param filePath The path to the file you want to load
        ///
        bool load(const std::string& filePath);

        /// \brief Loads model from memory
        ///
        /// \param vertexArray Container holding the vertex data
        /// \param indices Container holding index data
        ///
        bool load(const std::vector<Vertex>& vertexArray, const std::vector<unsigned int>& indexArray);


        /// \brief Returns index buffer
        ///
        const VertexBuffer& getIndexBuffer() const;

        /// \brief Returns vertex buffer
        ///
        const VertexBuffer& getVertexBuffer() const;

    private:

        VertexBuffer m_vertexbuffer;    ///< The vertex buffer
        VertexBuffer m_indexbuffer;     ///< The index buffer
    };
}

#endif

/// \class Model
/// \ingroup graphics
///
/// NOTE: Currently only supports .obj format.