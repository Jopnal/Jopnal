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

#ifndef JOP_RIGIDBODY_HPP
#define JOP_RIGIDBODY_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Physics/Collider.hpp>

//////////////////////////////////////////////


namespace jop
{
    class CollisionShape;

    class JOP_API RigidBody final : public Collider
    {
    private:

        RigidBody(const RigidBody& other, Object& newObj);

        JOP_GENERIC_COMPONENT_CLONE(RigidBody);
        JOP_DISALLOW_COPY_MOVE(RigidBody);

    public:

        /// Rigid body type
        ///
        enum class Type
        {
            Static,     ///< Non-moving body
            Dynamic,    ///< Moving body
            Kinematic,  ///< User-animated body

            // Sensor types (no collision response)
            StaticSensor,   ///< Non-moving
            KinematicSensor ///< User-animated
        };

    public:

        /// \brief Constructor
        ///
        /// \param object Reference to the object
        /// \param world The physics world
        /// \param shape The collision shape
        /// \param type Body type
        /// \param mass Mass, will default to 0 when type is static or kinematic
        /// \param group Collision group
        /// \param mask Collision mask
        ///
        RigidBody(Object& object, World& world, const CollisionShape& shape, const Type type = Type::Static, const float mass = 0.f, const int16 group = 1, const int16 mask = 1);

        /// \brief Destructor
        ///
        ~RigidBody() override;

        /// \brief
        RigidBody& setGravity(const glm::vec3& acceleration);

        /// \brief
        glm::vec3 getGravity()const;

        /// \brief
        RigidBody& setLinearFactor(const glm::vec3& linearFactor);

        /// \brief
        glm::vec3 getLinearFactor()const;

        /// \brief
        RigidBody& setAngularFactor(const glm::vec3& angularFactor);

        /// \brief
        glm::vec3 getAngularFactor()const;

        /// \brief
        RigidBody& applyForce(const glm::vec3& force, const glm::vec3& rel_pos);

        /// \brief
        RigidBody& applyImpulse(const glm::vec3& Impulse, const glm::vec3& rel_pos);

        /// \brief
        RigidBody& applyTorque(const glm::vec3& torque);

        /// \brief
        RigidBody& applyTorqueImpulse(const glm::vec3& torque);

        /// \brief
        RigidBody& setLinearVelocity(const glm::vec3& linearVelocity);

        /// \brief
        RigidBody& setAngularVelocity(const glm::vec3& angularVelocity);

        /// \brief
        RigidBody& applyCentralForce(const glm::vec3& force);

        /// \brief
        RigidBody& applyCentralImpulse(const glm::vec3& impulse);
    private:

        /// \brief Called when the bound object is set active/inactive
        ///
        /// Sets the physics body activity
        ///
        /// \param active Was the object set active?
        ///
        void setActive(const bool active) override;

        const Type m_type;  ///< The body type
        const float m_mass; ///< The mass
    };
}

#endif