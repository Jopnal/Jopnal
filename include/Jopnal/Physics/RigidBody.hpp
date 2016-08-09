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
#include <Jopnal/Physics/Joint/Joint.hpp>

//////////////////////////////////////////////


class btRigidBody;

namespace jop
{
    class CollisionShape;
    class Joint;

    class JOP_API RigidBody : public Collider
    {
    private:

        JOP_GENERIC_COMPONENT_CLONE(RigidBody);

        friend class Joint;

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

        /// Rigid body construction info
        ///
        class JOP_API ConstructInfo
        {
        private:

            JOP_DISALLOW_COPY_MOVE(ConstructInfo);

            friend class RigidBody;

        public:

            /// \brief Constructor
            ///
            /// \param shape The collision shape
            /// \param type Body type
            /// \param mass Mass, will default to 0 when type is static or kinematic
            ///
            ConstructInfo(const CollisionShape& shape, const Type type = Type::Static, const float mass = 0.f);

            int16 group;            ///< Collision filter group
            int16 mask;             ///< Collision filter mask

            float friction;         ///< Friction
            float rollingFriction;  ///< Rolling friction
            float restitution;      ///< Restitution

        private:

            const CollisionShape& m_shape;  ///< Collision shape
            const Type m_type;              ///< Body type
            const float m_mass;             ///< Mass
        };

    public:

        /// \brief Constructor
        ///
        /// \param object Reference to the object
        /// \param world The physics world
        /// \param info Construction info
        ///
        RigidBody(Object& object, World& world, const ConstructInfo& info);

        /// \brief Destructor
        ///
        virtual ~RigidBody() override;


        /// \brief Set gravity to the rigid body object
        ///
        /// \comm setBodyGravity
        ///
        /// \param acceleration Amount of the gravity to be applied as vector
        ///
        /// \return Reference to self
        ///
        RigidBody& setGravity(const glm::vec3& acceleration);

        /// \brief Get the gravity
        ///
        glm::vec3 getGravity() const;

        /// \brief Sets the linear factor for rigid body
        ///
        /// \comm setLinearFactor
        ///
        /// \param linearFactor Unique vector for linear factor
        ///
        /// \return Reference to self
        ///
        RigidBody& setLinearFactor(const glm::vec3& linearFactor);

        /// \brief Gets the linear factor as glm::vec3 
        ///
        glm::vec3 getLinearFactor() const;

        /// \brief Sets the angular factor for rigid body
        ///
        /// \comm setAngularFactor
        ///
        /// \param angularFactor Unique vector for angular factor
        ///
        /// \return Reference to self
        ///
        RigidBody& setAngularFactor(const glm::vec3& angularFactor);

        /// \brief Gets the angular factor as glm::vec3 
        ///
        glm::vec3 getAngularFactor() const;

        /// \brief Applies constant force to rigid bodies relative position
        ///
        /// \comm applyForce
        ///
        /// \param force Amount and direction of the force 
        /// \param rel_pos Vector for the relative position on rigid body that the force applies on
        ///
        /// \return Reference to self
        ///
        RigidBody& applyForce(const glm::vec3& force, const glm::vec3& rel_pos);

        /// \brief Applies an impulse to rigid bodies relative position
        ///
        /// \comm applyImpulse
        ///
        /// \param impulse Amount and direction of the impulse
        /// \param rel_pos Vector for the relative position on rigid body that the impulse applies on
        ///
        /// \return Reference to self
        ///
        RigidBody& applyImpulse(const glm::vec3& impulse, const glm::vec3& rel_pos);

        /// \brief Applies torque to the rigid body
        ///
        /// \comm applyTorque
        ///
        /// \param torque Amount and direction as vector of the applied torque
        ///
        /// \return Reference to self
        ///
        RigidBody& applyTorque(const glm::vec3& torque);

        /// \brief Applies torque impulse to the rigid body
        ///
        /// \comm applyTorqueImpulse
        ///
        /// \param torque Amount and direction as vector of the applied torque
        ///
        /// \return Reference to self
        ///
        RigidBody& applyTorqueImpulse(const glm::vec3& torque);

        /// \brief Sets linear velocity to the rigid body
        ///
        /// \comm setLinearVelocity
        ///
        /// \param linearVelocity Amount and direction of the linear velocity 
        ///
        /// \return Reference to self
        ///
        RigidBody& setLinearVelocity(const glm::vec3& linearVelocity);

        glm::vec3 getLinearVelocity() const;

        /// \brief Sets angular velocity to the rigid body
        ///
        /// \comm setAngularVelocity
        ///
        /// \param angularVelocity Amount and direction of the angular velocity
        ///
        /// \return Reference to self
        ///
        RigidBody& setAngularVelocity(const glm::vec3& angularVelocity);

        glm::vec3 getAngularVelocity() const;

        /// \brief Applies force to the rigid body's center 
        ///
        /// \comm applyCentralForce
        ///
        /// \param force Amount and direction of the applied force
        ///
        /// \return Reference to self
        ///
        RigidBody& applyCentralForce(const glm::vec3& force);

        /// \brief Applies impulse to the rigid body's center
        ///
        /// \comm applyCentralImpulse
        ///
        /// \param impulse Amount and direction of the applies impulse
        ///
        /// \return Reference to self
        ///
        RigidBody& applyCentralImpulse(const glm::vec3& impulse);

        /// \brief Clear all the forces affecting this body
        ///
        /// \comm clearForces
        ///
        /// \return Reference to self
        ///
        RigidBody& clearForces();

        std::pair<glm::vec3, glm::vec3> getLocalBounds() const;

        /// \brief Sets the RigidBody position to be same as the objects' transform.
        ///
        /// \return Reference to self.
        ///
        RigidBody& synchronizeTransform();


        void setAllowSleep(const bool allow);

        bool getAllowSleep() const;
        

        /// \brief Returns a pointer to a joint on the RigidBody whence called from.
        ///
        /// User can give an ID of the the joint which to return. If left empty, returns a pointer to the first joint the RigidBody has.
        ///
        /// \return Returns a pointer to the joint. Nullptr if not found.
        ///
        template<typename T>
        T* getJoint(unsigned int id = 0);

        /// \brief Breaks a joint from the RigidBody whence called from.
        ///
        /// User can give an ID of the the joint which to break. If left empty, breaks the first joint the RigidBody has.
        ///
        /// \return Returns true if successful.
        ///
        template<typename T>
        bool breakJoint(RigidBody& other, unsigned int IDthis = 0, unsigned int IDother = 0);

        //T = joint
        //this = A
        //Rigidbody& = B
        /// \brief Creates a joint between this RigidBody and another RigidBody.
        ///
        /// \param T Type of the joint to create. Applicable joints are derived from Joint.
        ///
        /// \return Returns a reference to the RigidBody whence called from.
        ///
        template<typename T, typename ... Args>
        T& link(RigidBody&, Args&&...);

    protected:

        Message::Result receiveMessage(const Message& message) override;

        const Type m_type;           ///< The body type
        const float m_mass;          ///< The mass
        btRigidBody* m_rigidBody;    ///< Pointer to derived rigid body pointer for convenience

        std::unordered_set<std::shared_ptr<Joint>> m_joints;
        bool m_allowSleep;
    };
#include <Jopnal/Physics/Inl/RigidBody.inl>
}

#endif