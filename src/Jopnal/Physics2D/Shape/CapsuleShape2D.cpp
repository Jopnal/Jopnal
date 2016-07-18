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

#include <Jopnal/Physics2D/Shape/CapsuleShape2D.hpp>

#include <Jopnal/STL.hpp>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>

#pragma warning(push)
#pragma warning(disable: 4127)

#pragma warning(pop)

#endif

//////////////////////////////////////////////


namespace jop
{
    CapsuleShape2D::CapsuleShape2D(const std::string& name)
        : CollisionShape2D(name)
    {}

    //////////////////////////////////////////////

    bool CapsuleShape2D::load(const float radius, const float height)
    {
        auto temp = std::make_unique<b2PolygonShape>();

        unsigned int accuracy = 4;

        float radians = glm::pi<float>() / accuracy;
        float theta = 0.f;

        float hheight = height / 2.f;

        std::vector<glm::vec2> points;
        points.reserve(accuracy * 2 + 2);
        {
            points.emplace_back(radius, hheight - radius);

            for (unsigned int i = 0; i < accuracy; ++i)
            {
                theta += radians;
                points.emplace_back(std::cos(theta) * radius, hheight - radius + (radius * std::sin(theta)));
            }

            points.emplace_back(-radius, -hheight + radius);

            for (unsigned int i = 0; i < accuracy; ++i)
            {
                theta += radians;
                points.emplace_back(std::cos(theta) * radius, -hheight + radius + (radius * std::sin(theta))); //works
            }
        }
        temp->Set(reinterpret_cast<const b2Vec2*>(points.data()), points.size());

        m_shape = std::move(temp);

        return true;
    }
}