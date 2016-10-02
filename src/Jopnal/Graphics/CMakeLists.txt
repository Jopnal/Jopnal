# Jopnal Graphics CMakeLists

set(__INCDIR_GRAPHICS ${PROJECT_SOURCE_DIR}/include/Jopnal/Graphics)
set(__INLDIR_GRAPHICS ${__INCDIR_GRAPHICS}/Inl)
set(__SRCDIR_GRAPHICS ${PROJECT_SOURCE_DIR}/src/Jopnal/Graphics)

# CMakeLists
set(__LISTS_GRAPHICS ${__SRCDIR_GRAPHICS}/CMakeLists.txt)
source_group("Graphics" FILES ${__LISTS_GRAPHICS})
list(APPEND SRC ${__LISTS_GRAPHICS})

# Include
set(__INC_GRAPHICS
    ${__INCDIR_GRAPHICS}/AnimatedSprite.hpp
    ${__INCDIR_GRAPHICS}/AnimationAtlas.hpp 
    ${__INCDIR_GRAPHICS}/Buffer.hpp
    ${__INCDIR_GRAPHICS}/Camera.hpp
    ${__INCDIR_GRAPHICS}/Color.hpp
    ${__INCDIR_GRAPHICS}/Drawable.hpp
    ${__INCDIR_GRAPHICS}/EnvironmentRecorder.hpp
    ${__INCDIR_GRAPHICS}/Font.hpp
    ${__INCDIR_GRAPHICS}/Glyph.hpp
    ${__INCDIR_GRAPHICS}/Image.hpp
    ${__INCDIR_GRAPHICS}/LightSource.hpp
    ${__INCDIR_GRAPHICS}/MainRenderTarget.hpp
    ${__INCDIR_GRAPHICS}/Material.hpp
    ${__INCDIR_GRAPHICS}/ModelLoader.hpp
    ${__INCDIR_GRAPHICS}/PostProcessor.hpp
    ${__INCDIR_GRAPHICS}/Renderer.hpp
    ${__INCDIR_GRAPHICS}/RenderPass.hpp
    ${__INCDIR_GRAPHICS}/RenderTarget.hpp
    ${__INCDIR_GRAPHICS}/RenderTexture.hpp
    ${__INCDIR_GRAPHICS}/Shader.hpp
    ${__INCDIR_GRAPHICS}/ShaderAssembler.hpp
    ${__INCDIR_GRAPHICS}/ShaderProgram.hpp
    ${__INCDIR_GRAPHICS}/SkyBox.hpp
    ${__INCDIR_GRAPHICS}/SkySphere.hpp
    ${__INCDIR_GRAPHICS}/Sprite.hpp
    ${__INCDIR_GRAPHICS}/Text.hpp
    ${__INCDIR_GRAPHICS}/Transform.hpp
    ${__INCDIR_GRAPHICS}/Vertex.hpp
    ${__INCDIR_GRAPHICS}/VertexBuffer.hpp
)
source_group("Graphics\\Headers" FILES ${__INC_GRAPHICS})
list(APPEND SRC ${__INC_GRAPHICS})

# Include - Culling
set(__INC_GRAPHICS_CULLING
    ${__INCDIR_GRAPHICS}/Culling/CullerComponent.hpp
)
source_group("Graphics\\Headers\\Culling" FILES ${__INC_GRAPHICS_CULLING})
list(APPEND SRC ${__INC_GRAPHICS_CULLING})

