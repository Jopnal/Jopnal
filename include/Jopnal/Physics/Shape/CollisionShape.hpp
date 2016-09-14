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

#ifndef JOP_COLLISIONSHAPE_HPP
#define JOP_COLLISIONSHAPE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Resource.hpp>
#include <glm/vec3.hpp>
#include <memory>

//////////////////////////////////////////////


class btCollisionShape;

namespace jop
{
    class JOP_API CollisionShape : public Resource
    {
    private:

        JOP_DISALLOW_COPY_MOVE(CollisionShape);

        friend class RigidBody;
        friend class PhantomBody;
        friend class CompoundShape;
        friend class Collider;

    protected:

        /// \copydoc Resource::Resource(const std::string&)
        ///
        CollisionShape(const std::string& name);

        /// \copydoc Resource::Resource(const Resource&, const std::string&)
        ///
        CollisionShape(const CollisionShape& other, const std::string& newName);

    public:

        /// \brief Virtual destructor
        ///
        virtual ~CollisionShape() override = 0;


        /// \brief Set the margin of this shape
        ///
        /// Usually you can leave this at the default.
        ///
        /// \param margin The margin to set
        ///
        void setMargin(const float margin);

        /// \brief Get the margin of this shape
        ///
        /// \return The margin
        ///
        float getMargin() const;

        /// \brief Set the local scaling
        ///
        /// This will scale the collision shape itself, but changes
        /// won't be seen in colliders until Collider::updateWorldBounds()
        /// is called on each one.
        ///
        /// \param The new scale to set
        ///
        void setLocalScale(const glm::vec3& scale);

        /// \brief Get the current local scaling
        ///
        /// \return The local scaling
        ///
        glm::vec3 getLocalScale() const;

    protected:

        std::unique_ptr<btCollisionShape> m_shape;  ///< Shape data
    };
}

/// \class jop::CollisionShape
/// \ingroup physics

#endif