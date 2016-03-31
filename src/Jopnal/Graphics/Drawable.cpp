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
    JOP_DERIVED_COMMAND_HANDLER(Component, Drawable)

        JOP_BIND_MEMBER_COMMAND_NORETURN(&Drawable::setModel, "setModel");
        JOP_BIND_MEMBER_COMMAND_NORETURN(&Drawable::setShader, "setShader");
       
        JOP_BIND_MEMBER_COMMAND(&Drawable::setReceiveLights, "setReceiveLights");
        JOP_BIND_MEMBER_COMMAND(&Drawable::setReceiveShadows, "setReceiveShadows");
        JOP_BIND_MEMBER_COMMAND(&Drawable::setCastShadows, "setCastShadows");
        JOP_BIND_MEMBER_COMMAND(&Drawable::setReflected, "setReflected");
        JOP_BIND_MEMBER_COMMAND(&Drawable::setRenderGroup, "setRenderGroup");

    JOP_END_COMMAND_HANDLER(Drawable)
}

namespace jop
{
    Drawable::Drawable(Object& object, const std::string& ID, Renderer& renderer)
        : Component         (object, ID),
          m_model           (Model::getDefault()),
          m_shader          (static_ref_cast<Shader>(Shader::getDefault().getReference())),
          m_rendererRef     (renderer),
          m_renderGroup     (0),
          m_flags           (ReceiveLights | ReceiveShadows | CastShadows | Reflected)   
    {
        renderer.bind(*this);
    }

    Drawable::Drawable(const Drawable& other, Object& newObj)
        : Component         (other, newObj),
          m_model           (other.m_model),
          m_shader          (other.m_shader),
          m_rendererRef     (other.m_rendererRef),
          m_renderGroup     (other.m_renderGroup),
          m_flags           (other.m_flags)
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
        draw(&camera, lights, *getShader());
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

    void Drawable::setRenderGroup(const uint8 group)
    {
        m_renderGroup = std::min(static_cast<uint8>(31), group);
    }

    //////////////////////////////////////////////

    uint8 Drawable::getRenderGroup() const
    {
        return m_renderGroup;
    }

    //////////////////////////////////////////////

    Drawable& Drawable::setModel(const Model& model, const bool loadMaterialShader)
    {
        m_model = model;

        if (loadMaterialShader && !m_model.getMaterial().expired())
            setShader(ShaderManager::getShader(m_model.getMaterial()->getAttributeField()));

        return *this;
    }

    //////////////////////////////////////////////

    const Model& Drawable::getModel() const
    {
        return m_model;
    }

    //////////////////////////////////////////////

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

    WeakReference<Shader> Drawable::getShader() const
    {
        return m_shader;
    }

    //////////////////////////////////////////////

    void Drawable::setReceiveLights(const bool receive)
    {
        m_flags = (receive ? m_flags | ReceiveLights : m_flags & ~(ReceiveLights));
    }

    //////////////////////////////////////////////

    bool Drawable::receiveLights() const
    {
        return (m_flags & ReceiveLights) != 0;
    }

    //////////////////////////////////////////////

    bool Drawable::lightTouches(const LightSource& light) const
    {
        // TODO: Take AABB into account
        return light.getType() == LightSource::Type::Directional || (this->getObject()->getPosition() - light.getObject()->getPosition()).length() < light.getRange();
    }

    //////////////////////////////////////////////

    void Drawable::setReceiveShadows(const bool receive)
    {
        m_flags = (receive ? m_flags | ReceiveShadows : m_flags & ~(ReceiveShadows));
    }

    //////////////////////////////////////////////

    bool Drawable::receiveShadows() const
    {
        return (m_flags & ReceiveShadows) != 0;
    }

    //////////////////////////////////////////////

    void Drawable::setCastShadows(const bool cast)
    {
        m_flags = (cast ? m_flags | CastShadows : m_flags & ~(CastShadows));
    }

    //////////////////////////////////////////////

    bool Drawable::castShadows() const
    {
        return (m_flags & CastShadows) != 0;
    }

    //////////////////////////////////////////////

    void Drawable::setReflected(const bool reflected)
    {
        m_flags = (reflected ? m_flags | Reflected : m_flags & ~(Reflected));
    }

    //////////////////////////////////////////////

    bool Drawable::isReflected() const
    {
        return (m_flags & Reflected) != 0;
    }

    //////////////////////////////////////////////

    bool Drawable::loadStateBase(Drawable& drawable, const Scene&, const json::Value& val)
    {
        drawable.setID(val.HasMember("id") && val["id"].IsString() ? val["id"].GetString() : "");

        if (val.HasMember("shader") && val["shader"].IsString())
        {
            const std::string shstr = val["shader"].GetString();

            if (ResourceManager::resourceExists<Shader>(shstr))
                drawable.setShader(ResourceManager::getExistingResource<Shader>(shstr));
            else
                JOP_DEBUG_WARNING("Couldn't find shader named \"" << shstr << "\" while loading drawable \"" << drawable.getID() << "\". Resorting to default");
        }
        if (val.HasMember("mesh") && val["mesh"].IsString())
        {
            const std::string mshstr = val["mesh"].GetString();

            if (ResourceManager::resourceExists<Mesh>(mshstr))
                drawable.m_model.setMesh(ResourceManager::getExistingResource<Mesh>(mshstr));
            else
                JOP_DEBUG_WARNING("Couldn't find mesh named \"" << mshstr << "\" while loading drawable \"" << drawable.getID() << "\". Resorting to default");
        }

        if (val.HasMember("material") && val["material"].IsString())
        {
            const std::string matstr = val["material"].GetString();

            if (ResourceManager::resourceExists<Material>(matstr))
                drawable.m_model.setMaterial(ResourceManager::getExistingResource<Material>(matstr));
            else
                JOP_DEBUG_WARNING("Couldn't find material named \"" << matstr << "\" while loading drawable \"" << drawable.getID() << "\". Resorting to default");
        }

        return true;
    }

    //////////////////////////////////////////////

    bool Drawable::saveStateBase(const Drawable& drawable, json::Value& val, json::Value::AllocatorType& alloc)
    {
        val.AddMember(json::StringRef("id"), json::StringRef(drawable.getID().c_str()), alloc);

        if (!drawable.m_shader.expired())
            val.AddMember(json::StringRef("shader"), json::StringRef(drawable.m_shader->getName().c_str()), alloc);

        if (!drawable.m_model.getMesh().expired())
            val.AddMember(json::StringRef("mesh"), json::StringRef(drawable.m_model.getMesh()->getName().c_str()), alloc);

        if (!drawable.getModel().getMaterial().expired())
            val.AddMember(json::StringRef("material"), json::StringRef(drawable.getModel().getMaterial()->getName().c_str()), alloc);

        return true;
    }
}