# Include - Mesh
set(__INC_GRAPHICS_MESH
    ${__INCDIR_GRAPHICS}/Mesh/BoxMesh.hpp
    ${__INCDIR_GRAPHICS}/Mesh/CapsuleMesh.hpp
    ${__INCDIR_GRAPHICS}/Mesh/CircleMesh.hpp
    ${__INCDIR_GRAPHICS}/Mesh/ConeMesh.hpp
    ${__INCDIR_GRAPHICS}/Mesh/CylinderMesh.hpp
    ${__INCDIR_GRAPHICS}/Mesh/Mesh.hpp
    ${__INCDIR_GRAPHICS}/Mesh/RectangleMesh.hpp
    ${__INCDIR_GRAPHICS}/Mesh/SphereMesh.hpp
)
source_group("Graphics\\Headers\\Mesh" FILES ${__INC_GRAPHICS_MESH})
list(APPEND SRC ${__INC_GRAPHICS_MESH})

# Include - Texture
set(__INC_GRAPHICS_TEXTURE    
    ${__INCDIR_GRAPHICS}/Texture/Cubemap.hpp
    ${__INCDIR_GRAPHICS}/Texture/Texture.hpp
    ${__INCDIR_GRAPHICS}/Texture/Texture2D.hpp
    ${__INCDIR_GRAPHICS}/Texture/TextureAtlas.hpp
    ${__INCDIR_GRAPHICS}/Texture/TextureSampler.hpp
)
source_group("Graphics\\Headers\\Texture" FILES ${__INC_GRAPHICS_TEXTURE})
list(APPEND SRC ${__INC_GRAPHICS_TEXTURE})

# Include - OpenGL
set(__INC_GRAPHICS_OPENGL
    ${__INCDIR_GRAPHICS}/OpenGL/GlState.hpp
    ${__INCDIR_GRAPHICS}/OpenGL/OpenGL.hpp
)
source_group("Graphics\\Headers\\OpenGL" FILES ${__INC_GRAPHICS_OPENGL})
list(APPEND SRC ${__INC_GRAPHICS_OPENGL})

# Inline - Graphics
set(__INL_GRAPHICS
    ${__INLDIR_GRAPHICS}/Renderer.inl
    ${__INLDIR_GRAPHICS}/ShaderProgram.inl
    ${__INLDIR_GRAPHICS}/TextureAtlas.inl
)
source_group("Graphics\\Inl" FILES ${__INL_GRAPHICS})
list(APPEND SRC ${__INL_GRAPHICS})

# Source
set(__SRC_GRAPHICS
    ${__SRCDIR_GRAPHICS}/AnimatedSprite.cpp
    ${__SRCDIR_GRAPHICS}/AnimationAtlas.cpp    
    ${__SRCDIR_GRAPHICS}/Buffer.cpp
    ${__SRCDIR_GRAPHICS}/Camera.cpp
    ${__SRCDIR_GRAPHICS}/Color.cpp
    ${__SRCDIR_GRAPHICS}/Drawable.cpp
    ${__SRCDIR_GRAPHICS}/EnvironmentRecorder.cpp
    ${__SRCDIR_GRAPHICS}/Font.cpp
    ${__SRCDIR_GRAPHICS}/Glyph.cpp
    ${__SRCDIR_GRAPHICS}/Image.cpp
    ${__SRCDIR_GRAPHICS}/LightSource.cpp
    ${__SRCDIR_GRAPHICS}/MainRenderTarget.cpp
    ${__SRCDIR_GRAPHICS}/Material.cpp
    ${__SRCDIR_GRAPHICS}/ModelLoader.cpp
    ${__SRCDIR_GRAPHICS}/PostProcessor.cpp
    ${__SRCDIR_GRAPHICS}/Renderer.cpp
    ${__SRCDIR_GRAPHICS}/RenderPass.cpp
    ${__SRCDIR_GRAPHICS}/RenderTarget.cpp
    ${__SRCDIR_GRAPHICS}/RenderTexture.cpp
    ${__SRCDIR_GRAPHICS}/Shader.cpp
    ${__SRCDIR_GRAPHICS}/ShaderAssembler.cpp
    ${__SRCDIR_GRAPHICS}/ShaderProgram.cpp
    ${__SRCDIR_GRAPHICS}/SkyBox.cpp
    ${__SRCDIR_GRAPHICS}/SkySphere.cpp
    ${__SRCDIR_GRAPHICS}/Sprite.cpp
    ${__SRCDIR_GRAPHICS}/Text.cpp
    ${__SRCDIR_GRAPHICS}/Transform.cpp
    ${__SRCDIR_GRAPHICS}/Vertex.cpp
    ${__SRCDIR_GRAPHICS}/VertexBuffer.cpp
)
source_group("Graphics\\Source" FILES ${__SRC_GRAPHICS})
list(APPEND SRC ${__SRC_GRAPHICS})

