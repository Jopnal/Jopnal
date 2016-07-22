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

    #include <Jopnal/Physics2D/Collider2D.hpp>

    #include <Jopnal/Core/Object.hpp>
    #include <Jopnal/Physics2D/World2D.hpp>
    #include <Box2D/Dynamics/b2Body.h>
    #include <Box2D/Dynamics/b2WorldCallbacks.h>
    #include <Box2D/Dynamics/b2World.h>
    #include <Box2D/Dynamics/b2Fixture.h>
    #include <Box2D/Dynamics/Contacts/b2Contact.h>

#endif

//////////////////////////////////////////////


namespace jop
{
    Collider2D::Collider2D(Object& object, World2D& world, const uint32 ID)
        : Component     (object, ID),
          m_body        (nullptr),
          m_worldRef2D  (world),
          m_listeners   ()
    {}

    Collider2D::Collider2D(const Collider2D& other, Object& newObj)
        : Component     (other, newObj),
          m_body        (nullptr),
          m_worldRef2D  (other.m_worldRef2D),
          m_listeners   (other.m_listeners)
    {}

    Collider2D::~Collider2D()
    {}

    //////////////////////////////////////////////

    void Collider2D::update(const float)
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

        // Visual studio 2013 doesn't compile without this-> ???
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

    //////////////////////////////////////////////

    bool Collider2D::checkContact(const Collider2D& other) const
    {
        auto ce = m_body->GetContactList();
        while (ce)
        {
            if (ce->other == other.m_body)
                return true;

            ce = ce->next;
        }
        return false;
    }

    //////////////////////////////////////////////

    bool Collider2D::checkRay(const glm::vec2& start, const glm::vec2& ray) const
    {
        struct Callback : b2RayCastCallback
        {
            const b2Body* body = nullptr;
            bool hit = false;
            float ReportFixture(b2Fixture* fix, const b2Vec2&, const b2Vec2&, float)
            {
                if (fix->GetBody() == body)
                {
                    hit = true;
                    return 0.f;
                }
                return 1.f;
            }
        } cb;

        cb.body = this->m_body;
        
        this->m_worldRef2D.m_worldData2D->RayCast(&cb, b2Vec2(start.x, start.y), b2Vec2(start.x + ray.x, start.y + ray.y));

        return cb.hit;
    }

    //////////////////////////////////////////////

    World2D& Collider2D::getWorld()
    {
        return m_worldRef2D;
    }

    //////////////////////////////////////////////

    const World2D& Collider2D::getWorld() const
    {
        return m_worldRef2D;
    }

    //////////////////////////////////////////////

    void Collider2D::registerListener(ContactListener2D& listener)
    {
        if (listener.m_collider != this)
        {
            if (listener.m_collider)
            {
                listener.m_collider->m_listeners.erase(&listener);
            }
            // Replace old collider with this
            listener.m_collider = this;

            // Check if this listener is already registered
            std::pair<std::set<ContactListener2D*>::iterator, bool> ret;
            ret = m_listeners.insert(&listener);

            if (ret.second == false)
            {
                // Erase the old and replace it with new
                m_listeners.erase(ret.first);
                m_listeners.insert(&listener);
            }
        }
        else
        {
            JOP_DEBUG_WARNING("Could not register listener for Collider2D: Listener is already registered for collider");
            return;
        }                
    }
    
}
