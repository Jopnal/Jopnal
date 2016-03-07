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
    namespace detail
    {
        struct WorldData
        {
            std::unique_ptr<btDefaultCollisionConfiguration>         config;
            std::unique_ptr<btCollisionDispatcher>                   dispatcher;
            std::unique_ptr<btBroadphaseInterface>                   overlappingPairCache;
            std::unique_ptr<btSequentialImpulseConstraintSolver>     solver;
            std::unique_ptr<btDiscreteDynamicsWorld>                 world;

            WorldData()
                : config                (std::make_unique<btDefaultCollisionConfiguration>()),
                  dispatcher            (std::make_unique<btCollisionDispatcher>(config.get())),
                  overlappingPairCache  (std::make_unique<btDbvtBroadphase>()),
                  solver                (std::make_unique<btSequentialImpulseConstraintSolver>()),
                  world                 (std::make_unique<btDiscreteDynamicsWorld>(dispatcher.get(), overlappingPairCache.get(), solver.get(), config.get()))
            {}
        };
    }

    //////////////////////////////////////////////

    PhysicsWorld::PhysicsWorld(Object& object, const std::string& ID)
        : Component     (object, ID),
          m_worldData   (std::make_unique<detail::WorldData>())
    {}

    PhysicsWorld::~PhysicsWorld()
    {
        // Need to define destructor because of forward declaration
    }

    //////////////////////////////////////////////

    void PhysicsWorld::fixedUpdate(const float timeStep)
    {
        m_worldData->world->stepSimulation(timeStep, 1, timeStep);
    }
}