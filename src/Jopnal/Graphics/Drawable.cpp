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

    #include <Jopnal/Core/Serializer.hpp>
    #include <Jopnal/Graphics/LightSource.hpp>
    #include <Jopnal/Graphics/Material.hpp>
    #include <Jopnal/Graphics/Mesh/Mesh.hpp>
    #include <Jopnal/Graphics/Model.hpp>
    #include <Jopnal/Graphics/Renderer.hpp>
    #include <Jopnal/Graphics/ShaderProgram.hpp>
    #include <Jopnal/Graphics/ShaderAssembler.hpp>
    #include <Jopnal/Utility/CommandHandler.hpp>

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
    Drawable::Drawable(Object& object, Renderer& renderer, const RenderPass::Pass pass)
        : Component     (object, 0),
          m_model       (Mesh::getDefault(), Material::getDefault()),
          m_shader      (),
          m_rendererRef (renderer),
          m_pass        (pass),
          m_flags       (ReceiveLights | ReceiveShadows | CastShadows | Reflected),
          m_renderGroup (0)  
    {
        renderer.bind(this, pass);
    }

    Drawable::Drawable(Object& object, RenderPass& pass)
        : Component     (object, 0),
          m_model       (Mesh::getDefault(), Material::getDefault()),
          m_shader      (),
          m_rendererRef (pass.getRenderer()),
          m_pass        (pass.whichPass()),
          m_flags       (ReceiveLights | ReceiveShadows | CastShadows | Reflected),
          m_renderGroup (0)
    {
        pass.bind(this);
    }

    Drawable::Drawable(const Drawable& other, Object& newObj)
        : Component     (other, newObj),
          m_model       (other.m_model),
          m_shader      (other.m_shader),
          m_rendererRef (other.m_rendererRef),
          m_pass        (other.m_pass),
          m_renderGroup (other.m_renderGroup),
          m_flags       (other.m_flags)
    {
        m_rendererRef.bind(this, m_pass);
    }

    Drawable::~Drawable()
    {
        m_rendererRef.unbind(this, m_pass);
    }

    //////////////////////////////////////////////

    void Drawable::draw(const glm::mat4& view, const glm::mat4& proj, const LightContainer& lights) const
    {
        
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

    Drawable& Drawable::setModel(const Model& model)
    {
        m_model = model;
        return *this;
    }

    //////////////////////////////////////////////

    const Model& Drawable::getModel() const
    {
        return m_model;
    }

    Model& Drawable::getModel()
    {
        return m_model;
    }

    //////////////////////////////////////////////

    Drawable& Drawable::setFlags(const uint32 flags)
    {
        m_flags = flags;
    }

    //////////////////////////////////////////////

    bool Drawable::hasFlag(const uint32 flag) const
    {
        return (m_flags & flag) != 0;
    }

    //////////////////////////////////////////////

    Message::Result Drawable::receiveMessage(const Message& message)
    {
        if (JOP_EXECUTE_COMMAND(Drawable, message.getString(), this) == Message::Result::Escape)
            return Message::Result::Escape;

        return Component::receiveMessage(message);
    }
}