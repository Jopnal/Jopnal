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
        : m_material    (),
          m_mesh        ()
    {
        setMaterial(Material::getDefault());
        setMesh(Mesh::getDefault());
    }

    Model::Model(const Mesh& mesh)
        : m_material    (),
          m_mesh        ()
    {
        setMaterial(Material::getDefault());
        setMesh(mesh);
    }

    Model::Model(const Material& material)
        : m_material    (),
          m_mesh        ()
    {
        setMaterial(material);
        setMesh(Mesh::getDefault());
    }

    Model::Model(const Mesh& mesh, const Material& material)
        : m_material    (),
          m_mesh        ()
    {
        setMaterial(material);
        setMesh(mesh);
    }

    //////////////////////////////////////////////

    const Mesh* Model::getMesh() const
    {
        return m_mesh.get();
    }

    //////////////////////////////////////////////

    Mesh* Model::getMesh()
    {
        return m_mesh.get();
    }

    //////////////////////////////////////////////

    Model& Model::setMesh(const Mesh& mesh)
    {
        m_mesh = static_ref_cast<Mesh>(mesh.getReference());
        return *this;
    }

    //////////////////////////////////////////////

    const Material* Model::getMaterial() const
    {
        return m_material.get();
    }

    //////////////////////////////////////////////

    Material* Model::getMaterial()
    {
        return m_material.get();
    }

    //////////////////////////////////////////////

    Model& Model::setMaterial(const Material& material)
    {
        m_material = static_ref_cast<Material>(material.getReference());
        return *this;
    }

    //////////////////////////////////////////////

    bool Model::isValid() const
    {
        return !m_mesh.expired() && !m_material.expired();
    }
}