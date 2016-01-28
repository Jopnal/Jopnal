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

// Headers
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
    Transform::Transform()
        : m_transform               (IdentityMatrix),
          m_invTransform            (IdentityMatrix),
          m_rotation                (1.f, 0.f, 0.f, 0.f),
          m_origin                  (0.f, 0.f, 0.f),
          m_scale                   (1.f, 1.f, 1.f),
          m_position                (0.f, 0.f, 0.f),
          m_transformNeedUpdate     (false),
          m_invTransformNeedsUpdate (false)
    {}

    //////////////////////////////////////////////

    const glm::mat4& Transform::getMatrix() const
    {
        if (m_transformNeedUpdate)
        {
            glm::translate(m_transform, m_position);
            m_transform *= m_rotation.operator glm::tmat4x4<float, glm::highp>();
            glm::scale(m_transform, m_scale);
            glm::translate(m_transform, m_origin);

            m_transformNeedUpdate = false;
        }

        return m_transform;
    }

    //////////////////////////////////////////////

    const glm::mat4& Transform::getInverseMatrix() const
    {
        if (m_invTransformNeedsUpdate)
        {
            m_invTransform = glm::inverse(m_transform);
            m_invTransformNeedsUpdate = false;
        }

        return m_invTransform;
    }

    //////////////////////////////////////////////

    Transform& Transform::setRotation(const float x, const float y, const float z)
    {
        return setRotation(glm::vec3(x, y, z));
    }

    //////////////////////////////////////////////

    Transform& Transform::setRotation(const glm::vec3& rotation)
    {
        return setRotation(glm::quat(rotation));
    }

    //////////////////////////////////////////////

    Transform& Transform::setRotation(const glm::quat& rotation)
    {
        m_rotation = rotation;
        m_transformNeedUpdate = true;
        m_invTransformNeedsUpdate = true;

        return *this;
    }

    //////////////////////////////////////////////

    const glm::quat& Transform::getRotation() const
    {
        return m_rotation;
    }

    //////////////////////////////////////////////

    Transform& Transform::setOrigin(const float x, const float y, const float z)
    {
        m_origin = glm::vec3(x, y, z);
        m_transformNeedUpdate = true;
        m_invTransformNeedsUpdate = true;

        return *this;
    }

    //////////////////////////////////////////////

    Transform& Transform::setOrigin(const glm::vec3& origin)
    {
        return setOrigin(origin.x, origin.y, origin.z);
    }

    //////////////////////////////////////////////

    const glm::vec3& Transform::getOrigin() const
    {
        return m_origin;
    }

    //////////////////////////////////////////////

    Transform& Transform::setScale(const float x, const float y, const float z)
    {
        m_scale = glm::vec3(x, y, z);
        m_transformNeedUpdate = true;
        m_invTransformNeedsUpdate = true;

        return *this;
    }

    //////////////////////////////////////////////

    Transform& Transform::setScale(const glm::vec3& scale)
    {
        return setScale(scale.x, scale.y, scale.z);
    }

    //////////////////////////////////////////////

    const glm::vec3& Transform::getScale() const
    {
        return m_scale;
    }

    //////////////////////////////////////////////

    Transform& Transform::setPosition(const float x, const float y, const float z)
    {
        m_position = glm::vec3(x, y, z);
        m_transformNeedUpdate = true;
        m_invTransformNeedsUpdate = true;

        return *this;
    }

    //////////////////////////////////////////////

    Transform& Transform::setPosition(const glm::vec3& origin)
    {
        return setPosition(origin.x, origin.y, origin.z);
    }

    //////////////////////////////////////////////

    const glm::vec3& Transform::getPosition() const
    {
        return m_position;
    }

    //////////////////////////////////////////////

    void Transform::lookAt(const glm::vec3& point)
    {
        static const glm::vec3 upVec(0.f, 1.f, 0.f);

        m_invTransform = glm::lookAt(m_position, point, upVec);
        m_transform = glm::inverse(m_invTransform);
        m_transformNeedUpdate = false;
        m_invTransformNeedsUpdate = false;
    }

    //////////////////////////////////////////////

    const glm::mat4 Transform::IdentityMatrix
    (
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f
    );
}