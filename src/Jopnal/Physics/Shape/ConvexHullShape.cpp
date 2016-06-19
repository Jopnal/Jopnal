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

	#include <Jopnal/Physics/Shape/ConvexHullShape.hpp>

    #include <Jopnal/Core/ResourceManager.hpp>
    #include <Jopnal/STL.hpp>
    #include <Bullet/btBulletCollisionCommon.h>

#endif

//////////////////////////////////////////////


namespace jop
{
    ConvexHullShape::ConvexHullShape(const std::string& name)
        : CollisionShape(name)
    {}

    //////////////////////////////////////////////

    bool ConvexHullShape::load(const std::vector<glm::vec3>& points)
    {
        return load(points, std::vector<unsigned int>());
    }

    //////////////////////////////////////////////

    bool ConvexHullShape::load(const std::vector<glm::vec3>& points, const std::vector<unsigned int>& indices)
    {
        if (points.empty())
            return false;

        if (indices.empty())
            m_shape = std::make_unique<btConvexHullShape>(&points[0][0], points.size(), sizeof(glm::vec3));

        else
        {
            std::vector<glm::vec3> temp;
            temp.reserve(indices.size());

            for (auto i : indices)
                temp.emplace_back(points[i]);

            m_shape = std::make_unique<btConvexHullShape>(&temp[0][0], temp.size(), sizeof(glm::vec3));
        }

        m_shape->setUserPointer(this);

        return true;
    }

    //////////////////////////////////////////////

    ConvexHullShape& ConvexHullShape::getDefault()
    {
        static WeakReference<ConvexHullShape> defShape;

        if (defShape.expired())
        {
            defShape = static_ref_cast<ConvexHullShape>(ResourceManager::getEmptyResource<ConvexHullShape>("jop_default_staticmeshshape").getReference());
            defShape->m_shape = std::make_unique<btCapsuleShape>(0.5f, 2.f);

            defShape->setPersistence(0);
        }

        return *defShape;
    }
}