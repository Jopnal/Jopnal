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
#include <Jopnal/Graphics/Mesh.hpp>
#include <vector>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Model
    {
    public:

        /// \brief Constructor
        ///
        Model();

        /// \brief Constructor for initializing with a mesh
        ///
        /// \param mesh Mesh to initialize with
        ///
        Model(const Mesh& mesh);

        /// \brief Constructor for initializing with a mesh and a material
        ///
        /// \param mesh Mesh to initialize with
        /// \param material Material to initialize with
        ///
        Model(const Mesh& mesh, const Material& material);

        /// \brief Loads a .obj model from file
        ///
        /// Loads .obj and copies data to their containers (positions, normals, texcoords, indices)
        /// Assigns data to index and vertex buffers
        ///
        /// \param filePath The path to the file you want to load
        /// \param options Extra options for loading
        ///
        bool load(const std::string& filePath, const Mesh::LoadOptions& options = Mesh::DefaultOptions);


        /// \brief Get the mesh
        ///
        /// \return Pointer to the mesh
        ///
        WeakReference<const Mesh> getMesh() const;

        /// \brief Set the mesh
        ///
        /// \param mesh Reference to the mesh
        ///
        /// \return Reference to self
        ///
        Model& setMesh(const Mesh& mesh);

        /// \brief Get the material
        ///
        /// \return Reference to the material
        ///
        WeakReference<Material> getMaterial();

        /// \copydoc getMaterial()
        ///
        WeakReference<const Material> getMaterial() const;

        /// \brief Set the material
        ///
        /// The material will be copied, so there's not need to keep the original around.
        /// Respectively, you'll need to pass it again should you make any changes to it.
        ///
        /// \param material The new material
        ///
        /// \return Reference to self
        ///
        Model& setMaterial(const Material& material);

        
        /// \copydoc Mesh::getVertexAmount()
        ///
        unsigned int getVertexAmount() const;

        /// \copydoc Mesh::getElementAmount()
        ///
        unsigned int getElementAmount() const;


        /// \brief Get the default model
        ///
        /// The default model is a cube with a size of 1.
        ///
        /// \return Reference to the model
        ///
        static const Model& getDefault();

    private:

        WeakReference<Material> m_material; ///< The material
        WeakReference<const Mesh> m_mesh;   ///< The mesh

    };
}

#endif