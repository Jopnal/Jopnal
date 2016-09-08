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

#ifndef JOP_FRUSTUMSHAPE_HPP
#define JOP_FRUSTUMSHAPE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Camera.hpp>
#include <Jopnal/Physics/Shape/ConvexHullShape.hpp>
#include <glm/gtx/quaternion.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API FrustumShape : public ConvexHullShape
    {
    public:
    
        /// \copydoc Resource::Resource(const std::string&)
        ///
        FrustumShape(const std::string& name);


        /// \brief Load this shape
        ///
        /// This will create a shape for an orthographic projection.
        ///
        /// \param clip The clipping planes
        /// \param size Size of the projection
        /// \param rotation Local rotation to apply
        ///
        /// \return True if successful
        ///
        bool load(const Camera::ClippingPlanes& clip, const glm::vec2& size, const glm::quat& rotation);

        /// \brief Load this shape
        ///
        /// This will create a shape for a perspective projection.
        ///
        /// \param clip The clipping planes
        /// \param size The vertical field of view in radians
        /// \param aspectRatio The aspect ratio (x / y)
        /// \param rotation Local rotation to apply
        ///
        /// \return True if successful
        ///
        bool load(const Camera::ClippingPlanes& clip, const float fov, const float aspectRatio, const glm::quat& rotation);
    };
}

/// \class jop::FrustumShape
/// \ingroup physics

#endif