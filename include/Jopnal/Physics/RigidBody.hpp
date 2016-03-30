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

class btRigidBody;

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


        /// \brief Sets unique gravity to the rigid body object
        ///
        /// \param acceleration Amount of the gravity to be applied as vector
        ///
        RigidBody& setGravity(const glm::vec3& acceleration);

        /// \brief Gets m_gravity as glm::vec3
        ///
        glm::vec3 getGravity()const;

        /// \brief Sets the linear factor for rigid body
        ///
        /// \param linearFactor Unique vector for linear factor
        ///
        RigidBody& setLinearFactor(const glm::vec3& linearFactor);

        /// \brief Gets the linear factor as glm::vec3 
        ///
        glm::vec3 getLinearFactor()const;

        /// \brief Sets the angular factor for rigid body
        ///
        /// \param angularFactor Unique vector for angular factor
        ///
        RigidBody& setAngularFactor(const glm::vec3& angularFactor);

        /// \brief Gets the angular factor as glm::vec3 
        ///
        glm::vec3 getAngularFactor()const;

        /// \brief Applies constant force to rigid bodies relative position
        ///
        /// \param force Amount and direction of the force 
        /// \param rel_pos Vector for the relative position on rigid body that the force applies on
        ///
        RigidBody& applyForce(const glm::vec3& force, const glm::vec3& rel_pos);

        /// \brief Applies an impulse to rigid bodies relative position
        ///
        /// \param impulse Amount and direction of the impulse
        /// \param rel_pos Vector for the relative position on rigid body that the impulse applies on
        ///
        RigidBody& applyImpulse(const glm::vec3& Impulse, const glm::vec3& rel_pos);

        /// \brief Applies torque to the rigid body
        ///
        /// \param torque Amount and direction as vector of the applied torque
        ///
        RigidBody& applyTorque(const glm::vec3& torque);

        /// \brief Applies torque impulse to the rigid body
        ///
        /// \param torque Amount and direction as vector of the applied torque
        ///
        RigidBody& applyTorqueImpulse(const glm::vec3& torque);

        /// \brief Sets linear velocity to the rigid body
        ///
        /// \param linearVelocity Amount and direction of the linear velocity 
        ///
        RigidBody& setLinearVelocity(const glm::vec3& linearVelocity);

        /// \brief Sets angular velocity to the rigid body
        ///
        /// \param angularVelocity Amount and direction of the angular velocity
        ///
        RigidBody& setAngularVelocity(const glm::vec3& angularVelocity);

        /// \brief Applies force to the rigid body's center 
        ///
        /// \param force Amount and direction of the applied force
        ///
        RigidBody& applyCentralForce(const glm::vec3& force);

        /// \brief Applies impulse to the rigid body's center
        ///
        /// \param impulse Amount and direction of the applies impulse
        ///
        RigidBody& applyCentralImpulse(const glm::vec3& impulse);

        /// \brief Clear all the forces affecting this body
        ///
        void clearForces();

    private:

        /// \brief Called when the bound object is set active/inactive
        ///
        /// Sets the physics body activity
        ///
        /// \param active Was the object set active?
        ///
        void setActive(const bool active) override;

        const Type m_type;           ///< The body type
        const float m_mass;          ///< The mass
        btRigidBody* m_rigidBody;    ///< Unique rigidbody identifier
    };
}

#endif