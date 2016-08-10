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

/// \defgroup graphics Graphics
///
/// #TODO Detailed decription

// Headers
#include <Jopnal/Graphics/Mesh/BoxMesh.hpp>
#include <Jopnal/Graphics/Buffer.hpp>
#include <Jopnal/Graphics/Camera.hpp>
#include <Jopnal/Graphics/Mesh/CapsuleMesh.hpp>
#include <Jopnal/Graphics/Mesh/CircleMesh.hpp>
#include <Jopnal/Graphics/Color.hpp>
#include <Jopnal/Graphics/Mesh/ConeMesh.hpp>
#include <Jopnal/Graphics/Mesh/CylinderMesh.hpp>
#include <Jopnal/Graphics/Texture/Cubemap.hpp>
#include <Jopnal/Graphics/Drawable.hpp>
#include <Jopnal/Graphics/EnvironmentRecorder.hpp>
#include <Jopnal/Graphics/Drawable.hpp>
#include <Jopnal/Graphics/OpenGL/GlState.hpp>
#include <Jopnal/Graphics/LightSource.hpp>
#include <Jopnal/Graphics/MainRenderTarget.hpp>
#include <Jopnal/Graphics/Material.hpp>
#include <Jopnal/Graphics/Mesh/Mesh.hpp>
#include <Jopnal/Graphics/Model.hpp>
#include <Jopnal/Graphics/ModelLoader.hpp>
#include <Jopnal/Graphics/PostProcessor.hpp>
#include <Jopnal/Graphics/Mesh/RectangleMesh.hpp>
#include <Jopnal/Graphics/RenderTarget.hpp>
#include <Jopnal/Graphics/RenderTexture.hpp>
#include <Jopnal/Graphics/Renderer.hpp>
#include <Jopnal/Graphics/Shader.hpp>
#include <Jopnal/Graphics/ShaderAssembler.hpp>
#include <Jopnal/Graphics/SkyBox.hpp>
#include <Jopnal/Graphics/SkySphere.hpp>
#include <Jopnal/Graphics/Mesh/SphereMesh.hpp>
#include <Jopnal/Graphics/Texture/Texture2D.hpp>
#include <Jopnal/Graphics/Texture/TextureSampler.hpp>
#include <Jopnal/Graphics/Transform.hpp>
#include <Jopnal/Graphics/Vertex.hpp>
#include <Jopnal/Graphics/VertexBuffer.hpp>
#include <Jopnal/Graphics/ShaderPreprocessor.hpp>
#include <Jopnal/Graphics/ShaderProgram.hpp>
#include <Jopnal/Graphics/Glyph.hpp>
#include <Jopnal/Graphics/Image.hpp>
#include <Jopnal/Graphics/Font.hpp>
#include <Jopnal/Graphics/Text.hpp>
#include <Jopnal/Graphics/Texture/TextureAtlas.hpp>
#include <Jopnal/Graphics/AnimatedSprite.hpp>
#include <Jopnal/Graphics/AnimationAtlas.hpp>