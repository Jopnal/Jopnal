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

    #include <Jopnal/Graphics/Material.hpp>

    #include <Jopnal/Core/Object.hpp>
    #include <Jopnal/Core/ResourceManager.hpp>
    #include <Jopnal/Core/SettingManager.hpp>
    #include <Jopnal/Graphics/LightSource.hpp>
    #include <Jopnal/Graphics/Texture/Texture2D.hpp>
    #include <Jopnal/Graphics/ShaderAssembler.hpp>
    #include <Jopnal/Graphics/ShaderProgram.hpp>

#endif

//////////////////////////////////////////////


namespace
{
    template<typename T>
    inline int castIndex(const T map)
    {
        return static_cast<int>(map);
    }

    const std::array<jop::Color, 4>& getDefaultColors()
    {
        using namespace jop;

        static const std::array<Color, 4> colors =
        {
            Color(SettingManager::get<std::string>("engine@Graphics|Shading|Material|sDefaultAmbient", "000000FF")),
            Color(SettingManager::get<std::string>("engine@Graphics|Shading|Material|sDefaultDiffuse", "FFFFFFFF")),
            Color(SettingManager::get<std::string>("engine@Graphics|Shading|Material|sDefaultSpecular", "000000FF")),
            Color(SettingManager::get<std::string>("engine@Graphics|Shading|Material|sDefaultEmission", "00000000"))
        };

        return colors;
    }
}

namespace jop
{
    Material::Material(const std::string& name, const bool autoAttributes)
        : Resource              (name),
          m_reflection          (getDefaultColors()),
          m_reflectivity        (0.5f),
          m_attributes          (),
          m_shininess           (1.f),
          m_maps                (),
		  m_shader				(),
          m_autoAttribs         (autoAttributes),
		  m_updateShader		(true)
    {
        if (autoAttributes)
            setMap(Map::Diffuse, Texture2D::getDefault());
    }

    Material::Material(const Material& other, const std::string& newName)
        : Resource              (newName),
          m_reflection          (other.m_reflection),
          m_reflectivity        (other.m_reflectivity),
          m_attributes          (other.m_attributes),
          m_shininess           (other.m_shininess),
          m_maps                (other.m_maps),
          m_shader              (other.m_shader),
          m_autoAttribs         (other.m_autoAttribs),
          m_updateShader        (other.m_updateShader)
    {}

    //////////////////////////////////////////////

    void Material::sendToShader(ShaderProgram& shader, const glm::vec3* const camPos) const
    {
        if (shader.bind())
        {
            static const std::string strCache[] =
            {
                /* 0 */  "u_CameraPosition",
                /* 1 */  "u_Material.ambient",
                /* 2 */  "u_Material.diffuse",
                /* 3 */  "u_Material.specular",
                /* 4 */  "u_Material.emission",
                /* 5 */  "u_Material.shininess",
                /* 6 */  "u_Material.reflectivity",
                /* 7 */  "u_DiffuseMap",
                /* 8 */  "u_SpecularMap",
                /* 9 */  "u_EmissionMap",
                /* 10 */ "u_OpacityMap",
                /* 11 */ "u_GlossMap",
                /* 12 */ "u_EnvironmentMap",
                /* 13 */ "u_ReflectionMap"
            };

            // Send camera position to shader
            if (camPos && hasAttribute(Attribute::__Lighting | Attribute::EnvironmentMap))
                shader.setUniform(strCache[0], *camPos);

            if (hasAttribute(Attribute::__Lighting))
            {
                shader.setUniform(strCache[1], m_reflection[castIndex(Reflection::Ambient)].asRGBAVector());
                shader.setUniform(strCache[2], m_reflection[castIndex(Reflection::Diffuse)].asRGBAVector());
                shader.setUniform(strCache[3], m_reflection[castIndex(Reflection::Specular)].asRGBAVector());
                shader.setUniform(strCache[4], m_reflection[castIndex(Reflection::Emission)].asRGBAVector());
                shader.setUniform(strCache[5], m_shininess);

                if (hasAttribute(Attribute::EnvironmentMap))
                    shader.setUniform(strCache[6], m_reflectivity);
            }

            if (hasAttribute(Attribute::DiffuseMap) && getMap(Map::Diffuse))
                shader.setUniform(strCache[7], *getMap(Material::Map::Diffuse), castIndex(Map::Diffuse));

            if (hasAttribute(Attribute::SpecularMap) && getMap(Map::Specular))
                shader.setUniform(strCache[8], *getMap(Map::Specular), castIndex(Map::Specular));

            if (hasAttribute(Attribute::EmissionMap) && getMap(Map::Emission))
                shader.setUniform(strCache[9], *getMap(Map::Emission), castIndex(Map::Emission));

            if (hasAttribute(Attribute::OpacityMap) && getMap(Map::Opacity))
                shader.setUniform(strCache[10], *getMap(Map::Opacity), castIndex(Map::Opacity));

            if (hasAttribute(Attribute::GlossMap) && getMap(Map::Gloss))
                shader.setUniform(strCache[11], *getMap(Map::Gloss), castIndex(Map::Gloss));

            if (hasAttribute(Attribute::EnvironmentMap) && getMap(Material::Map::Environment))
            {
                shader.setUniform(strCache[12], *getMap(Material::Map::Environment), castIndex(Map::Environment));

                if (hasAttribute(Attribute::ReflectionMap) && getMap(Map::Reflection))
                    shader.setUniform(strCache[13], *getMap(Material::Map::Reflection), castIndex(Map::Reflection));
            }
        }
    }

