# Jopnal Source CMakeLists

# Include macros
include(${PROJECT_SOURCE_DIR}/cmake/Macros.cmake)

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

# Physics2D
include(${CMAKE_CURRENT_SOURCE_DIR}/Physics2D/CMakeLists.txt)

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


# Add target
jopAddLibrary(jopnal SOURCES ${SRC})
              
# Add the resource target as a dependency
if (TARGET jopnal-resources) 
    add_dependencies(jopnal jopnal-resources)
endif()

# Precompiled header
if (NOT JOP_COMPILER_GCC AND JOP_USE_PRECOMPILED_HEADER)

    add_definitions(-DJOP_PRECOMPILED_HEADER)
    add_definitions(-DJOP_PRECOMPILED_HEADER_FILE=<Jopnal/Precompiled/Precompiled.hpp>)
                                                                  
    set_target_properties(jopnal PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "${CMAKE_CURRENT_SOURCE_DIR}/Precompiled/Precompiled.hpp")
    cotire(jopnal)

else()

    add_definitions(-DJOP_PRECOMPILED_HEADER_FILE=<Jopnal/Precompiled/PrecompiledEmpty.hpp>)

endif()

######################################
# External libraries
######################################

if (JOP_OS_ANDROID)

    find_host_package(Git)

    if (NOT GIT_FOUND)
        message(FATAL_ERROR "Git was not found! Please specify the advanced option GIT_EXECUTABLE to point to git executable")
    endif()

endif()

include(ExternalProject)
include_directories(${PROJECT_SOURCE_DIR}/extlibs/headers)

# Preprocessor symbols
add_definitions(-DSTBI_FAILURE_USERMSG)

if (JOP_OS_ANDROID)

    set(ANDROID_OPTIONS -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
                        -DANDROID_NATIVE_API_LEVEL=${ANDROID_API_MIN}
                        -DANDROID_TOOLCHAIN_MAKE=${ANDROID_TOOLCHAIN_MAKE}
                        -DANDROID_ABI=${ANDROID_ABI}
                        -DANDROID_STL=${ANDROID_STL}
                        -DANDROID_NDK=${ANDROID_NDK}
    )

endif()

set(PREFIX_DIR "${PROJECT_BINARY_DIR}/extlibs")
set(DL_DIR "${PROJECT_SOURCE_DIR}/extlibs/temp")

# PhysFS
ExternalProject_Add(PhysFS
    
                    PREFIX ${PREFIX_DIR}
                    SOURCE_DIR ${DL_DIR}/PhysFS
                    STAMP_DIR ${DL_DIR}/PhysFS-stamp
    
                    GIT_REPOSITORY "https://github.com/Jopnal/physfs.git"
                    GIT_TAG "master"
                    
                    UPDATE_COMMAND ""
                    PATCH_COMMAND ""
                    INSTALL_COMMAND ""
                    
                    CMAKE_ARGS -DPHYSFS_BUILD_SHARED=OFF
                               -DPHYSFS_ARCHIVE_7Z=OFF
                               ${ANDROID_OPTIONS}
                            
                    BUILD_BYPRODUCTS ${PREFIX_DIR}/src/PhysFS-build/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}physfs${CMAKE_STATIC_LIBRARY_SUFFIX}
                    
                    
)
add_dependencies(jopnal PhysFS)

ExternalProject_Get_Property(PhysFS SOURCE_DIR)
include_directories(${SOURCE_DIR}/src)

ExternalProject_Get_Property(PhysFS BINARY_DIR)
list(APPEND EXT_LIBS ${BINARY_DIR}/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}physfs${CMAKE_STATIC_LIBRARY_SUFFIX})


# Bullet
set(BULLET_LIB_COLLISION ${CMAKE_STATIC_LIBRARY_PREFIX}BulletCollision${CMAKE_STATIC_LIBRARY_SUFFIX})
set(BULLET_LIB_DYNAMICS ${CMAKE_STATIC_LIBRARY_PREFIX}BulletDynamics${CMAKE_STATIC_LIBRARY_SUFFIX})
set(BULLET_LIB_SOFTBODY ${CMAKE_STATIC_LIBRARY_PREFIX}BulletSoftBody${CMAKE_STATIC_LIBRARY_SUFFIX})
set(BULLET_LIB_LINEARMATH ${CMAKE_STATIC_LIBRARY_PREFIX}LinearMath${CMAKE_STATIC_LIBRARY_SUFFIX})

