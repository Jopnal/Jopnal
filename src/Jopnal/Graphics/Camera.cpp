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

        JOP_BIND_MEMBER_COMMAND(&Camera::setProjectionMode, "setProjectionMode");
        JOP_BIND_MEMBER_COMMAND(&Camera::setClippingPlanes, "setClippingPlanes");
        JOP_BIND_MEMBER_COMMAND((void (Camera::*)(const float, const float))&Camera::setSize, "setSize");
        JOP_BIND_MEMBER_COMMAND(&Camera::setAspectRatio, "setAspectRatio");
        JOP_BIND_MEMBER_COMMAND(&Camera::setFieldOfView, "setFieldOfView");

    JOP_END_COMMAND_HANDLER(Camera)

    JOP_REGISTER_LOADABLE(jop, Camera)[](Object& obj, const Scene& scene, const json::Value& val) -> bool
    {
        Camera::Projection proj = val.HasMember("projection") && val["projection"].IsUint() ?
                                  static_cast<Camera::Projection>(std::max(1u, val["projection"].GetUint())) :
                                  Camera::Projection::Perspective;

        auto& cam = obj.createComponent<Camera>(proj);

        if (val.HasMember("clipping") && val["clipping"].IsArray() && val["clipping"].Size() >= 2)
        {
            auto& clip = val["clipping"];

            if (clip[0u].IsDouble() && clip[1u].IsDouble())
                cam.setClippingPlanes(static_cast<float>(clip[0u].GetDouble()), static_cast<float>(clip[1u].GetDouble()));
            else
                JOP_DEBUG_WARNING("Incorrect clipping values specified for camera component, while loading object \"" << obj.getID() << "\"");
        }

        if (val.HasMember("projdata") && val["projdata"].IsArray() && val["projdata"].Size() >= 2)
        {
            auto& pd = val["projdata"];

            if (!pd[0u].IsDouble() || !pd[1u].IsDouble())
            {
                JOP_DEBUG_WARNING("Incorrect projection data values specified for camera component, while loading object \"" << obj.getID() << "\"");
                return true;
            }

            if (proj == Camera::Projection::Perspective)
            {
                cam.setFieldOfView(static_cast<float>(pd[0u].GetDouble()));
                cam.setAspectRatio(static_cast<float>(pd[1u].GetDouble()));
            }
            else
                cam.setSize(static_cast<float>(pd[0u].GetDouble()), static_cast<float>(pd[1u].GetDouble()));
        }

        return Drawable::loadStateBase(cam, scene, val);
    }
    JOP_END_LOADABLE_REGISTRATION(Camera)

    JOP_REGISTER_SAVEABLE(jop, Camera)[](const Component& comp, json::Value& val, json::Value::AllocatorType& alloc) -> bool
    {
        auto& cam = static_cast<const Camera&>(comp);

        val.AddMember(json::StringRef("projection"), static_cast<unsigned int>(cam.getProjectionMode()), alloc);

        auto& clip = val.AddMember(json::StringRef("clipping"), json::kArrayType, alloc)["clipping"];
        clip.PushBack(cam.getClippingPlanes().first, alloc)
            .PushBack(cam.getClippingPlanes().second, alloc);

        auto& pd = val.AddMember(json::StringRef("projdata"), json::kArrayType, alloc);

        if (cam.getProjectionMode() == Camera::Projection::Perspective)
        {
            auto size = cam.getSize();
            pd.PushBack(size.x, alloc)
              .PushBack(size.y, alloc);
        }
        else
        {
            pd.PushBack(cam.getFieldOfView(), alloc)
              .PushBack(cam.getAspectRatio(), alloc);
        }

        return Drawable::saveStateBase(cam, val, alloc);
    }
    JOP_END_SAVEABLE_REGISTRATION(Camera)
}

namespace jop
{

    Camera::Camera(Object& object, const Projection mode)
        : Drawable                  (object, "Camera"),
          m_projectionMatrix        (),
          m_projData                ({{0.f, 0.f}}),
          m_clippingPlanes          (0.f, 0.f),
          m_mode                    (mode),
          m_projectionNeedUpdate    (true)
    {
        const float x = static_cast<const float>(SettingManager::getUint("uDefaultWindowSizeX", 1280));
        const float y = static_cast<const float>(SettingManager::getUint("uDefaultWindowSizeY", 720));

        if (mode == Projection::Orthographic)
        {
            setID("OrthoCamera");
            setClippingPlanes(SettingManager::getFloat("fOrthoCameraClipNear", -1.f), SettingManager::getFloat("fOrthoCameraClipFar", 1.f));
            setSize(x, y);
        }
        else
        {
            setID("PerspCamera");
            setClippingPlanes(SettingManager::getFloat("fPerspCameraClipNear", 1.f), SettingManager::getFloat("fPerspCameraClipFar", 9999999.f));
            setFieldOfView(SettingManager::getFloat("fPerspCameraFovY", 80.f));
            setSize(x, y);
        }
    }

    Camera::Camera(const Camera& other)
        : Drawable                  (other),
          m_projectionMatrix        (other.m_projectionMatrix),
          m_projData                (other.m_projData),
          m_clippingPlanes          (other.m_clippingPlanes),
          m_mode                    (other.m_mode),
          m_projectionNeedUpdate    (other.m_projectionNeedUpdate)
    {}

    //////////////////////////////////////////////

    Camera* Camera::clone() const
    {
        auto ptr = std::make_unique<Camera>(*this);
        return ptr.release();
    }

    //////////////////////////////////////////////

    void Camera::draw(const Camera&)
    {}

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

    //////////////////////////////////////////////

    const Camera& Camera::getDefault()
    {
        static Object obj("Default Camera Object");
        static const Camera& cam = obj.createComponent<Camera>(static_cast<const Projection>(std::min(1u, SettingManager::getUint("uDefaultCameraMode", 1))));

        return cam;
    }
}