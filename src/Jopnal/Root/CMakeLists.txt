# Jopnal Root CMakeLists

set(__INCDIR_ROOT ${PROJECT_SOURCE_DIR}/include/Jopnal)

# Include
set(__INC_ROOT
    ${__INCDIR_ROOT}/Audio.hpp
    ${__INCDIR_ROOT}/Config.hpp
    ${__INCDIR_ROOT}/Core.hpp
    ${__INCDIR_ROOT}/Export.hpp
    ${__INCDIR_ROOT}/Graphics.hpp
    ${__INCDIR_ROOT}/Header.hpp
    ${__INCDIR_ROOT}/HelperMacros.hpp
    ${__INCDIR_ROOT}/Jopnal.hpp
    ${__INCDIR_ROOT}/OS.hpp
    ${__INCDIR_ROOT}/Physics.hpp
    ${__INCDIR_ROOT}/Types.hpp
    ${__INCDIR_ROOT}/Utility.hpp
    ${__INCDIR_ROOT}/Window.hpp
    ${__INCDIR_ROOT}/STL.hpp

	${PROJECT_SOURCE_DIR}/src/Jopnal/Root/CMakeLists.txt
)
source_group("Root" FILES ${__INC_ROOT})
list(APPEND SRC ${__INC_ROOT})