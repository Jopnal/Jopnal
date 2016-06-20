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
#include <fstream>
#include <rapidjson/filereadstream.h>
#include <rapidjson/reader.h>

//////////////////////////////////////////////


namespace jop
{
	ModelLoader::Options::Options()
		: forceDiffuseAlpha(false),
		collapseTree(true),
		fixInfacingNormals(true)
	{}

	//////////////////////////////////////////////

	ModelLoader::ModelLoader(Object& obj)
		: Component(obj, "modelloader"),
		m_path()
	{}

	ModelLoader::ModelLoader(const ModelLoader& other, Object& obj)
		: Component(other, obj),
		m_path(other.m_path)
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

		void loadCameras(const aiScene& scene, WeakReference<Object> root)
		{
			if (!scene.HasCameras())
				return;

			auto& renderer = root->getScene().getRenderer();

			for (std::size_t i = 0; i < scene.mNumCameras; ++i)
			{
				auto& cam = *scene.mCameras[i];

				WeakReference<Object> node;

				if (root->getID() == cam.mName.C_Str())
					node = root;
				else if ((node = root->findChild(cam.mName.C_Str(), true)).expired())
				{
					JOP_DEBUG_WARNING("Couldn't find node \"" << cam.mName.C_Str() << "\" while loading camera");
					continue;
				}

				if (cam.mAspect <= 0.f)
				{
					JOP_DEBUG_WARNING("Couldn't load camera, aspect ratio must be more than zero");
					continue;
				}

				auto& comp = node->createComponent<Camera>(renderer, Camera::Projection::Perspective);

				comp.setAspectRatio(cam.mAspect);
				comp.setClippingPlanes(cam.mClipPlaneNear, cam.mClipPlaneFar);
				comp.setFieldOfView(cam.mHorizontalFOV / cam.mAspect);
			}
		}

		void loadLights(const aiScene& scene, WeakReference<Object> root)
		{
			if (!scene.HasLights())
				return;

			auto& renderer = root->getScene().getRenderer();

			for (std::size_t i = 0; i < scene.mNumLights; ++i)
			{
				auto& light = *scene.mLights[i];

				WeakReference<Object> node;

				if (root->getID() == light.mName.C_Str())
					node = root;
				else if ((node = root->findChild(light.mName.C_Str(), true)).expired())
				{
					JOP_DEBUG_WARNING("Couldn't find node \"" << light.mName.C_Str() << "\" while loading light source");
					continue;
				}

				static const LightSource::Type types[] =
				{
					LightSource::Type::Directional,
					LightSource::Type::Point,
					LightSource::Type::Spot
				};

				auto& comp = node->createComponent<LightSource>(renderer, types[light.mType]);

				comp.setAttenuation(light.mAttenuationConstant, light.mAttenuationLinear, light.mAttenuationQuadratic);
				comp.setCutoff(light.mAngleInnerCone, light.mAngleOuterCone);

				auto& amb = light.mColorAmbient;
				auto& dif = light.mColorDiffuse;
				auto& spe = light.mColorSpecular;

				comp.setIntensity(Color(amb.r, amb.g, amb.b), Color(dif.r, dif.g, dif.b), Color(spe.r, spe.g, spe.b));
			}
		}