    //////////////////////////////////////////////

    void Material::getShaderPreprocessorDef(const uint64 attribs, std::string& str)
    {
        using m = Material::Attribute;

        // Diffuse map
        if (attribs & m::DiffuseMap)
            str += "#define JMAT_DIFFUSEMAP\n";

        // Specular map
        if (attribs & m::SpecularMap)
            str += "#define JMAT_SPECULARMAP\n";

        // Emission map
        if (attribs & m::EmissionMap)
            str += "#define JMAT_EMISSIONMAP\n";

        // Environment map
        if (attribs & m::EnvironmentMap)
            str += "#define JMAT_ENVIRONMENTMAP\n";

        // Reflection map
        if (attribs & m::ReflectionMap)
            str += "#define JMAT_REFLECTIONMAP\n";

        // Reflection map
        if (attribs & m::OpacityMap)
            str += "#define JMAT_OPACITYMAP\n";

        // Gloss map
        if (attribs & m::GlossMap)
            str += "#define JMAT_GLOSSMAP\n";

        // Lighting
        if (attribs & m::__Lighting)
        {
            str += "#define JMAT_LIGHTING\n";

            static const std::string maxLights =
                "#define JMAT_MAX_POINT_LIGHTS " + std::to_string(LightSource::getMaximumLights(LightSource::Type::Point)) +
                "\n#define JMAT_MAX_DIRECTIONAL_LIGHTS " + std::to_string(LightSource::getMaximumLights(LightSource::Type::Directional)) +
                "\n#define JMAT_MAX_SPOT_LIGHTS " + std::to_string(LightSource::getMaximumLights(LightSource::Type::Spot)) + "\n";

            // Phong model
            if (attribs & m::Phong)
                str += "#define JMAT_PHONG\n" + maxLights;

            // Blinn-phong model
            if (attribs & m::BlinnPhong)
                str += "#define JMAT_BLINNPHONG\n";
        }
    }

    //////////////////////////////////////////////

    Material& Material::setReflection(const Reflection reflection, const Color color)
    {
        m_reflection[static_cast<int>(reflection)] = color;
        return addAttributes(Attribute::DefaultLighting * m_autoAttribs);
    }

    //////////////////////////////////////////////

    Material& Material::setReflection(const Color& ambient, const Color& diffuse, const Color& specular, const Color& emission)
    {
        m_reflection[1] = diffuse;
        m_reflection[2] = specular;
        m_reflection[3] = emission;

        return setReflection(Reflection::Ambient, ambient);
    }

    //////////////////////////////////////////////

    const Color& Material::getReflection(const Reflection reflection) const
    {
        return m_reflection[static_cast<int>(reflection)];
    }

    //////////////////////////////////////////////

    Material& Material::setShininess(const float value)
    {
        m_shininess = std::max(1.f, value);

        return addAttributes(Attribute::DefaultLighting * m_autoAttribs);
    }

    //////////////////////////////////////////////