# Source - Culling
set(__SRC_GRAPHICS_CULLING
    ${__SRCDIR_GRAPHICS}/Culling/CullerComponent.cpp
    ${__SRCDIR_GRAPHICS}/Culling/CullingWorld.cpp
    ${__SRCDIR_GRAPHICS}/Culling/CullingWorld.hpp
)
source_group("Graphics\\Source\\Culling" FILES ${__SRC_GRAPHICS_CULLING})
list(APPEND SRC ${__SRC_GRAPHICS_CULLING})

# Source - Mesh
set(__SRC_GRAPHICS_MESH
    ${__SRCDIR_GRAPHICS}/Mesh/BoxMesh.cpp
    ${__SRCDIR_GRAPHICS}/Mesh/CapsuleMesh.cpp
    ${__SRCDIR_GRAPHICS}/Mesh/CircleMesh.cpp
    ${__SRCDIR_GRAPHICS}/Mesh/ConeMesh.cpp
    ${__SRCDIR_GRAPHICS}/Mesh/CylinderMesh.cpp
    ${__SRCDIR_GRAPHICS}/Mesh/Mesh.cpp
    ${__SRCDIR_GRAPHICS}/Mesh/RectangleMesh.cpp
    ${__SRCDIR_GRAPHICS}/Mesh/SphereMesh.cpp
)
source_group("Graphics\\Source\\Mesh" FILES ${__SRC_GRAPHICS_MESH})
list(APPEND SRC ${__SRC_GRAPHICS_MESH})

# Source - Texture
set(__SRC_GRAPHICS_TEXTURE
    ${__SRCDIR_GRAPHICS}/Texture/Cubemap.cpp
    ${__SRCDIR_GRAPHICS}/Texture/Texture.cpp
    ${__SRCDIR_GRAPHICS}/Texture/Texture2D.cpp
    ${__SRCDIR_GRAPHICS}/Texture/TextureAtlas.cpp
    ${__SRCDIR_GRAPHICS}/Texture/TextureSampler.cpp
)
source_group("Graphics\\Source\\Texture" FILES ${__SRC_GRAPHICS_TEXTURE})
list(APPEND SRC ${__SRC_GRAPHICS_TEXTURE})

# Source - OpenGL
set(__SRC_GRAPHICS_OPENGL
    ${__SRCDIR_GRAPHICS}/OpenGL/EglCheck.cpp
    ${__SRCDIR_GRAPHICS}/OpenGL/EglCheck.hpp
    ${__SRCDIR_GRAPHICS}/OpenGL/GlCheck.cpp
    ${__SRCDIR_GRAPHICS}/OpenGL/GlCheck.hpp
    ${__SRCDIR_GRAPHICS}/OpenGL/GlState.cpp
    ${__SRCDIR_GRAPHICS}/OpenGL/OpenGL.cpp
)
if (NOT JOP_OS_ANDROID)

    list(APPEND __SRC_GRAPHICS_OPENGL
        ${__SRCDIR_GRAPHICS}/OpenGL/gl_4.5.c
        ${__SRCDIR_GRAPHICS}/OpenGL/gl_4.5.h
    )

endif()
source_group("Graphics\\Source\\OpenGL" FILES ${__SRC_GRAPHICS_OPENGL})
list(APPEND SRC ${__SRC_GRAPHICS_OPENGL})