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
#include <glm/mat4x4.hpp>
#include <glm/gtx/quaternion.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Transform
    {
    public:

        /// Transformation variable helper structure
        ///
        struct JOP_API Variables
        {
            /// \brief Default constructor
            ///
            Variables() = default;

            /// \brief Overloaded constructor
            ///
            /// \param posX The X position
            /// \param posY The Y position
            /// \param posZ The Z position
            /// \param scaX The X scale
            /// \param scaY The Y scale
            /// \param scaZ The Z scale
            /// \param qW The rotation W component
            /// \param qX The rotation X component
            /// \param qY The rotation Y component
            /// \param qZ The rotation Z component
            ///
            Variables(const float posX, const float posY, const float posZ,
                      const float scaX, const float scaY, const float scaZ,
                      const float qW, const float qX, const float qY, const float qZ);

            /// \brief Overloaded constructor
            ///
            /// \param pos Position
            /// \param sca Scale
            /// \param rot Rotation
            ///
            Variables(const glm::vec3& pos, const glm::vec3& sca, const glm::quat& rot);

            glm::vec3 position; ///< Position
            glm::vec3 scale;    ///< Scale
            glm::quat rotation; ///< Rotation
        };

    public:
    
        /// \brief Default constructor
        ///
        /// Sets the identity matrix
        ///
        Transform();

        /// \brief Constructor
        ///
        /// \param matrix The initial matrix
        ///
        explicit Transform(const glm::mat4& matrix);
        
        /// \brief Get the transformation matrix
        ///
        /// \return Reference to the internal matrix
        ///
        glm::mat4& getMatrix();

        /// \copydoc getMatrix()
        ///
        const glm::mat4& getMatrix() const;

        /// \brief Rotate this transform
        ///
        /// \param rotation The rotation offset
        ///
        void rotate(const glm::quat& rotation);
        
        /// \brief Scale this transform
        ///
        /// \param scale Scale multiplier
        ///
        void scale(const glm::vec3& scale);

        /// \brief Translate this transform
        ///
        /// \param translation Translation offset
        ///
        void translate(const glm::vec3& translation);

        /// \brief Transform a bounding box
        ///
        /// \param min The minimum coordinates
        /// \param max The maximum coordinates
        ///
        void transformBounds(glm::vec3& min, glm::vec3& max) const;

        /// \brief Multiply this transform by another
        ///
        /// \param right The right transform
        ///
        /// \return Reference to self
        ///
        Transform& operator *=(const Transform& right);

        /// \brief Multiply this transform by another
        ///
        /// \param right The right Transform
        ///
        /// \return The combined transform
        ///
        Transform operator *(const Transform& right) const;

        static const glm::mat4 IdentityMatrix;  ///< The identity matrix
        static const glm::vec3 Front;           ///< The default normalized front vector
        static const glm::vec3 Right;           ///< The default normalized right vector
        static const glm::vec3 Up;              ///< The default normalized up vector

    protected:

        glm::mat4 m_matrix; ///< The transformation matrix
    };
}

/// \class jop::Transform
/// \ingroup graphics

#endif