    float Material::getShininess() const
    {
        return m_shininess;
    }

    //////////////////////////////////////////////

    Material& Material::setReflectivity(const float reflectivity)
    {
        m_reflectivity = reflectivity;

        return addAttributes(Attribute::EnvironmentMap * m_autoAttribs);
    }

    //////////////////////////////////////////////

    float Material::getReflectivity() const
    {
        return m_reflectivity;
    }

    //////////////////////////////////////////////

    namespace detail
    {
        static const int mapAttribs[] =
        {
            Material::Attribute::DiffuseMap,
            Material::Attribute::SpecularMap,
            Material::Attribute::EmissionMap,
            Material::Attribute::EnvironmentMap,
            Material::Attribute::ReflectionMap,
            Material::Attribute::OpacityMap,
            Material::Attribute::GlossMap
        };
    }

    Material& Material::setMap(const Map map, const Texture& tex)
    {
        const int mapIndex = static_cast<int>(map);
        m_maps[mapIndex] = static_ref_cast<const Texture>(tex.getReference());

        return addAttributes(detail::mapAttribs[mapIndex] * m_autoAttribs);
    }

    //////////////////////////////////////////////

    Material& Material::removeMap(const Map map)
    {
        const int mapIndex = static_cast<int>(map);
        m_maps[mapIndex].reset();

        return removeAttributes(detail::mapAttribs[mapIndex] * m_autoAttribs);
    }

    //////////////////////////////////////////////

    const Texture* Material::getMap(const Map map) const
    {
        return m_maps[static_cast<int>(map)].get();
    }

    //////////////////////////////////////////////

    Material& Material::setAttributes(const uint64 attribs)
    {
        m_attributes = attribs;
        m_updateShader = true;

        return *this;
    }

    //////////////////////////////////////////////

    uint64 Material::getAttributes() const
    {
        return m_attributes;
    }

    //////////////////////////////////////////////

    bool Material::hasAttribute(const uint64 attrib) const
    {
        return (m_attributes & attrib) != 0;
    }

    //////////////////////////////////////////////

    bool Material::hasAttributes(const uint64 attribs) const
    {
        return (m_attributes & attribs) == attribs;
    }

    //////////////////////////////////////////////

    bool Material::compareAttributes(const uint64 attribs) const
    {
        return m_attributes == attribs;
    }

    //////////////////////////////////////////////

    Material& Material::addAttributes(const uint64 attribs)
    {
        return setAttributes(getAttributes() | attribs);
    }

    //////////////////////////////////////////////

    Material& Material::removeAttributes(const uint64 attribs)
    {
        return setAttributes(getAttributes() & ~attribs);
    }

    //////////////////////////////////////////////

    bool Material::hasAlpha() const
    {
        auto& ref = m_reflection;

        return ref[0].alpha < 1.f || ref[1].alpha < 1.f || ref[2].alpha < 1.f || ref[3].alpha < 1.f                           ||
               hasAttributes(Attribute::OpacityMap)                                                                           ||
              (hasAttributes(Attribute::DiffuseMap)  && getMap(Map::Diffuse)  && getMap(Map::Diffuse)->getPixelDepth() > 3)   ||
              (hasAttributes(Attribute::EmissionMap) && getMap(Map::Emission) && getMap(Map::Emission)->getPixelDepth() > 3);
	}

	//////////////////////////////////////////////

	Material& Material::setAutoAttributes(const bool autoAttribs)
	{
		m_autoAttribs = autoAttribs;
		return *this;
	}

	//////////////////////////////////////////////

	bool Material::hasAutoAttributes() const
	{
		return m_autoAttribs;
	}

    //////////////////////////////////////////////

    Material& Material::getDefault()
    {
        static WeakReference<Material> defMat;

        if (defMat.expired())
        {
            defMat = static_ref_cast<Material>(ResourceManager::getEmpty<Material>("jop_default_material", true).getReference());
            defMat->setPersistence(0);
        }

        return *defMat;
    }

	//////////////////////////////////////////////

	ShaderProgram& Material::getShader() const
	{
		if (m_updateShader || m_shader.expired())
		{
			m_shader = static_ref_cast<ShaderProgram>(ShaderAssembler::getShader(getAttributes()).getReference());
			m_updateShader = false;
		}

        return *m_shader;
	}
}