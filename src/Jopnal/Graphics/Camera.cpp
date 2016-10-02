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

    #include <Jopnal/Graphics/Camera.hpp>

    #include <Jopnal/Core/Engine.hpp>
    #include <Jopnal/Core/SettingManager.hpp>
    #include <Jopnal/Core/Object.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/Renderer.hpp>
    #include <Jopnal/Utility/CommandHandler.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>
    #include <Jopnal/Graphics/Culling/CullerComponent.hpp>
    #include <Jopnal/Physics/Shape/FrustumShape.hpp>
    #include <glm/gtc/matrix_transform.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_COMMAND_HANDLER(Camera)

        JOP_BIND_MEMBER_COMMAND(&Camera::setProjectionMode, "setProjectionMode");
        JOP_BIND_MEMBER_COMMAND(&Camera::setClippingPlanes, "setClippingPlanes");
        JOP_BIND_MEMBER_COMMAND((Camera& (Camera::*)(const float, const float))&Camera::setSize, "setSize");
        JOP_BIND_MEMBER_COMMAND(&Camera::setAspectRatio, "setAspectRatio");
        JOP_BIND_MEMBER_COMMAND(&Camera::setFieldOfView, "setFieldOfView");
        JOP_BIND_MEMBER_COMMAND(&Camera::setViewport, "setViewport");

    JOP_END_COMMAND_HANDLER(Camera)
}

namespace jop
{
    Camera::Camera(Object& object, Renderer& renderer, const Projection mode)
        : Component                 (object, 0),
          detail::CullerComponent   (object, cullingEnabled() ? renderer.getCullingWorld() : nullptr, Type::Camera),
          m_projectionMatrix        (),
          m_renderTexture           (),
          m_drawables               (),
          m_lights                  (),
          m_shape                   (),
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
            m_clippingPlanes = ClippingPlanes(SM::get<float>("engine@Graphics|DefaultOrthographicCamera|fClipNear", -1.f), SM::get<float>("engine@Graphics|DefaultOrthographicCamera|fClipFar", 1.f));

            const glm::vec2 size(Engine::getMainWindow().getSize());
            setSize(2.f, size.y / size.x * 2.f);
        }
        else
        {
            m_clippingPlanes = ClippingPlanes(SM::get<float>("engine@Graphics|DefaultPerspectiveCamera|fClipNear", 1.f), SM::get<float>("engine@Graphics|DefaultPerspectiveCamera|fClipFar", 1000.f));
            m_projData.perspective.fov = SettingManager::get<float>("engine@Graphics|DefaultPerspectiveCamera|fFoV", glm::radians(55.f));
            setSize(Engine::getMainRenderTarget().getSize());
        }

        if (cullingEnabled())
            renderer.getCullingWorld()->bind(this);

        renderer.bind(this);
    }

    Camera::Camera(const Camera& other, Object& newObj)
        : Component                 (other, newObj),
          detail::CullerComponent   (other, newObj),
          m_projectionMatrix        (other.m_projectionMatrix),
          m_renderTexture           (),
          m_drawables               (),
          m_lights                  (),
          m_shape                   (),
          m_viewPort                (other.m_viewPort),
          m_projData                (other.m_projData),
          m_clippingPlanes          (other.m_clippingPlanes),
          m_rendererRef             (other.m_rendererRef),
          m_renderMask              (other.m_renderMask),
          m_mode                    (other.m_mode),
          m_projectionNeedUpdate    (other.m_projectionNeedUpdate)
    {
        if (cullingEnabled())
        {
            updateShape();
            m_rendererRef.getCullingWorld()->bind(this);
        }

        m_rendererRef.bind(this);
    }
    
    Camera::~Camera()
    {
        if (cullingEnabled())
            m_rendererRef.getCullingWorld()->unbind(this);

        m_rendererRef.unbind(this);
    }

    //////////////////////////////////////////////

    void Camera::update()
    {
        if (cullingEnabled())
        {
            m_drawables.clear();

            auto& w = m_worldRef->m_worldData->world;
            auto  b = static_cast<btPairCachingGhostObject*>(m_body.get());

            w->getDispatcher()->dispatchAllCollisionPairs(b->getOverlappingPairCache(), w->getDispatchInfo(), w->getDispatcher());

            auto& pairArray = b->getOverlappingPairCache()->getOverlappingPairArray();
            btManifoldArray manifoldArray;
            
            for (int i = 0; i < pairArray.size(); ++i)
            {
                manifoldArray.clear();
            
                auto& pair = pairArray[i];
            
                if (pair.m_algorithm)
                    pair.m_algorithm->getAllContactManifolds(manifoldArray);
                
                for (int m = 0; m < manifoldArray.size(); ++m)
                {
                    auto manifold = manifoldArray[m];
                
                    for (int cp = 0; cp < manifold->getNumContacts(); ++cp)
                    {
                        if (manifold->getContactPoint(cp).getDistance() < 0.f)
                        {
                            auto cullComp = static_cast<const CullerComponent*>((manifold->getBody0() == m_body.get() ? manifold->getBody1() : manifold->getBody0())->getUserPointer());
                
                            switch (cullComp->getType())
                            {
                                case Type::Drawable:
                                    m_drawables.insert(static_cast<const Drawable*>(cullComp));
                                    goto NextObject;
                            }
                        }
                    }
                
                    {
                        NextObject:
                        continue;
                    }
                }
            }
        }
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

        return updateShape();
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

        return updateShape();
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

        return updateShape();
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

        return updateShape();
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

        return updateShape();
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

        glCheck(glViewport(p.x, p.y, s.x, s.y));
        glCheck(glScissor(p.x, p.y, s.x, s.y));
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

    //////////////////////////////////////////////

    bool Camera::inView(const Drawable& drawable) const
    {
        return !detail::CullerComponent::cullingEnabled() || !drawable.isCulled() || m_drawables.find(&drawable) != m_drawables.end();
    }

    //////////////////////////////////////////////

    Message::Result Camera::receiveMessage(const Message& message)
    {
        if (JOP_EXECUTE_COMMAND(Camera, message.getString(), this) == Message::Result::Escape)
            return Message::Result::Escape;

        return Component::receiveMessage(message); 
    }

    //////////////////////////////////////////////

    Camera& Camera::updateShape()
    {
        if (cullingEnabled())
        {
            if (!m_shape)
            {
                m_shape = std::make_unique<FrustumShape>("");
                m_shape->setMargin(0.f);
            }

            if (getProjectionMode() == Projection::Perspective)
                m_shape->load(getClippingPlanes(), getAspectRatio(), glm::quat());
            else
                m_shape->load(getClippingPlanes(), getSize(), glm::quat());

            setCollisionShape(*m_shape);
            updateWorldBounds();
        }

        return *this;
    }

    //////////////////////////////////////////////

    bool Camera::shouldCollide(const CullerComponent& other) const
    {
        switch (other.getType())
        {
            case Type::Drawable:
                return (getRenderMask() & (1 << static_cast<const Drawable&>(other).getRenderGroup())) > 0;
        }

        return false;
    }
}