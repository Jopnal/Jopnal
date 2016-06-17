# Jopnal Source CMakeLists

# Include macros
include(${PROJECT_SOURCE_DIR}/cmake/Macros.cmake)

# Let CMake know about additional library paths
if (NOT JOP_OS_LINUX)

    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${PROJECT_SOURCE_DIR}/extlibs/include"
                                                 "${PROJECT_SOURCE_DIR}/extlibs/include/PhysFS"
                                                 "${PROJECT_SOURCE_DIR}/extlibs/include/Bullet")

    include_directories(${CMAKE_INCLUDE_PATH})

endif()

if (JOP_OS_WINDOWS)

    if (JOP_COMPILER_GCC)

        if (ARCH_32BITS)

            set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/lib/mingw/x86")
            set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/bin/x86")

        elseif (ARCH_64BITS)

            set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/lib/mingw/x64")
            set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/bin/x64")

        endif()

    elseif (JOP_COMPILER_MSVC)

        if (JOP_MSVC_VERSION LESS 14)

            if (ARCH_32BITS)

                set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/lib/msvc/x86")
                set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/lib/msvc/x86/${JOP_MSVC_VERSION}")

            elseif (ARCH_64BITS)

                set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/lib/msvc/x64")
                set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/lib/msvc/x64/${JOP_MSVC_VERSION}")

            endif()

        else ()

            if (ARCH_32BITS)
                set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/lib/msvc-universal/x86")

            elseif (ARCH_64BITS)
                set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/lib/msvc-universal/x64")

            endif()

        endif()

    endif()

elseif (JOP_OS_ANDROID)
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROJECT_SOURCE_DIR}/extlibs/lib/android/${ANDROID_ABI}")
endif()

# Add the sources path
include_directories(${PROJECT_SOURCE_DIR}/src)

# Set the output directory for Jopnal libraries
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

# Add main sub directory
if (JOP_OS_WINDOWS OR JOP_OS_ANDROID)
    add_subdirectory(Main)
endif()

# Jopnal library
set(INCROOT ${PROJECT_SOURCE_DIR}/include/Jopnal)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Jopnal)

# Audio
set(INCROOT_AUD ${PROJECT_SOURCE_DIR}/include/Jopnal/Audio)
set(SRCROOT_AUD ${PROJECT_SOURCE_DIR}/src/Jopnal/Audio)

set(INC_AUD
    ${INCROOT_AUD}/Listener.hpp
    ${INCROOT_AUD}/SoundBuffer.hpp
    ${INCROOT_AUD}/SoundEffect.hpp
    ${INCROOT_AUD}/SoundSource.hpp
    ${INCROOT_AUD}/SoundStream.hpp
)
set(SRC_AUD
    ${SRCROOT_AUD}/Listener.cpp
    ${SRCROOT_AUD}/SoundBuffer.cpp
    ${SRCROOT_AUD}/SoundEffect.cpp
    ${SRCROOT_AUD}/SoundSource.cpp
    ${SRCROOT_AUD}/SoundStream.cpp
)
source_group("Audio\\Headers" FILES ${INC_AUD})
source_group("Audio\\Source" FILES ${SRC_AUD})


# Core
set(INCROOT_COR ${PROJECT_SOURCE_DIR}/include/Jopnal/Core)
set(INLROOT_COR ${INCROOT_COR}/Inl)
set(SRCROOT_COR ${PROJECT_SOURCE_DIR}/src/Jopnal/Core)

