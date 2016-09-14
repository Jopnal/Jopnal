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

    #include <Jopnal/Physics/Shape/FrustumShape.hpp>

    #include <glm/gtx/rotate_vector.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    FrustumShape::FrustumShape(const std::string& name)
        : ConvexHullShape(name)
    {}

    //////////////////////////////////////////////

    bool FrustumShape::load(const Camera::ClippingPlanes& clip, const glm::vec2& size, const glm::quat& rotation)
    {
        const auto half = size * 0.5f;

        const std::vector<glm::vec3> points =
        {
            // Near
            rotation * glm::vec3(-half.x, -half.y, clip.first),    // Lower left
            rotation * glm::vec3( half.x, -half.y, clip.first),    // Lower right
            rotation * glm::vec3( half.x,  half.y, clip.first),    // Upper right
            rotation * glm::vec3(-half.x,  half.y, clip.first),    // Upper left

            // Far
            rotation * glm::vec3(-half.x, -half.y, clip.second),   // Lower left
            rotation * glm::vec3( half.x, -half.y, clip.second),   // Lower right
            rotation * glm::vec3( half.x,  half.y, clip.second),   // Upper right
            rotation * glm::vec3(-half.x,  half.y, clip.second),   // Upper left
        };

        return ConvexHullShape::load(points);
    }

    //////////////////////////////////////////////

    bool FrustumShape::load(const Camera::ClippingPlanes& clip, const float fov, const float aspectRatio, const glm::quat& rotation)
    {
        const auto halfFov = fov * 0.5f;

        const glm::vec3 nearHalf = glm::abs(glm::rotateY(glm::rotateX(glm::vec3(0.f, 0.f, -clip.first), halfFov), halfFov * aspectRatio));
        const glm::vec3 farHalf = glm::abs(glm::rotateY(glm::rotateX(glm::vec3(0.f, 0.f, -clip.second), halfFov), halfFov * aspectRatio));

        const std::vector<glm::vec3> points =
        {
            // Near
            rotation * glm::vec3(-nearHalf.x, -nearHalf.y, -clip.first),   // Lower left
            rotation * glm::vec3( nearHalf.x, -nearHalf.y, -clip.first),   // Lower right
            rotation * glm::vec3( nearHalf.x,  nearHalf.y, -clip.first),   // Upper right
            rotation * glm::vec3(-nearHalf.x,  nearHalf.y, -clip.first),   // Upper left

            // Far
            rotation * glm::vec3(-farHalf.x, -farHalf.y, -clip.second),    // Lower left
            rotation * glm::vec3( farHalf.x, -farHalf.y, -clip.second),    // Lower right
            rotation * glm::vec3( farHalf.x,  farHalf.y, -clip.second),    // Upper right
            rotation * glm::vec3(-farHalf.x,  farHalf.y, -clip.second),    // Upper left
        };

        return ConvexHullShape::load(points);
    }
}