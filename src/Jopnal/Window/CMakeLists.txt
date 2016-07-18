# Jopnal Window CMakeLists

set(__INCDIR_WINDOW ${PROJECT_SOURCE_DIR}/include/Jopnal/Window)
set(__INLDIR_WINDOW ${__INCDIR_WINDOW}/Inl)
set(__SRCDIR_WINDOW ${PROJECT_SOURCE_DIR}/src/Jopnal/Window)

# CMakeLists
set(__LISTS_WINDOW ${__SRCDIR_WINDOW}/CMakeLists.txt)
source_group("Window" FILES ${__LISTS_WINDOW})
list(APPEND SRC ${__LISTS_WINDOW})

# Include
set(__INC_WINDOW
    ${__INCDIR_WINDOW}/Controller.hpp
    ${__INCDIR_WINDOW}/Keyboard.hpp
    ${__INCDIR_WINDOW}/Mouse.hpp
    ${__INCDIR_WINDOW}/Sensor.hpp
    ${__INCDIR_WINDOW}/VideoInfo.hpp
    ${__INCDIR_WINDOW}/Window.hpp
    ${__INCDIR_WINDOW}/WindowEventHandler.hpp
    ${__INCDIR_WINDOW}/WindowHandle.hpp
)
source_group("Window\\Headers" FILES ${__INC_WINDOW})
list(APPEND SRC ${__INC_WINDOW})

# Inline
set(__INL_WINDOW
    ${__INLDIR_WINDOW}/Window.inl
)
source_group("Window\\Inl" FILES ${__INL_WINDOW})
list(APPEND SRC ${__INL_WINDOW})


# Source
set(__SRC_WINDOW
    ${__SRCDIR_WINDOW}/Keyboard.cpp
    ${__SRCDIR_WINDOW}/Sensor.cpp
    ${__SRCDIR_WINDOW}/SensorManager.cpp
    ${__SRCDIR_WINDOW}/SensorManager.hpp
    ${__SRCDIR_WINDOW}/VideoInfo.cpp
    ${__SRCDIR_WINDOW}/Window.cpp
    ${__SRCDIR_WINDOW}/WindowEventHandler.cpp
)
source_group("Window\\Source" FILES ${__SRC_WINDOW})
list(APPEND SRC ${__SRC_WINDOW})

# Source - Desktop
set(__SRC_WINDOW_DESKTOP
    ${__SRCDIR_WINDOW}/Desktop/SensorImpl.cpp
    ${__SRCDIR_WINDOW}/Desktop/SensorImpl.hpp
    ${__SRCDIR_WINDOW}/Desktop/VideoInfoImpl.cpp
    ${__SRCDIR_WINDOW}/Desktop/VideoInfoImpl.hpp
    ${__SRCDIR_WINDOW}/Desktop/WindowImpl.cpp
    ${__SRCDIR_WINDOW}/Desktop/WindowImpl.hpp
)
source_group("Window\\Source\\Desktop" FILES ${__SRC_WINDOW_DESKTOP})
list(APPEND SRC ${__SRC_WINDOW_DESKTOP})

# Source - Android
set(__SRC_WINDOW_ANDROID
    ${__SRCDIR_WINDOW}/Android/SensorImpl.cpp
    ${__SRCDIR_WINDOW}/Android/SensorImpl.hpp
    ${__SRCDIR_WINDOW}/Android/VideoInfoImpl.cpp
    ${__SRCDIR_WINDOW}/Android/VideoInfoImpl.hpp
    ${__SRCDIR_WINDOW}/Android/WindowImpl.cpp
    ${__SRCDIR_WINDOW}/Android/WindowImpl.hpp
)
source_group("Window\\Source\\Android" FILES ${__SRC_WINDOW_ANDROID})
list(APPEND SRC ${__SRC_WINDOW_ANDROID})