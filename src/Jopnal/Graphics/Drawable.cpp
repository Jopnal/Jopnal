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

    #include <Jopnal/Graphics/Drawable.hpp>

    #include <Jopnal/Core/SettingManager.hpp>
    #include <Jopnal/Core/Serializer.hpp>
    #include <Jopnal/Graphics/LightSource.hpp>
    #include <Jopnal/Graphics/Material.hpp>
    #include <Jopnal/Graphics/Mesh/Mesh.hpp>
    #include <Jopnal/Graphics/Renderer.hpp>
    #include <Jopnal/Graphics/ShaderProgram.hpp>
    #include <Jopnal/Graphics/ShaderAssembler.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>
    #include <Jopnal/Graphics/Culling/CullerComponent.hpp>
    #include <Jopnal/Utility/CommandHandler.hpp>
    #include <glm/gtc/type_ptr.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_COMMAND_HANDLER(Drawable)

        JOP_BIND_MEMBER_COMMAND(&Drawable::setModel, "setModel");
        JOP_BIND_MEMBER_COMMAND(&Drawable::setRenderGroup, "setRenderGroup");

    JOP_END_COMMAND_HANDLER(Drawable)
}

namespace jop
{
    Drawable::ProjectionInfo::ProjectionInfo(const glm::mat4& view, const glm::mat4& proj, const glm::vec3& camPos)
        : viewMatrix        (view),
          projectionMatrix  (proj),
          cameraPosition    (camPos)
    {}

    //////////////////////////////////////////////

    Drawable::Drawable(Object& object, Renderer& renderer, const bool cull)
        : Drawable(object, renderer, RenderPass::getDefaultType(), RenderPass::getDefaultWeight(), cull)
    {}

    Drawable::Drawable(Object& object, Renderer& renderer, const RenderPass::Pass pass, const uint32 weight, const bool cull)
        : Component                 (object, 0),
          detail::CullerComponent   (object, cull ? renderer.getCullingWorld() : nullptr, detail::CullerComponent::Type::Drawable),
          m_color                   (),
          m_mesh                    (),
          m_material                (),
          m_shader                  (),
          m_attributes              (0),
          m_rendererRef             (renderer),
          m_pass                    (pass),
          m_weight                  (weight),
          m_flags                   (ReceiveLights | ReceiveShadows | CastShadows | Reflected),
          m_renderGroup             (0)
    {
        setModel(Mesh::getDefault(), Material::getDefault());

        renderer.bind(this, pass, weight);

        if (cull)
            renderer.getCullingWorld()->bind(this);
    }

    Drawable::Drawable(const Drawable& other, Object& newObj)
        : Component                 (other, newObj),
          detail::CullerComponent   (other, newObj),
          m_color                   (other.m_color),
          m_mesh                    (other.m_mesh),
          m_material                (other.m_material),
          m_shader                  (other.m_shader),
          m_attributes              (other.m_attributes),
          m_rendererRef             (other.m_rendererRef),
          m_pass                    (other.m_pass),
          m_weight                  (other.m_weight),
          m_renderGroup             (other.m_renderGroup),
          m_flags                   (other.m_flags)
    {
        m_rendererRef.bind(this, m_pass, m_weight);

        if (isCulled())
            m_rendererRef.getCullingWorld()->bind(this);
    }

    Drawable::~Drawable()
    {
        m_rendererRef.unbind(this, m_pass, m_weight);

        if (isCulled())
            m_rendererRef.getCullingWorld()->unbind(this);
    }

    //////////////////////////////////////////////

    void Drawable::update(const float deltaTime)
    {
        
    }

    //////////////////////////////////////////////

    void Drawable::draw(const ProjectionInfo& proj, const LightContainer& lights) const
    {
        if (m_mesh.expired())
            return;

        auto& mesh = *getMesh();

        {
            auto& shdr = getShader();
            auto& modelMat = getObject()->getTransform().getMatrix();
            const auto VMMatrix = proj.viewMatrix * modelMat;

            shdr.setUniform("u_PVMMatrix", proj.projectionMatrix * VMMatrix);

            if (!mesh.hasVertexComponent(Mesh::Color))
            {
                // Default vertex color
                // Used if the mesh itself doesn't have colors
                glCheck(glVertexAttrib4fv(Mesh::VertexIndex::Color, &getColor().colors[0]));
            }

            if (getMaterial())
            {
                auto& mat = *getMaterial();
                if (mat.getAttributes() & Material::LightingAttribs)
                {
                    shdr.setUniform("u_VMMatrix", VMMatrix);
                    shdr.setUniform("u_NMatrix", glm::transpose(glm::inverse(glm::mat3(VMMatrix))));
                    lights.sendToShader(shdr, *this, proj.viewMatrix);
                }

                mat.sendToShader(shdr);
            }
            static const DynamicSetting<bool> validateSetting("engine@Debug|bValidateShaders", false);

            if (validateSetting.value && !shdr.validate())
                return;
        }

        mesh.draw();
    }