set(INC_COR
    ${INCROOT_COR}/Component.hpp
    ${INCROOT_COR}/DebugHandler.hpp
    ${INCROOT_COR}/Engine.hpp
    ${INCROOT_COR}/FileLoader.hpp
    ${INCROOT_COR}/Object.hpp
    ${INCROOT_COR}/Resource.hpp
    ${INCROOT_COR}/ResourceManager.hpp
    ${INCROOT_COR}/Scene.hpp
    ${INCROOT_COR}/SerializeInfo.hpp
    ${INCROOT_COR}/Serializer.hpp
    ${INCROOT_COR}/SettingCallback.hpp
    ${INCROOT_COR}/SettingManager.hpp
    ${INCROOT_COR}/Subsystem.hpp
)
set(INL_COR
    ${INCROOT_COR}/Inl/DebugHandler.inl
    ${INCROOT_COR}/Inl/Engine.inl
    ${INCROOT_COR}/Inl/Object.inl
    ${INCROOT_COR}/Inl/ResourceManager.inl
    ${INCROOT_COR}/Inl/Scene.inl
    ${INCROOT_COR}/Inl/Serializer.inl
    ${INCROOT_COR}/Inl/SettingCallback.inl
)
set(SRC_COR
    ${SRCROOT_COR}/Component.cpp
    ${SRCROOT_COR}/DebugHandler.cpp
    ${SRCROOT_COR}/Engine.cpp
    ${SRCROOT_COR}/FileLoader.cpp
    ${SRCROOT_COR}/Object.cpp
    ${SRCROOT_COR}/Resource.cpp
    ${SRCROOT_COR}/ResourceManager.cpp
    ${SRCROOT_COR}/Scene.cpp
    ${SRCROOT_COR}/SerializeInfo.cpp
    ${SRCROOT_COR}/Serializer.cpp
    ${SRCROOT_COR}/SettingCallback.cpp
    ${SRCROOT_COR}/SettingManager.cpp
    ${SRCROOT_COR}/Subsystem.cpp
)
source_group("Core\\Headers" FILES ${INC_COR})
source_group("Core\\Inl" FILES ${INL_COR})
source_group("Core\\Source" FILES ${SRC_COR})


# Graphics
set(INCROOT_GRA ${PROJECT_SOURCE_DIR}/include/Jopnal/Graphics)
set(SRCROOT_GRA ${PROJECT_SOURCE_DIR}/src/Jopnal/Graphics)

set(INC_GRA_MESH
    ${INCROOT_GRA}/BoxMesh.hpp
    ${INCROOT_GRA}/Mesh.hpp
    ${INCROOT_GRA}/RectangleMesh.hpp
    ${INCROOT_GRA}/SphereMesh.hpp
)
set(INC_GRA_TEXTURE
    ${INCROOT_GRA}/Cubemap.hpp
    ${INCROOT_GRA}/CubemapDepth.hpp
    ${INCROOT_GRA}/Texture.hpp
    ${INCROOT_GRA}/Texture2D.hpp
    ${INCROOT_GRA}/Texture2DDepth.hpp
    ${INCROOT_GRA}/TextureSampler.hpp
)
set(INC_GRA
    ${INCROOT_GRA}/Buffer.hpp
    ${INCROOT_GRA}/Camera.hpp
    ${INCROOT_GRA}/Color.hpp
    ${INCROOT_GRA}/Drawable.hpp
    ${INCROOT_GRA}/EnvironmentRecorder.hpp
    ${INCROOT_GRA}/Font.hpp
    ${INCROOT_GRA}/GenericDrawable.hpp
    ${INCROOT_GRA}/GlState.hpp
    ${INCROOT_GRA}/LightSource.hpp
    ${INCROOT_GRA}/Material.hpp
    ${INCROOT_GRA}/Model.hpp
    ${INCROOT_GRA}/ModelLoader.hpp
    ${INCROOT_GRA}/OpenGL.hpp
    ${INCROOT_GRA}/PostProcessor.hpp
    ${INCROOT_GRA}/Renderer.hpp
    ${INCROOT_GRA}/RenderTarget.hpp
    ${INCROOT_GRA}/RenderTexture.hpp
    ${INCROOT_GRA}/Shader.hpp
    ${INCROOT_GRA}/ShaderAssembler.hpp
    ${INCROOT_GRA}/SkyBox.hpp
    ${INCROOT_GRA}/SkySphere.hpp
    ${INCROOT_GRA}/Text.hpp
    ${INCROOT_GRA}/Transform.hpp
    ${INCROOT_GRA}/Vertex.hpp
    ${INCROOT_GRA}/VertexBuffer.hpp
)
set(SRC_GRA_MESH
    ${SRCROOT_GRA}/BoxMesh.cpp
    ${SRCROOT_GRA}/Mesh.cpp
    ${SRCROOT_GRA}/RectangleMesh.cpp
    ${SRCROOT_GRA}/SphereMesh.cpp
)
set(SRC_GRA_TEXTURE
    ${SRCROOT_GRA}/Cubemap.cpp
    ${SRCROOT_GRA}/CubemapDepth.cpp
    ${SRCROOT_GRA}/Texture.cpp
    ${SRCROOT_GRA}/Texture2D.cpp
    ${SRCROOT_GRA}/Texture2DDepth.cpp
    ${SRCROOT_GRA}/TextureSampler.cpp
)
set(SRC_GRA
    ${SRCROOT_GRA}/Buffer.cpp
    ${SRCROOT_GRA}/Camera.cpp
    ${SRCROOT_GRA}/Color.cpp
    ${SRCROOT_GRA}/Drawable.cpp
    ${SRCROOT_GRA}/EnvironmentRecorder.cpp
    ${SRCROOT_GRA}/Font.cpp
    ${SRCROOT_GRA}/GenericDrawable.cpp
    ${SRCROOT_GRA}/GlCheck.cpp
    ${SRCROOT_GRA}/GlCheck.hpp
    ${SRCROOT_GRA}/GlState.cpp
    ${SRCROOT_GRA}/LightSource.cpp
    ${SRCROOT_GRA}/Material.cpp
    ${SRCROOT_GRA}/Model.cpp
    ${SRCROOT_GRA}/ModelLoader.cpp
    ${SRCROOT_GRA}/OpenGL.cpp
    ${SRCROOT_GRA}/PostProcessor.cpp
    ${SRCROOT_GRA}/Renderer.cpp
    ${SRCROOT_GRA}/RenderTarget.cpp
    ${SRCROOT_GRA}/RenderTexture.cpp
    ${SRCROOT_GRA}/Shader.cpp
    ${SRCROOT_GRA}/ShaderAssembler.cpp
    ${SRCROOT_GRA}/SkyBox.cpp
    ${SRCROOT_GRA}/SkySphere.cpp
    ${SRCROOT_GRA}/Text.cpp
    ${SRCROOT_GRA}/Transform.cpp
    ${SRCROOT_GRA}/Vertex.cpp
    ${SRCROOT_GRA}/VertexBuffer.cpp
)
source_group("Graphics\\Headers" FILES ${INC_GRA})
source_group("Graphics\\Headers\\Mesh" FILES ${INC_GRA_MESH})
source_group("Graphics\\Headers\\Texture" FILES ${INC_GRA_TEXTURE})
source_group("Graphics\\Source" FILES ${SRC_GRA})
source_group("Graphics\\Source\\Mesh" FILES ${SRC_GRA_MESH})
source_group("Graphics\\Source\\Texture" FILES ${SRC_GRA_TEXTURE})

