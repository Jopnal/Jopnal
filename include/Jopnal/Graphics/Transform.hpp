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

#ifndef JOP_TRANSFORM_HPP
#define JOP_TRANSFORM_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/MathInclude.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Transform
    {
    public:
    
        /// \brief Default constructor
        ///
        /// Sets the identity matrix
        ///
        Transform();


        /// \brief Get the transformation matrix
        ///
        /// \return Reference to the internal matrix
        ///
        const glm::mat4& getMatrix() const;

        /// \brief Get the inverse transformation matrix
        ///
        /// \return Reference to the internal inverse matrix
        ///
        const glm::mat4& getInverseMatrix() const;


        /// \brief Set the rotation
        ///
        /// This version uses euler angles
        ///
        /// \param x The X angle
        /// \param y The Y angle
        /// \param z The Z angle
        ///
        /// \return Reference to this
        ///
        Transform& setRotation(const float x, const float y, const float z);

        /// \brief Set the rotation
        ///
        /// \param rotation Rotation vector in euler angles
        ///
        /// \return Reference to this
        ///
        Transform& setRotation(const glm::vec3& rotation);

        /// \brief Set the rotation
        ///
        /// \param rotation Quaternion with the rotation to set
        ///
        /// \return Reference to self
        ///
        Transform& setRotation(const glm::quat& rotation);

        /// \brief Get the rotation
        ///
        /// \return Quaternion with the rotation
        ///
        const glm::quat& getRotation() const;


        /// \brief Set the origin
        ///
        /// \param x The X component
        /// \param y The Y component
        /// \param z The Z component
        ///
        /// \return Reference to self
        ///
        Transform& setOrigin(const float x, const float y, const float z);

        /// \brief Set the origin
        ///
        /// \param origin Vector with the origin to set
        ///
        /// \return Reference to self
        ///
        Transform& setOrigin(const glm::vec3& origin);

        /// \brief Get the origin
        ///
        /// \return Vector with the origin
        ///
        const glm::vec3& getOrigin() const;


        /// \brief Set the scale
        ///
        /// 1.f means the original scale
        ///
        /// \param x The X component
        /// \param y The Y component
        /// \param z The Z component
        ///
        /// \return Reference to self
        ///
        Transform& setScale(const float x, const float y, const float z);

        /// \brief Set the scale
        ///
        /// \param scale Vector with the scale to set
        ///
        /// \return Reference to self
        ///
        Transform& setScale(const glm::vec3& scale);

        /// \brief Get the scale
        ///
        /// \return Vector with the scale
        ///
        const glm::vec3& getScale() const;


        /// \brief Set the position
        ///
        /// \param x The X component
        /// \param y The Y component
        /// \param z The Z component
        ///
        /// \return Reference to self
        ///
        Transform& setPosition(const float x, const float y, const float z);

        /// \brief Set the position
        ///
        /// \param position Vector with the position to set
        ///
        /// \return Reference to self
        ///
        Transform& setPosition(const glm::vec3& position);

        /// \brief Get the position
        ///
        /// \return Vector with the position
        ///
        const glm::vec3& getPosition() const;


        /// \brief Set this transform to look at a certain point
        ///
        /// This function will override the position, scale and rotation until
        /// changed again.
        ///
        /// \param point The point to look at
        ///
        void lookAt(const glm::vec3& point);


        /// The identity matrix
        ///
        static const glm::mat4 IdentityMatrix;

		/// \brief Set the move
		///
		/// \param x The X conponent
		/// \param y The Y component
		/// \param z The Z component
		///
		/// \returns values in vec3
		///
		Transform& move(const float x, const float y, const float z);

		///
		/// \brief Get the move
		///
		/// This function will change the position, according given values
		///
		/// \return reference to self
		///
		Transform& move(const glm::vec3& move);



		/// \brief Set the rotate
		///
		/// \param x The X conponent
		/// \param y The Y component
		/// \param z The Z component
		///
		/// \returns values in vec3
		///
		Transform& rotate(const float x, const float y, const float z);
		///
		/// \brief Get the move
		///
		/// This function will rotate the graphic, according given values
		///
		/// \return reference to self
		///
		Transform& rotate(const glm::quat& rotate);
		///
		/// \brief Get the move
		///
		/// This function returns given rotation
		///
		/// \return new rotation values
		///
		Transform& rotate(const glm::vec3& rotation);
		


		/// \brief Set the scale
		///
		/// \param x The X conponent
		/// \param y The Y component
		/// \param z The Z component
		///
		/// \returns values in vec3
		///
		Transform& scale(const float x, const float y, const float z);
		///
		/// \brief Get the move
		///
		/// This function will change scale of the given graphics, according
		/// given values.
		///
		/// \return reference to self
		///
		Transform& scale(const glm::vec3& scale);
		


		/// \brief Set the moveOrigin
		///
		/// \param x The X conponent
		/// \param y The Y component
		/// \param z The Z component
		///
		/// \returns values in vec3
		///
		Transform& moveOrigin(const float x, const float y, const float z);
		///
		/// \brief Get the move
		///
		/// This function will change the origin, according given values
		///
		/// \return reference to self
		///
		Transform& moveOrigin(const glm::vec3& moveOrigin);




    protected:

        mutable glm::mat4 m_transform;
        mutable glm::mat4 m_invTransform;
        mutable glm::quat m_rotation;
        glm::vec3 m_origin;
        glm::vec3 m_scale;
        glm::vec3 m_position;
        mutable bool m_transformNeedUpdate;
        mutable bool m_invTransformNeedsUpdate;
        
    };
}

#endif

/// \class Transform
/// \ingroup graphics
///
/// #TODO Detailed description