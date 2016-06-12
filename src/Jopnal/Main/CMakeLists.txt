# This file is adapted from SFML (http://www.sfml-dev.org/)

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

# jopnal-activity
if (JOP_OS_ANDROID)
    sfml_add_library(jopnal-activity SOURCES ${PROJECT_SOURCE_DIR}/src/Jopnal/Main/AndroidActivity.cpp)
endif()