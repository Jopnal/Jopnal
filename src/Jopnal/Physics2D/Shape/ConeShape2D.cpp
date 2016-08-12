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

    #include <Jopnal/Physics2D/Shape/ConeShape2D.hpp>

    #include <Jopnal/STL.hpp>
    #include <Box2D/Collision/Shapes/b2PolygonShape.h>
    #include <glm/vec2.hpp>
    #include <array>

#endif

//////////////////////////////////////////////

namespace jop
{
    ConeShape2D::ConeShape2D(const std::string& name)
        : CollisionShape2D(name)
    {}

    //////////////////////////////////////////////

    bool ConeShape2D::load(const float width, const float height)
    {
        auto temp = std::make_unique<b2PolygonShape>();

        const std::array<glm::vec2, 3> points =
        {
            glm::vec2(0.f, height*0.5f),
            glm::vec2(-width * 0.5f, -height*0.5f),
            glm::vec2(width * 0.5f, -height*0.5f)
        };

        temp->Set(reinterpret_cast<const b2Vec2*>(points.data()), points.size());
        m_shape = std::move(temp);

        return true;
    }
}