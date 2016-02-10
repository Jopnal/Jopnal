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

        struct LoadOptions
        {
            Transform transform;
            bool centerOrigin;
            bool flipV;
            bool generateNormals;
        };

    public:

        Model();

        /// \brief Loads a .obj model from file
        ///
        /// Loads .obj and copies data to their containers (positions, normals, texcoords, indices)
        /// Assigns data to index and vertex buffers
        ///
        /// \param filePath The path to the file you want to load
        ///
        bool load(const std::string& filePath);

        ///
        ///
        bool load(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

        ///
        ///
        bool load(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Material& material);


        std::weak_ptr<const Mesh> getMesh() const;

        const Material& getMaterial() const;


        std::size_t getElementAmount() const;


        /// \brief Get the default model
        ///
        /// The default model is a cube with a size of 1.
        ///
        /// \return Reference to the model
        ///
        static std::weak_ptr<Model> getDefault();

    private:

        std::weak_ptr<Mesh> m_mesh;
        Material m_material;

    };
}

#endif