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
#include <Jopnal/Graphics/Material.hpp>
#include <Jopnal/Graphics/Vertex.hpp>
#include <Jopnal/Graphics/Transform.hpp>
#include <vector>

//////////////////////////////////////////////


namespace jop
{
    class Mesh;

    class JOP_API Model : public Resource
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

        /// \brief Constructor
        ///
        /// \param name Name of this resource. Must be the file path if applicable.
        ///
        Model(const std::string& name);

        /// \brief Loads a .obj model from file
        ///
        /// Loads .obj and copies data to their containers (positions, normals, texcoords, indices)
        /// Assigns data to index and vertex buffers
        ///
        /// \param filePath The path to the file you want to load
        /// \param options Extra options for loading
        ///
        bool load(const std::string& filePath, const LoadOptions& options = DefaultOptions);

        /// \brief Load a model from vertices and indices
        ///
        /// This will bind the default material
        ///
        /// \param vertices The vertices
        /// \param indices The indices
        ///
        /// \return True if successful
        ///
        bool load(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

        /// \brief Load a model from vertices and indices
        ///
        /// \param vertices The vertices
        /// \param indices The indices
        /// \param material The material to use
        ///
        /// \return True if successful
        ///
        bool load(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Material& material);


        /// \brief Get the mesh
        ///
        /// \return Pointer to the mesh
        ///
        std::weak_ptr<const Mesh> getMesh() const;

        /// \brief Set the mesh
        ///
        /// \param mesh Reference to the mesh
        ///
        void setMesh(const Mesh& mesh);

        /// \brief Get the material
        ///
        /// \return Reference to the material
        ///
        const Material& getMaterial() const;

        /// \brief Set the material
        ///
        /// The material will be copied, so there's not need to keep the original around.
        /// Respectively, you'll need to pass it again should you make any changes to it.
        ///
        /// \param material The new material
        ///
        void setMaterial(const Material& material);


        /// \brief Get the element amount
        ///
        /// This will return zero if there are no indices.
        ///
        /// \return The element amount
        ///
        std::size_t getElementAmount() const;


        /// \brief Get the default model
        ///
        /// The default model is a cube with a size of 1.
        ///
        /// \return Reference to the model
        ///
        static Model& getDefault();

    private:

        std::weak_ptr<const Mesh> m_mesh;   ///< The mesh
        Material m_material;                ///< The material

    };
}

#endif