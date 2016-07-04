# Jopnal Engine CMakeLists

# 3.1 is the minimum for pdb file generation
cmake_minimum_required(VERSION 3.1.0)

# Helper for defining options
macro(jopSetOption var default type docstring)

    if (NOT DEFINED ${var})
    
        set(${var} ${default})
        
    endif()

    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)

endmacro()


# Configuration option
jopSetOption(CMAKE_BUILD_TYPE Release STRING "Type of build (Debug or Release)")

# Android options, must be set in the beginning

# API level, must be at least 18 for GLES 3.0
jopSetOption(ANDROID_API_MIN 18 STRING "Choose the Android API level (greater or equal to 18)")
# Setup version value for the toolchain
set(ANDROID_NATIVE_API_LEVEL ${ANDROID_API_MIN})

# Set toolchain to use
jopSetOption(ANDROID_TOOLCHAIN_MAKE "arm-linux-androideabi-4.9" STRING "Choose the android toolchain to use")

# Enable RTTI
set(ANDROID_RTTI TRUE)

# NDK path
jopSetOption(ANDROID_NDK "$ENV{ANDROID_NDK}" PATH "Path to the Android NDK")
# STL implementation
jopSetOption(ANDROID_STL c++_shared STRING "Choose the STL implementation")

# Default to v7a for harware floating point
if(NOT ANDROID_ABI)
    set(ANDROID_ABI armeabi-v7a)
endif()

# Project name
project(Jopnal)


# Include config
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

# Hide non-essential options
mark_as_advanced(COTIRE_ADDITIONAL_PREFIX_HEADER_IGNORE_EXTENSIONS)
mark_as_advanced(COTIRE_ADDITIONAL_PREFIX_HEADER_IGNORE_PATH)
mark_as_advanced(COTIRE_DEBUG)
mark_as_advanced(COTIRE_MAXIMUM_NUMBER_OF_UNITY_INCLUDES)
mark_as_advanced(COTIRE_MINIMUM_NUMBER_OF_TARGET_SOURCES)
mark_as_advanced(COTIRE_UNITY_SOURCE_EXCLUDE_EXTENSIONS)
mark_as_advanced(COTIRE_VERBOSE)
mark_as_advanced(PHYSFS_INCLUDE_DIR)
mark_as_advanced(PHYSFS_LIBRARY)
mark_as_advanced(PHYSFS_LIBRARY)
mark_as_advanced(CMAKE_CONFIGURATION_TYPES)
mark_as_advanced(CMAKE_INSTALL_PREFIX)
mark_as_advanced(BULLET_INCLUDE_DIR)
mark_as_advanced(ANDROID_NATIVE_API_LEVEL)


# Cotire for precompiled headers
set (COTIRE_PCH_MEMORY_SCALING_FACTOR "500")
include(cotire)

# Include the configuration file
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake)

# Option for precompiled headers
set(USE_PRECOMPILED_HEADER_DEFAULT TRUE)

# Set precompiled header utilization to false on android & MinGW, as
# it tends to crash the compiler
if (JOP_OS_ANDROID OR (JOP_OS_WINDOWS AND JOP_COMPILER_GCC))
    set(USE_PRECOMPILED_HEADER_DEFAULT FALSE)
endif() 
   
jopSetOption(USE_PRECOMPILED_HEADER ${USE_PRECOMPILED_HEADER_DEFAULT} BOOL "TRUE to use precompiled header")

if (USE_PRECOMPILED_HEADER AND JOP_COMPILER_GCC)
    message(WARNING "Using precompiled header may crash GCC. Keep this in mind")
endif()

# Extract extlibs.7z        
if ((NOT IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/bin) OR
    (NOT IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/include) OR
    (NOT IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/lib))

    execute_process(COMMAND 7z x -y extlibs.7z
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/extlibs
                    RESULT_VARIABLE DECOMPRESS_RESULT)

    if (${DECOMPRESS_RESULT} GREATER 0)
        message(WARNING "Received errors during extlibs.7z extraction. Manual extraction might be necessary")
    endif()

else ()

    message(STATUS "External libraries seem to be already extracted")

endif()

# Version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 3)

# Header path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Option for choosing a shared build
jopSetOption(BUILD_SHARED_LIBS TRUE BOOL "TRUE to build Jopnal as shared, FALSE to build as static")

# Option for choosing the OpenGL implementation
jopSetOption(JOP_OPENGL_ES ${OPENGL_ES} BOOL "TRUE to use an OpenGL ES, FALSE to use a desktop OpenGL")