		std::vector<const Material*> getMaterials(const aiScene& scene, const ModelLoader::Options& options) //no scene intake
		{
			auto& m = ResourceManager::getEmptyResource<Material>("jop_material_" + getHex(), true).setAttributeField(Material::Attribute::AmbientConstant);

			std::string fileName = "model.jop";

			namespace rj = rapidjson;

			std::ifstream rFile(fileName);
			if (!rFile.is_open())
			{
				printf("Cannot open modelfile\n");
			}

			FILE* fp = fopen(fileName.c_str(), "rb");


			//rj::StringBuffer rBuff;

			char rBuff[65536];
			rj::FileReadStream frs(fp, rBuff, sizeof(rBuff));


			rj::Document doc;

			doc.ParseStream(frs);

			//doc.SetObject();
			if (doc.HasParseError())
			{
				printf("PARSE ERROR");
			}

			if (doc.HasMember("materials"))
			{
				std::vector<const Material*> mats;
				mats.reserve(doc["materials"].Size());

				for (auto& mat : doc["materials"])
				{
					if (mat.HasMember("reflection"))
					{
						float r, g, b, a;
						for (int ref = 0; ref < mat["reflection"].Size() / 4; ++ref)
						{
							r = mat["reflection"][ref * 4 + 0].GetDouble();
							g = mat["reflection"][ref * 4 + 1].GetDouble();
							b = mat["reflection"][ref * 4 + 2].GetDouble();
							a = mat["reflection"][ref * 4 + 3].GetDouble();
							m.setReflection(mat["reflection"][ref], r, g, b, a); //modify this function parameters
						}
					}


					bool hadShininess = false;
					if (mat.HasMember("shininess"))
					{
						hadShininess = true;
						m.setShininess(mat["shininess"].GetDouble());
					}

					if (mat.HasMember("reflectivity"))
					{
						m.setReflectivity(mat["reflectivity"].GetDouble());
					}


					if (mat.HasMember("textures"))
					{
						if (!mat["textures"].IsNull)
						{
							for (auto& texts : doc["materials"]["textures"])
							{
								if (texts.HasMember("texturePath"))
								{






									auto processTexFlags = [&texts](const aiMaterial& aiMat, Material& jopMat, const ModelLoader::Options& options, const aiTextureType type, const int index, Texture& tex)
									{
										// Flags
										{
											aiTextureFlags flags;
											aiMat.Get(AI_MATKEY_TEXFLAGS(type, index), flags);
											switch (flags)
											{
												//case aiTextureFlags_Invert:

											case aiTextureFlags_UseAlpha:
												jopMat.addAttributes(Material::Attribute::DiffuseAlpha);
												break;
											case aiTextureFlags_IgnoreAlpha:
												jopMat.removeAttributes(Material::Attribute::DiffuseAlpha);
												break;
											}
										}

										if (options.forceDiffuseAlpha && type == aiTextureType_DIFFUSE)
											jopMat.addAttributes(Material::Attribute::DiffuseAlpha);


										//wrapping
										if (texts["texturePath"].HasMember("wrapmode"))
										{
											tex.getSampler().setRepeatMode(static_cast<TextureSampler::Repeat>(texts["wrapmode"].GetInt()));
										}
									};

									// Diffuse
									if (mat["texturePath"].HasMember("type"))
									{
										//auto typ = static_cast<Material::Map>(mat["texturePath"]["type"].GetInt());

										if (static_cast<Material::Map>(mat["texturePath"]["type"].GetInt()) > Material::Map::Gloss)
										{
											for (std::size_t i = 0; i < mat["texturePath"]["type"].GetInt(); ++i)
											{

												using A = Material::Attribute;
												using M = Material::Map;
												M map;

												// Diffuse
												if (!m.hasAttribute(A::DiffuseMap) && strstr(mat["texturePath"], "dif")) //fix these somehow ///returns a pointer to 1. occurrence
													map = M::Diffuse;

												// Specular
												else if (!m.hasAttribute(A::SpecularMap) && strstr(mat["texturePath"].C_Str(), "spec"))
													map = M::Specular;

												// Emission
												else if (!m.hasAttribute(A::EmissionMap) && strstr(mat["texturePath"].C_Str(), "emis"))
													map = M::Emission;

												// Reflection
												else if (!m.hasAttribute(A::ReflectionMap) && strstr(path.C_Str(), "refl"))
													map = M::Reflection;

												// Opacity
												else if (!m.hasAttribute(A::OpacityMap) && (strstr(path.C_Str(), "opa") || strstr(path.C_Str(), "alp")))
													map = M::Opacity;

												// Gloss
												else if (!m.hasAttribute(A::GlossMap) && strstr(path.C_Str(), "glo"))
													map = M::Gloss;

												// Not identified
												else
													continue;

												auto& tex = ResourceManager::getResource<Texture2D>(mat["texturePath"], map == M::Diffuse || map == M::Emission);
												m.setMap(map, tex);

												processTexFlags(m, options, mat["texturePath"]["type"].GetInt(), i, tex);
											}
										}
										else
										{
											auto& tex = ResourceManager::getResource<Texture2D>(mat["texturePath"], true);
											m.setMap(static_cast<Material::Map>(mat["texturePath"]["type"].GetInt()), tex);

											processTexFlags(m, options, mat["texturePath"]["type"].GetInt(), 0, tex); //modify this function 1. intake & output
										}
									}

								}
							}
						}
					}
					mats.push_back(&m);
				}
			}
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
					sizeof(glm::vec3) * mesh.HasTangentsAndBitangents() * 2 +
					sizeof(Color)     * mesh.HasVertexColors(0)
					)
					* mesh.mNumVertices
					);

				//turhaa alkaa
				/*

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
				*/
				//turhaa p‰‰ttyy

				//components tiedostosta
				const uint32 comps = Mesh::Position

					| (mesh.HasTextureCoords(0)         * Mesh::TexCoords)
					| (mesh.HasNormals()                * Mesh::Normal)
					| (mesh.HasTangentsAndBitangents()  * Mesh::Tangents)
					| (mesh.HasVertexColors(0)          * Mesh::Color)
					;
				//length + lukum‰‰r‰						//index
				meshes.push_back(std::make_pair(&ResourceManager::getNamedResource<Mesh>("jop_mesh_" + getHex(), vertBuf.data(), vertBuf.size(), comps, indBuf.data(), elemSize, mesh.mNumFaces * 3), mesh.mMaterialIndex));
			}

			return meshes;
		}

		bool processNode(const aiNode& node, WeakReference<Object> object, Renderer& rend, const std::vector<std::pair<const Mesh*, unsigned int>>& meshes, const std::vector<const Material*>& mats)
		{
			// Name
			if (node.mName.length)
				object->setID(node.mName.C_Str());

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
				const WeakReference<Object> thisObj = parentObject->createChild("");

				if (!makeNodes(thisNode, thisObj, rend, meshes, mats))
					return false;
			}

			return true;
		}
	}

	bool ModelLoader::load(const std::string& path, const Options& options)
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
			explicit SceneDealloc(Assimp::Importer& i) : imp(&i){}
			~SceneDealloc(){ imp->FreeScene(); }

		} sceneDeallocator(imp);

		static const unsigned int preProcess = 0

			| aiProcessPreset_TargetRealtime_Fast
			| aiProcess_TransformUVCoords
			| (aiProcess_FixInfacingNormals * options.fixInfacingNormals)
			| aiProcess_FlipUVs
			| aiProcess_ImproveCacheLocality
			| aiProcess_FindInvalidData
			| aiProcess_ValidateDataStructure
			| aiProcess_OptimizeMeshes
			| aiProcess_RemoveRedundantMaterials
			| (aiProcess_OptimizeGraph * options.collapseTree)
			;

		const aiScene* scene = imp.ReadFile(path, preProcess);

		if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0)
		{
			JOP_DEBUG_ERROR("Failed to load Model: " << imp.GetErrorString());
			return false;
		}

		JOP_DEBUG_INFO("Model loaded successfully, building object tree...");

		if (detail::makeNodes(*scene->mRootNode, getObject(), getObject()->getScene().getRenderer(), detail::getMeshes(*scene), detail::getMaterials(*scene, options)))
		{
			detail::loadLights(*scene, getObject());
			detail::loadCameras(*scene, getObject());

			m_path = path;
			JOP_DEBUG_INFO("Successfully loaded model \"" << path << "\", took " << clock.getElapsedTime().asSeconds() << "s");
			return true;
		}

		return false;
	}

}