list(APPEND INC_GRA ${INC_GRA_MESH} ${INC_GRA_TEXTURE})
list(APPEND SRC_GRA ${SRC_GRA_MESH} ${SRC_GRA_TEXTURE})


# Physics
set(INCROOT_PHY ${PROJECT_SOURCE_DIR}/include/Jopnal/Physics)
set(SRCROOT_PHY ${PROJECT_SOURCE_DIR}/src/Jopnal/Physics)

set(INC_PHY_SHAPE
    ${INCROOT_PHY}/Shape/BoxShape.hpp
    ${INCROOT_PHY}/Shape/CapsuleShape.hpp
    ${INCROOT_PHY}/Shape/CollisionShape.hpp
    ${INCROOT_PHY}/Shape/CompoundShape.hpp
    ${INCROOT_PHY}/Shape/ConeShape.hpp
    ${INCROOT_PHY}/Shape/ConvexHullShape.hpp
    ${INCROOT_PHY}/Shape/CylinderShape.hpp
    ${INCROOT_PHY}/Shape/FrustumShape.hpp
    ${INCROOT_PHY}/Shape/InfinitePlaneShape.hpp
    ${INCROOT_PHY}/Shape/RectangleShape.hpp
    ${INCROOT_PHY}/Shape/SphereShape.hpp
    ${INCROOT_PHY}/Shape/TerrainShape.hpp
)
set(INC_PHY
    ${INCROOT_PHY}/Collider.hpp
    ${INCROOT_PHY}/ContactInfo.hpp
    ${INCROOT_PHY}/PhantomBody.hpp
    ${INCROOT_PHY}/RayInfo.hpp
    ${INCROOT_PHY}/RigidBody.hpp
    ${INCROOT_PHY}/World.hpp
)
set(SRC_PHY_DETAIL
    ${SRCROOT_PHY}/Detail/WorldImpl.cpp
    ${SRCROOT_PHY}/Detail/WorldImpl.hpp
)
set(SRC_PHY_SHAPE
    ${SRCROOT_PHY}/Shape/BoxShape.cpp
    ${SRCROOT_PHY}/Shape/CapsuleShape.cpp
    ${SRCROOT_PHY}/Shape/CollisionShape.cpp
    ${SRCROOT_PHY}/Shape/CompoundShape.cpp
    ${SRCROOT_PHY}/Shape/ConeShape.cpp
    ${SRCROOT_PHY}/Shape/ConvexHullShape.cpp
    ${SRCROOT_PHY}/Shape/CylinderShape.cpp
    ${SRCROOT_PHY}/Shape/FrustumShape.cpp
    ${SRCROOT_PHY}/Shape/InfinitePlaneShape.cpp
    ${SRCROOT_PHY}/Shape/RectangleShape.cpp
    ${SRCROOT_PHY}/Shape/SphereShape.cpp
    ${SRCROOT_PHY}/Shape/TerrainShape.cpp
)
set(SRC_PHY
    ${SRCROOT_PHY}/Collider.cpp
    ${SRCROOT_PHY}/ContactInfo.cpp
    ${SRCROOT_PHY}/PhantomBody.cpp
    ${SRCROOT_PHY}/RayInfo.cpp
    ${SRCROOT_PHY}/RigidBody.cpp
    ${SRCROOT_PHY}/World.cpp
)
source_group("Physics\\Headers" FILES ${INC_PHY})
source_group("Physics\\Headers\\Shape" FILES ${INC_PHY_SHAPE})
source_group("Physics\\Source" FILES ${SRC_PHY})
source_group("Physics\\Source\\Detail" FILES ${SRC_PHY_DETAIL})
source_group("Physics\\Source\\Shape" FILES ${SRC_PHY_SHAPE})

