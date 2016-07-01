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

#ifndef JOP_RAYINFO_HPP
#define JOP_RAYINFO_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <glm/vec3.hpp>

//////////////////////////////////////////////


namespace jop
{
    class Collider;

    struct JOP_API RayInfo
    {
        /// \brief Default constructor
        ///
        RayInfo();

        /// \brief Constructor
        ///
        /// \param coll Pointer to the collider
        /// \param pnt Hit point
        /// \param norm Hit normal
        ///
        RayInfo(Collider* coll, const glm::vec3& pnt, const glm::vec3& norm);


        Collider* collider; ///< Collider component, can be nullptr
        glm::vec3 point;    ///< Ray hit point in world coordinates
        glm::vec3 normal;   ///< Local ray hit normal
    };
}

#endif