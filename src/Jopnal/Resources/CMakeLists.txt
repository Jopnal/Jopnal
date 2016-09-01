# Jopnal Resources CMakeLists

# Add resource files & build step
if (CMAKE_HOST_WIN32)

    set(__SRCDIR_RESOURCES ${PROJECT_SOURCE_DIR}/tools/Jopresource/res)

    # Fonts
    set(__SRC_FONTS
        ${__SRCDIR_RESOURCES}/Fonts/kongtext.ttf
    )
    source_group("Fonts" FILES ${__SRC_FONTS})
    list(APPEND RES ${__SRC_FONTS})
    
    # Shader plugins
    set(__SRC_SHADERPLUGINS
        ${__SRCDIR_RESOURCES}/ShaderPlugins/compatibility.jops
        ${__SRCDIR_RESOURCES}/ShaderPlugins/lighting.jops
    )
    source_group("ShadersPlugins" FILES ${__SRC_SHADERPLUGINS})
    list(APPEND RES ${__SRC_SHADERPLUGINS})
    
    # Shaders
    set(__SRC_SHADERS
        ${__SRCDIR_RESOURCES}/Shaders/brightFilter.frag
        ${__SRCDIR_RESOURCES}/Shaders/default.frag
        ${__SRCDIR_RESOURCES}/Shaders/default.vert
        ${__SRCDIR_RESOURCES}/Shaders/defaultUber.frag
        ${__SRCDIR_RESOURCES}/Shaders/defaultUber.vert
        ${__SRCDIR_RESOURCES}/Shaders/depthRecord.frag
        ${__SRCDIR_RESOURCES}/Shaders/depthRecord.vert
        ${__SRCDIR_RESOURCES}/Shaders/depthRecordPoint.frag
        ${__SRCDIR_RESOURCES}/Shaders/depthRecordPoint.geom
        ${__SRCDIR_RESOURCES}/Shaders/depthRecordPoint.vert
        ${__SRCDIR_RESOURCES}/Shaders/gaussianBlur.frag
        ${__SRCDIR_RESOURCES}/Shaders/physicsDebug.frag
        ${__SRCDIR_RESOURCES}/Shaders/physicsDebug.vert
        ${__SRCDIR_RESOURCES}/Shaders/postProcess.frag
        ${__SRCDIR_RESOURCES}/Shaders/postProcess.vert
    )
    source_group("Shaders" FILES ${__SRC_SHADERS})
    list(APPEND RES ${__SRC_SHADERS})
    
    # Sounds
    set(__SRC_SOUNDS
        ${__SRCDIR_RESOURCES}/Sounds/beep.wav
    )
    source_group("Sounds" FILES ${__SRC_SOUNDS})
    list(APPEND RES ${__SRC_SOUNDS})
    
    # Textures
    set(__SRC_TEXTURES
        ${__SRCDIR_RESOURCES}/Textures/checker.png
        ${__SRCDIR_RESOURCES}/Textures/error.png
    )
    source_group("Textures" FILES ${__SRC_TEXTURES})
    list(APPEND RES ${__SRC_TEXTURES})

    # Descriptor
    list(APPEND RES ${PROJECT_SOURCE_DIR}/tools/Jopresource/desc.json)
    
    add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/tools/Jopresource/Resources.hpp
                              ${PROJECT_SOURCE_DIR}/tools/Jopresource/Resources.cpp
                       COMMAND ${PROJECT_SOURCE_DIR}/tools/Jopresource/jopres desc.json
                       DEPENDS ${RES} ${PROJECT_SOURCE_DIR}/tools/Jopresource/desc.json
                       WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tools/Jopresource)
    
    add_custom_target(jopnal-resources
                      SOURCES ${RES}
                      DEPENDS ${PROJECT_SOURCE_DIR}/tools/Jopresource/Resources.hpp
                              ${PROJECT_SOURCE_DIR}/tools/Jopresource/Resources.cpp)
    
    set_target_properties(jopnal-resources PROPERTIES FOLDER "jopnal")
    
else ()

    message(WARNING "Resource compiler is provided only for Windows, using pre-assembled Jopnal resource files")

endif()

# Add header & source to jopnal-resources
set(__SRC_RESOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Resources.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Resources.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources/CMakeLists.txt
)
source_group("Resources" FILES ${__SRC_RESOURCES})
list(APPEND SRC ${__SRC_RESOURCES})