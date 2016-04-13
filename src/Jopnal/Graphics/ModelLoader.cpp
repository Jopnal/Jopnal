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
        std::string getHex()
        {
        #ifdef JOP_OS_WINDOWS
            const __int64 timePoint = __rdtsc();
        #endif

            std::ostringstream ss;
            ss << std::hex << timePoint;
            return ss.str();
        }

        std::vector<const Material*> getMaterials(const aiScene& scene)
        {
            std::vector<const Material*> mats;
            mats.reserve(scene.mNumMaterials);

            for (std::size_t i = 0; i < scene.mNumMaterials; ++i)
            {
                auto& mat = *scene.mMaterials[i];

                auto& m = ResourceManager::getEmptyResource<Material>("jop_material_" + getHex(), true).setAttributeField(Material::Attribute::AmbientConstant);

                // Reflection
                {
                    aiColor3D col;

                    // Ambient color
                    mat.Get(AI_MATKEY_COLOR_AMBIENT, col);
                    m.setReflection(Material::Reflection::Ambient, Color(col.r, col.g, col.b));

                    // Diffuse color
                    mat.Get(AI_MATKEY_COLOR_DIFFUSE, col);
                    m.setReflection(Material::Reflection::Diffuse, Color(col.r, col.g, col.b));

                    // Specular color
                    mat.Get(AI_MATKEY_COLOR_SPECULAR, col);
                    m.setReflection(Material::Reflection::Specular, Color(col.r, col.g, col.b));

                    // Emission color
                    mat.Get(AI_MATKEY_COLOR_EMISSIVE, col);
                    m.setReflection(Material::Reflection::Emission, Color(col.r, col.g, col.b));
                }

                // Shininess
                {
                    float shin;
                    mat.Get(AI_MATKEY_SHININESS, shin);
                    m.setShininess(shin);
                }

                // Lighting model
                {
                    aiShadingMode mode;
                    mat.Get(AI_MATKEY_SHADING_MODEL, mode);

                    switch (mode)
                    {
                        case aiShadingMode_Flat:
                            m.addAttributes(Material::Attribute::Flat);
                            break;
                        case aiShadingMode_Gouraud:
                            m.addAttributes(Material::Attribute::Gouraud);
                            break;
                        case aiShadingMode_Phong:
                            m.addAttributes(Material::Attribute::Phong);
                            break;
                        case aiShadingMode_Blinn:
                            m.addAttributes(Material::Attribute::BlinnPhong);
                            break;
                        default:
                            m.addAttributes(Material::Attribute::DefaultLighting);
                    }
                }

                // Textures
                {
                    // Diffuse
                    if (mat.GetTextureCount(aiTextureType_DIFFUSE))
                    {
                        aiString path;
                        mat.GetTexture(aiTextureType_DIFFUSE, 0, &path);

                        if (path.length)
                            m.setMap(Material::Map::Diffuse, ResourceManager::getResource<Texture2D>(path.C_Str()));
                    }

                    // Specular
                    if (mat.GetTextureCount(aiTextureType_SPECULAR))
                    {
                        aiString path;
                        mat.GetTexture(aiTextureType_SPECULAR, 0, &path);

                        if (path.length)
                            m.setMap(Material::Map::Specular, ResourceManager::getResource<Texture2D>(path.C_Str()));
                    }

                    // Shininess
                    if (!m.hasAttribute(Material::Attribute::SpecularMap) && mat.GetTextureCount(aiTextureType_SHININESS))
                    {
                        aiString path;
                        mat.GetTexture(aiTextureType_SHININESS, 0, &path);

                        if (path.length)
                            m.setMap(Material::Map::Specular, ResourceManager::getResource<Texture2D>(path.C_Str()));
                    }

                    // Emission
                    if (mat.GetTextureCount(aiTextureType_EMISSIVE))
                    {
                        aiString path;
                        mat.GetTexture(aiTextureType_EMISSIVE, 0, &path);

                        if (path.length)
                            m.setMap(Material::Map::Emission, ResourceManager::getResource<Texture2D>(path.C_Str()));
                    }

                    // Reflection
                    if (mat.GetTextureCount(aiTextureType_REFLECTION))
                    {
                        aiString path;
                        mat.GetTexture(aiTextureType_REFLECTION, 0, &path);

                        if (path.length)
                            m.setMap(Material::Map::Reflection, ResourceManager::getResource<Texture2D>(path.C_Str()));
                    }
                    
                    // Opacity
                    if (mat.GetTextureCount(aiTextureType_OPACITY))
                    {
                        aiString path;
                        mat.GetTexture(aiTextureType_OPACITY, 0, &path);

                        if (path.length)
                            m.setMap(Material::Map::Opacity, ResourceManager::getResource<Texture2D>(path.C_Str()));
                    }

                    // Normal
                    /*if (mat.GetTextureCount(aiTextureType_AMBIENT))
                    {
                        aiString path;
                        mat.GetTexture(aiTextureType_AMBIENT, 0, &path);

                        if (path.length)
                            m.setMap(Material
                    }*/
                }

                mats.push_back(&m);
            }

            return mats;
        }

        std::vector<std::pair<const Mesh*, unsigned int>> getMeshes(const aiScene& scene)
        {
            std::vector<std::pair<const Mesh*, unsigned int>> meshes;
            meshes.reserve(scene.mNumMeshes);

            for (std::size_t i = 0; i < scene.mNumMeshes; ++i)
            {
                const aiMesh& mesh = *scene.mMeshes[i];
                if (!mesh.mNumVertices)
                    continue;
                
                // Vertices
                std::vector<uint8> vertBuf
                (
                    (sizeof(glm::vec3) +
                    sizeof(glm::vec2) * mesh.HasTextureCoords(0) +
                    sizeof(glm::vec3) * mesh.HasNormals() +
                    sizeof(glm::vec3) * 2 * mesh.HasTangentsAndBitangents() +
                    sizeof(Color) * mesh.HasVertexColors(0)
                    )
                    * mesh.mNumVertices
                );

                for (std::size_t j = 0, vertIndex = 0; j < mesh.mNumVertices; ++j)
                {
                    // Position
                    {
                        auto& pos = mesh.mVertices[j];
                        reinterpret_cast<glm::vec3&>(vertBuf[vertIndex]) = glm::vec3(pos.x, pos.y, pos.z);
                        vertIndex += sizeof(glm::vec3);
                    }

                    // Tex coordinates
                    if (mesh.HasTextureCoords(0))
                    {
                        auto& tc = mesh.mTextureCoords[0][j];
                        reinterpret_cast<glm::vec2&>(vertBuf[vertIndex]) = glm::vec2(tc.x, tc.y);
                        vertIndex += sizeof(glm::vec2);
                    }

                    // Normal
                    if (mesh.HasNormals())
                    {
                        auto& norm = mesh.mNormals[j];
                        reinterpret_cast<glm::vec3&>(vertBuf[vertIndex]) = glm::vec3(norm.x, norm.y, norm.z);
                        vertIndex += sizeof(glm::vec3);
                    }

                    // Tangents
                    if (mesh.HasTangentsAndBitangents())
                    {
                        auto& tang = mesh.mTangents[j], bitang = mesh.mBitangents[j];
                        reinterpret_cast<glm::vec3&>(vertBuf[vertIndex]) = glm::vec3(tang.x, tang.y, tang.z);
                        vertIndex += sizeof(glm::vec3);

                        reinterpret_cast<glm::vec3&>(vertBuf[vertIndex]) = glm::vec3(bitang.x, bitang.y, bitang.z);
                        vertIndex += sizeof(glm::vec3);
                    }

                    // Color
                    if (mesh.HasVertexColors(0))
                    {
                        auto& col = mesh.mColors[0][j];
                        reinterpret_cast<Color&>(vertBuf[vertIndex]) = Color(col.r, col.g, col.b, col.a);
                        vertIndex += sizeof(Color);
                    }
                }

                // Indices
                std::vector<uint8> indBuf;
                const auto elemSize = Mesh::getElementSize(mesh.mNumVertices * 3);
                if (mesh.HasFaces())
                {
                    indBuf.resize(elemSize * mesh.mNumFaces * 3);

                    for (std::size_t j = 0, bufIndex = 0; j < mesh.mNumFaces; ++j, bufIndex += elemSize * 3)
                    {
                        auto& face = mesh.mFaces[j];

                        switch (elemSize)
                        {
                            case sizeof(UCHAR) :
                                indBuf[bufIndex] = static_cast<UCHAR>(face.mIndices[0]);
                                indBuf[bufIndex + sizeof(UCHAR)] = static_cast<UCHAR>(face.mIndices[1]);
                                indBuf[bufIndex + sizeof(UCHAR) * 2] = static_cast<UCHAR>(face.mIndices[2]);
                                break;

                            case sizeof(USHORT) :
                                reinterpret_cast<USHORT&>(indBuf[bufIndex]) = static_cast<USHORT>(face.mIndices[0]);
                                reinterpret_cast<USHORT&>(indBuf[bufIndex + sizeof(USHORT)]) = static_cast<USHORT>(face.mIndices[1]);
                                reinterpret_cast<USHORT&>(indBuf[bufIndex + sizeof(USHORT) * 2]) = static_cast<USHORT>(face.mIndices[2]);
                                break;

                            case sizeof(UINT) :
                                reinterpret_cast<UINT&>(indBuf[bufIndex]) = face.mIndices[0];
                                reinterpret_cast<UINT&>(indBuf[bufIndex + sizeof(UINT)]) = face.mIndices[1];
                                reinterpret_cast<UINT&>(indBuf[bufIndex + sizeof(UINT) * 2]) = face.mIndices[2];
                        }
                    }
                }

                const uint32 comps = Mesh::Position

                    | (mesh.HasTextureCoords(0)         * Mesh::TexCoords)
                    | (mesh.HasNormals()                * Mesh::Normal)
                    | (mesh.HasTangentsAndBitangents()  * Mesh::Tangent)
                    | (mesh.HasVertexColors(0)          * Mesh::Color)
                    ;

                meshes.push_back(std::make_pair(&ResourceManager::getNamedResource<Mesh>("jop_mesh_" + getHex(), vertBuf.data(), vertBuf.size(), comps, indBuf.data(), elemSize, mesh.mNumFaces * 3), mesh.mMaterialIndex));
            }

            return meshes;
        }

        bool processNode(const aiNode& node, WeakReference<Object> object, Renderer& rend, const std::vector<std::pair<const Mesh*, unsigned int>>& meshes, const std::vector<const Material*>& mats)
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
                auto& drawable = object->createComponent<GenericDrawable>(rend);

                auto& pair = meshes[node.mMeshes[i]];
                drawable.setModel(Model(*pair.first, *mats[pair.second]));
            }

            return true;
        }

        bool makeNodes(const aiNode& parentNode, WeakReference<Object> parentObject, Renderer& rend, const std::vector<std::pair<const Mesh*, unsigned int>>& meshes, const std::vector<const Material*>& mats)
        {
            if (!processNode(parentNode, parentObject, rend, meshes, mats))
                return false;

            for (std::size_t i = 0; i < parentNode.mNumChildren; ++i)
            {
                const aiNode& thisNode = *parentNode.mChildren[i];
                const WeakReference<Object> thisObj = parentObject->createChild(thisNode.mName.length > 0 ? thisNode.mName.C_Str() : "object");

                if (!makeNodes(thisNode, thisObj, rend, meshes, mats))
                    return false;
            }

            return true;
        }
    }

    bool ModelLoader::load(const std::string& path)
    {
        if (path.empty())
            return false;

    #if JOP_CONSOLE_VERBOSITY >= 2
        Clock clock;
    #endif

        auto& imp = FileSystemInitializer::getImporter();

        struct SceneDealloc
        {
            Assimp::Importer* imp;
            SceneDealloc(Assimp::Importer& i) : imp(&i){}
            ~SceneDealloc(){ imp->FreeScene(); }

        } SceneDealloc(imp);

        static const unsigned int preProcess = 0

            | aiProcessPreset_TargetRealtime_Fast
            | aiProcess_TransformUVCoords
            | aiProcess_FixInfacingNormals
            | aiProcess_FlipUVs
            | aiProcess_ImproveCacheLocality
            | aiProcess_FindInvalidData
            | aiProcess_ValidateDataStructure
            | aiProcess_OptimizeMeshes
            ;

        const aiScene* scene = imp.ReadFile(path, preProcess);
        
        if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0)
        {
            JOP_DEBUG_ERROR("Failed to load Model: " << imp.GetErrorString());
            return false;
        }

        JOP_DEBUG_INFO("Model loaded successfully, building object tree...");

        if (detail::makeNodes(*scene->mRootNode, getObject(), getObject()->getScene().getRenderer(), detail::getMeshes(*scene), detail::getMaterials(*scene)))
        {
            m_path = path;
            JOP_DEBUG_INFO("Successfully loaded model \"" << path << "\", took " << clock.getElapsedTime().asSeconds() << "s");
            return true;
        }

        return false;
    }
}