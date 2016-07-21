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

#include <Jopnal/Physics2D/Shape/CompoundShape2D.hpp>

#include <Jopnal/Core/DebugHandler.hpp>
#include <Jopnal/STL.hpp>    

#pragma warning(push)
#pragma warning(disable: 4127)

#pragma warning(pop)

#endif

//////////////////////////////////////////////

namespace jop
{
    CompoundShape2D::CompoundShape2D(const std::string& name)
        : CollisionShape2D(name)
    {
        auto temp = std::make_unique<b2PolygonShape>();
        temp->SetAsBox(1.f, 1.f);
        m_shape = std::move(temp);
        m_isCompound = true;
    }

    CompoundShape2D::~CompoundShape2D()
    {
    }

    //////////////////////////////////////////////

    void CompoundShape2D::addChild(CollisionShape2D& childShape, const Transform::Variables& childTransform)
    {
        if (!childShape.m_shape)
        {
            JOP_DEBUG_ERROR("Compound shape \"" << getName() << "\": Tried to add an empty child shape");
            return;
        }

        if (m_shapes.find(childShape.m_shape.get()) != m_shapes.end())
            return;

        auto pos = b2Vec2(childTransform.position.x, childTransform.position.y);
        auto cs = childTransform.scale;
        auto cr = glm::eulerAngles( childTransform.rotation);

        

        switch (childShape.m_shape->GetType())
        {

        case b2Shape::Type::e_polygon:
        {
            auto ptr = static_cast<b2PolygonShape*>(childShape.m_shape.get());

            for (unsigned int i = 0; i < sizeof(ptr->m_vertices) / sizeof(ptr->m_vertices[0]); ++i)
            {
                //Scale
                ptr->m_vertices[i].x *= cs.x;
                ptr->m_vertices[i].y *= cs.y;

                //Rotation
                const auto rot = glm::rotate(glm::vec2(ptr->m_vertices[i].x, ptr->m_vertices[i].y), cr.z);
                ptr->m_vertices[i] = b2Vec2(rot.x, rot.y);

                //Position
                ptr->m_vertices[i] += pos;
            }
            ptr->m_centroid += pos;
            
            m_shapes.emplace(ptr);
            break;
        }

        case b2Shape::Type::e_chain:
        {
            auto ptr = static_cast<b2ChainShape*>(childShape.m_shape.get());

            for (unsigned int i = 0; i < sizeof(ptr->m_vertices) / sizeof(ptr->m_vertices[0]); ++i)
            {
                //Scale
                ptr->m_vertices[i].x *= cs.x;
                ptr->m_vertices[i].y *= cs.y;

                //Rotation
                const auto rot = glm::rotate(glm::vec2(ptr->m_vertices[i].x, ptr->m_vertices[i].y), cr.z);
                ptr->m_vertices[i] = b2Vec2(rot.x, rot.y);

                //Position
                ptr->m_vertices[i] += pos;
            }

            m_shapes.emplace(ptr);
            break;
        }

        case b2Shape::Type::e_circle:
        {
            auto ptr = static_cast<b2CircleShape*>(childShape.m_shape.get());
            
            //Position
            ptr->m_p += pos;

            //Scale
            ptr->m_radius *= cs.x != 0.f ? cs.x : cs.y != 0.f ? cs.y : 1.f;
            
            m_shapes.emplace(ptr);
            break;
        }

        default:
            JOP_ASSERT(false, "CompoundShape2D addChild: shape type not supported.");
            //do something?
        }
    }
}