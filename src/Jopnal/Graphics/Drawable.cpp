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

    JOP_END_COMMAND_HANDLER(Drawable)
}

namespace jop
{
    Drawable::Drawable(Object& object, const std::string& ID)
        : Component         (object, ID),
          m_model           (Model::getDefault()),
          m_boundToLayers   (),
          m_shader          (static_ref_cast<Shader>(Shader::getDefault().getReference())),
          m_receiveLights   (true)
    {}

    //////////////////////////////////////////////

    Drawable::Drawable(const Drawable& other)
        : Component(other),
          m_model(other.m_model),
          m_boundToLayers(),
          m_shader(other.m_shader),
          m_receiveLights(other.m_receiveLights)
    {
        for (auto& i : other.m_boundToLayers)
            i->addDrawable(*this);
    }

    Drawable::~Drawable()
    {
        for (auto itr = m_boundToLayers.begin(); itr != m_boundToLayers.end(); ++itr)
            (*itr)->removeDrawable(getID());
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

    const std::unordered_set<Layer*> Drawable::getBoundLayers() const
    {
        return m_boundToLayers;
    }

    //////////////////////////////////////////////

    void Drawable::setReceiveLights(const bool receive)
    {
        m_receiveLights = receive;
    }

    //////////////////////////////////////////////

    bool Drawable::receiveLights() const
    {
        return m_receiveLights;
    }

    //////////////////////////////////////////////

    bool Drawable::loadStateBase(Drawable& drawable, const Scene& scene, const json::Value& val)
    {
        drawable.setID(val.HasMember("id") && val["id"].IsString() ? val["id"].GetString() : "");

        std::vector<Layer*> layers;
        if (val.HasMember("layers") && val["layers"].IsArray())
        {
            for (auto& i : val["layers"])
            {
                if (!i.IsString())
                    continue;

                auto layer = scene.getLayer(i.GetString());

                if (!layer.expired())
                    layers.push_back(layer.get());
            }
        }

        for (auto itr = layers.begin(); itr != layers.end(); ++itr)
            (*itr)->addDrawable(drawable);

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

        auto& boundLayers = drawable.getBoundLayers();
        if (!boundLayers.empty())
        {
            auto& layers = val.AddMember(json::StringRef("layers"), json::kArrayType, alloc)["layers"];

            for (auto& i : boundLayers)
                layers.PushBack(json::StringRef(i->getID().c_str()), alloc);
        }

        if (!drawable.m_shader.expired())
            val.AddMember(json::StringRef("shader"), json::StringRef(drawable.m_shader->getName().c_str()), alloc);

        if (!drawable.m_model.getMesh().expired())
            val.AddMember(json::StringRef("mesh"), json::StringRef(drawable.m_model.getMesh()->getName().c_str()), alloc);

        if (!drawable.getModel().getMaterial().expired())
            val.AddMember(json::StringRef("material"), json::StringRef(drawable.getModel().getMaterial()->getName().c_str()), alloc);

        return true;
    }
}