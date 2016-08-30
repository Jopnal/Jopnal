include(CMakeParseArguments)

# Add a new target
macro(jopAddLibrary target)

    # Parse the arguments
    cmake_parse_arguments(THIS "" "" "SOURCES" ${ARGN})

    # Create the target
    add_library(${target} ${THIS_SOURCES})

    # Define the export symbol of the module
    set_target_properties(${target} PROPERTIES DEFINE_SYMBOL JOP_EXPORT_API)

    # Adjust the output file prefix/suffix to match our conventions
    if (BUILD_SHARED_LIBS)

        if (JOP_OS_WINDOWS)

            set_target_properties(${target} PROPERTIES DEBUG_POSTFIX d)

        else()
            set_target_properties(${target} PROPERTIES DEBUG_POSTFIX d)

        endif()

        if (JOP_OS_WINDOWS AND JOP_COMPILER_GCC)

            set_target_properties(${target} PROPERTIES PREFIX "")
            set_target_properties(${target} PROPERTIES IMPORT_SUFFIX ".a")

        endif()

    else()

        set_target_properties(${target} PROPERTIES DEBUG_POSTFIX sd)
        set_target_properties(${target} PROPERTIES RELEASE_POSTFIX s)
        set_target_properties(${target} PROPERTIES MINSIZEREL_POSTFIX s)
        set_target_properties(${target} PROPERTIES RELWITHDEBINFO_POSTFIX s)

    endif()

    # Set the version and soversion of the target for compatible systems
    if (NOT JOP_OS_ANDROID)

        set_target_properties(${target} PROPERTIES SOVERSION ${VERSION_MAJOR}.${VERSION_MINOR})
        set_target_properties(${target} PROPERTIES VERSION ${VERSION_MAJOR}.${VERSION_MINOR})

    endif()

    # Set the target's folder (for IDEs that support it)
    set_target_properties(${target} PROPERTIES FOLDER "jopnal")

    # For Visual Studio on Windows, export debug symbols (PDB files) to lib directory
    if (JOP_GENERATE_PDB)

        # PDB files are only generated in Debug and RelWithDebInfo configurations, find out which one
        if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
            set(JOP_PDB_POSTFIX "d")

        else()
            set(JOP_PDB_POSTFIX "")

        endif()

        if (BUILD_SHARED_LIBS)
            # DLLs export debug symbols in the linker PDB (the compiler PDB is an intermediate file)
            set_target_properties(${target} PROPERTIES
                                  PDB_NAME "${target}${JOP_PDB_POSTFIX}"
                                  PDB_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

        else()
            # Static libraries have no linker PDBs, thus the compiler PDBs are relevant
            set_target_properties(${target} PROPERTIES
                                  COMPILE_PDB_NAME "${target}s${JOP_PDB_POSTFIX}"
                                  COMPILE_PDB_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

        endif()

    endif()

    # if using gcc >= 4.0 or clang >= 3.0 on a non-Windows platform, we must hide public symbols by default
    # (exported ones are explicitly marked)
    if(NOT JOP_OS_WINDOWS AND ((JOP_COMPILER_GCC AND NOT JOP_GCC_VERSION VERSION_LESS "4") OR (JOP_COMPILER_CLANG AND NOT JOP_CLANG_VERSION VERSION_LESS "3")))
        set_target_properties(${target} PROPERTIES COMPILE_FLAGS -fvisibility=hidden)
    endif()

    # jopnal-activity is the bootstrap activity and must not depend on stlport_shared
    if (JOP_OS_ANDROID)

        if (${target} MATCHES "jopnal-activity")

            set_target_properties(${target} PROPERTIES COMPILE_FLAGS -fpermissive)
            set_target_properties(${target} PROPERTIES LINK_FLAGS "-landroid -llog")
            set(CMAKE_CXX_CREATE_SHARED_LIBRARY ${CMAKE_CXX_CREATE_SHARED_LIBRARY_WITHOUT_STL})

        else()
            set(CMAKE_CXX_CREATE_SHARED_LIBRARY ${CMAKE_CXX_CREATE_SHARED_LIBRARY_WITH_STL})

        endif()
        
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

    endif()

    # add the install rule
    install(TARGETS ${target}
            RUNTIME DESTINATION bin COMPONENT bin
            LIBRARY DESTINATION lib${LIB_SUFFIX} COMPONENT bin
            ARCHIVE DESTINATION lib${LIB_SUFFIX} COMPONENT devel
            FRAMEWORK DESTINATION ${CMAKE_INSTALL_FRAMEWORK_PREFIX} COMPONENT bin)

endmacro()

# Add example
macro(jopAddExample target)

    # Parse the arguments
    cmake_parse_arguments(THIS "" "" "SOURCES" ${ARGN})

    # Set a source group for the sources
    source_group("src" FILES ${THIS_SOURCES})

    # Create the executable target
    if (JOP_OS_WINDOWS)
    
        add_executable(${target} WIN32 ${THIS_SOURCES})
        target_link_libraries(${target} jopnal-main)
        
    else ()
        add_executable(${target} ${THIS_SOURCES})
        
    endif()

    # Set the target's folder
    set_target_properties(${target} PROPERTIES FOLDER "examples")

    # Link the target to Jopnal
    target_link_libraries(${target} jopnal)
    
    # Add Jopnal as a dependency
    add_dependencies(${target} jopnal)

    # Add the install rule
    install(TARGETS ${target}
            RUNTIME DESTINATION ${INSTALL_MISC_DIR}/examples/${target} COMPONENT examples
            BUNDLE DESTINATION ${INSTALL_MISC_DIR}/examples/${target} COMPONENT examples)

    # Install the example's sources
    install(FILES ${THIS_SOURCES}
            DESTINATION ${INSTALL_MISC_DIR}/examples/${target}
            COMPONENT examples)

    # Copy the example's resources
    set(EXAMPLE_RESOURCES "${CMAKE_SOURCE_DIR}/examples/${target}/Resources")
    
    add_custom_command(TARGET ${target} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E make_directory
                       ${EXAMPLE_RESOURCES} $<TARGET_FILE_DIR:${target}>/../Resources)
    
    if (EXISTS ${EXAMPLE_RESOURCES})
    
        add_custom_command(TARGET ${target} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy_directory
                           ${EXAMPLE_RESOURCES} $<TARGET_FILE_DIR:${target}>/Resources)
        
    endif()
    
    # Copy dll's on Windows
    if (JOP_OS_WINDOWS)
    
        if (BUILD_SHARED_LIBS)
        
            add_custom_command(TARGET ${target} POST_BUILD
                               DEPENDS ALL
                               COMMAND ${CMAKE_COMMAND} -E copy_directory
                               ${PROJECT_BINARY_DIR}/lib/${CMAKE_CFG_INTDIR} $<TARGET_FILE_DIR:${target}>)
        
        endif()
        
        add_custom_command(TARGET ${target} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy_directory
                           ${PROJECT_BINARY_DIR}/extlibs/dll $<TARGET_FILE_DIR:${target}>)
    
    endif()

endmacro()


# Macro to find packages on the host OS
# This is the same as in the toolchain file, here for Nsight Tegra VS
if(CMAKE_VS_PLATFORM_NAME STREQUAL "Tegra-Android")

    macro(find_host_package)
        set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
        set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
        set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
        if(CMAKE_HOST_WIN32)
            set(WIN32 1)
            set(UNIX)
        elseif(CMAKE_HOST_APPLE)
            set(APPLE 1)
            set(UNIX)
        endif()
        find_package(${ARGN})
        set(WIN32)
        set(APPLE)
        set(UNIX 1)
        set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
        set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
        set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    endmacro()

endif()