    //////////////////////////////////////////////

    Renderer& Drawable::getRendrer()
    {
        return m_rendererRef;
    }

    //////////////////////////////////////////////

    const Renderer& Drawable::getRenderer() const
    {
        return m_rendererRef;
    }

    //////////////////////////////////////////////

    Drawable& Drawable::setRenderGroup(const uint8 group)
    {
        m_renderGroup = std::min(static_cast<uint8>(31), group);
        return *this;
    }

    //////////////////////////////////////////////

    uint8 Drawable::getRenderGroup() const
    {
        return m_renderGroup;
    }

    //////////////////////////////////////////////

    Drawable& Drawable::setModel(const Mesh& mesh, const Material& material)
    {
        return setMesh(mesh).setMaterial(material);
    }

    //////////////////////////////////////////////

    Drawable& Drawable::setMesh(const Mesh& mesh)
    {
        m_mesh = static_ref_cast<const Mesh>(mesh.getReference());

        if (isCulled())
        {
            setCollisionShape(mesh.getCullingShape());
            updateWorldBounds();
        }

        return *this;
    }

    //////////////////////////////////////////////

    Drawable& Drawable::setMaterial(const Material& material)
    {
        m_material = static_ref_cast<const Material>(material.getReference());
        return *this;
    }

    //////////////////////////////////////////////

    const Mesh* Drawable::getMesh() const
    {
        return m_mesh.get();
    }

    //////////////////////////////////////////////

    const Material* Drawable::getMaterial() const
    {
        return m_material.get();
    }

    //////////////////////////////////////////////

    Drawable& Drawable::setColor(const Color& color)
    {
        m_color = color;
        return *this;
    }

    //////////////////////////////////////////////

    const Color& Drawable::getColor() const
    {
        return m_color;
    }

    //////////////////////////////////////////////

    const std::pair<glm::vec3, glm::vec3>& Drawable::getLocalBounds() const
    {
        if (getMesh())
            return getMesh()->getBounds();

        static const auto dummy = std::make_pair(glm::vec3(), glm::vec3());

        return dummy;
    }

    //////////////////////////////////////////////

    std::pair<glm::vec3, glm::vec3> Drawable::getGlobalBounds() const
    {
        if (getMesh())
        {
            auto bounds = getLocalBounds();
            getObject()->getTransform().transformBounds(bounds.first, bounds.second);

            return bounds;
        }

        return std::make_pair(glm::vec3(), glm::vec3());
    }

    //////////////////////////////////////////////

    Drawable& Drawable::setFlags(const uint32 flags)
    {
        m_flags = flags;
        return *this;
    }

    //////////////////////////////////////////////

    bool Drawable::hasFlag(const uint32 flag) const
    {
        return (m_flags & flag) != 0;
    }

    //////////////////////////////////////////////

    uint64 Drawable::getAttributes() const
    {
        return m_attributes;
    }

    //////////////////////////////////////////////

    bool Drawable::hasAlpha() const
    {
        return getColor().alpha < 1.0 || (getMaterial() && getMaterial()->hasAlpha());
    }

    //////////////////////////////////////////////

    bool Drawable::isCulled() const
    {
        return m_worldRef != nullptr;
    }

    //////////////////////////////////////////////

    Message::Result Drawable::receiveMessage(const Message& message)
    {
        if (JOP_EXECUTE_COMMAND(Drawable, message.getString(), this) == Message::Result::Escape)
            return Message::Result::Escape;

        return Component::receiveMessage(message);
    }

    //////////////////////////////////////////////

    ShaderProgram& Drawable::getShader() const
    {
        return m_shader.expired() ? (getMaterial() ? getMaterial()->getShader() : ShaderProgram::getDefault()) : *m_shader;
    }

    //////////////////////////////////////////////

    void Drawable::setOverrideShader(ShaderProgram& shader)
    {
        m_shader = static_ref_cast<ShaderProgram>(shader.getReference());
    }

    //////////////////////////////////////////////

    void Drawable::removeOverrideShader()
    {
        m_shader.reset();
    }

    //////////////////////////////////////////////

    bool Drawable::hasOverrideShader() const
    {
        return !m_shader.expired();
    }

    //////////////////////////////////////////////

    std::string Drawable::getShaderPreprocessorDef(const uint64 attributes)
    {
        std::string str;

        if (attributes & Attribute::__SkyBox)
            str += "#define JDRW_SKYBOX\n";

        if (attributes & Attribute::__SkySphere)
            str += "#define JDRW_SKYSPHERE\n";

        return str;
    }
}