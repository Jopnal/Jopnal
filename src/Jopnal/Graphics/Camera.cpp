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
    Camera::Camera(Object& object, const Projection mode)
        : Component                 (object, "Camera"),
          m_projectionMatrix        (),
          m_projData                ({{0.f, 0.f}}),
          m_clippingPlanes          (0.f, 0.f),
          m_mode                    (mode),
          m_projectionNeedUpdate    (true)
    {
        const float x = static_cast<const float>(SettingManager::getUint("uDefaultWindowSizeX", 1024));
        const float y = static_cast<const float>(SettingManager::getUint("uDefaultWindowSizeY", 600));

        if (mode == Projection::Orthographic)
        {
            setID("OrthoCamera");
            setClippingPlanes(SettingManager::getFloat("fOrthoCameraClipNear", -1.f), SettingManager::getFloat("fOrthoCameraClipFar", 1.f));
            setSize(x, y);
        }
        else
        {
            setID("PerspCamera");
            setClippingPlanes(SettingManager::getFloat("fPerspCameraClipNear", 1.f), SettingManager::getFloat("fPerspCameraClipFar", FLT_MAX - 1.f));
            setFieldOfView(SettingManager::getFloat("fPerspCameraFovY", 80.f));
            setSize(x, y);
        }
    }

    Camera::Camera(const Camera& other)
        : Component                 (other),
          m_projectionMatrix        (other.m_projectionMatrix),
          m_projData                (other.m_projData),
          m_clippingPlanes          (other.m_clippingPlanes),
          m_mode                    (other.m_mode),
          m_projectionNeedUpdate    (other.m_projectionNeedUpdate)
    {}

    Camera::~Camera()
    {}

    //////////////////////////////////////////////

    Camera* Camera::clone() const
    {
        auto ptr = std::make_unique<Camera>(*this);
        return ptr.release();
    }

    //////////////////////////////////////////////

    const glm::mat4& Camera::getProjectionMatrix() const
    {
        if (m_projectionNeedUpdate)
        {
            if (m_mode == Projection::Orthographic)
            {
                const auto& o = m_projData.orthographic;
                m_projectionMatrix = glm::ortho(-o.sizeX * 0.5f, o.sizeX * 0.5f, -o.sizeY * 0.5f, o.sizeY * 0.5f, m_clippingPlanes.first, m_clippingPlanes.second);
            }
            else
            {
                const auto& p = m_projData.perspective;
                m_projectionMatrix = glm::perspective(p.fov, p.aspectRatio, m_clippingPlanes.first, m_clippingPlanes.second);
            }

            m_projectionNeedUpdate = false;
        }

        return m_projectionMatrix;
    }

    //////////////////////////////////////////////

    const glm::mat4& Camera::getViewMatrix() const
    {
        return getObject().getInverseMatrix();
    }

    //////////////////////////////////////////////

    void Camera::setProjectionMode(const Projection mode)
    {
        m_mode = mode;
        m_projectionNeedUpdate = true;
    }

    //////////////////////////////////////////////

    Camera::Projection Camera::getProjectionMode() const
    {
        return m_mode;
    }

    //////////////////////////////////////////////

    void Camera::setClippingPlanes(const float clipNear, const float clipFar)
    {
        m_clippingPlanes = ClippingPlanes(clipNear, clipFar);
        m_projectionNeedUpdate = true;
    }

    //////////////////////////////////////////////

    const Camera::ClippingPlanes& Camera::getClippingPlanes() const
    {
        return m_clippingPlanes;
    }

    //////////////////////////////////////////////

    void Camera::setSize(const glm::vec2& size)
    {
        setSize(size.x, size.y);
    }

    //////////////////////////////////////////////

    void Camera::setSize(const float x, const float y)
    {
        if (m_mode == Projection::Orthographic)
        {
            m_projData.orthographic.sizeX = x;
            m_projData.orthographic.sizeY = y;
            m_projectionNeedUpdate = true;
        }
        else
            setAspectRatio(x / y);
    }

    //////////////////////////////////////////////

    glm::vec2 Camera::getSize() const
    {
        return glm::vec2(m_projData.orthographic.sizeX, m_projData.orthographic.sizeY);
    }

    //////////////////////////////////////////////

    void Camera::setAspectRatio(const float ratio)
    {
        m_projData.perspective.aspectRatio = ratio;
        m_projectionNeedUpdate = true;
    }

    //////////////////////////////////////////////

    float Camera::getAspectRatio() const
    {
        return m_projData.perspective.aspectRatio;
    }

    //////////////////////////////////////////////

    void Camera::setFieldOfView(const float fovY)
    {
        m_projData.perspective.fov = fovY;
        m_projectionNeedUpdate = true;
    }

    //////////////////////////////////////////////

    float Camera::getFieldOfView() const
    {
        return m_projData.perspective.fov;
    }
}