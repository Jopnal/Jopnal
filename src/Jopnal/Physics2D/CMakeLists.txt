# Jopnal Physics CMakeLists

set(__INCDIR_PHYSICS2D ${PROJECT_SOURCE_DIR}/include/Jopnal/Physics2D)
set(__SRCDIR_PHYSICS2D ${PROJECT_SOURCE_DIR}/src/Jopnal/Physics2D)

# CMakeLists
set(__LISTS_PHYSICS2D ${__SRCDIR_PHYSICS2D}/CMakeLists.txt)
source_group("Physics2D" FILES ${__LISTS_PHYSICS2D})
list(APPEND SRC ${__LISTS_PHYSICS2D})

# Include
set(__INC_PHYSICS2D
    ${__INCDIR_PHYSICS2D}/Collider2D.hpp
    ${__INCDIR_PHYSICS2D}/RayInfo2D.hpp
    ${__INCDIR_PHYSICS2D}/RigidBody2D.hpp
    ${__INCDIR_PHYSICS2D}/World2D.hpp
)
source_group("Physics2D\\Headers" FILES ${__INC_PHYSICS2D})
list(APPEND SRC ${__INC_PHYSICS2D})

# Include - Shape
set(__INC_PHYSICS2D_SHAPE
    ${__INCDIR_PHYSICS2D}/Shape/CircleShape2D.hpp
    ${__INCDIR_PHYSICS2D}/Shape/CollisionShape2D.hpp
    ${__INCDIR_PHYSICS2D}/Shape/ConvexHullShape2D.hpp
    ${__INCDIR_PHYSICS2D}/Shape/RectangleShape2D.hpp
    ${__INCDIR_PHYSICS2D}/Shape/TerrainShape2D.hpp
)
source_group("Physics2D\\Headers\\Shape" FILES ${__INC_PHYSICS2D_SHAPE})
list(APPEND SRC ${__INC_PHYSICS2D_SHAPE})

# Source
set(__SRC_PHYSICS2D
    ${__SRCDIR_PHYSICS2D}/Collider2D.cpp
    ${__SRCDIR_PHYSICS2D}/RayInfo2D.cpp
    ${__SRCDIR_PHYSICS2D}/RigidBody2D.cpp
    ${__SRCDIR_PHYSICS2D}/World2D.cpp
)
source_group("Physics2D\\Source" FILES ${__SRC_PHYSICS2D})
list(APPEND SRC ${__SRC_PHYSICS2D})

# Source - Shape
set(__SRC_PHYSICS2D_SHAPE
    ${__SRCDIR_PHYSICS2D}/Shape/CircleShape2D.cpp
    ${__SRCDIR_PHYSICS2D}/Shape/CollisionShape2D.cpp
    ${__SRCDIR_PHYSICS2D}/Shape/ConvexHullShape2D.cpp
    ${__SRCDIR_PHYSICS2D}/Shape/RectangleShape2D.cpp
    ${__SRCDIR_PHYSICS2D}/Shape/TerrainShape2D.cpp
)
source_group("Physics2D\\Source\\Shape" FILES ${__SRC_PHYSICS2D_SHAPE})
list(APPEND SRC ${__SRC_PHYSICS2D_SHAPE})