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
    JOP_REGISTER_LOADABLE(jop, Material)[](const void*, const json::Value& val) -> bool
    {
        if (!val.HasMember("name") || !val["name"].IsString())
        {
            JOP_DEBUG_ERROR("Couldn't load material, no name found");
            return false;
        }
        Material::AttribType attr = Material::DefaultAttributes;
        if (val.HasMember("attributes") && val["attributes"].IsUint())
            attr = val["attributes"].GetUint();
        else
            JOP_DEBUG_WARNING("No attribute field found in material \"" << val["name"].GetString() << "\", using defaults");

        auto& m = ResourceManager::getEmptyResource<Material>(val["name"].GetString(), attr);
        //m.setPersistent(val.HasMember("persistent") && val["persistent"].IsBool() ? val["persistent"].GetBool() : false);

        if (val.HasMember("reflection") && val["reflection"].IsArray() && val["reflection"].Size() >= 4)
        {
            auto& refArr = val["reflection"];
            for (auto& i : refArr)
                if (!i.IsUint())
                    goto RefDone;

            m.setReflection(Color(refArr[0u].GetUint()), Color(refArr[1u].GetUint()), Color(refArr[2u].GetUint()), Color(refArr[3u].GetUint()));
        }

        RefDone:

        if (val.HasMember("shininess") && val["shininess"].IsDouble())
            m.setShininess(static_cast<float>(val["shininess"].GetDouble()));

        if (val.HasMember("diffmap") && val["diffmap"].IsString())
            m.setMap(Material::Map::Diffuse, ResourceManager::getResource<Texture2D>(val["diffmap"].GetString()));

        if (val.HasMember("specmap") && val["specmap"].IsString())
            m.setMap(Material::Map::Specular, ResourceManager::getResource<Texture2D>(val["specmap"].GetString()));

        if (val.HasMember("emissmap") && val["emissmap"].IsString())
            m.setMap(Material::Map::Emission, ResourceManager::getResource<Texture2D>(val["emissmap"].GetString()));

        return true;
    }
    JOP_END_LOADABLE_REGISTRATION(Material)

    JOP_REGISTER_SAVEABLE(jop, Material)[](const void* material, json::Value& val, json::Value::AllocatorType& alloc)
    {
        auto& ref = *static_cast<const Material*>(material);

        val.AddMember(json::StringRef("name"), json::StringRef(ref.getName().c_str()), alloc);
        //val.AddMember(json::StringRef("persistent"), ref.isPersistent(), alloc);

        val.AddMember(json::StringRef("attributes"), ref.getAttributeField(), alloc);

        val.AddMember(json::StringRef("reflection"), json::kArrayType, alloc)["reflection"]
           .PushBack(ref.getReflection(Material::Reflection::Ambient).asInteger(), alloc)
           .PushBack(ref.getReflection(Material::Reflection::Diffuse).asInteger(), alloc)
           .PushBack(ref.getReflection(Material::Reflection::Specular).asInteger(), alloc)
           .PushBack(ref.getReflection(Material::Reflection::Emission).asInteger(), alloc);

        val.AddMember(json::StringRef("shininess"), ref.getShininess(), alloc);

        auto diff = ref.getMap(Material::Map::Diffuse);
        if (!diff.expired())
            val.AddMember(json::StringRef("diffmap"), json::StringRef(diff->getName().c_str()), alloc);

        auto spec = ref.getMap(Material::Map::Specular);
        if (!diff.expired())
            val.AddMember(json::StringRef("specmap"), json::StringRef(spec->getName().c_str()), alloc);

        auto emiss = ref.getMap(Material::Map::Emission);
        if (!diff.expired())
            val.AddMember(json::StringRef("emissmap"), json::StringRef(emiss->getName().c_str()), alloc);

        return true;
    }
    JOP_END_SAVEABLE_REGISTRATION(Material);
}

namespace
{
    static const int ns_ambIndex = static_cast<int>(jop::Material::Reflection::Ambient);
    static const int ns_diffIndex = static_cast<int>(jop::Material::Reflection::Diffuse);
    static const int ns_specIndex = static_cast<int>(jop::Material::Reflection::Specular);
    static const int ns_emissIndex = static_cast<int>(jop::Material::Reflection::Emission);

    static const int ns_diffMapIndex = static_cast<int>(jop::Material::Map::Diffuse);
    static const int ns_specMapIndex = static_cast<int>(jop::Material::Map::Specular);
    static const int ns_emissMapIndex = static_cast<int>(jop::Material::Map::Emission);
    static const int ns_envMapIndex = static_cast<int>(jop::Material::Map::Environment);
    static const int ns_reflMapIndex = static_cast<int>(jop::Material::Map::Reflection);
}

namespace jop
{
    const Material::AttribType Material::DefaultAttributes = Material::Attribute::AmbientConstant
                                                           | Material::Attribute::Material
                                                           | Material::Attribute::Diffusemap
                                                           | Material::Attribute::Specularmap
                                                           | Material::Attribute::Emissionmap
                                                           | Material::Attribute::Phong;

