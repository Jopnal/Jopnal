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

/// \defgroup physics2d Physics2D

// Headers
#include <Jopnal/Physics2D/Collider2D.hpp>
#include <Jopnal/Physics2D/Joint/GearJoint2D.hpp>
#include <Jopnal/Physics2D/Joint/PistonJoint2D.hpp>
#include <Jopnal/Physics2D/Joint/PulleyJoint2D.hpp>
#include <Jopnal/Physics2D/Joint/RopeJoint2D.hpp>
#include <Jopnal/Physics2D/Joint/RotationJoint2D.hpp>
#include <Jopnal/Physics2D/Joint/WeldJoint2D.hpp>
#include <Jopnal/Physics2D/Joint/WheelJoint2D.hpp>
#include <Jopnal/Physics2D/Joint/Joint2D.hpp>
#include <Jopnal/Physics2D/RayInfo2D.hpp>
#include <Jopnal/Physics2D/RigidBody2D.hpp>
#include <Jopnal/Physics2D/Shape/CapsuleShape2D.hpp>
#include <Jopnal/Physics2D/Shape/CircleShape2D.hpp>
#include <Jopnal/Physics2D/Shape/CollisionShape2D.hpp>
#include <Jopnal/Physics2D/Shape/CompoundShape2D.hpp>
#include <Jopnal/Physics2D/Shape/ConeShape2D.hpp>
#include <Jopnal/Physics2D/Shape/ConvexHullShape2D.hpp>
#include <Jopnal/Physics2D/Shape/RectangleShape2D.hpp>
#include <Jopnal/Physics2D/Shape/TerrainShape2D.hpp>
#include <Jopnal/Physics2D/World2D.hpp>
#include <Jopnal/Physics2D/ContactListener2D.hpp>
#include <Jopnal/Physics2D/ContactInfo2D.hpp>