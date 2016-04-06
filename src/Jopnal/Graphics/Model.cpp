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

    Model::Model(const Mesh& mesh, const Material& material)
        : m_material    (),
          m_mesh        ()
    {
        setMaterial(material);
        setMesh(mesh);
    }

    Model::Model(const Mesh& mesh)
        : m_material    (),
          m_mesh        ()
    {
        setMaterial(Material::getDefault());
        setMesh(mesh);
    }

    //////////////////////////////////////////////

    bool Model::load(const std::string& filePath)
    {
        auto& imp = *FileSystemInitializer::g_Importer;

        struct SceneDealloc
        {
            Assimp::Importer* imp;
            SceneDealloc(Assimp::Importer& i) : imp(&i){}
            ~SceneDealloc(){imp->FreeScene();}

        }SceneDealloc(imp);

        const aiScene* scene = imp.ReadFile(filePath, aiProcessPreset_TargetRealtime_Fast | aiProcess_TransformUVCoords | aiProcess_FixInfacingNormals | aiProcess_PreTransformVertices);

        if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0)
        {
            JOP_DEBUG_ERROR("Failed to load Model: " << imp.GetErrorString());
            return false;
        }

        auto& mesh = *scene->mMeshes[0];
        auto& mat = *scene->mMaterials[mesh.mMaterialIndex];
        
        auto tex = mat.GetTextureCount(aiTextureType_DIFFUSE);

        for (std::size_t i = 0; i < mat.mNumProperties; ++i)
        {
            auto prop = mat.mProperties[i];
            int j =  0;
        }

        return true;
    }

    //////////////////////////////////////////////

    const Mesh* Model::getMesh() const
    {
        return m_mesh.get();
    }

    //////////////////////////////////////////////

    Model& Model::setMesh(const Mesh& mesh)
    {
        m_mesh = static_ref_cast<const Mesh>(mesh.getReference());
        return *this;
    }

    //////////////////////////////////////////////

    const Material* Model::getMaterial() const
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

    unsigned int Model::getVertexAmount() const
    {
        return m_mesh.expired() ? 0 : m_mesh->getVertexAmount();
    }

    //////////////////////////////////////////////

    unsigned int Model::getElementAmount() const
    {
        return m_mesh.expired() ? 0 : m_mesh->getElementAmount();
    }

    //////////////////////////////////////////////

    bool Model::isValid() const
    {
        return !m_mesh.expired() && !m_material.expired();
    }

    //////////////////////////////////////////////

    const Model& Model::getDefault()
    {
        static WeakReference<const Mesh> defMesh;
        static Model model;

        if (defMesh.expired())
        {
            defMesh = static_ref_cast<const Mesh>(Mesh::getDefault().getReference());

            model.setMesh(*defMesh);
        }

        return model;
    }
}