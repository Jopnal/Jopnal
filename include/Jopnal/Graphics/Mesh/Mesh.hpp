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
#include <vector>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Mesh : public Resource
    {
    private:

        JOP_DISALLOW_COPY_MOVE(Mesh);

    public:

        /// Vertex components
        ///
        enum VertexComponent : uint32
        {
            Position    = 1,
            TexCoords   = 1 << 1,
            Normal      = 1 << 2,
            Tangent     = 1 << 3,
            BiTangent   = 1 << 4,
            Color       = 1 << 5
        };

    public:

        /// \brief Default constructor
        ///
        /// \param name Name of this mesh
        ///
        Mesh(const std::string& name);


        /// \brief Load mesh from memory
        ///
        /// This function takes the vertex data in binary format. The order of vertex components:
        /// Position, texture coordinated, normal, tangent, bi-tangent, color
        ///
        /// \param vertexData Pointer to the vertex data
        /// \param vertexBytes Size of the vertex data buffer in bytes
        /// \param vertexComponents The vertex components
        /// \param indexData Pointer to the index data
        /// \param indexSize Size of a <b>single</b> index in bytes
        /// \param indexAmount Amount of indices
        ///
        /// \return True if loaded successfully
        /// 
        bool load(const void* vertexData, const uint32 vertexBytes, const uint32 vertexComponents, const void* indexData = nullptr, const uint16 indexSize = 0, const uint32 indexAmount = 0);

        /// \brief Load mesh from memory using default vertex format
        ///
        /// \param vertexArray Container holding the vertex data
        /// \param indexArray Container holding index data
        ///
        /// \return True if successfully loaded
        ///
        bool load(const std::vector<Vertex>& vertexArray, const std::vector<unsigned int>& indexArray);

        void updateVertexAttributes(const uint64 materialAttribs) const;

        void destroy();

        const std::pair<glm::vec3, glm::vec3>& getBounds() const;

        /// \brief Get the vertex amount
        ///
        /// \return The vertex amount
        ///
        unsigned int getVertexAmount() const;

        /// \brief Get the vertex size
        ///
        /// \return Vertex size in bytes
        ///
        uint16 getVertexSize() const;

        /// \brief Get the byte offset for the given component
        ///
        /// \warning If the mesh doesn't have the queried component, the resulting
        ///          behavior is undefined
        ///
        /// \param component The vertex component
        ///
        /// \return The byte offset. This can be passed to glVertexAttribPointer() etc.
        ///
        void* getVertexOffset(const VertexComponent component) const;

        /// \brief Check if this mesh has a vertex component
        ///
        /// \param component The component to check
        ///
        /// \return True if the component exists
        ///
        bool hasVertexComponent(const uint32 component) const;

        /// \brief Get the element (index) amount
        ///
        /// \return The element amount
        ///
        unsigned int getElementAmount() const;

        /// \brief Get the element size
        ///
        /// \return Element size in bytes
        ///
        uint16 getElementSize() const;

        /// \brief Get the element type OpenGL enum
        ///
        /// The returned enum can be used with glDrawElements.
        ///
        /// \return The element type OpenGL enum
        ///
        unsigned int getElementEnum() const;

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

        void updateBounds(const glm::vec3& min, const glm::vec3& max);


        /// \brief Get the size of a vertex with the given format
        ///
        /// \param components Vertex components
        ///
        /// \return Size of the vertex
        ///
        static uint16 getVertexSize(const uint32 components);

        /// \brief Get the default mesh
        ///
        /// \return Reference to the mesh
        ///
        static Mesh& getDefault();

    private:

        VertexBuffer m_vertexbuffer;    ///< The vertex buffer
        VertexBuffer m_indexbuffer;     ///< The index buffer
        std::pair<glm::vec3, glm::vec3> m_bounds;
        uint32 m_vertexComponents;      ///< Vertex components this mesh has
        uint16 m_elementSize;           ///< Element size
        uint16 m_vertexSize;            ///< Vertex size
    };
}

#endif

/// \class jop::Mesh
/// \ingroup graphics