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
            Color(SettingManager::get<std::string>("engine@Graphics|Shading|Material|sDefaultEmission", "000000FF"))
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
          m_autoAttribs         (autoAttributes)
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
          m_autoAttribs         (other.m_autoAttribs)
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
                /* 7 */  "u_Emission",
                /* 8 */  "u_DiffuseMap",
                /* 9 */  "u_SpecularMap",
                /* 10 */ "u_EmissionMap",
                /* 11 */ "u_OpacityMap",
                /* 12 */ "u_GlossMap",
                /* 13 */ "u_EnvironmentMap",
                /* 14 */ "u_ReflectionMap"
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
            else
                shader.setUniform(strCache[7], m_reflection[castIndex(Reflection::Emission)].asRGBAVector());

            if (hasAttribute(Attribute::DiffuseMap) && getMap(Map::Diffuse))
                shader.setUniform(strCache[8], *getMap(Material::Map::Diffuse), castIndex(Map::Diffuse));

            if (hasAttribute(Attribute::SpecularMap) && getMap(Map::Specular))
                shader.setUniform(strCache[9], *getMap(Map::Specular), castIndex(Map::Specular));

            if (hasAttribute(Attribute::EmissionMap) && getMap(Map::Emission))
                shader.setUniform(strCache[10], *getMap(Map::Emission), castIndex(Map::Emission));

            if (hasAttribute(Attribute::OpacityMap) && getMap(Map::Opacity))
                shader.setUniform(strCache[11], *getMap(Map::Opacity), castIndex(Map::Opacity));

            if (hasAttribute(Attribute::GlossMap) && getMap(Map::Gloss))
                shader.setUniform(strCache[12], *getMap(Map::Gloss), castIndex(Map::Gloss));


            if (hasAttribute(Attribute::EnvironmentMap) && getMap(Material::Map::Environment))
            {
                shader.setUniform(strCache[13], *getMap(Material::Map::Environment), castIndex(Map::Environment));

                if (hasAttribute(Attribute::ReflectionMap) && getMap(Map::Reflection))
                    shader.setUniform(strCache[14], *getMap(Material::Map::Reflection), castIndex(Map::Reflection));
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
        }
    }

    //////////////////////////////////////////////

    Material& Material::setReflection(const Reflection reflection, const Color color)
    {
        m_reflection[static_cast<int>(reflection)] = color;

        return addAttributes(Attribute::DefaultLighting * m_autoAttribs * (reflection != Reflection::Emission));
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

    Material& Material::setReflection(const Preset preset)
    {
        static const std::tuple<Color, Color, Color, float> presets[] =
        {
            //                               Ambient                                 Diffuse                                       Specular                   Shininess
            std::make_tuple(Color(0.0215f,   0.1745f,   0.0215f),   Color(0.07568f,  0.61424f,    0.07568f),    Color(0.633f,      0.727811f,   0.633f),      128 * 0.6f),          // Emerald
            std::make_tuple(Color(0.135f,    0.2225f,   0.1575f),   Color(0.54f,     0.89f,       0.63f),       Color(0.316228f,   0.316228f,   0.316228f),   128 * 0.1f),          // Jade
            std::make_tuple(Color(0.05375f,  0.05f,     0.06625f),  Color(0.18275f,  0.17f,       0.22525f),    Color(0.332741f,   0.328634f,   0.346435f),   128 * 0.3f),          // Obsidian
            std::make_tuple(Color(0.25f,     0.20725f,  0.20725f),  Color(1.0f,      0.829f,      0.829f),      Color(0.296648f,   0.296648f,   0.296648f),   128 * 0.088f),        // Pearl
            std::make_tuple(Color(0.1745f,   0.01175f,  0.01175f),  Color(0.61424f,  0.04136f,    0.04136f),    Color(0.727811f,   0.626959f,   0.626959f),   128 * 0.6f),          // Ruby
            std::make_tuple(Color(0.1f,      0.18725f,  0.1745f),   Color(0.396f,    0.74151f,    0.69102f),    Color(0.297254f,   0.30829f,    0.306678f),   128 * 0.1f),          // Turquoise
            std::make_tuple(Color(0.329412f, 0.223529f, 0.027451f), Color(0.780392f, 0.568627f,   0.113725f),   Color(0.992157f,   0.941176f,   0.807843f),   128 * 0.21794872f),   // Brass
            std::make_tuple(Color(0.2125f,   0.1275f,   0.054f),    Color(0.714f,    0.4284f,     0.18144f),    Color(0.393548f,   0.271906f,   0.166721f),   128 * 0.2f),          // Bronze
            std::make_tuple(Color(0.25f,     0.25f,     0.25f),     Color(0.4f,      0.4f,        0.4f),        Color(0.774597f,   0.774597f,   0.774597f),   128 * 0.6f),          // Chrome
            std::make_tuple(Color(0.19125f,  0.0735f,   0.0225f),   Color(0.7038f,   0.27048f,    0.0828f),     Color(0.256777f,   0.137622f,   0.086014f),   128 * 0.1f),          // Copper
            std::make_tuple(Color(0.24725f,  0.1995f,   0.0745f),   Color(0.75164f,  0.60648f,    0.22648f),    Color(0.628281f,   0.555802f,   0.366065f),   128 * 0.4f),          // Gold
            std::make_tuple(Color(0.19225f,  0.19225f,  0.19225f),  Color(0.50754f,  0.50754f,    0.50754f),    Color(0.508273f,   0.508273f,   0.508273f),   128 * 0.4f),          // Silver
            std::make_tuple(Color(0.0f,      0.0f,      0.0f),      Color(0.01f,     0.01f,       0.01f),       Color(0.50f,       0.50f,       0.50f),       128 * 0.25f),         // BlackPlastic
            std::make_tuple(Color(0.0f,      0.1f,      0.06f),     Color(0.0f,      0.50980392f, 0.50980392f), Color(0.50196078f, 0.50196078f, 0.50196078f), 128 * 0.25f),         // CyanPlastic
            std::make_tuple(Color(0.0f,      0.0f,      0.0f),      Color(0.1f,      0.35f,       0.1f),        Color(0.45f,       0.55f,       0.45f),       128 * 0.25f),         // GreenPlastic
            std::make_tuple(Color(0.0f,      0.0f,      0.0f),      Color(0.5f,      0.0f,        0.0f),        Color(0.7f,        0.6f,        0.6f),        128 * 0.25f),         // RedPlastic
            std::make_tuple(Color(0.0f,      0.0f,      0.0f),      Color(0.55f,     0.55f,       0.55f),       Color(0.70f,       0.70f,       0.70f),       128 * 0.25f),         // WhitePlastic
            std::make_tuple(Color(0.0f,      0.0f,      0.0f),      Color(0.5f,      0.5f,        0.0f),        Color(0.6f,        0.6f,        0.5f),        128 * 0.25f),         // YellowPlastic
            std::make_tuple(Color(0.02f,     0.02f,     0.02f),     Color(0.01f,     0.01f,       0.01f),       Color(0.4f,        0.4f,        0.4f),        128 * 0.078125f),     // BlackRubber
            std::make_tuple(Color(0.0f,      0.05f,     0.05f),     Color(0.4f,      0.5f,        0.5f),        Color(0.04f,       0.7f,        0.7f),        128 * 0.078125f),     // CyanRubber
            std::make_tuple(Color(0.0f,      0.05f,     0.0f),      Color(0.4f,      0.5f,        0.4f),        Color(0.04f,       0.7f,        0.7f),        128 * 0.078125f),     // GreenRubber
            std::make_tuple(Color(0.05f,     0.0f,      0.0f),      Color(0.5f,      0.4f,        0.4f),        Color(0.7f,        0.04f,       0.04f),       128 * 0.078125f),     // RedRubber
            std::make_tuple(Color(0.05f,     0.05f,     0.05f),     Color(0.5f,      0.5f,        0.5f),        Color(0.7f,        0.7f,        0.7f),        128 * 0.078125f),     // WhiteRubber
            std::make_tuple(Color(0.05f,     0.05f,     0.0f),      Color(0.5f,      0.5f,        0.4f),        Color(0.7f,        0.7f,        0.04f),       128 * 0.078125f)      // YellowRubber
        };
        
        auto& pres = presets[static_cast<int>(preset)];

        return setReflection(std::get<0>(pres), std::get<1>(pres), std::get<2>(pres), Color::Black).setShininess(std::get<3>(pres));
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

    Material& Material::setAttributeField(const uint64 attribs)
    {
        m_attributes = attribs;
        return *this;
    }

    //////////////////////////////////////////////

    uint64 jop::Material::getAttributeField() const
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
        return setAttributeField(getAttributeField() | attribs);
    }

    //////////////////////////////////////////////

    Material& Material::removeAttributes(const uint64 attribs)
    {
        return setAttributeField(getAttributeField() & ~attribs);
    }

    //////////////////////////////////////////////

    bool Material::hasAlpha() const
    {
        auto& ref = m_reflection;

        return ref[0].alpha < 1.f || ref[1].alpha < 1.f || ref[2].alpha < 1.f || ref[3].alpha < 1.f                             ||
               hasAttributes(Attribute::OpacityMap)                                                                             ||
              (hasAttributes(Attribute::DiffuseMap)     && getMap(Map::Diffuse)     && getMap(Map::Diffuse)->getDepth() > 3)    ||
              (hasAttributes(Attribute::EmissionMap)    && getMap(Map::Emission)    && getMap(Map::Emission)->getDepth() > 3);
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
}