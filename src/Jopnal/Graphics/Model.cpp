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

// Headers
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
    Model::Model()
        : m_material    (Material::getDefault()),
          m_mesh        ()
    {}

    //////////////////////////////////////////////

    Model::Model(const Mesh& mesh, const Material& material)
        : m_material    (material),
          m_mesh        ()
    {
        setMesh(mesh);
    }

    //////////////////////////////////////////////

    Model::Model(const Mesh& mesh)
        : m_material    (Material::getDefault()),
          m_mesh        ()
    {
        setMesh(mesh);
    }

    //////////////////////////////////////////////

    std::weak_ptr<const Mesh> Model::getMesh() const
    {
        return m_mesh;
    }

    //////////////////////////////////////////////

    void Model::setMesh(const Mesh& mesh)
    {
        m_mesh = std::weak_ptr<const Mesh>(std::static_pointer_cast<const Mesh>(mesh.shared_from_this()));
    }

    //////////////////////////////////////////////

    const Material& Model::getMaterial() const
    {
        return m_material;
    }

    //////////////////////////////////////////////

    void Model::setMaterial(const Material& material)
    {
        m_material = material;
    }

    //////////////////////////////////////////////

    unsigned int Model::getVertexAmount() const
    {
        return m_mesh.expired() ? 0 : m_mesh.lock()->getVertexAmount();
    }

    //////////////////////////////////////////////

    unsigned int Model::getElementAmount() const
    {
        return m_mesh.expired() ? 0 : m_mesh.lock()->getElementAmount();
    }

    //////////////////////////////////////////////

    const Model& Model::getDefault()
    {
        static std::weak_ptr<const Mesh> defMesh;
        static Model model;

        if (defMesh.expired())
        {
            defMesh = std::static_pointer_cast<const Mesh>(Mesh::getDefault().shared_from_this());

            model.setMesh(*std::static_pointer_cast<const Mesh>(defMesh.lock()));
        }

        return model;
    }
}