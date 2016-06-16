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

// Headers
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

	#include <Jopnal/Graphics/Transform.hpp>

    #include <glm/gtc/matrix_transform.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    Transform::Transform()
        : m_matrix(IdentityMatrix)
    {}

    Transform::Transform(const glm::mat4& matrix)
        : m_matrix(matrix)
    {}

    //////////////////////////////////////////////

    glm::mat4& Transform::getMatrix()
    {
        return m_matrix;
    }

    const glm::mat4& Transform::getMatrix() const
    {
        return m_matrix;
    }

    //////////////////////////////////////////////

    void Transform::rotate(const glm::quat& rotation)
    {
        m_matrix *= glm::mat4_cast(rotation);
    }

    //////////////////////////////////////////////

    void Transform::scale(const glm::vec3& scale)
    {
        m_matrix = glm::scale(m_matrix, scale);
    }

    //////////////////////////////////////////////

    void Transform::translate(const glm::vec3& translation)
    {
        m_matrix = glm::translate(m_matrix, translation);
    }

    //////////////////////////////////////////////

    Transform& Transform::operator *=(const Transform& right)
    {
        m_matrix *= right.getMatrix();
        return *this;
    }

    //////////////////////////////////////////////

    Transform Transform::operator *(const Transform& right) const
    {
        Transform newTransform(m_matrix);
        newTransform *= right;

        return newTransform;
    }

    //////////////////////////////////////////////

    const glm::mat4 Transform::IdentityMatrix
    (
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f
    );

    const glm::vec3 Transform::Front(0.f, 0.f, -1.f);

    const glm::vec3 Transform::Right(1.f, 0.f, 0.f);

    const glm::vec3 Transform::Up(0.f, 1.f, 0.f);
}