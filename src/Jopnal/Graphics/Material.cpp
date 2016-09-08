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
    template<typename T, typename Ret = std::underlying_type<T>::type>
    inline Ret castEnum(const T map)
    {
        return static_cast<Ret>(map);
    }
    
    bool checkMap(const jop::uint64 attr, const jop::Material::Map map)
    {
        return (attr & (1ull << castEnum(map))) != 0;
    };

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
    const uint64 Material::LightingAttribs =
        static_cast<jop::uint64>(jop::Material::LightingModel::Gouraud) |
        static_cast<jop::uint64>(jop::Material::LightingModel::Flat) |
        static_cast<jop::uint64>(jop::Material::LightingModel::Phong) |
        static_cast<jop::uint64>(jop::Material::LightingModel::BlinnPhong);

    //////////////////////////////////////////////

    Material::Material(const std::string& name)
        : Resource          (name),
          m_reflection      (getDefaultColors()),
          m_reflectivity    (0.f),
          m_attributes      (),
          m_shininess       (1.f),
          m_maps            (),
		  m_shader		    (),
		  m_updateShader    (true)
    {}

    Material::Material(const Material& other, const std::string& newName)
        : Resource          (newName),
          m_reflection      (other.m_reflection),
          m_reflectivity    (other.m_reflectivity),
          m_attributes      (other.m_attributes),
          m_shininess       (other.m_shininess),
          m_maps            (other.m_maps),
          m_shader          (other.m_shader),
          m_updateShader    (other.m_updateShader)
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
            if (camPos && (m_attributes & (LightingAttribs | 1ull << castEnum(Map::Environment))))
                shader.setUniform(strCache[0], *camPos);

            if (m_attributes & LightingAttribs)
            {
                shader.setUniform(strCache[1], m_reflection[castEnum(Reflection::Ambient)].asRGBAVector());
                shader.setUniform(strCache[2], m_reflection[castEnum(Reflection::Diffuse)].asRGBAVector());
                shader.setUniform(strCache[3], m_reflection[castEnum(Reflection::Specular)].asRGBAVector());
                shader.setUniform(strCache[4], m_reflection[castEnum(Reflection::Emission)].asRGBAVector());
                shader.setUniform(strCache[5], m_shininess);

                if (checkMap(m_attributes, Map::Environment))
                    shader.setUniform(strCache[6], m_reflectivity);
            }

            if (checkMap(m_attributes, Map::Diffuse0) && getMap(Map::Diffuse0))
                shader.setUniform(strCache[7], *getMap(Material::Map::Diffuse0), castEnum<Map, unsigned int>(Map::Diffuse0));

            if (checkMap(m_attributes, Map::Specular) && getMap(Map::Specular))
                shader.setUniform(strCache[8], *getMap(Map::Specular), castEnum<Map, unsigned int>(Map::Specular));

            if (checkMap(m_attributes, Map::Emission) && getMap(Map::Emission))
                shader.setUniform(strCache[9], *getMap(Map::Emission), castEnum<Map, unsigned int>(Map::Emission));

            if (checkMap(m_attributes, Map::Opacity) && getMap(Map::Opacity))
                shader.setUniform(strCache[10], *getMap(Map::Opacity), castEnum<Map, unsigned int>(Map::Opacity));

            if (checkMap(m_attributes, Map::Gloss) && getMap(Map::Gloss))
                shader.setUniform(strCache[11], *getMap(Map::Gloss), castEnum<Map, unsigned int>(Map::Gloss));

            if (checkMap(m_attributes, Map::Environment) && getMap(Map::Environment))
            {
                shader.setUniform(strCache[12], *getMap(Map::Environment), castEnum<Map, unsigned int>(Map::Environment));

                if (checkMap(m_attributes, Map::Reflection) && getMap(Map::Reflection))
                    shader.setUniform(strCache[13], *getMap(Map::Reflection), castEnum<Map, unsigned int>(Map::Reflection));
            }
        }
    }

    //////////////////////////////////////////////

    Material& Material::setLightingModel(const LightingModel model)
    {
        if (model != getLightingModel())
        {
            (m_attributes &= ~(LightingAttribs)) |= castEnum(model);
            m_updateShader = true;
        }

        return *this;
    }

    //////////////////////////////////////////////

    Material::LightingModel Material::getLightingModel() const
    {
        return static_cast<LightingModel>(m_attributes & LightingAttribs);
    }

    //////////////////////////////////////////////

    Material& Material::setReflection(const Reflection reflection, const Color& color)
    {
        m_reflection[static_cast<int>(reflection)] = color;
        return *this;
    }

    //////////////////////////////////////////////

    Material& Material::setReflection(const Color& ambient, const Color& diffuse, const Color& specular, const Color& emission)
    {
        m_reflection[0] = ambient;
        m_reflection[1] = diffuse;
        m_reflection[2] = specular;
        m_reflection[3] = emission;

        return *this;
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
        return *this;
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
        return *this;
    }

    //////////////////////////////////////////////

    float Material::getReflectivity() const
    {
        return m_reflectivity;
    }

    //////////////////////////////////////////////

    Material& Material::setMap(const Map map, const Texture& tex)
    {
        m_maps[static_cast<int>(map)] = static_ref_cast<const Texture>(tex.getReference());
        m_attributes |= (1ull << castEnum(map));

        return *this;
    }

    //////////////////////////////////////////////

    Material& Material::removeMap(const Map map)
    {
        m_maps[static_cast<int>(map)].reset();
        m_attributes &= ~(1 << castEnum(map));

        return *this;
    }

    //////////////////////////////////////////////

    const Texture* Material::getMap(const Map map) const
    {
        return m_maps[static_cast<int>(map)].get();
    }

    //////////////////////////////////////////////

    bool Material::hasAlpha() const
    {
        auto& ref = m_reflection;

        return ((m_attributes & LightingAttribs) && (ref[0].alpha * ref[1].alpha * ref[2].alpha < 1.f)) ||
               ((m_attributes & (1ull << castEnum(Map::Opacity))) && getMap(Map::Opacity))              ||
               ((m_attributes & (1ull << castEnum(Map::Diffuse0))) && getMap(Map::Diffuse0) && getMap(Map::Diffuse0)->getPixelDepth() > 3);
	}

    //////////////////////////////////////////////

    Material& Material::getDefault()
    {
        static WeakReference<Material> defMat;

        if (defMat.expired())
        {
            defMat = static_ref_cast<Material>(ResourceManager::getEmpty<Material>("jop_default_material").getReference());
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

    //////////////////////////////////////////////

    uint64 Material::getAttributes() const
    {
        return m_attributes;
    }

    //////////////////////////////////////////////

    std::string Material::getShaderPreprocessorDef(const uint64 attributes)
    {
        std::string str;

        // Diffuse map
        if (checkMap(attributes, Map::Diffuse0))
            str += "#define JMAT_DIFFUSEMAP\n";

        // Specular map
        if (checkMap(attributes, Map::Specular))
            str += "#define JMAT_SPECULARMAP\n";

        // Emission map
        if (checkMap(attributes, Map::Emission))
            str += "#define JMAT_EMISSIONMAP\n";

        // Environment map
        if (checkMap(attributes, Map::Environment))
            str += "#define JMAT_ENVIRONMENTMAP\n";

        // Reflection map
        if (checkMap(attributes, Map::Reflection))
            str += "#define JMAT_REFLECTIONMAP\n";

        // Reflection map
        if (checkMap(attributes, Map::Opacity))
            str += "#define JMAT_OPACITYMAP\n";

        // Gloss map
        if (checkMap(attributes, Map::Gloss))
            str += "#define JMAT_GLOSSMAP\n";

        // Lighting
        if (attributes & LightingAttribs)
        {
            str += "#define JMAT_LIGHTING\n";

            static const std::string maxLights =
                "#define JMAT_MAX_POINT_LIGHTS " + std::to_string(LightSource::getMaximumLights(LightSource::Type::Point)) +
                "\n#define JMAT_MAX_DIRECTIONAL_LIGHTS " + std::to_string(LightSource::getMaximumLights(LightSource::Type::Directional)) +
                "\n#define JMAT_MAX_SPOT_LIGHTS " + std::to_string(LightSource::getMaximumLights(LightSource::Type::Spot)) + "\n";

            // Phong model
            if (attributes & castEnum(LightingModel::Phong))
            {
                str += "#define JMAT_PHONG\n" + maxLights;

                // Blinn-phong model
                if (attributes & castEnum(LightingModel::BlinnPhong))
                    str += "#define JMAT_BLINNPHONG\n";
            }
            else if (attributes & castEnum(LightingModel::Gouraud))
            {
                str += "#define JMAT_GOURAUD\n";

                if (attributes & castEnum(LightingModel::Flat))
                    str += "#define JMAT_FLAT\n";
            }

        #if defined(JOP_OPENGL_ES) && JOP_MIN_OPENGL_ES_VERSION < 300

            // Should the shadow map be unpacked?
            if (gl::getVersionMajor() < 3 && !JOP_CHECK_GL_EXTENSION(OES_depth_texture))
                str += "#define JOP_UNPACK_DEPTH\n";

        #endif
        }

        return str;
    }
}