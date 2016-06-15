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
#include <Jopnal/Utility/SafeReferenceable.hpp>

//////////////////////////////////////////////


namespace jop
{
    class Mesh;
    class Material;

    class JOP_API Model
    {
    public:

        /// \brief Constructor
        ///
        Model();

        /// \brief Constructor for initializing with a mesh
        ///
        /// Note that a model is not valid until it has a material also.
        ///
        /// \param mesh Mesh to initialize with
        ///
        Model(const Mesh& mesh);

        Model(const Material& material);

        /// \brief Constructor for initializing with a mesh and a material
        ///
        /// \param mesh Mesh to initialize with
        /// \param material Material to initialize with
        ///
        Model(const Mesh& mesh, const Material& material);


        Mesh* getMesh();

        /// \brief Get the mesh
        ///
        /// \return Pointer to the mesh
        ///
        const Mesh* getMesh() const;

        /// \brief Set the mesh
        ///
        /// \param mesh Reference to the mesh
        ///
        /// \return Reference to self
        ///
        Model& setMesh(const Mesh& mesh);


        Material* getMaterial();

        /// \brief Get the material
        ///
        /// \return Pointer to the material. Nullptr if none bound
        ///
        const Material* getMaterial() const;

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


        /// \brief Check if this model is valid
        ///
        /// The model is valid if both a valid mesh and material are present.
        ///
        /// \return True if valid
        ///
        bool isValid() const;

    private:

        WeakReference<Material> m_material; ///< The material
        WeakReference<Mesh> m_mesh;         ///< The mesh
    };
}

#endif