list(APPEND INC_PHY ${INC_PHY_SHAPE})
list(APPEND SRC_PHY ${SRC_PHY_SHAPE} ${SRC_PHY_DETAIL})


# Root
set(INCROOT_ROOT ${PROJECT_SOURCE_DIR}/include/Jopnal)
set(SRCROOT_ROOT ${PROJECT_SOURCE_DIR}/src/Jopnal)

set(INC_ROOT
    ${INCROOT_ROOT}/Audio.hpp
    ${INCROOT_ROOT}/Config.hpp
    ${INCROOT_ROOT}/Core.hpp
    ${INCROOT_ROOT}/Export.hpp
    ${INCROOT_ROOT}/Graphics.hpp
    ${INCROOT_ROOT}/Header.hpp
    ${INCROOT_ROOT}/HelperMacros.hpp
    ${INCROOT_ROOT}/Jopnal.hpp
    ${INCROOT_ROOT}/OS.hpp
    ${INCROOT_ROOT}/Physics.hpp
    ${INCROOT_ROOT}/Types.hpp
    ${INCROOT_ROOT}/Utility.hpp
    ${INCROOT_ROOT}/Window.hpp
    ${INCROOT_ROOT}/Android/STL.hpp
)
set(SRC_ROOT
    ${SRCROOT_ROOT}/Precompiled/Precompiled.hpp
    ${PROJECT_SOURCE_DIR}/tools/Jopresource/Resources.hpp
    ${PROJECT_SOURCE_DIR}/tools/Jopresource/Resources.cpp
)
if (NOT JOP_OS_ANDROID)

	set(SRC_ROOT ${SRC_ROOT}
		${SRCROOT_ROOT}/OpenGL/gl_4.5.c
		${SRCROOT_ROOT}/OpenGL/gl_4.5.h)

endif()

source_group("Root\\Headers" FILES ${INC_ROOT})
source_group("Root\\Source" FILES ${SRC_ROOT})


# Utility
set(INCROOT_UTI ${PROJECT_SOURCE_DIR}/include/Jopnal/Utility)
set(INLROOT_UTI ${INCROOT_UTI}/Inl)
set(SRCROOT_UTI ${PROJECT_SOURCE_DIR}/src/Jopnal/Utility)

set(INC_UTI
    ${INCROOT_UTI}/Assert.hpp
    ${INCROOT_UTI}/Clock.hpp
    ${INCROOT_UTI}/CommandHandler.hpp
    ${INCROOT_UTI}/DateTime.hpp
    ${INCROOT_UTI}/DirectoryWatcher.hpp
    ${INCROOT_UTI}/Json.hpp
    ${INCROOT_UTI}/Message.hpp
    ${INCROOT_UTI}/Randomizer.hpp
    ${INCROOT_UTI}/SafeReferenceable.hpp
    ${INCROOT_UTI}/Thread.hpp
)
set(INL_UTI
    ${INLROOT_UTI}/CommandHandler.inl
    ${INLROOT_UTI}/CommandParser.inl
    ${INLROOT_UTI}/Message.inl
    ${INLROOT_UTI}/Randomizer.inl
    ${INLROOT_UTI}/SafeReferenceable.inl
    ${INLROOT_UTI}/Thread.inl
)