    //////////////////////////////////////////////

    Material::Material(const std::string& name)
        : Resource      (name),
          m_reflection  (),
          m_attributes  (DefaultAttributes),
          m_shininess   (1.f),
          m_maps        ()
    {
        setReflection
        (
            Color::Black,
            Color::White,
            Color::White,
            Color::Black
        )
        .setMap(Map::Diffuse, Texture::getDefault());
    }

    //////////////////////////////////////////////

    Material::Material(const std::string& name, const AttribType attributes)
        : Resource      (name),
          m_reflection  (),
          m_attributes  (attributes),
          m_shininess   (1.f),
          m_maps        ()
    {
        setReflection
        (
            Color::Black,
            Color::White,
            Color::White,
            Color::Black
        )
        .setMap(Map::Diffuse, Texture::getDefault());
    }

    //////////////////////////////////////////////

    void Material::sendToShader(Shader& shader) const
    {
        if (shader.bind())
        {
            if (hasAttribute(Attribute::Material))
            {
                shader.setUniform("u_Material.ambient", m_reflection[ns_ambIndex].asRGBFloatVector());
                shader.setUniform("u_Material.diffuse", m_reflection[ns_diffIndex].asRGBFloatVector());
                shader.setUniform("u_Material.specular", m_reflection[ns_specIndex].asRGBFloatVector());
                shader.setUniform("u_Material.emission", m_reflection[ns_emissIndex].asRGBFloatVector());
                shader.setUniform("u_Material.shininess", m_shininess);
            }
            else
                shader.setUniform("u_Emission", m_reflection[ns_emissIndex].asRGBFloatVector());

            if (hasAttribute(Attribute::Diffusemap) && !getMap(Map::Diffuse).expired())
                shader.setUniform("u_DiffuseMap", *getMap(Material::Map::Diffuse), ns_diffMapIndex);

            if (hasAttribute(Attribute::Specularmap) && !getMap(Map::Specular).expired())
                shader.setUniform("u_SpecularMap", *getMap(Map::Specular), ns_specMapIndex);

            if (hasAttribute(Attribute::Emissionmap) && !getMap(Map::Emission).expired())
                shader.setUniform("u_EmissionMap", *getMap(Map::Emission), ns_emissMapIndex);

            if (hasAttribute(Attribute::EnvironmentMap) && !getMap(Material::Map::Environment).expired())
                shader.setUniform("u_EnvironmentMap", *getMap(Material::Map::Environment), ns_envMapIndex);

            if (hasAttribute(Attribute::ReflectionMap) && !getMap(Map::Reflection).expired())
                shader.setUniform("u_ReflectionMap", *getMap(Material::Map::Reflection), ns_reflMapIndex);
        }
    }

    //////////////////////////////////////////////

    Material& Material::setReflection(const Reflection reflection, const Color color)
    {
        m_reflection[static_cast<int>(reflection)] = color;
        return *this;
    }

    //////////////////////////////////////////////

    Material& Material::setReflection(const Color ambient, const Color diffuse, const Color specular, const Color emission)
    {
        return setReflection(Reflection::Ambient, ambient)
              .setReflection(Reflection::Diffuse, diffuse)
              .setReflection(Reflection::Specular, specular)
              .setReflection(Reflection::Emission, emission);
    }

    //////////////////////////////////////////////

    Color Material::getReflection(const Reflection reflection) const
    {
        return m_reflection[static_cast<int>(reflection)];
    }

    //////////////////////////////////////////////

    Material& Material::setShininess(const float value)
    {
        m_shininess = value;
        return *this;
    }

    //////////////////////////////////////////////

    float Material::getShininess() const
    {
        return m_shininess;
    }

    //////////////////////////////////////////////

    Material& Material::setMap(const Map map, const Texture& tex)
    {
        m_maps[static_cast<int>(map) - 1] = static_ref_cast<const Texture>(tex.getReference());
        return *this;
    }

    //////////////////////////////////////////////

    WeakReference<const Texture> Material::getMap(const Map map) const
    {
        return m_maps[static_cast<int>(map) - 1];
    }

    //////////////////////////////////////////////

    void Material::setAttributeField(const AttribType attribs)
    {
        m_attributes = attribs;
    }

    //////////////////////////////////////////////

    Material::AttribType jop::Material::getAttributeField() const
    {
        return m_attributes;
    }

    //////////////////////////////////////////////

    bool Material::hasAttribute(const AttribType attrib) const
    {
        return (m_attributes & attrib) != 0;
    }

    //////////////////////////////////////////////

    Material& Material::getDefault()
    {
        static WeakReference<Material> defMat;

        if (defMat.expired())
        {
            defMat = static_ref_cast<Material>(ResourceManager::getEmptyResource<Material>("jop_default_material", Attribute::Diffusemap).getReference());
            defMat->setManaged(true);
        }

        return *defMat;
    }
}