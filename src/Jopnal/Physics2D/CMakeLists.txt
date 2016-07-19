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
	${__INCDIR_PHYSICS2D}/Joint2D.hpp
    ${__INCDIR_PHYSICS2D}/RayInfo2D.hpp
    ${__INCDIR_PHYSICS2D}/RigidBody2D.hpp
    ${__INCDIR_PHYSICS2D}/World2D.hpp
)
source_group("Physics2D\\Headers" FILES ${__INC_PHYSICS2D})
list(APPEND SRC ${__INC_PHYSICS2D})

# Include - Joint
set(__INC_PHYSICS2D_JOINT
	${__INCDIR_PHYSICS2D}/Joint/PistonJoint2D.hpp
	${__INCDIR_PHYSICS2D}/Joint/RopeJoint2D.hpp
	${__INCDIR_PHYSICS2D}/Joint/RotationJoint2D.hpp
)
source_group("Physics2D\\Headers\\Joint" FILES ${__INC_PHYSICS2D_JOINT})
list(APPEND SRC ${__INC_PHYSICS2D_JOINT})

# Include - Shape
set(__INC_PHYSICS2D_SHAPE
	${__INCDIR_PHYSICS2D}/Shape/CapsuleShape2D.hpp
    ${__INCDIR_PHYSICS2D}/Shape/CircleShape2D.hpp
    ${__INCDIR_PHYSICS2D}/Shape/CollisionShape2D.hpp
	${__INCDIR_PHYSICS2D}/Shape/CompoundShape2D.hpp
	${__INCDIR_PHYSICS2D}/Shape/ConeShape2D.hpp
    ${__INCDIR_PHYSICS2D}/Shape/ConvexHullShape2D.hpp
    ${__INCDIR_PHYSICS2D}/Shape/RectangleShape2D.hpp
    ${__INCDIR_PHYSICS2D}/Shape/TerrainShape2D.hpp
)
source_group("Physics2D\\Headers\\Shape" FILES ${__INC_PHYSICS2D_SHAPE})
list(APPEND SRC ${__INC_PHYSICS2D_SHAPE})

# Inline
set(__INL_CORE
    ${__INCDIR_PHYSICS2D}/Inl/RigidBody2D.inl
)
source_group("Physics2D\\Inl" FILES ${__INL_CORE})
list(APPEND SRC ${__INL_CORE})

# Source
set(__SRC_PHYSICS2D
    ${__SRCDIR_PHYSICS2D}/Collider2D.cpp
	${__SRCDIR_PHYSICS2D}/Joint2D.cpp
    ${__SRCDIR_PHYSICS2D}/RayInfo2D.cpp
    ${__SRCDIR_PHYSICS2D}/RigidBody2D.cpp
    ${__SRCDIR_PHYSICS2D}/World2D.cpp
)
source_group("Physics2D\\Source" FILES ${__SRC_PHYSICS2D})
list(APPEND SRC ${__SRC_PHYSICS2D})

# Source - Joint
set(__SRC_PHYSICS2D_JOINT
	${__SRCDIR_PHYSICS2D}/Joint/PistonJoint2D.cpp
	${__SRCDIR_PHYSICS2D}/Joint/RopeJoint2D.cpp
	${__SRCDIR_PHYSICS2D}/Joint/RotationJoint2D.cpp
)
source_group("Physics2D\\Source\\Joint" FILES ${__SRC_PHYSICS2D_JOINT})
list(APPEND SRC ${__SRC_PHYSICS2D_JOINT})

# Source - Shape
set(__SRC_PHYSICS2D_SHAPE
	${__SRCDIR_PHYSICS2D}/Shape/CapsuleShape2D.cpp
    ${__SRCDIR_PHYSICS2D}/Shape/CircleShape2D.cpp
    ${__SRCDIR_PHYSICS2D}/Shape/CollisionShape2D.cpp
	${__SRCDIR_PHYSICS2D}/Shape/CompoundShape2D.cpp
	${__SRCDIR_PHYSICS2D}/Shape/ConeShape2D.cpp
    ${__SRCDIR_PHYSICS2D}/Shape/ConvexHullShape2D.cpp
    ${__SRCDIR_PHYSICS2D}/Shape/RectangleShape2D.cpp
    ${__SRCDIR_PHYSICS2D}/Shape/TerrainShape2D.cpp
)
source_group("Physics2D\\Source\\Shape" FILES ${__SRC_PHYSICS2D_SHAPE})
list(APPEND SRC ${__SRC_PHYSICS2D_SHAPE})