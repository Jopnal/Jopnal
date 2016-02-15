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

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Mesh : public Resource
    {
    public:

        /// \brief Struct for passing extra options to the load function
        ///
        struct LoadOptions
        {
            /// \brief Default constructor
            ///
            LoadOptions() = default;

            /// \brief Constructor for initialization
            ///
            /// \param centerOrigin_ Center the origin?
            /// \param flipV_ Flip the V texture coordinate?
            /// \param generateNormals_ Generate normals in case they don't exist?
            ///
            LoadOptions(const bool centerOrigin_, const bool flipV_, const bool generateNormals_);

            Transform transform;    ///< Transform for pre-transforming the vertices
            bool centerOrigin;      ///< Center the origin?
            bool flipV;             ///< Flip the V texture coordinate?
            bool generateNormals;   ///< Generate normals in case they don't exist
        };

        /// The default load options
        ///
        static const LoadOptions DefaultOptions;

    public:

        /// \brief Default constructor
        ///
        /// \param name Name of this buffer
        ///
        Mesh(const std::string& name);


        /// \brief Loads a .obj model from file
        ///
        /// Loads .obj and copies data to their containers (positions, normals, texcoords, indices)
        /// Assigns data to index and vertex buffers
        ///
        /// \param filePath The path to the file you want to load
        /// \param options Extra options for loading
        ///
        bool load(const std::string& filePath, const LoadOptions& options = DefaultOptions);

        /// \brief Loads a .obj model from file
        ///
        /// Loads .obj and copies data to their containers (positions, normals, texcoords, indices)
        /// Assigns data to index and vertex buffers
        ///
        /// \param filePath The path to the file you want to load
        /// \param options Extra options for loading
        ///
        bool load(const std::string& filePath, Material& material, const LoadOptions& options = DefaultOptions);

        /// \brief Loads model from memory
        ///
        /// \param vertexArray Container holding the vertex data
        /// \param indexArray Container holding index data
        ///
        /// \return True if successful
        ///
        bool load(const std::vector<Vertex>& vertexArray, const std::vector<unsigned int>& indexArray);


        unsigned int getVertexAmount() const;

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