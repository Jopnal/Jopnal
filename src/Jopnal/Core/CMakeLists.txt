# Jopnal Core CMakeLists

set(__INCDIR_CORE ${PROJECT_SOURCE_DIR}/include/Jopnal/Core)
set(__INLDIR_CORE ${__INCDIR_CORE}/Inl)
set(__SRCDIR_CORE ${PROJECT_SOURCE_DIR}/src/Jopnal/Core)

# CMakeLists
set(__LISTS_CORE ${__SRCDIR_CORE}/CMakeLists.txt)
source_group("Core" FILES ${__LISTS_CORE})
list(APPEND SRC ${__LISTS_CORE})

# Include
set(__INC_CORE
    ${__INCDIR_CORE}/Component.hpp
    ${__INCDIR_CORE}/DebugHandler.hpp
    ${__INCDIR_CORE}/Engine.hpp
    ${__INCDIR_CORE}/FileLoader.hpp
    ${__INCDIR_CORE}/Object.hpp
    ${__INCDIR_CORE}/Resource.hpp
    ${__INCDIR_CORE}/ResourceManager.hpp
    ${__INCDIR_CORE}/Scene.hpp
    ${__INCDIR_CORE}/SerializeInfo.hpp
    ${__INCDIR_CORE}/Serializer.hpp
    ${__INCDIR_CORE}/SettingCallback.hpp
    ${__INCDIR_CORE}/SettingManager.hpp
    ${__INCDIR_CORE}/Subsystem.hpp
)
source_group("Core\\Headers" FILES ${__INC_CORE})
list(APPEND SRC ${__INC_CORE})

# Inline
set(__INL_CORE
    ${__INCDIR_CORE}/Inl/DebugHandler.inl
    ${__INCDIR_CORE}/Inl/Engine.inl
    ${__INCDIR_CORE}/Inl/Object.inl
    ${__INCDIR_CORE}/Inl/ResourceManager.inl
    ${__INCDIR_CORE}/Inl/Scene.inl
    ${__INCDIR_CORE}/Inl/Serializer.inl
    ${__INCDIR_CORE}/Inl/SettingCallback.inl
    ${__INCDIR_CORE}/Inl/SettingManager.inl
)
source_group("Core\\Inl" FILES ${__INL_CORE})
list(APPEND SRC ${__INL_CORE})

# Source
set(__SRC_CORE
    ${__SRCDIR_CORE}/Component.cpp
    ${__SRCDIR_CORE}/DebugHandler.cpp
    ${__SRCDIR_CORE}/Engine.cpp
    ${__SRCDIR_CORE}/FileLoader.cpp
    ${__SRCDIR_CORE}/Object.cpp
    ${__SRCDIR_CORE}/Resource.cpp
    ${__SRCDIR_CORE}/ResourceManager.cpp
    ${__SRCDIR_CORE}/Scene.cpp
    ${__SRCDIR_CORE}/SerializeInfo.cpp
    ${__SRCDIR_CORE}/Serializer.cpp
    ${__SRCDIR_CORE}/SettingCallback.cpp
    ${__SRCDIR_CORE}/SettingManager.cpp
    ${__SRCDIR_CORE}/Subsystem.cpp
)
source_group("Core\\Source" FILES ${__SRC_CORE})
list(APPEND SRC ${__SRC_CORE})

# Android
set(__SRC_CORE_ANDROID
    ${__SRCDIR_CORE}/Android/ActivityState.cpp
    ${__SRCDIR_CORE}/Android/ActivityState.hpp
    ${__SRCDIR_CORE}/Android/GooglePlayService.cpp
    ${__SRCDIR_CORE}/Android/GooglePlayService.hpp
)
source_group("Core\\Source\\Android" FILES ${__SRC_CORE_ANDROID})
list(APPEND SRC ${__SRC_CORE_ANDROID})

# Windows
set(__SRC_CORE_WINDOWS
    ${__SRCDIR_CORE}/Win32/Win32.hpp
)
source_group("Core\\Source\\Win32" FILES ${__SRC_CORE_WINDOWS})
list(APPEND SRC ${__SRC_CORE_WINDOWS})