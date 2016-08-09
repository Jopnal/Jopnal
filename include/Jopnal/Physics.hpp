// Jopnal Engine C++ Library
// Copyright (c) 2016 Team Jopnal
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgement in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

//////////////////////////////////////////////

/// \defgroup physics Physics
///
/// #TODO Detailed decription

// Headers
#include <Jopnal/Physics/Collider.hpp>
#include <Jopnal/Physics/Joint/Joint.hpp>
#include <Jopnal/Physics/Joint/GearJoint.hpp>
#include <Jopnal/Physics/Joint/PistonJoint.hpp>
#include <Jopnal/Physics/Joint/RotationJoint.hpp>
#include <Jopnal/Physics/Joint/RopeJoint.hpp>
#include <Jopnal/Physics/Joint/WeldJoint.hpp>
#include <Jopnal/Physics/Joint/WheelJoint.hpp>
#include <Jopnal/Physics/PhantomBody.hpp>
#include <Jopnal/Physics/RayInfo.hpp>
#include <Jopnal/Physics/RigidBody.hpp>
#include <Jopnal/Physics/Shape/BoxShape.hpp>
#include <Jopnal/Physics/Shape/CapsuleShape.hpp>
#include <Jopnal/Physics/Shape/CollisionShape.hpp>
#include <Jopnal/Physics/Shape/CompoundShape.hpp>
#include <Jopnal/Physics/Shape/ConeShape.hpp>
#include <Jopnal/Physics/Shape/ConvexHullShape.hpp>
#include <Jopnal/Physics/Shape/CylinderShape.hpp>
#include <Jopnal/Physics/Shape/FrustumShape.hpp>
#include <Jopnal/Physics/Shape/InfinitePlaneShape.hpp>
#include <Jopnal/Physics/Shape/RectangleShape.hpp>
#include <Jopnal/Physics/Shape/SphereShape.hpp>
#include <Jopnal/Physics/Shape/TerrainShape.hpp>
#include <Jopnal/Physics/World.hpp>
#include <Jopnal/Physics/ContactListener.hpp>