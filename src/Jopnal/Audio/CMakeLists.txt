# Jopnal Audio CMakeLists

set(__INCDIR_AUDIO ${PROJECT_SOURCE_DIR}/include/Jopnal/Audio)
set(__SRCDIR_AUDIO ${PROJECT_SOURCE_DIR}/src/Jopnal/Audio)

# CMakeLists
set(__LISTS_AUDIO ${__SRCDIR_AUDIO}/CMakeLists.txt)
source_group("Audio" FILES ${__LISTS_AUDIO})
list(APPEND SRC ${__LISTS_AUDIO})

# Include
set(__INC_AUDIO
    ${__INCDIR_AUDIO}/AudioDevice.hpp
    ${__INCDIR_AUDIO}/Listener.hpp
    ${__INCDIR_AUDIO}/SoundBuffer.hpp
    ${__INCDIR_AUDIO}/SoundEffect.hpp
    ${__INCDIR_AUDIO}/SoundSource.hpp
    ${__INCDIR_AUDIO}/SoundStream.hpp
)
source_group("Audio\\Headers" FILES ${__INC_AUDIO})
list(APPEND SRC ${__INC_AUDIO})

# Source
set(__SRC_AUDIO
    ${__SRCDIR_AUDIO}/AlTry.cpp
    ${__SRCDIR_AUDIO}/AlTry.hpp
    ${__SRCDIR_AUDIO}/AudioDevice.cpp
    ${__SRCDIR_AUDIO}/AudioReader.hpp
    ${__SRCDIR_AUDIO}/AudioReader.cpp
    ${__SRCDIR_AUDIO}/Listener.cpp
    ${__SRCDIR_AUDIO}/SoundBuffer.cpp
    ${__SRCDIR_AUDIO}/SoundEffect.cpp
    ${__SRCDIR_AUDIO}/SoundSource.cpp
    ${__SRCDIR_AUDIO}/SoundStream.cpp
)
source_group("Audio\\Source" FILES ${__SRC_AUDIO})
list(APPEND SRC ${__SRC_AUDIO})