if (JOP_OS_WINDOWS)

    set(SRC_UTI_WIN32
        ${SRCROOT_UTI}/Win32/DirectoryWatcherImpl.cpp
        ${SRCROOT_UTI}/Win32/DirectoryWatcherImpl.hpp
        ${SRCROOT_UTI}/Win32/ThreadImpl.cpp
        ${SRCROOT_UTI}/Win32/ThreadImpl.hpp
    )

else ()

	set(SRC_UTI_UNIX
        ${SRCROOT_UTI}/Unix/ThreadImpl.cpp
        ${SRCROOT_UTI}/Unix/ThreadImpl.hpp)

endif()

set(SRC_UTI
    ${SRCROOT_UTI}/Assert.cpp
    ${SRCROOT_UTI}/Clock.cpp
    ${SRCROOT_UTI}/CommandHandler.cpp
    ${SRCROOT_UTI}/DateTime.cpp
    ${SRCROOT_UTI}/DirectoryWatcher.cpp
    ${SRCROOT_UTI}/Json.cpp
    ${SRCROOT_UTI}/Message.cpp
    ${SRCROOT_UTI}/Randomizer.cpp
    ${SRCROOT_UTI}/Thread.cpp
)
source_group("Utility\\Headers" FILES ${INC_UTI})
source_group("Utility\\Inl" FILES ${INL_UTI})
source_group("Utility\\Source" FILES ${SRC_UTI})
if (JOP_OS_WINDOWS)

    source_group("Utility\\Source\\Win32" FILES ${SRC_UTI_WIN32})
    list(APPEND SRC_UTI ${SRC_UTI_WIN32})

else ()

	source_group("Utility\\Source\\Unix" FILES ${SRC_UTI_UNIX})
	list(APPEND SRC_UTI ${SRC_UTI_UNIX})

endif()


# Window
set(INCROOT_WIN ${PROJECT_SOURCE_DIR}/include/Jopnal/Window)
set(INLROOT_WIN ${INCROOT_WIN}/Inl)
set(SRCROOT_WIN ${PROJECT_SOURCE_DIR}/src/Jopnal/Window)

set(INC_WIN
    ${INCROOT_WIN}/Controller.hpp
    ${INCROOT_WIN}/Keyboard.hpp
    ${INCROOT_WIN}/Mouse.hpp
    ${INCROOT_WIN}/VideoInfo.hpp
    ${INCROOT_WIN}/Window.hpp
    ${INCROOT_WIN}/WindowEventHandler.hpp
    ${INCROOT_WIN}/WindowHandle.hpp
)
set(INL_WIN
    ${INLROOT_WIN}/Window.inl
)

if (JOP_OS_WINDOWS OR JOP_OS_LINUX)

    set(SRC_WIN_DESKTOP
        ${SRCROOT_WIN}/Desktop/VideoInfoImpl.cpp
        ${SRCROOT_WIN}/Desktop/VideoInfoImpl.hpp
        ${SRCROOT_WIN}/Desktop/WindowImpl.cpp
        ${SRCROOT_WIN}/Desktop/WindowImpl.hpp
    )

elseif (JOP_OS_ANDROID)

	set(SRC_WIN_ANDROID
        ${SRCROOT_WIN}/Android/VideoInfoImpl.cpp
        ${SRCROOT_WIN}/Android/VideoInfoImpl.hpp
        ${SRCROOT_WIN}/Android/WindowImpl.cpp
        ${SRCROOT_WIN}/Android/WindowImpl.hpp)

endif()

set(SRC_WIN
    ${SRCROOT_WIN}/Keyboard.cpp
    ${SRCROOT_WIN}/VideoInfo.cpp
    ${SRCROOT_WIN}/Window.cpp
    ${SRCROOT_WIN}/WindowEventHandler.cpp
)
source_group("Window\\Headers" FILES ${INC_WIN})
source_group("Window\\Inl" FILES ${INL_WIN})
source_group("Window\\Source" FILES ${SRC_WIN})
if (JOP_OS_WINDOWS OR JOP_OS_LINUX)

    source_group("Window\\Source\\Desktop" FILES ${SRC_WIN_DESKTOP})
    list(APPEND SRC_WIN ${SRC_WIN_DESKTOP})

