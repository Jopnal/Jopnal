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
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
    ConvexHullShape2D::ConvexHullShape2D(const std::string& name)
        : CollisionShape2D(name)
    {}

    //////////////////////////////////////////////

    bool ConvexHullShape2D::load(const std::vector<glm::vec2>& points)
    {
        return load(points, std::vector<unsigned int>());
    }

    //////////////////////////////////////////////

    bool ConvexHullShape2D::load(const std::vector<glm::vec2>& points, const std::vector<unsigned int>& indices)
    {

        //maxpolygonvertices = 8 by default

        if (8 >= points.size() >= 3)
            return false;

        else if (indices.empty())
        {
            b2PolygonShape ps;
            for (int i = 0; i < points.size(); ++i)
                ps.m_vertices[i] = b2Vec2(points[i].x, points[i].y);

            m_shape = std::make_unique<b2PolygonShape>(ps);
        }

        else
        {
            b2PolygonShape ps;
            for (auto i : indices)
                ps.m_vertices[i] = b2Vec2(points[i].x, points[i].y);

            m_shape = std::make_unique<b2PolygonShape>(ps);
        }
        //m_shape->setUserPointer(this);
        return true;
    }

    //////////////////////////////////////////////

    ConvexHullShape2D& ConvexHullShape2D::getDefault()
    {
        static WeakReference<ConvexHullShape2D> defShape;

        if (defShape.expired())
        {
            defShape = static_ref_cast<ConvexHullShape>(ResourceManager::getEmptyResource<ConvexHullShape>("jop_default_staticmeshshape").getReference());
            defShape->m_shape = std::make_unique<btCapsuleShape>(0.5f, 2.f);

            defShape->setPersistence(0);
        }

        return *defShape;
    }
}