# Android options
if (JOP_OS_ANDROID)

    # Make sure there's the android library available
    if (${ANDROID_API_MIN} LESS 18)
        message(FATAL_ERROR "Android API level must be equal or greater than 18. Please adjust the CMake variable 'ANDROID_API_MIN'")
    endif()

    if (NOT ANDROID_NDK)
        message(FATAL_ERROR "The Android NDK couldn't be found. Please adjust the CMake variable 'ANDROID_NDK' to point to the NDK directory")
    endif()

    # CMake doesn't support defining the STL with Nsight Tegra
    if (CMAKE_VS_PLATFORM_NAME STREQUAL "Tegra-Android")
        message(WARNING "CMake might not properly support setting the STL. Make sure to adjust all generated library projects!")
    endif()

    # Install everything in $NDK/sources/
    set(CMAKE_INSTALL_PREFIX ${ANDROID_NDK}/sources/jopnal)

    # Install libs in a subdirectory named after the ABI
    set(LIB_SUFFIX "/${ANDROID_ABI}")

    # Pass shared STL configuration
    if (ANDROID_STL MATCHES "_shared")
        add_definitions("-DSTL_LIBRARY=${ANDROID_STL}")
    endif()

    # Workaround to compile activity without the stl library as a dependency
    set(CMAKE_CXX_CREATE_SHARED_LIBRARY_WITH_STL ${CMAKE_CXX_CREATE_SHARED_LIBRARY})
    set(CMAKE_CXX_CREATE_SHARED_LIBRARY_WITHOUT_STL "<CMAKE_CXX_COMPILER> <CMAKE_SHARED_LIBRARY_CXX_FLAGS> <LANGUAGE_COMPILE_FLAGS> <LINK_FLAGS> <CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS> <SONAME_FLAG><TARGET_SONAME> -o <TARGET> <OBJECTS> <LINK_LIBRARIES>")

else()

    unset(ANDROID_ABI CACHE)
    unset(ANDROID_API_MIN CACHE)
    unset(ANDROID_STL CACHE)
    unset(ANDROID_NATIVE_API_LEVEL CACHE)
    unset(ANDROID_NDK CACHE)

endif()

# Define JOP_STATIC to build static library
if (NOT BUILD_SHARED_LIBS)
    add_definitions(-DJOP_STATIC)
endif()

if (JOP_COMPILER_MSVC)

    jopSetOption(JOP_GENERATE_PDB TRUE BOOL "True to generate PDB debug symbol files, FALSE otherwise")
	
	# - Disable deprecation warnings
	# - Set warning level to 4
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS /W4)

endif()

# Define JOP_OPENGL_ES if configured
if (JOP_OPENGL_ES)

    add_definitions(-DJOP_OPENGL_ES)
    add_definitions(-DGL_GLEXT_PROTOTYPES)

endif()

# Force inclusion of available c++11 features
set(CMAKE_CXX_STANDARD 11)

# Enable project folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

# Add the subdirectories
add_subdirectory(src/Jopnal)

# Setup the install rules
install(DIRECTORY include
        DESTINATION .
        COMPONENT devel
        FILES_MATCHING PATTERN "*.hpp" PATTERN "*.inl" PATTERN "*.h")

if (JOP_GENERATE_PDB)
    install(DIRECTORY ${PROJECT_BINARY_DIR}/lib
            DESTINATION .
            COMPONENT devel
            FILES_MATCHING PATTERN "*.pdb")
endif()

install(FILES LICENSE DESTINATION ${INSTALL_MISC_DIR})
install(FILES README.md DESTINATION ${INSTALL_MISC_DIR})

# Install 3rd-party libraries and tools
if (JOP_OS_WINDOWS)

    if (ARCH_32BITS)

        install(DIRECTORY extlibs/bin/x86/ DESTINATION bin)

        if (JOP_COMPILER_MSVC AND JOP_MSVC_VERSION LESS 14)
            install(DIRECTORY extlibs/lib/msvc/x86/ DESTINATION lib)

        elseif (JOP_COMPILER_MSVC)
            install(DIRECTORY extlibs/lib/msvc-universal/x86/ DESTINATION lib)

        else ()
            install(DIRECTORY extlibs/lib/mingw/x86/ DESTINATION lib)

        endif()

    elseif (ARCH_64BITS)

        install(DIRECTORY extlibs/bin/x64/ DESTINATION bin)

        if (JOP_COMPILER_MSVC AND JOP_MSVC_VERSION LESS 14)
            install(DIRECTORY extlibs/lib/msvc/x64/ DESTINATION lib)

        elseif (JOP_COMPILER_MSVC)
            install(DIRECTORY extlibs/lib/msvc-universal/x64/ DESTINATION lib)

        else ()
            install(DIRECTORY extlibs/lib/mingw/x64/ DESTINATION lib)

        endif()

    endif()

elseif (JOP_OS_ANDROID)

    # Install extlibs
    install(DIRECTORY extlibs/lib/android/${ANDROID_ABI} DESTINATION extlibs/lib)
    install(FILES extlibs/Android.mk DESTINATION extlibs)

    # Install Android.mk
    if (BUILD_SHARED_LIBS)
        install(FILES tools/Android/MakefileShared/Android.mk DESTINATION .)
    else ()
        install(FILES tools/Android/MakefileStatic/Android.mk DESTINATION .)
    endif()
    
endif()