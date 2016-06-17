# Jopnal Resources CMakeLists

# Add resource files & build step
if (JOP_OS_WINDOWS)

    set(SRCROOT_RES ${PROJECT_SOURCE_DIR}/tools/Jopresource/res)

    # Fonts
    set(SRC_FONTS
        ${SRCROOT_RES}/Fonts/kongtext.ttf)
    source_group("Resources\\Fonts" FILES ${SRC_FONTS})
    
    # Shaders
    set(SRC_SHADERS
        ${SRCROOT_RES}/Shaders/default.frag
        ${SRCROOT_RES}/Shaders/default.vert
        ${SRCROOT_RES}/Shaders/defaultUber.frag
        ${SRCROOT_RES}/Shaders/defaultUber.vert
        ${SRCROOT_RES}/Shaders/depthRecord.frag
        ${SRCROOT_RES}/Shaders/depthRecord.vert
        ${SRCROOT_RES}/Shaders/depthRecordPoint.frag
        ${SRCROOT_RES}/Shaders/depthRecordPoint.geom
        ${SRCROOT_RES}/Shaders/depthRecordPoint.vert
        ${SRCROOT_RES}/Shaders/gaussianBlur.frag
        ${SRCROOT_RES}/Shaders/physicsDebug.frag
        ${SRCROOT_RES}/Shaders/physicsDebug.vert
        ${SRCROOT_RES}/Shaders/postProcess.frag
        ${SRCROOT_RES}/Shaders/postProcess.vert)
    source_group("Resources\\Shaders" FILES ${SRC_SHADERS})
    
    # Sounds
    set(SRC_SOUNDS
        ${SRCROOT_RES}/Sounds/beep.wav)
    source_group("Resources\\Sounds" FILES ${SRC_SOUNDS})
    
    # Textures
    set(SRC_TEXTURES
        ${SRCROOT_RES}/Textures/checker.png
        ${SRCROOT_RES}/Textures/error.png)
    source_group("Resources\\Textures" FILES ${SRC_TEXTURES})
    
    set(SRC_RES ${SRC_FONTS} ${SRC_SHADERS} ${SRC_SOUNDS} ${SRC_TEXTURES})
    
    add_custom_target(jopnal-resources
                      SOURCES ${SRC_RES})
                      
    add_custom_command(TARGET jopnal-resources
                       PRE_BUILD
                       COMMAND jopres desc.json
                       WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tools/Jopresource
                       DEPENDS ${SRC_RES})
                       
else ()

    message(WARNING "Resource compiler is provided only for Windows, using pre-assembled Jopnal resource files")

endif()