ExternalProject_Add(Bullet
    
                    PREFIX ${PREFIX_DIR}
                    SOURCE_DIR ${DL_DIR}/Bullet
                    STAMP_DIR ${DL_DIR}/Bullet-stamp
    
                    GIT_REPOSITORY "https://github.com/Jopnal/bullet3.git"
                    GIT_TAG "master"
                    
                    UPDATE_COMMAND ""
                    PATCH_COMMAND ""
                    INSTALL_COMMAND ""
                    
                    CMAKE_ARGS -DBUILD_BULLET2_DEMOS=OFF
                               -DBUILD_BULLET3=OFF
                               -DBUILD_CPU_DEMOS=OFF
                               -DBUILD_EXTRAS=OFF
                               -DBUILD_OPENGL3_DEMOS=OFF
                               -DBUILD_UNIT_TESTS=OFF
                               -DUSE_GLUT=OFF
                               -DUSE_GRAPHICAL_BENCHMARK=OFF
                               -DUSE_MSVC_RUNTIME_LIBRARY_DLL=ON
                               ${ANDROID_OPTIONS}
                            
                    BUILD_BYPRODUCTS ${PREFIX_DIR}/src/Bullet-build/lib/${BULLET_LIB_DYNAMICS}
                                     ${PREFIX_DIR}/src/Bullet-build/lib/${BULLET_LIB_COLLISION}
                                     ${PREFIX_DIR}/src/Bullet-build/lib/${BULLET_LIB_SOFTBODY}
                                     ${PREFIX_DIR}/src/Bullet-build/lib/${BULLET_LIB_LINEARMATH}
)
add_dependencies(jopnal Bullet)

ExternalProject_Get_Property(Bullet SOURCE_DIR)
include_directories(${SOURCE_DIR}/src)

ExternalProject_Get_Property(Bullet BINARY_DIR)
set(BULLET_LIB_DIR ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR})

if (NOT JOP_OS_ANDROID)
    set(BULLET_LIB_SUFFIX _Debug)
endif()

list(APPEND EXT_LIBS debug ${BULLET_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}BulletDynamics${BULLET_LIB_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
                             debug ${BULLET_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}BulletCollision${BULLET_LIB_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
                             debug ${BULLET_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}BulletSoftBody${BULLET_LIB_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}
                             debug ${BULLET_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}LinearMath${BULLET_LIB_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX})

list(APPEND EXT_LIBS optimized ${BULLET_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}BulletDynamics${CMAKE_STATIC_LIBRARY_SUFFIX}
                             optimized ${BULLET_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}BulletCollision${CMAKE_STATIC_LIBRARY_SUFFIX}
                             optimized ${BULLET_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}BulletSoftBody${CMAKE_STATIC_LIBRARY_SUFFIX}
                             optimized ${BULLET_LIB_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}LinearMath${CMAKE_STATIC_LIBRARY_SUFFIX})

# Because Bullet seems to build the libs in inconsistent directories
if (NOT JOP_OS_WINDOWS)

    add_custom_command(TARGET Bullet POST_BUILD
        
        COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/src/BulletCollision/${BULLET_LIB_COLLISION} ${BINARY_DIR}/lib/${BULLET_LIB_COLLISION}
        COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/src/BulletDynamics/${BULLET_LIB_DYNAMICS} ${BINARY_DIR}/lib/${BULLET_LIB_DYNAMICS}
        COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/src/BulletSoftBody/${BULLET_LIB_SOFTBODY} ${BINARY_DIR}/lib/${BULLET_LIB_SOFTBODY}
        COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/src/LinearMath/${BULLET_LIB_LINEARMATH} ${BINARY_DIR}/lib/${BULLET_LIB_LINEARMATH}
    )
    
endif()


# GLFW
if (NOT JOP_OS_ANDROID)

    ExternalProject_Add(GLFW
        
                        PREFIX ${PREFIX_DIR}
                        SOURCE_DIR ${DL_DIR}/GLFW
                        STAMP_DIR ${DL_DIR}/GLFW-stamp
        
                        GIT_REPOSITORY "https://github.com/glfw/glfw.git"
                        GIT_TAG "3bb5c459d63d7cf9c990213e39303d9ba5eaebcc"
                        
                        UPDATE_COMMAND ""
                        PATCH_COMMAND ""
                        INSTALL_COMMAND ""
                        
                        CMAKE_ARGS -DGLFW_BUILD_DOCS=OFF
                                   -DGLFW_BUILD_EXAMPLES=OFF
                                   -DGLFW_BUILD_TESTS=OFF
                                   -DGLFW_INSTALL=OFF
                            
                        BUILD_BYPRODUCTS ${PREFIX_DIR}/src/GLFW-build/src/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX}
    )
    add_dependencies(jopnal GLFW)

    ExternalProject_Get_Property(GLFW SOURCE_DIR)
    include_directories(${SOURCE_DIR}/include)

    ExternalProject_Get_Property(GLFW BINARY_DIR)
    list(APPEND EXT_LIBS ${BINARY_DIR}/src/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX})

