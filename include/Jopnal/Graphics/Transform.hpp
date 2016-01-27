// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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