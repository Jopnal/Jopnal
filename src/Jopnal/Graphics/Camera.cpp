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
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
    JOP_DERIVED_COMMAND_HANDLER(Component, Camera)

        JOP_BIND_MEMBER_COMMAND_NORETURN(&Camera::setProjectionMode, "setProjectionMode");
        JOP_BIND_MEMBER_COMMAND_NORETURN(&Camera::setClippingPlanes, "setClippingPlanes");
        JOP_BIND_MEMBER_COMMAND_NORETURN((Camera& (Camera::*)(const float, const float))&Camera::setSize, "setSize");
        JOP_BIND_MEMBER_COMMAND_NORETURN(&Camera::setAspectRatio, "setAspectRatio");
        JOP_BIND_MEMBER_COMMAND_NORETURN(&Camera::setFieldOfView, "setFieldOfView");
        JOP_BIND_MEMBER_COMMAND_NORETURN(&Camera::setViewport, "setViewport");

    JOP_END_COMMAND_HANDLER(Camera)
}

namespace jop
{

    Camera::Camera(Object& object, Renderer& renderer, const Projection mode)
        : Component                 (object, 0),
          m_renderTexture           (),
          m_projectionMatrix        (),
          m_viewPort                (glm::vec2(0.f), glm::vec2(1.f)),
          m_projData                ({{0.f, 0.f}}),
          m_clippingPlanes          (0.f, 0.f),
          m_rendererRef             (renderer),
          m_renderMask              (1),
          m_mode                    (mode),
          m_projectionNeedUpdate    (true)
    {
        typedef SettingManager SM;

        if (mode == Projection::Orthographic)
        {
            setClippingPlanes(SM::get<float>("engine@Graphics|DefaultOrthographicCamera|fClipNear", -1.f), SM::get<float>("engine@Graphics|DefaultOrthographicCamera|fClipFar", 1.f));
            setSize(Engine::getMainWindow().getSize());
        }
        else
        {
            setClippingPlanes(SM::get<float>("engine@Graphics|DefaultPerspectiveCamera|fClipNear", 1.f), SM::get<float>("engine@Graphics|DefaultPerspectiveCamera|fClipFar", 9999999.f));
            setFieldOfView(SettingManager::get<float>("engine@Graphics|DefaultPerspectiveCamera|fFovYRad", glm::radians(55.f)));
            setSize(Engine::getMainRenderTarget().getSize());
        }

        renderer.bind(*this);
    }

    Camera::Camera(const Camera& other, Object& newObj)
        : Component                 (other, newObj),
          m_projectionMatrix        (other.m_projectionMatrix),
          m_renderTexture           (),
          m_viewPort                (other.m_viewPort),
          m_projData                (other.m_projData),
          m_clippingPlanes          (other.m_clippingPlanes),
          m_rendererRef             (other.m_rendererRef),
          m_renderMask              (other.m_renderMask),
          m_mode                    (other.m_mode),
          m_projectionNeedUpdate    (other.m_projectionNeedUpdate)
    {
        m_rendererRef.bind(*this);
    }
    
    Camera::~Camera()
    {
        m_rendererRef.unbind(*this);
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
        return getObject()->getInverseTransform().getMatrix();
    }

    //////////////////////////////////////////////

    Camera& Camera::setRenderMask(const uint32 mask)
    {
        m_renderMask = mask;
        return *this;
    }

    //////////////////////////////////////////////

    uint32 Camera::getRenderMask() const
    {
        return m_renderMask;
    }

    //////////////////////////////////////////////

    Camera& Camera::setProjectionMode(const Projection mode)
    {
        m_mode = mode;
        m_projectionNeedUpdate = true;

        return *this;
    }

    //////////////////////////////////////////////

    Camera::Projection Camera::getProjectionMode() const
    {
        return m_mode;
    }

    //////////////////////////////////////////////

    Camera& Camera::setClippingPlanes(const float clipNear, const float clipFar)
    {
        m_clippingPlanes = ClippingPlanes(clipNear, clipFar);
        m_projectionNeedUpdate = true;

        return *this;
    }

    //////////////////////////////////////////////

    const Camera::ClippingPlanes& Camera::getClippingPlanes() const
    {
        return m_clippingPlanes;
    }

    //////////////////////////////////////////////

    Camera& Camera::setSize(const glm::vec2& size)
    {
        return setSize(size.x, size.y);
    }

    //////////////////////////////////////////////

    Camera& Camera::setSize(const float x, const float y)
    {
        if (m_mode == Projection::Orthographic)
        {
            m_projData.orthographic.sizeX = x;
            m_projData.orthographic.sizeY = y;
            m_projectionNeedUpdate = true;
        }
        else
            setAspectRatio(x / y);

        return *this;
    }

    //////////////////////////////////////////////

    glm::vec2 Camera::getSize() const
    {
        return glm::vec2(m_projData.orthographic.sizeX, m_projData.orthographic.sizeY);
    }

    //////////////////////////////////////////////

    Camera& Camera::setAspectRatio(const float ratio)
    {
        m_projData.perspective.aspectRatio = ratio;
        m_projectionNeedUpdate = true;

        return *this;
    }

    //////////////////////////////////////////////

    float Camera::getAspectRatio() const
    {
        return m_projData.perspective.aspectRatio;
    }

    //////////////////////////////////////////////

    Camera& Camera::setFieldOfView(const float fovY)
    {
        m_projData.perspective.fov = fovY;
        m_projectionNeedUpdate = true;

        return *this;
    }

    //////////////////////////////////////////////

    float Camera::getFieldOfView() const
    {
        return m_projData.perspective.fov;
    }

    //////////////////////////////////////////////

    Camera& Camera::setViewport(const glm::vec2& start, const glm::vec2& size)
    {
        m_viewPort = std::make_pair(start, size);
        return *this;
    }

    //////////////////////////////////////////////

    const Camera::ViewPort& Camera::getViewport() const
    {
        return m_viewPort;
    }

    //////////////////////////////////////////////

    void Camera::applyViewport(const RenderTarget& mainTarget) const
    {
        const glm::uvec2 mainSize = m_renderTexture.isValid() ? m_renderTexture.getSize() : mainTarget.getSize();

        const auto p = glm::ivec2(m_viewPort.first * glm::vec2(mainSize));
        const auto s = glm::ivec2(m_viewPort.second * glm::vec2(mainSize)) - p;

        glCheck(gl::Viewport(p.x, p.y, s.x, s.y));
    }

    //////////////////////////////////////////////

    RenderTexture& Camera::getRenderTexture()
    {
        return m_renderTexture;
    }

    //////////////////////////////////////////////

    const RenderTexture& Camera::getRenderTexture() const
    {
        return m_renderTexture;
    }

    //////////////////////////////////////////////

    glm::vec3 Camera::getPickRay(const glm::vec2& mouseCoords, const RenderTarget& target) const
    {
        return glm::normalize(glm::unProject
               (
                   glm::vec3(glm::vec2(mouseCoords.x, target.getSize().y - mouseCoords.y), 1.f),
                   getViewMatrix(),
                   getProjectionMatrix(),
                   glm::vec4(glm::vec2(0.f), glm::vec2(target.getSize()))
               ));
    }
}