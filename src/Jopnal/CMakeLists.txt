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

# Audio
include(${CMAKE_CURRENT_SOURCE_DIR}/Audio/CMakeLists.txt)

# Core
include(${CMAKE_CURRENT_SOURCE_DIR}/Core/CMakeLists.txt)

# Graphics
include(${CMAKE_CURRENT_SOURCE_DIR}/Graphics/CMakeLists.txt)

# Physics
include(${CMAKE_CURRENT_SOURCE_DIR}/Physics/CMakeLists.txt)

# Precompiled
include(${CMAKE_CURRENT_SOURCE_DIR}/Precompiled/CMakeLists.txt)

# Resources
include(${CMAKE_CURRENT_SOURCE_DIR}/Resources/CMakeLists.txt)

# Root
include(${CMAKE_CURRENT_SOURCE_DIR}/Root/CMakeLists.txt)

# Utility
include(${CMAKE_CURRENT_SOURCE_DIR}/Utility/CMakeLists.txt)

# Window
include(${CMAKE_CURRENT_SOURCE_DIR}/Window/CMakeLists.txt)


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
    list(APPEND EXT_LIBS z android log EGL GLESv3 OpenSLES)
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

# Preprocessor symbols
add_definitions(-DSTBI_FAILURE_USERMSG)

# Add target
jopAddLibrary(jopnal SOURCES ${SRC} EXTERNAL_LIBS ${EXT_LIBS})
              
# Add the resource target as a dependency
if (TARGET jopnal-resources) 
    add_dependencies(jopnal jopnal-resources)
endif()

# Precompiled header
if (NOT JOP_COMPILER_GCC AND USE_PRECOMPILED_HEADER)

    add_definitions(-DJOP_PRECOMPILED_HEADER)
	add_definitions(-DJOP_PRECOMPILED_HEADER_FILE=<Jopnal/Precompiled/Precompiled.hpp>)
                                                                  
    set_target_properties(jopnal PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "${CMAKE_CURRENT_SOURCE_DIR}/Precompiled/Precompiled.hpp")
    cotire(jopnal)

else()

	add_definitions(-DJOP_PRECOMPILED_HEADER_FILE=<Jopnal/Precompiled/PrecompiledEmpty.hpp>)

endif()