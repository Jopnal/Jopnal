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

    #include <Jopnal/Graphics/Culling/CullingWorld.hpp>

#endif

//////////////////////////////////////////////


namespace jop { namespace detail
{
    struct CullBroadphaseCallback : btOverlapFilterCallback
    {
        bool needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const override
        {
            auto c0 = static_cast<const CullerComponent*>(static_cast<const btCollisionObject*>(proxy0->m_clientObject)->getUserPointer());
            auto c1 = static_cast<const CullerComponent*>(static_cast<const btCollisionObject*>(proxy1->m_clientObject)->getUserPointer());

            return (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) > 0 &&
                   (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask) > 0 &&
                   (c0->shouldCollide(*c1) || c1->shouldCollide(*c0));
        }
    };

    //////////////////////////////////////////////

    CullingWorld::CullingWorld(Object& object, Renderer& renderer)
        : World(object, renderer)
    {
        m_bpCallback = std::make_unique<CullBroadphaseCallback>();
        m_worldData->world->getPairCache()->setOverlapFilterCallback(m_bpCallback.get());
        m_ghostCallback = std::make_unique<btGhostPairCallback>();
        m_worldData->world->getPairCache()->setInternalGhostPairCallback(m_ghostCallback.get());

        setGravity(glm::vec3());
        setDebugMode(true);
    }

    //////////////////////////////////////////////

    void CullingWorld::update(const float deltaTime)
    {
        for (auto& i : m_cullerComps)
        {
            for (auto& j : i)
                j->updateTransform();
        }

        World::update(deltaTime);

        for (auto& i : m_cullerComps[static_cast<int>(CullerComponent::Type::Camera)])
            static_cast<::jop::Camera*>(i)->update();

        // lights
        // envrecorders
    }

    //////////////////////////////////////////////

    void CullingWorld::bind(::jop::detail::CullerComponent* comp)
    {
        m_cullerComps[static_cast<int>(comp->getType())].insert(comp);
    }

    //////////////////////////////////////////////

    void CullingWorld::unbind(::jop::detail::CullerComponent* comp)
    {
        m_cullerComps[static_cast<int>(comp->getType())].erase(comp);
    }
}}