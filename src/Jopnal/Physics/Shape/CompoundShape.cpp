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

// Headers
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

	#include <Jopnal/Physics/Shape/CompoundShape.hpp>

    #include <Jopnal/Core/DebugHandler.hpp>
    #include <Jopnal/STL.hpp>
    #include <btBulletCollisionCommon.h>

#endif

//////////////////////////////////////////////


namespace jop
{
    CompoundShape::CompoundShape(const std::string& name)
        : CollisionShape(name)
    {
        m_shape = std::make_unique<btCompoundShape>();
        m_shape->setUserPointer(this);
    }

    //////////////////////////////////////////////

    void CompoundShape::addChild(CollisionShape& childShape, const Transform::Variables& childTransform)
    {
        if (!childShape.m_shape)
        {
            JOP_DEBUG_ERROR("Compound shape \"" << getName() << "\": Tried to add an empty child shape");
            return;
        }

        auto shape = static_cast<btCompoundShape*>(m_shape.get());

        auto& r = childTransform.rotation;
        auto& p = childTransform.position;
        const btTransform transform(btQuaternion(r.x, r.y, r.z, r.w), btVector3(p.x, p.y, p.z));

        shape->addChildShape(transform, childShape.m_shape.get());
    }
}