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

#ifndef JOP_BOXSHAPE_HPP
#define JOP_BOXSHAPE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics/Shape/CollisionShape.hpp>
#include <glm/vec3.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API BoxShape : public CollisionShape
    {
    public:

        /// \brief Constructor
        ///
        /// \param name Name of the resource
        ///
        BoxShape(const std::string& name);

        /// \brief Copy constructor
        ///
        BoxShape(const BoxShape& other, const std::string& newName);


        /// \brief Load this shape
        ///
        /// \param size Size of the box
        ///
        /// \return True if successful
        ///
        bool load(const float size);

        /// \brief Load this shape using different extents
        ///
        /// \param extents Extents of the box
        ///
        /// \return True if successful
        ///
        bool load(const glm::vec3& extents);
    };
}

/// \class jop::BoxShape
/// \ingroup physics

#endif