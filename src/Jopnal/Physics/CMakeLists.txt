# Jopnal Physics CMakeLists

set(__INCDIR_PHYSICS ${PROJECT_SOURCE_DIR}/include/Jopnal/Physics)
set(__SRCDIR_PHYSICS ${PROJECT_SOURCE_DIR}/src/Jopnal/Physics)

# CMakeLists
set(__LISTS_PHYSICS ${__SRCDIR_PHYSICS}/CMakeLists.txt)
source_group("Physics" FILES ${__LISTS_PHYSICS})
list(APPEND SRC ${__LISTS_PHYSICS})

# Include
set(__INC_PHYSICS
    ${__INCDIR_PHYSICS}/Collider.hpp
    ${__INCDIR_PHYSICS}/ContactInfo.hpp
    ${__INCDIR_PHYSICS}/ContactListener.hpp
    ${__INCDIR_PHYSICS}/Joint.hpp
	${__INCDIR_PHYSICS}/PhantomBody.hpp
    ${__INCDIR_PHYSICS}/RayInfo.hpp
    ${__INCDIR_PHYSICS}/RigidBody.hpp
    ${__INCDIR_PHYSICS}/World.hpp
)
source_group("Physics\\Headers" FILES ${__INC_PHYSICS})
list(APPEND SRC ${__INC_PHYSICS})

# Include - Joint
set(__INC_PHYSICS_JOINT
	${__INCDIR_PHYSICS}/Joint/PistonJoint.hpp
	${__INCDIR_PHYSICS}/Joint/RotationJoint.hpp
	${__INCDIR_PHYSICS}/Joint/WeldJoint.hpp
	${__INCDIR_PHYSICS}/Joint/WheelJoint.hpp
)
source_group("Physics\\Headers\\Joint" FILES ${__INC_PHYSICS_JOINT})
list(APPEND SRC ${__INC_PHYSICS_JOINT})

# Include - Shape
set(__INC_PHYSICS_SHAPE
    ${__INCDIR_PHYSICS}/Shape/BoxShape.hpp
    ${__INCDIR_PHYSICS}/Shape/CapsuleShape.hpp
    ${__INCDIR_PHYSICS}/Shape/CollisionShape.hpp
    ${__INCDIR_PHYSICS}/Shape/CompoundShape.hpp
    ${__INCDIR_PHYSICS}/Shape/ConeShape.hpp
    ${__INCDIR_PHYSICS}/Shape/ConvexHullShape.hpp
    ${__INCDIR_PHYSICS}/Shape/CylinderShape.hpp
    ${__INCDIR_PHYSICS}/Shape/FrustumShape.hpp
    ${__INCDIR_PHYSICS}/Shape/InfinitePlaneShape.hpp
    ${__INCDIR_PHYSICS}/Shape/RectangleShape.hpp
    ${__INCDIR_PHYSICS}/Shape/SphereShape.hpp
    ${__INCDIR_PHYSICS}/Shape/TerrainShape.hpp
)
source_group("Physics\\Headers\\Shape" FILES ${__INC_PHYSICS_SHAPE})
list(APPEND SRC ${__INC_PHYSICS_SHAPE})

# Inline
set(__INL_CORE
    ${__INCDIR_PHYSICS}/Inl/RigidBody.inl
)
source_group("Physics\\Inl" FILES ${__INL_CORE})
list(APPEND SRC ${__INL_CORE})

# Source
set(__SRC_PHYSICS
    ${__SRCDIR_PHYSICS}/Collider.cpp
    ${__SRCDIR_PHYSICS}/ContactInfo.cpp
    ${__SRCDIR_PHYSICS}/ContactListener.cpp
    ${__SRCDIR_PHYSICS}/Joint.cpp
	${__SRCDIR_PHYSICS}/PhantomBody.cpp
    ${__SRCDIR_PHYSICS}/RayInfo.cpp
    ${__SRCDIR_PHYSICS}/RigidBody.cpp
    ${__SRCDIR_PHYSICS}/World.cpp
)
source_group("Physics\\Source" FILES ${__SRC_PHYSICS})
list(APPEND SRC ${__SRC_PHYSICS})

# Source - Joint
set(__SRC_PHYSICS_JOINT
	${__SRCDIR_PHYSICS}/Joint/PistonJoint.cpp
	${__SRCDIR_PHYSICS}/Joint/RotationJoint.cpp
	${__SRCDIR_PHYSICS}/Joint/WeldJoint.cpp
	${__SRCDIR_PHYSICS}/Joint/WheelJoint.cpp
)
source_group("Physics\\Source\\Joint" FILES ${__SRC_PHYSICS_JOINT})
list(APPEND SRC ${__SRC_PHYSICS_JOINT})

# Source - Shape
set(__SRC_PHYSICS_SHAPE
    ${__SRCDIR_PHYSICS}/Shape/BoxShape.cpp
    ${__SRCDIR_PHYSICS}/Shape/CapsuleShape.cpp
    ${__SRCDIR_PHYSICS}/Shape/CollisionShape.cpp
    ${__SRCDIR_PHYSICS}/Shape/CompoundShape.cpp
    ${__SRCDIR_PHYSICS}/Shape/ConeShape.cpp
    ${__SRCDIR_PHYSICS}/Shape/ConvexHullShape.cpp
    ${__SRCDIR_PHYSICS}/Shape/CylinderShape.cpp
    ${__SRCDIR_PHYSICS}/Shape/FrustumShape.cpp
    ${__SRCDIR_PHYSICS}/Shape/InfinitePlaneShape.cpp
    ${__SRCDIR_PHYSICS}/Shape/RectangleShape.cpp
    ${__SRCDIR_PHYSICS}/Shape/SphereShape.cpp
    ${__SRCDIR_PHYSICS}/Shape/TerrainShape.cpp
)
source_group("Physics\\Source\\Shape" FILES ${__SRC_PHYSICS_SHAPE})
list(APPEND SRC ${__SRC_PHYSICS_SHAPE})

# Source - Detail
set(__SRC_PHYSICS_DETAIL
    ${__SRCDIR_PHYSICS}/Detail/WorldImpl.cpp
    ${__SRCDIR_PHYSICS}/Detail/WorldImpl.hpp
)
source_group("Physics\\Source\\Detail" FILES ${__SRC_PHYSICS_DETAIL})
list(APPEND SRC ${__SRC_PHYSICS_DETAIL})