endif()

# OpenGL/ES
if (JOP_OS_ANDROID)

    if (ANDROID_API_MIN GREATER 17)
        set(ES_LIB GLESv3)
    else ()
        set(ES_LIB GLESv2)
    endif()
    
    list(APPEND EXT_LIBS android log EGL ${ES_LIB} OpenSLES)

elseif (NOT JOP_OPENGL_ES)

    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIR})
    list(APPEND EXT_LIBS ${OPENGL_gl_LIBRARY})

endif()

if (JOP_OPENGL_ES AND JOP_OS_LINUX)

    find_package(EGL REQUIRED)
    find_package(GLES REQUIRED)
    include_directories(${EGL_INCLUDE_DIR} ${GLES_INCLUDE_DIR})
    list(APPEND EXT_LIBS ${EGL_LIBRARY} ${GLES_LIBRARY})

endif()

# OpenAL
if (JOP_OS_WINDOWS)
    set(OPENAL_LIB_NAME OpenAL32.lib)
else ()
    set(OPENAL_LIB_NAME ${CMAKE_SHARED_LIBRARY_PREFIX}openal${CMAKE_SHARED_LIBRARY_SUFFIX})
endif()

ExternalProject_Add(OpenAL
    
                    PREFIX ${PREFIX_DIR}
                    SOURCE_DIR ${DL_DIR}/OpenAL
                    STAMP_DIR ${DL_DIR}/OpenAL-stamp
    
                    GIT_REPOSITORY "https://github.com/kcat/openal-soft.git"
                    GIT_TAG "0189b923a233e269b4c8f9d17f6475ace5e2e4de"
                    
                    UPDATE_COMMAND ""
                    PATCH_COMMAND ""
                    INSTALL_COMMAND ""
                    
                    CMAKE_ARGS -DALSOFT_INSTALL=OFF
                               -DALSOFT_TESTS=OFF
                               -DALSOFT_UTILS=ON
                               ${ANDROID_OPTIONS}
                            
                    BUILD_BYPRODUCTS ${PREFIX_DIR}/src/OpenAL-build/${OPENAL_LIB_NAME}
)
add_dependencies(jopnal OpenAL)

ExternalProject_Get_Property(OpenAL SOURCE_DIR)
include_directories(${SOURCE_DIR}/include)

ExternalProject_Get_Property(OpenAL BINARY_DIR)
list(APPEND EXT_LIBS ${BINARY_DIR}/${CMAKE_CFG_INTDIR}/${OPENAL_LIB_NAME})

if (JOP_OS_ANDROID)
    install(FILES ${BINARY_DIR}/${CMAKE_CFG_INTDIR}/${OPENAL_LIB_NAME} DESTINATION extlibs/lib/${ANDROID_ABI_DIR})
    
elseif(JOP_OS_WINDOWS)

    add_custom_command(TARGET OpenAL POST_BUILD
        
        COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/${CMAKE_CFG_INTDIR}/OpenAL32.dll ${PREFIX_DIR}/dll/OpenAL32.dll)
    
endif()

# Ogg
ExternalProject_Add(Ogg
                    
                    PREFIX ${PREFIX_DIR}
                    SOURCE_DIR ${DL_DIR}/Ogg
                    STAMP_DIR ${DL_DIR}/Ogg-stamp
                    
                    GIT_REPOSITORY "https://git.xiph.org/ogg.git"
                    GIT_TAG "18c401c6bc8814d06f3ae53ebf5d4399f90871cc"
                    
                    UPDATE_COMMAND ""
                    PATCH_COMMAND ""
                    INSTALL_COMMAND ""

                    CMAKE_ARGS ${ANDROID_OPTIONS}
                    
                    BUILD_BYPRODUCTS ${PREFIX_DIR}/src/Ogg-build/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}ogg${CMAKE_STATIC_LIBRARY_SUFFIX}
)
ExternalProject_Get_Property(Ogg SOURCE_DIR)
include_directories(${SOURCE_DIR}/include)

ExternalProject_Get_Property(Ogg BINARY_DIR)
set(OGG_LIBRARY ${BINARY_DIR}/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}ogg${CMAKE_STATIC_LIBRARY_SUFFIX})

