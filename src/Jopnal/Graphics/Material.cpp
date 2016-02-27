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


namespace
{
    static const int ns_ambIndex = static_cast<int>(jop::Material::Reflection::Ambient);
    static const int ns_diffIndex = static_cast<int>(jop::Material::Reflection::Diffuse);
    static const int ns_specIndex = static_cast<int>(jop::Material::Reflection::Specular);
    static const int ns_emissIndex = static_cast<int>(jop::Material::Reflection::Emission);

    static const int ns_diffMapIndex = static_cast<int>(jop::Material::Map::Diffuse);
    static const int ns_specMapIndex = static_cast<int>(jop::Material::Map::Specular);
    static const int ns_emissMapIndex = static_cast<int>(jop::Material::Map::Emission);
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

    Material::Material()
        : m_reflection  (),
          m_attributes  (DefaultAttributes),
          m_shininess   (1.f),
          m_maps        ()
    {
        setReflection
        (
            Color::Black,
            Color::White,
            Color::White,
            Color::White
        )
        .setMap(Map::Diffuse, Texture::getDefault());
    }

    //////////////////////////////////////////////

    Material::Material(const AttribType attributes)
        : m_reflection  (),
          m_attributes  (attributes),
          m_shininess   (1.f),
          m_maps        ()
    {
        setMap(Map::Diffuse, Texture::getDefault());
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

            if (hasAttribute(Attribute::Diffusemap) && !m_maps[ns_diffMapIndex].expired())
                shader.setUniform("u_DiffuseMap", *m_maps[ns_diffMapIndex], ns_diffMapIndex);

            if (hasAttribute(Attribute::Specularmap) && !m_maps[ns_specMapIndex].expired())
                shader.setUniform("u_SpecularMap", *m_maps[ns_specMapIndex], ns_specMapIndex);

            if (hasAttribute(Attribute::Emissionmap) && !m_maps[ns_emissMapIndex].expired())
                shader.setUniform("u_EmissionMap", *m_maps[ns_emissMapIndex], ns_emissMapIndex);
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
        m_maps[static_cast<int>(map)] = static_ref_cast<const Texture>(tex.getReference());
        return *this;
    }

    //////////////////////////////////////////////

    WeakReference<const Texture> Material::getMap(const Map map) const
    {
        return m_maps[static_cast<int>(map)];
    }

    //////////////////////////////////////////////

    void Material::setAttributeField(const AttribType attribs)
    {
        m_attributes = attribs;
    }

    //////////////////////////////////////////////

    bool Material::hasAttribute(const AttribType attrib) const
    {
        return (m_attributes & attrib) != 0;
    }

    //////////////////////////////////////////////

    const Material& Material::getDefault()
    {
        static Material def(Attribute::Diffusemap);
        return def;
    }
}