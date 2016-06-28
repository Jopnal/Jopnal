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

    bool RectangleShape2D::load(const float size)
    {
        return load(float(size));
    }

    //////////////////////////////////////////////

    bool RectangleShape2D::load(const b2Vec2& extents)
    {
        const b2Vec2 half = b2Vec2(extents.x * 0.5f, extents.y * 0.5f);

        const b2Vec2 points[] =
        {
            b2Vec2( -half.x, -half.y),
            b2Vec2(  half.x, -half.y),
            b2Vec2(  half.x,  half.y),
            b2Vec2( -half.x,  half.x)
        };

        m_shape = std::make_unique<b2PolygonShape>(points[0]);
        m_shape->setUserPointer(this);

        return true;
    }
}