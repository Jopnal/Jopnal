# Jopnal Main CMakeLists

set(__INCDIR ${PROJECT_SOURCE_DIR}/include/Jopnal/Main)
set(__SRCDIR ${CMAKE_CURRENT_SOURCE_DIR})

# Sources
if (JOP_OS_WINDOWS OR JOP_OS_ANDROID)
    list(APPEND SRC_MAIN ${__SRCDIR}/Main.cpp ${__SRCDIR}/CMakeLists.txt)
else()
    return()
endif()

source_group("" FILES ${SRC_MAIN})

# jopnal-main target
add_library(jopnal-main STATIC ${SRC_MAIN})

# Set debug suffix
set_target_properties(jopnal-main PROPERTIES DEBUG_POSTFIX d)

# Set target's folder
set_target_properties(jopnal-main PROPERTIES FOLDER "jopnal")

# Setup install rule
install(TARGETS jopnal-main ARCHIVE DESTINATION lib${LIB_SUFFIX} COMPONENT devel)

# Add bootstrap activity
if (JOP_OS_ANDROID)

    set(WAS_SHARED_LIBS ${BUILD_SHARED_LIBS})

    set(BUILD_SHARED_LIBS TRUE)
    jopAddLibrary(jopnal-activity SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Activity.cpp)
    
    set(BUILD_SHARED_LIBS ${WAS_SHARED_LIBS})
    
endif()