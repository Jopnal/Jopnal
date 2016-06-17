# Jopnal Main CMakeLists

set(INCROOT ${PROJECT_SOURCE_DIR}/include/Jopnal/Main)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Jopnal/Main)

# Sources
if (JOP_OS_WINDOWS OR JOP_OS_ANDROID)
    set(SRC ${SRC} ${SRCROOT}/Main.cpp)

else()
    return()

endif()

source_group("" FILES ${SRC})

# jopnal-main target
add_library(jopnal-main STATIC ${SRC})

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