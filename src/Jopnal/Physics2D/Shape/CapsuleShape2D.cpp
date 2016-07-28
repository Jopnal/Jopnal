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
        : CompoundShape2D(name),
        m_rect(""),
        m_ball1(""),
        m_ball2("")
    {}

    //////////////////////////////////////////////

    bool CapsuleShape2D::load(const float width, const float height)
    {
        float radius = width * 0.5f;

        Transform::Variables rect(
            glm::vec3(0.f, 0.f, 0.f),
            glm::vec3(1.f, 1.f, 1.f),
            glm::quat(1.f, 0.f, 0.f, 0.f)
            );

        if (width >= height)
        {
            //make a ball
            m_ball1.load(radius);
            addChild(m_ball1, rect);
            return true;
        }

        Transform::Variables ball1 = rect;
        ball1.position.y = height * 0.5f - radius;
        
        Transform::Variables ball2 = rect;
        ball2.position.y = -height * 0.5f + radius;

        m_rect.load(width, height - width);
        m_ball1.load(radius);
        m_ball2.load(radius);

        addChild(m_rect, rect);
        addChild(m_ball1, ball1);
        addChild(m_ball2, ball2);

        return true;
    }
}