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
    Collider2D::Collider2D(Object& object, World2D& world, const uint32 ID)
        : Component(object, ID),

        m_body(nullptr),
        m_worldRef2D(world)
    {
    }

    Collider2D::Collider2D(const Collider2D& other, Object& newObj)
        : Component(other, newObj),
        m_body(nullptr),
        m_worldRef2D(other.m_worldRef2D)
    {}

    Collider2D::~Collider2D()
    {}

    void Collider2D::update(const float deltaTime)
    {
        switch (m_body->GetType())
        {
        case b2BodyType::b2_dynamicBody:
        {
            auto& pos = m_body->GetPosition();
            getObject()->setPosition(pos.x, pos.y, 0.f);
            getObject()->setRotation(0.f, 0.f, m_body->GetAngle());
            break;
        }
        case b2BodyType::b2_kinematicBody:
        {
            auto& pos = getObject()->getGlobalPosition();
            m_body->SetTransform(b2Vec2(pos.x, pos.y), glm::eulerAngles(getObject()->getGlobalRotation()).z);
            break;
        }
        }
    }

    //////////////////////////////////////////////


    bool Collider2D::checkOverlap(const Collider2D& other) const
    {
        if (this == &other)
            return false;

        struct Callback : b2QueryCallback
        {
            bool overlapping = false;
            const b2Body* bod = nullptr;

            bool ReportFixture(b2Fixture* fix) override
            {
                overlapping = bod == fix->GetBody();
                return !overlapping;
            }
        } cb;

        cb.bod = other.m_body;

        auto fixList = this->m_body->GetFixtureList();
        b2AABB totalaabb(fixList->GetAABB(0));

        while (fixList)
        {
            totalaabb.Combine(fixList->GetAABB(0));
            fixList = fixList->GetNext();
        }
        this->m_worldRef2D.m_worldData2D->QueryAABB(&cb, totalaabb);

        return cb.overlapping;
    }



    bool Collider2D::checkContact(const Collider2D& other) const
    {
        for (b2ContactEdge* ce = m_body->GetContactList(); ce; ce = ce->next)
        {
            if (ce->other == other.m_body)
            {
                return true;
            }
        }
        return false;
    }


    bool Collider2D::checkRay(const glm::vec2& start, const glm::vec2& ray) const
    {
        struct Callback : b2RayCastCallback
        {
            bool hit = false;
            float ReportFixture(b2Fixture* fix, const b2Vec2& point, const b2Vec2& normal, float fraction)
            {
                hit = true;
                return 0.f;
            }
        } cb;

        this->m_worldRef2D.m_worldData2D->RayCast(&cb, b2Vec2(start.x, start.y), b2Vec2(start.x + ray.x, start.y + ray.y));
        return cb.hit;
    }
}
