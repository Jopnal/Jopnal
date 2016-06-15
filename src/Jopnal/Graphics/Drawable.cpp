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
    #include <Jopnal/Graphics/Model.hpp>
    #include <Jopnal/Graphics/Shader.hpp>
    #include <Jopnal/Utility/CommandHandler.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    JOP_REGISTER_COMMAND_HANDLER(Drawable)

        JOP_BIND_MEMBER_COMMAND(&Drawable::setModel, "setModel");
        JOP_BIND_MEMBER_COMMAND(&Drawable::setShader, "setShader");

        JOP_BIND_MEMBER_COMMAND(&Drawable::setReceiveLights, "setReceiveLights");
        JOP_BIND_MEMBER_COMMAND(&Drawable::setReceiveShadows, "setReceiveShadows");
        JOP_BIND_MEMBER_COMMAND(&Drawable::setCastShadows, "setCastShadows");
        JOP_BIND_MEMBER_COMMAND(&Drawable::setReflected, "setReflected");
        JOP_BIND_MEMBER_COMMAND(&Drawable::setRenderGroup, "setRenderGroup");
        JOP_BIND_MEMBER_COMMAND(&Drawable::setID, "setID");

    JOP_END_COMMAND_HANDLER(Drawable)

    bool jop__baseDrawableLoadFunc(JOP_COMPONENT_LOAD_ARGS)
    {
        // mesh & material

        return Serializer::callSingleFunc<Component, Serializer::FunctionID::Load>(Serializer::getSerializeID<Component>(), comp, val);
    }
    bool jop__baseDrawableSaveFunc(JOP_COMPONENT_SAVE_ARGS)
    {
        // mesh & material

        return Serializer::callSingleFunc<Component, Serializer::FunctionID::Save>(Serializer::getSerializeID<Component>(), comp, val, alloc);
    }

    JOP_REGISTER_SERIALIZER_NO_FACTORY(jop, Drawable, jop__baseDrawableLoadFunc, jop__baseDrawableSaveFunc);
}

namespace jop
{
    Drawable::Drawable(Object& object, Renderer& renderer, const uint32 ID)
        : Component     (object, ID),
          m_model       (Mesh::getDefault(), Material::getDefault()),
          m_shader      (),
          m_rendererRef (renderer),
          m_alphaMult   (1.f),
          m_renderGroup (0),
          m_flags       (ReceiveLights | ReceiveShadows | CastShadows | Reflected)   
    {
        renderer.bind(*this);
    }

    Drawable::Drawable(const Drawable& other, Object& newObj)
        : Component     (other, newObj),
          m_model       (other.m_model),
          m_shader      (other.m_shader),
          m_rendererRef (other.m_rendererRef),
          m_alphaMult   (other.m_alphaMult),
          m_renderGroup (other.m_renderGroup),
          m_flags       (other.m_flags)
    {
        m_rendererRef.bind(*this);
    }

    Drawable::~Drawable()
    {
        m_rendererRef.unbind(*this);
    }

    //////////////////////////////////////////////

    void Drawable::draw(const Camera& camera, const LightContainer& lights) const
    {
        auto shdr = ((m_model.getMaterial() == nullptr || !m_model.getMaterial()->getShader()) && m_shader.expired()) ? m_shader.get() : m_model.getMaterial()->getShader();

        draw(&camera, lights, shdr == nullptr ? Shader::getDefault() : *shdr);
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

    Drawable& Drawable::setShader(Shader& shader)
    {
        m_shader = static_ref_cast<Shader>(shader.getReference());
        return *this;
    }

    //////////////////////////////////////////////

    Drawable& Drawable::removeShader(const bool loadMaterialShader)
    {
        if (loadMaterialShader && m_model.getMaterial())
            m_shader = static_ref_cast<Shader>(ShaderAssembler::getShader(m_model.getMaterial()->getAttributeField()).getReference());
        else
            m_shader.reset();

        return *this;
    }

    //////////////////////////////////////////////

    Shader* Drawable::getShader() const
    {
        return m_shader.get();
    }

    //////////////////////////////////////////////

    Drawable& Drawable::setReceiveLights(const bool receive)
    {
        m_flags = (receive ? m_flags | ReceiveLights : m_flags & ~(ReceiveLights));
        return *this;
    }

    //////////////////////////////////////////////

    bool Drawable::receiveLights() const
    {
        return (m_flags & ReceiveLights) != 0 && (m_model.getMaterial() != nullptr && m_model.getMaterial()->hasAttribute(Material::Attribute::__Lighting));
    }

    //////////////////////////////////////////////

    bool Drawable::lightTouches(const LightSource& light) const
    {
        return light.checkRange(*this);
    }

    //////////////////////////////////////////////

    Drawable& Drawable::setReceiveShadows(const bool receive)
    {
        m_flags = (receive ? m_flags | ReceiveShadows : m_flags & ~(ReceiveShadows));
        return *this;
    }

    //////////////////////////////////////////////

    bool Drawable::receiveShadows() const
    {
        return (m_flags & ReceiveShadows) != 0;
    }

    //////////////////////////////////////////////

    Drawable& Drawable::setCastShadows(const bool cast)
    {
        m_flags = (cast ? m_flags | CastShadows : m_flags & ~(CastShadows));
        return *this;
    }

    //////////////////////////////////////////////

    bool Drawable::castShadows() const
    {
        return (m_flags & CastShadows) != 0;
    }

    //////////////////////////////////////////////

    Drawable& Drawable::setReflected(const bool reflected)
    {
        m_flags = (reflected ? m_flags | Reflected : m_flags & ~(Reflected));
        return *this;
    }

    //////////////////////////////////////////////

    bool Drawable::isReflected() const
    {
        return (m_flags & Reflected) != 0;
    }

    //////////////////////////////////////////////

    void Drawable::setAlphaMultiplier(const float mult)
    {
        m_alphaMult = mult;
    }

    //////////////////////////////////////////////

    float Drawable::getAlphaMultiplier() const
    {
        return m_alphaMult;
    }

    //////////////////////////////////////////////

    Message::Result Drawable::receiveMessage(const Message& message)
    {
        if (JOP_EXECUTE_COMMAND(Drawable, message.getString(), this) == Message::Result::Escape)
            return Message::Result::Escape;

        return Component::receiveMessage(message);
    }
}