# Vorbis
ExternalProject_Add(Vorbis
                    
                    PREFIX ${PREFIX_DIR}
                    SOURCE_DIR ${DL_DIR}/Vorbis
                    STAMP_DIR ${DL_DIR}/Vorbis-stamp
                    
                    GIT_REPOSITORY "https://git.xiph.org/vorbis.git"
                    GIT_TAG "d8ffc480787fcd8b5bb7203d6e0acf3bbfb2dd02"
                    
                    UPDATE_COMMAND ""
                    PATCH_COMMAND ""
                    INSTALL_COMMAND ""

                    CMAKE_ARGS -DOGG_INCLUDE_DIRS=${SOURCE_DIR}/include
                               -DOGG_LIBRARIES=${BINARY_DIR}/${CMAKE_CFG_INTDIR}
                               ${ANDROID_OPTIONS}
                    
                    BUILD_BYPRODUCTS ${PREFIX_DIR}/src/Vorbis-build/lib/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}vorbisfile${CMAKE_STATIC_LIBRARY_SUFFIX}
                                     ${PREFIX_DIR}/src/Vorbis-build/lib/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}vorbisenc${CMAKE_STATIC_LIBRARY_SUFFIX}
                                     ${PREFIX_DIR}/src/Vorbis-build/lib/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}vorbis${CMAKE_STATIC_LIBRARY_SUFFIX}
                                     
                    DEPENDS Ogg
)
add_dependencies(jopnal Vorbis)

ExternalProject_Get_Property(Vorbis SOURCE_DIR)
include_directories(${SOURCE_DIR}/include)

ExternalProject_Get_Property(Vorbis BINARY_DIR)
list(APPEND EXT_LIBS ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}vorbisfile${CMAKE_STATIC_LIBRARY_SUFFIX}
                             ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}vorbisenc${CMAKE_STATIC_LIBRARY_SUFFIX}
                             ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}vorbis${CMAKE_STATIC_LIBRARY_SUFFIX}
                             ${OGG_LIBRARY}) # Apparently ogg has to be linked after vorbis to avoid linker errors

# Box2D
ExternalProject_Add(Box2D
    
                    PREFIX ${PREFIX_DIR}
                    SOURCE_DIR ${DL_DIR}/Box2D
                    STAMP_DIR ${DL_DIR}/Box2D-stamp
    
                    GIT_REPOSITORY "https://github.com/Jopnal/Box2D.git"
                    GIT_TAG "master"
                    
                    UPDATE_COMMAND ""
                    PATCH_COMMAND ""
                    INSTALL_COMMAND ""
                    
                    CMAKE_ARGS -DBOX2D_BUILD_EXAMPLES=OFF
                               ${ANDROID_OPTIONS}
                    
                    BUILD_BYPRODUCTS ${PREFIX_DIR}/src/Box2D-build/lib/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}Box2D${CMAKE_STATIC_LIBRARY_SUFFIX}
)
add_dependencies(jopnal Box2D)

ExternalProject_Get_Property(Box2D SOURCE_DIR)
include_directories(${SOURCE_DIR}/include)

ExternalProject_Get_Property(Box2D BINARY_DIR)
list(APPEND EXT_LIBS ${BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}Box2D${CMAKE_STATIC_LIBRARY_SUFFIX})

# Combine static libraries on VC++
if (JOP_COMPILER_MSVC AND NOT BUILD_SHARED_LIBS)

    set(DEBUG_LIBRARIES "")
    set(RELEASE_LIBRARIES "")
    set(ADD_DEBUG FALSE)
    set(ADD_RELEASE FALSE)
    
    foreach(lib ${EXT_LIBS})
    
        if (${lib} MATCHES "debug")
        
            set(ADD_DEBUG TRUE)
            continue()
        
        elseif (${lib} MATCHES "optimized")
        
            set(ADD_RELEASE TRUE)
            continue()
        
        endif()
        
        if (NOT ${lib} MATCHES ".*\\.lib")
             set(lib ${lib}.lib)
        endif()
        
        if (ADD_DEBUG)
            set(DEBUG_LIBRARIES "${DEBUG_LIBRARIES} \"${lib}\"")
            set(ADD_DEBUG FALSE)
            
        elseif (ADD_RELEASE)
            set(RELEASE_LIBRARIES "${RELEASE_LIBRARIES} \"${lib}\"")
            set(ADD_RELEASE FALSE)
            
        else ()
            set(DEBUG_LIBRARIES "${DEBUG_LIBRARIES} \"${lib}\"")
            set(RELEASE_LIBRARIES "${RELEASE_LIBRARIES} \"${lib}\"")
            
        endif()
        
    endforeach()
     
    set_target_properties(jopnal PROPERTIES STATIC_LIBRARY_FLAGS_DEBUG ${DEBUG_LIBRARIES})
    set_target_properties(jopnal PROPERTIES STATIC_LIBRARY_FLAGS_RELEASE ${LIBRARIES} ${RELEASE_LIBRARIES})
    set_target_properties(jopnal PROPERTIES STATIC_LIBRARY_FLAGS "/ignore:4221 /ignore:4006")

else ()
    target_link_libraries(jopnal ${EXT_LIBS})
    
endif()