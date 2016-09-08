# Jopnal wrecking ball example CMakeLists
#
# Jopnal license applies

set(__SRCDIR ${PROJECT_SOURCE_DIR}/examples/wrecking_ball/src)

set(SRC ${__SRCDIR}/main.cpp)

jopAddExample(wrecking_ball
              SOURCES ${SRC})