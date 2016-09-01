# Jopnal Precompiled CMakeList

set(__SRCDIR_PRECOMPILED ${PROJECT_SOURCE_DIR}/src/Jopnal/Precompiled)

# Source
set(__SRC_PRECOMPILED
    ${__SRCDIR_PRECOMPILED}/Precompiled.hpp
    ${__SRCDIR_PRECOMPILED}/PrecompiledEmpty.hpp
    ${__SRCDIR_PRECOMPILED}/CMakeLists.txt
)
source_group("Precompiled" FILES ${__SRC_PRECOMPILED})
list(APPEND SRC ${__SRC_PRECOMPILED})