# Jopnal spinning box example CMakeLists
#
# Jopnal license applies

set(__SRCDIR ${PROJECT_SOURCE_DIR}/examples/spinning_box/src)

set(SRC ${__SRCDIR}/main.cpp)

jopAddExample(spinning_box
              SOURCES ${SRC})