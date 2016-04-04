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

#ifndef JOP_MESH_HPP
#define JOP_MESH_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Resource.hpp>
#include <Jopnal/Graphics/VertexBuffer.hpp>
#include <Jopnal/Graphics/Vertex.hpp>

//////////////////////////////////////////////


namespace jop
{
    class Material;

    class JOP_API Mesh : public Resource
    {
    public:

        /// Vertex components
        ///
        enum VertexComponent : uint16
        {
            Position    = 1,
            TexCoords   = 1 << 2,
            Normal      = 1 << 3,
            Tangent     = 1 << 4,
            Color       = 1 << 5,
            BoneWeight  = 1 << 6,

            Index       = 1 << 7
        };

    public:

        /// \brief Default constructor
        ///
        /// \param name Name of this mesh
        ///
        Mesh(const std::string& name);


        bool load(const std::vector<unsigned char>& vertexData, const std::vector<unsigned char>& indices);

        /// \brief Loads model from memory
        ///
        /// \param vertexArray Container holding the vertex data
        /// \param indexArray Container holding index data
        ///
        /// \return True if successfully loaded
        ///
        bool load(const std::vector<Vertex>& vertexArray, const std::vector<unsigned int>& indexArray);


        /// \brief Get the vertex amount
        ///
        /// \return The vertex amount
        ///
        unsigned int getVertexAmount() const;

        /// \brief Get the element (index) amount
        ///
        /// \return The element amount
        ///
        unsigned int getElementAmount() const;


        /// \brief Returns index buffer
        ///
        /// \return Reference to the index buffer
        ///
        const VertexBuffer& getIndexBuffer() const;

        /// \brief Returns vertex buffer
        ///
        /// \return Reference to the index buffer
        ///
        const VertexBuffer& getVertexBuffer() const;

        /// \brief Get the default mesh
        ///
        /// \return Reference to the mesh
        ///
        static Mesh& getDefault();

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