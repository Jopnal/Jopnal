# OS detection
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

    set(JOP_OS_WINDOWS 1)
    set(OPENGL_ES 0)

    # Detect the architecture
    include(CheckTypeSize)
    check_type_size(void* SIZEOF_VOID_PTR)

    if("${SIZEOF_VOID_PTR}" STREQUAL "8")
        set(ARCH_64BITS 1)
    else()
        set(ARCH_32BITS 1)
    endif()

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

    set(JOP_OS_UNIX 1)

    if (ANDROID)
        set(JOP_OS_ANDROID 1)
        set(OPENGL_ES 1)
    else ()
        set(JOP_OS_LINUX 1)
        set(OPENGL_ES 0)
    endif()

elseif (${CMAKE_SYSTEM_NAME} MATCHES "Android")

    set(JOP_OS_ANDROID 1)
    set(OPENGL_ES 1)

else ()

    message(FATAL_ERROR "Unsupported operating system")
    return()

endif()

# Detect the compiler & version
if (CMAKE_CXX_COMPILER MATCHES ".*clang[+][+]" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

   set(JOP_COMPILER_CLANG 1)

   execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "--version" OUTPUT_VARIABLE CLANG_VERSION_OUTPUT)
   string(REGEX REPLACE ".*clang version ([0-9]+\\.[0-9]+).*" "\\1" JOP_CLANG_VERSION "${CLANG_VERSION_OUTPUT}")

elseif (CMAKE_COMPILER_IS_GNUCXX)

    set(JOP_COMPILER_GCC 1)

    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-dumpversion" OUTPUT_VARIABLE GCC_VERSION_OUTPUT)
    string(REGEX REPLACE "([0-9]+\\.[0-9]+).*" "\\1" JOP_GCC_VERSION "${GCC_VERSION_OUTPUT}")

    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "--version" OUTPUT_VARIABLE GCC_COMPILER_VERSION)
    string(REGEX MATCHALL ".*(tdm[64]*-[1-9]).*" JOP_COMPILER_GCC_TDM "${GCC_COMPILER_VERSION}")

    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-dumpmachine" OUTPUT_VARIABLE GCC_MACHINE)
    string(STRIP "${GCC_MACHINE}" GCC_MACHINE)

    if(${GCC_MACHINE} MATCHES ".*w64.*")
        set(JOP_COMPILER_GCC_W64 1)
    endif()

elseif(MSVC)

    set(JOP_COMPILER_MSVC 1)

    # VC++12 is the minimum supported version

    if (MSVC_VERSION EQUAL 1800)
        set(JOP_MSVC_VERSION 12)

    elseif (MSVC_VERSION EQUAL 1900)
        set(JOP_MSVC_VERSION 14)

    else ()
        message(FATAL_ERROR "Unsupported VC++ version, minimum is 12")

    endif()

else()

    message(FATAL_ERROR "Unsupported compiler")
    return()

endif()

# Define the install directory for miscellaneous files
if (JOP_OS_WINDOWS)
    set(INSTALL_MISC_DIR .)

elseif (JOP_OS_LINUX)
    set(INSTALL_MISC_DIR share/JOPNAL)

elseif (JOP_OS_ANDROID)
    set(INSTALL_MISC_DIR ${ANDROID_NDK}/sources/jopnal)

endif()