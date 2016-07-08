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

//////////////////////////////////////////////



namespace jop { namespace detail
{
    const b2Vec2 Gravity2D = { 0.0f, 9.81f }; //b2World constructor wants gravity

    WorldImpl2D::WorldImpl2D()
        : //config                (std::make_unique<btDefaultCollisionConfiguration>()),
        //dispatcher            (std::make_unique<btCollisionDispatcher>(config.get())),
        //overlappingPairCache  (std::make_unique<btDbvtBroadphase>()),
        //solver                (std::make_unique<btSequentialImpulseConstraintSolver>()),
        world2D                   (std::make_unique<b2World>(Gravity2D))                   //SettingManager::get<float>("engine@Physics|DefaultWorld|fGravity", -9.81f))
    {
    #ifdef JOP_DEBUG_MODE
        //world->setDebugDrawer(debugDraw);
    #else
        debugDraw;
    #endif
    }

    //////////////////////////////////////////////

    WorldImpl2D::~WorldImpl2D()
    {
        delete &world2D;

    #ifdef JOP_DEBUG_MODE
        //delete world->getDebugDrawer();
    #endif
    }
}}