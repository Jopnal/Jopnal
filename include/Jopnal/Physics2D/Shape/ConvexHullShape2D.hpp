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

#ifndef JOP_STATICMESHSHAPE2D_HPP
#define JOP_STATICMESHSHAPE2D_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics2D/Shape/CollisionShape2D.hpp>
#include <glm/vec2.hpp>
#include <vector>

//////////////////////////////////////////////

namespace jop
{
    class JOP_API ConvexHullShape2D : public CollisionShape2D
    {
    public:

        /// \brief Constructor
        ///
        /// \param name Name of the resource
        ///
        ConvexHullShape2D(const std::string& name);

        /// \brief Load this shape
        ///
        /// \param points Unindexed vertices (triangles)
        ///
        /// \return True if successful
        /// 
        bool load(const std::vector<glm::vec2>& points);

        /// \brief Load this shape using indexed points
        ///
        /// Make sure that the indices are within the bounds of the vertex array. This function
        /// will not check for overflows.
        ///
        /// \param points Indexed vertices (triangles)
        /// \param indices Indices
        ///
        /// \return True if successful
        ///
        bool load(const std::vector<glm::vec2>& points, const std::vector<unsigned int>& indices);

        /// \brief Get the default mesh shape
        ///
        /// \return Reference to the shape
        ///
        static ConvexHullShape2D& getDefault();
    };
}

/// \class jop::ConvexHullShape2D
/// \ingroup physics2d

#endif