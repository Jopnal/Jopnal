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
#include <Jopnal/Physics/Shape/BoxShape.hpp>
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

        /// vertex attribute indices
        ///
        struct VertexIndex
        {
            enum : uint32
            {
                Position,
                TexCoords,
                Normal,
                Color,
                ModelMatrix,
                Tangent = ModelMatrix + 4,
                BiTangent
            };
        };

    public:

        /// \brief Constructor
        ///
        /// Does not initialize any vertices.
        ///
        /// \param name Name of the resource
        ///
        Mesh(const std::string& name);

        /// \brief Copy constructor
        ///
        /// \warning A mesh on OpenGL ES 2.0 cannot be copied. The resulting copied
        ///          mesh won't be valid.
        ///
        /// \param other The other mesh to be copied
        /// \param newName Name of the new mesh
        ///
        Mesh(const Mesh& other, const std::string& newName);


        /// \brief Load mesh from memory
        ///
        /// This function takes the vertex data in binary format. The order of vertex components:
        /// Position, texture coordinated, normal, tangent, bi-tangent, color
        ///
        /// \param vertexData Pointer to the vertex data
        /// \param vertexBytes Size of the vertex data buffer in bytes
        /// \param vertexComponents The vertex components
        /// \param indexData Pointer to the index data
        /// \param indexSize Size of a **single** index in bytes
        /// \param indexAmount Amount of indices
        /// \param calculateBounds Automatically calculate the bounds?
        ///
        /// \return True if loaded successfully
        /// 
        bool load(const void* vertexData, const uint32 vertexBytes, const uint32 vertexComponents, const void* indexData = nullptr, const uint16 indexSize = 0, const uint32 indexAmount = 0, const bool calculateBounds = false);

        /// \brief Load mesh from memory using default vertex format
        ///
        /// \param vertexArray Container holding the vertex data
        /// \param indexArray Container holding index data
        /// \param calculateBounds Automatically calculate the bounds?
        ///
        /// \return True if successfully loaded
        ///
        bool load(const std::vector<Vertex>& vertexArray, const std::vector<unsigned int>& indexArray, const bool calculateBounds = false);

        /// \brief Draw this mesh
        ///
        /// Using this function requires that the shader state has been properly configured.
        /// This function will set up the vertex attributes pointers, buffer bindings and call
        /// glDrawElements or glDrawArrays depending on whether there are indices or not.
        ///
        void draw() const;

        /// \brief Destroy this mesh
        ///
        /// After this call, this mesh won't be valid and cannot be used in drawing.
        /// The vertex & index data is removed from the GPU memory.
        ///
        void destroy();

        /// \brief Get the bounds of this mesh
        ///
        /// \return Bounds of this mesh
        ///
        const std::pair<glm::vec3, glm::vec3>& getBounds() const;

        /// \brief Get the vertex amount
        ///
        /// \return The vertex amount
        ///
        unsigned int getVertexAmount() const;

        /// \brief Get the total vertex size
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
        /// \return The byte offset. This can be passed to glVertexAttribPointer etc.
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
        /// \return The size of a **single** in bytes
        ///
        /// \see getElementAmount()
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

        /// \brief Manually update the bounds of this mesh
        ///
        /// \param min The minimum coordinated
        /// \param max The maximum coordinates
        ///
        void updateBounds(const glm::vec3& min, const glm::vec3& max);

        /// \brief Get the culling shape
        ///
        /// \return Reference to the internal shape
        ///
        BoxShape& getCullingShape() const;

        /// \brief Get the size of a vertex with the given format
        ///
        /// \param components Vertex components
        ///
        /// \return Size of the vertex
        ///
        static uint16 getVertexSize(const uint32 components);

        /// \brief Get the default mesh
        ///
        /// The default mesh is a box with a size of 1.
        ///
        /// \return Reference to the mesh
        ///
        static Mesh& getDefault();

    private:

        bool updateVertexAttributes() const;


        VertexBuffer m_vertexbuffer;                ///< The vertex buffer
        VertexBuffer m_indexbuffer;                 ///< The index buffer
        mutable BoxShape m_shape;                   ///< Culling shape
        std::pair<glm::vec3, glm::vec3> m_bounds;   ///< Mesh bounds
        uint32 m_vertexComponents;                  ///< Vertex components this mesh has
        uint16 m_elementSize;                       ///< Element size
        uint16 m_vertexSize;                        ///< Vertex size
    };
}

/// \class jop::Mesh
/// \ingroup graphics

#endif