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

#ifndef JOP_WORLDIMPL_HPP
#define JOP_WORLDIMPL_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Bullet/btBulletDynamicsCommon.h>
#include <memory>

//////////////////////////////////////////////


namespace jop { namespace detail
{
    struct WorldImpl final
    {
        explicit WorldImpl(btIDebugDraw* debugDraw);

        ~WorldImpl();


        std::unique_ptr<btDefaultCollisionConfiguration>         config;
        std::unique_ptr<btCollisionDispatcher>                   dispatcher;
        std::unique_ptr<btBroadphaseInterface>                   overlappingPairCache;
        std::unique_ptr<btSequentialImpulseConstraintSolver>     solver;
        std::unique_ptr<btDiscreteDynamicsWorld>                 world;

    };
}}

#endif