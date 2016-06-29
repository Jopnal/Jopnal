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
    RectangleShape2D::RectangleShape2D(const std::string& name)
        : CollisionShape2D(name)
    {}

    //////////////////////////////////////////////

    bool RectangleShape2D::load(const float sizeX, const float sizeY)
    {
        return load(glm::vec2(sizeX, sizeY));
    }

    //////////////////////////////////////////////

    bool RectangleShape2D::load(const glm::vec2& extents)
    {
        const b2Vec2 half = b2Vec2(extents.x * 0.5f, extents.y * 0.5f);

        const b2Vec2 points[] =
        {
            b2Vec2(-half.x, -half.y),
            b2Vec2(half.x, -half.y),
            b2Vec2(half.x, half.y),
            b2Vec2(-half.x, half.x)
        };

        

        b2PolygonShape ps;

        //all shape members must be set here by hand??

        ps.Set(points, 4); //too hardcode? //a rectangle has four corners[citation needed]

        m_shape = std::make_unique<b2PolygonShape>(ps);

        //m_shape->setUserPointer(this); //do we need something like this? //from bullet

        return true;
    }
}