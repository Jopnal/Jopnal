# Jopnal Utility CMakeLists

set(__INCDIR_UTILITY ${PROJECT_SOURCE_DIR}/include/Jopnal/Utility)
set(__INLDIR_UTILITY ${__INCDIR_UTILITY}/Inl)
set(__SRCDIR_UTILITY ${PROJECT_SOURCE_DIR}/src/Jopnal/Utility)

# CMakeLists
set(__LISTS_UTILITY ${__SRCDIR_UTILITY}/CMakeLists.txt)
source_group("Utility" FILES ${__LISTS_UTILITY})
list(APPEND SRC ${__LISTS_UTILITY})

# Include
set(__INC_UTILITY
    ${__INCDIR_UTILITY}/Assert.hpp
    ${__INCDIR_UTILITY}/Clock.hpp
    ${__INCDIR_UTILITY}/CommandHandler.hpp
    ${__INCDIR_UTILITY}/DateTime.hpp
    ${__INCDIR_UTILITY}/DirectoryWatcher.hpp
    ${__INCDIR_UTILITY}/Json.hpp
    ${__INCDIR_UTILITY}/Message.hpp
    ${__INCDIR_UTILITY}/Randomizer.hpp
    ${__INCDIR_UTILITY}/SafeReferenceable.hpp
    ${__INCDIR_UTILITY}/Thread.hpp
)
source_group("Utility\\Headers" FILES ${__INC_UTILITY})
list(APPEND SRC ${__INC_UTILITY})

# Inline
set(__INL_UTILITY
    ${__INLDIR_UTILITY}/CommandHandler.inl
    ${__INLDIR_UTILITY}/CommandParser.inl
    ${__INLDIR_UTILITY}/Message.inl
    ${__INLDIR_UTILITY}/Randomizer.inl
    ${__INLDIR_UTILITY}/SafeReferenceable.inl
    ${__INLDIR_UTILITY}/Thread.inl
)
source_group("Utility\\Inl" FILES ${__INL_UTILITY})
list(APPEND SRC ${__INL_UTILITY})

# Source
set(__SRC_UTILITY
    ${__SRCDIR_UTILITY}/Assert.cpp
    ${__SRCDIR_UTILITY}/Clock.cpp
    ${__SRCDIR_UTILITY}/CommandHandler.cpp
    ${__SRCDIR_UTILITY}/DateTime.cpp
    ${__SRCDIR_UTILITY}/DirectoryWatcher.cpp
    ${__SRCDIR_UTILITY}/Json.cpp
    ${__SRCDIR_UTILITY}/Message.cpp
    ${__SRCDIR_UTILITY}/Randomizer.cpp
    ${__SRCDIR_UTILITY}/Thread.cpp
)
source_group("Utility\\Source" FILES ${__SRC_UTILITY})
list(APPEND SRC ${__SRC_UTILITY})

# Source - Windows
set(__SRC_UTILITY_WIN32
    ${__SRCDIR_UTILITY}/Win32/DirectoryWatcherImpl.cpp
    ${__SRCDIR_UTILITY}/Win32/DirectoryWatcherImpl.hpp
    ${__SRCDIR_UTILITY}/Win32/ThreadImpl.cpp
    ${__SRCDIR_UTILITY}/Win32/ThreadImpl.hpp
)
source_group("Utility\\Source\\Win32" FILES ${__SRC_UTILITY_WIN32})
list(APPEND SRC ${__SRC_UTILITY_WIN32})

# Source - Unix
set(__SRC_UTILITY_UNIX
    ${__SRCDIR_UTILITY}/Unix/ThreadImpl.cpp
    ${__SRCDIR_UTILITY}/Unix/ThreadImpl.hpp
)
source_group("Utility\\Source\\Unix" FILES ${__SRC_UTILITY_UNIX})
list(APPEND SRC ${__SRC_UTILITY_UNIX})