elseif (JOP_OS_ANDROID)

	source_group("Window\\Source\\Android" FILES ${SRC_WIN_ANDROID})
    list(APPEND SRC_WIN ${SRC_WIN_ANDROID})

endif()


# Setup dependencies

# PhysFS
if (NOT JOP_OS_ANDROID)
    find_package(PhysFS REQUIRED)
else ()
    find_host_package(PhysFS REQUIRED)
endif()

if (NOT PHYSFS_FOUND)
    message(FATAL_ERROR "Couldn't find PhysFS")
endif()

include_directories(${PHYSFS_INCLUDE_DIR})
list(APPEND EXT_LIBS ${PHYSFS_LIBRARY})


# Bullet
if (NOT JOP_OS_ANDROID)
    find_package(Bullet REQUIRED)
else ()
    find_host_package(Bullet REQUIRED)
endif()

if (NOT BULLET_FOUND)
    message(FATAL_ERROR "Couldn't find Bullet")
endif()

include_directories(${BULLET_INCLUDE_DIRS})
list(APPEND EXT_LIBS ${BULLET_LIBRARIES})

# GLFW
if (NOT JOP_OS_ANDROID)

    find_package(GLFW REQUIRED)

    if (NOT GLFW_FOUND)
        message(FATAL_ERROR "Couldn't find GLFW")
    endif()

    include_directories(${GLFW_INCLUDE_DIR})
    list(APPEND EXT_LIBS ${GLFW_glfw_LIBRARY})

endif()


# OpenGL/ES
if (NOT JOP_OPENGL_ES)

    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIR})

endif()

if (JOP_OPENGL_ES AND JOP_OS_LINUX)

    find_package(EGL REQUIRED)
    find_package(GLES REQUIRED)
    include_directories(${EGL_INCLUDE_DIR} ${GLES_INCLUDE_DIR})

endif()

if (NOT JOP_OPENGL_ES)
    list(APPEND EXT_LIBS ${OPENGL_gl_LIBRARY})
endif()

if (JOP_OPENGL_ES AND JOP_OS_LINUX)
    list(APPEND EXT_LIBS ${EGL_LIBRARY} ${GLES_LIBRARY})
endif()

if (JOP_OS_ANDROID)
    list(APPEND EXT_LIBS z android log EGL GLESv3 OpenSLES Bullet)
endif()


# OpenAL
if (NOT JOP_OS_ANDROID)
    find_package(OpenAL REQUIRED)
else ()
    find_host_package(OpenAL REQUIRED)
endif()

if (NOT OPENAL_FOUND)
    message(FATAL_ERROR "Couldn't find OpenAL")
endif()

include_directories(${OPENAL_INCLUDE_DIR})
list(APPEND EXT_LIBS ${OPENAL_LIBRARY})


# Include resource files
include(${CMAKE_CURRENT_SOURCE_DIR}/Resources/CMakeLists.txt)


# Preprocessor symbols
add_definitions(-DSTBI_FAILURE_USERMSG)


# Add target
jopAddLibrary(jopnal 
              SOURCES ${INC_AUD} ${SRC_AUD}
                      ${INC_COR} ${INL_COR} ${SRC_COR}
                      ${INC_GRA} ${SRC_GRA}
                      ${INC_PHY} ${SRC_PHY}
                      ${INC_ROOT} ${SRC_ROOT}
                      ${INC_UTI} ${INL_UTI} ${SRC_UTI}
                      ${INC_WIN} ${INL_WIN} ${SRC_WIN}
                      ${SRC_ANDROID}
              EXTERNAL_LIBS ${EXT_LIBS})
              
# Add the resource target as a dependency
if (TARGET jopnal-resources) 
    add_dependencies(jopnal jopnal-resources)
endif()

# Precompiled header
if (NOT JOP_COMPILER_GCC)

    add_definitions(-DJOP_PRECOMPILED_HEADER)
	add_definitions(-DJOP_PRECOMPILED_HEADER_FILE=<Jopnal/Precompiled/Precompiled.hpp>)
                                                                  
    set_target_properties(jopnal PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "${CMAKE_CURRENT_SOURCE_DIR}/Precompiled/Precompiled.hpp")
    cotire(jopnal)

else()

	add_definitions(-DJOP_PRECOMPILED_HEADER_FILE=<Jopnal/Precompiled/PrecompiledEmpty.hpp>)

endif()