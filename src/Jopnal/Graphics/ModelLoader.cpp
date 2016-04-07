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
//    in a product, an acknowledgement in the product documentation would be
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
    ModelLoader::ModelLoader(Object& obj)
        : Component (obj, "modelloader"),
          m_path    ()
    {}

    ModelLoader::ModelLoader(const ModelLoader& other, Object& obj)
        : Component (other, obj),
          m_path    (other.m_path)
    {}

    //////////////////////////////////////////////

    namespace detail
    {
        bool processNode(const aiScene& scene, const aiNode& node, WeakReference<Object> object)
        {
            // Transform
            {
                aiVector3D scale, pos; aiQuaternion rot;
                node.mTransformation.Decompose(scale, rot, pos);

                object->setScale(scale.x, scale.y, scale.z)
                       .setRotation(glm::quat(rot.w, rot.x, rot.y, rot.z))
                       .setPosition(pos.x, pos.y, pos.z);
            }

            // Meshes (drawables)
            for (std::size_t i = 0; i < node.mNumMeshes; ++i)
            {
                const aiMesh& mesh = *scene.mMeshes[node.mMeshes[i]];
                
            }

            return true;
        }

        bool makeNodes(const aiScene& scene, const aiNode& parentNode, WeakReference<Object> parentObject)
        {
            if (!processNode(scene, parentNode, parentObject))
                return false;

            for (std::size_t i = 0; i < parentNode.mNumChildren; ++i)
            {
                const aiNode& thisNode = *parentNode.mChildren[i];
                const WeakReference<Object> thisObj = parentObject->createChild(thisNode.mName.C_Str());

                if (!makeNodes(scene, thisNode, thisObj))
                    return false;
            }

            return true;
        }
    }

    bool ModelLoader::load(const std::string& path)
    {
        if (path.empty())
            return false;

        auto& imp = *FileSystemInitializer::g_Importer;

        struct SceneDealloc
        {
            Assimp::Importer* imp;
            SceneDealloc(Assimp::Importer& i) : imp(&i){}
            ~SceneDealloc(){ imp->FreeScene(); }

        }SceneDealloc(imp);

        static const unsigned int preProcess = 0

          | aiProcessPreset_TargetRealtime_Fast
          | aiProcess_TransformUVCoords
          | aiProcess_FixInfacingNormals
          //| aiProcess_FlipUVs
          | aiProcess_ImproveCacheLocality
          | aiProcess_FindInvalidData
          | aiProcess_OptimizeMeshes
          | aiProcess_OptimizeGraph
          ;

        const aiScene* scene = imp.ReadFile(path, preProcess);

        if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0)
        {
            JOP_DEBUG_ERROR("Failed to load Model: " << imp.GetErrorString());
            return false;
        }

        if (detail::makeNodes(*scene, *scene->mRootNode, getObject()))
        {
            m_path = path;
            return true;
        }

        return false;
    }
}