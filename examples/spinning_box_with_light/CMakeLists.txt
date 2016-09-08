# Jopnal spinning box with light example CMakeLists
#
# Jopnal license applies

set(__SRCDIR ${PROJECT_SOURCE_DIR}/examples/spinning_box_with_light/src)

set(SRC ${__SRCDIR}/main.cpp)

jopAddExample(spinning_box_with_light
              SOURCES ${SRC})