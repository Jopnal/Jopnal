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
    //JOP_REGISTER_LOADABLE(jop, Material)[](const void*, const json::Value& val) -> bool
    //{
    //    if (!val.HasMember("name") || !val["name"].IsString())
    //    {
    //        JOP_DEBUG_ERROR("Couldn't load material, no name found");
    //        return false;
    //    }
    //    Material::AttribType attr = Material::Attribute::Default;
    //    if (val.HasMember("attributes") && val["attributes"].IsUint())
    //        attr = val["attributes"].GetUint();
    //    else
    //        JOP_DEBUG_WARNING("No attribute field found in material \"" << val["name"].GetString() << "\", using defaults");

    //    auto& m = ResourceManager::getEmptyResource<Material>(val["name"].GetString(), attr);
    //    //m.setPersistent(val.HasMember("persistent") && val["persistent"].IsBool() ? val["persistent"].GetBool() : false);

    //    if (val.HasMember("reflection") && val["reflection"].IsArray() && val["reflection"].Size() >= 4)
    //    {
    //        auto& refArr = val["reflection"];
    //        for (auto& i : refArr)
    //            if (!i.IsUint())
    //                goto RefDone;

    //        m.setReflection(Color(refArr[0u].GetUint()), Color(refArr[1u].GetUint()), Color(refArr[2u].GetUint()), Color(refArr[3u].GetUint()));
    //    }

    //    RefDone:

    //    //if (val.HasMember("shininess") && val["shininess"].IsDouble())
    //    //    m.setShininess(static_cast<float>(val["shininess"].GetDouble()));
    //    //
    //    //if (val.HasMember("diffmap") && val["diffmap"].IsString())
    //    //    m.setMap(Material::Map::Diffuse, ResourceManager::getResource<Texture2D>(val["diffmap"].GetString()));
    //    //
    //    //if (val.HasMember("specmap") && val["specmap"].IsString())
    //    //    m.setMap(Material::Map::Specular, ResourceManager::getResource<Texture2D>(val["specmap"].GetString()));
    //    //
    //    //if (val.HasMember("emissmap") && val["emissmap"].IsString())
    //    //    m.setMap(Material::Map::Emission, ResourceManager::getResource<Texture2D>(val["emissmap"].GetString()));

    //    return true;
    //}
    //JOP_END_LOADABLE_REGISTRATION(Material)

    //JOP_REGISTER_SAVEABLE(jop, Material)[](const void* material, json::Value& val, json::Value::AllocatorType& alloc)
    //{
    //    auto& ref = *static_cast<const Material*>(material);

    //    val.AddMember(json::StringRef("name"), json::StringRef(ref.getName().c_str()), alloc);
    //    //val.AddMember(json::StringRef("persistent"), ref.isPersistent(), alloc);

    //    val.AddMember(json::StringRef("attributes"), ref.getAttributeField(), alloc);

    //    val.AddMember(json::StringRef("reflection"), json::kArrayType, alloc)["reflection"]
    //       .PushBack(ref.getReflection(Material::Reflection::Ambient).asInteger(), alloc)
    //       .PushBack(ref.getReflection(Material::Reflection::Diffuse).asInteger(), alloc)
    //       .PushBack(ref.getReflection(Material::Reflection::Specular).asInteger(), alloc)
    //       .PushBack(ref.getReflection(Material::Reflection::Emission).asInteger(), alloc);

    //    val.AddMember(json::StringRef("shininess"), ref.getShininess(), alloc);

    //    auto diff = ref.getMap(Material::Map::Diffuse);
    //    if (diff)
    //        val.AddMember(json::StringRef("diffmap"), json::StringRef(diff->getName().c_str()), alloc);

    //    auto spec = ref.getMap(Material::Map::Specular);
    //    if (spec)
    //        val.AddMember(json::StringRef("specmap"), json::StringRef(spec->getName().c_str()), alloc);

    //    auto emiss = ref.getMap(Material::Map::Emission);
    //    if (emiss)
    //        val.AddMember(json::StringRef("emissmap"), json::StringRef(emiss->getName().c_str()), alloc);

    //    return true;
    //}
    //JOP_END_SAVEABLE_REGISTRATION(Material);
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
    static const int ns_opacMapIndex = static_cast<int>(jop::Material::Map::Opacity);
    static const int ns_glossMapIndex = static_cast<int>(jop::Material::Map::Gloss);

    const std::array<jop::Color, 4>& getDefaultColors()
    {
        static const std::array<jop::Color, 4> colors =
        {
            jop::Color(jop::SettingManager::get<std::string>("engine@Graphics|Shading|Material|sDefaultAmbient", "000000FF")),
            jop::Color(jop::SettingManager::get<std::string>("engine@Graphics|Shading|Material|sDefaultDiffuse", "FFFFFFFF")),
            jop::Color(jop::SettingManager::get<std::string>("engine@Graphics|Shading|Material|sDefaultSpecular", "000000FF")),
            jop::Color(jop::SettingManager::get<std::string>("engine@Graphics|Shading|Material|sDefaultEmission", "000000FF"))
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
          m_shader              (),
          m_attributesChanged   (false),
          m_autoAttribs         (autoAttributes)
    {
        setMap(Map::Diffuse, Texture2D::getDefault());
    }

    Material::Material(const std::string& name, const AttribType attributes, const bool autoAttributes)
        : Resource              (name),
          m_reflection          (getDefaultColors()),
          m_attributes          (attributes),
          m_shininess           (1.f),
          m_maps                (),
          m_attributesChanged   (true),
          m_autoAttribs         (autoAttributes)
    {
        if (hasAttribute(Attribute::DiffuseMap))
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
          m_attributesChanged   (other.m_attributesChanged),
          m_autoAttribs         (other.m_autoAttribs)
    {}

    //////////////////////////////////////////////

    void Material::sendToShader(Shader& shader, const Camera* camera, const float alphaMult) const
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
                /* 14 */ "u_ReflectionMap",
                /* 15 */ "u_AlphaMult"
            };

            if (!hasAttribute(Attribute::__SkyBox))
            {
                // Send camera position to shader
                if (camera && hasAttribute(Attribute::__Lighting | Attribute::EnvironmentMap))
                    shader.setUniform(strCache[0], camera->getObject()->getGlobalPosition());

                if (hasAttribute(Attribute::__Lighting))
                {
                    shader.setUniform(strCache[1], m_reflection[ns_ambIndex].asRGBAVector());
                    shader.setUniform(strCache[2], m_reflection[ns_diffIndex].asRGBAVector());
                    shader.setUniform(strCache[3], m_reflection[ns_specIndex].asRGBAVector());
                    shader.setUniform(strCache[4], m_reflection[ns_emissIndex].asRGBAVector());
                    shader.setUniform(strCache[5], m_shininess);

                    if (hasAttribute(Attribute::EnvironmentMap))
                        shader.setUniform(strCache[6], m_reflectivity);
                }
                else
                    shader.setUniform(strCache[7], m_reflection[ns_emissIndex].asRGBAVector());

                if (hasAttribute(Attribute::DiffuseMap) && getMap(Map::Diffuse))
                    shader.setUniform(strCache[8], *getMap(Material::Map::Diffuse), ns_diffMapIndex);

                if (hasAttribute(Attribute::SpecularMap) && getMap(Map::Specular))
                    shader.setUniform(strCache[9], *getMap(Map::Specular), ns_specMapIndex);

                if (hasAttribute(Attribute::EmissionMap) && getMap(Map::Emission))
                    shader.setUniform(strCache[10], *getMap(Map::Emission), ns_emissMapIndex);

                if (hasAttribute(Attribute::OpacityMap) && getMap(Map::Opacity))
                    shader.setUniform(strCache[11], *getMap(Map::Opacity), ns_opacMapIndex);

                if (hasAttribute(Attribute::GlossMap) && getMap(Map::Gloss))
                    shader.setUniform(strCache[12], *getMap(Map::Gloss), ns_glossMapIndex);
            }

            shader.setUniform(strCache[15], alphaMult);

            if (hasAttribute(Attribute::EnvironmentMap) && getMap(Material::Map::Environment))
            {
                shader.setUniform(strCache[13], *getMap(Material::Map::Environment), ns_envMapIndex);

                if (hasAttribute(Attribute::ReflectionMap) && getMap(Map::Reflection))
                    shader.setUniform(strCache[14], *getMap(Material::Map::Reflection), ns_reflMapIndex);
            }
        }
    }

    //////////////////////////////////////////////

    Shader* Material::getShader() const
    {
        if (m_shader.expired() || m_attributesChanged)
        {
            m_shader = static_ref_cast<Shader>(ShaderAssembler::getShader(m_attributes).getReference());
            m_attributesChanged = false;
        }

        return m_shader.get();
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
        m_reflection[0] = ambient;
        m_reflection[1] = diffuse;
        m_reflection[2] = specular;

        return setReflection(Reflection::Emission, emission);
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
        const int mapIndex = static_cast<int>(map) - 1;
        m_maps[mapIndex] = static_ref_cast<const Texture>(tex.getReference());

        return addAttributes(detail::mapAttribs[mapIndex] * m_autoAttribs);
    }

    //////////////////////////////////////////////

    Material& Material::removeMap(const Map map)
    {
        const int mapIndex = static_cast<int>(map) - 1;
        m_maps[mapIndex].reset();

        return removeAttributes(detail::mapAttribs[mapIndex] * m_autoAttribs);
    }

    //////////////////////////////////////////////

    const Texture* Material::getMap(const Map map) const
    {
        return m_maps[static_cast<int>(map) - 1].get();
    }

    //////////////////////////////////////////////

    Material& Material::setAttributeField(const AttribType attribs)
    {
        m_attributesChanged = m_attributesChanged || m_attributes != attribs;

        m_attributes = attribs;
        return *this;
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

    bool Material::hasAttributes(const AttribType attribs) const
    {
        return (m_attributes & attribs) == attribs;
    }

    //////////////////////////////////////////////

    bool Material::compareAttributes(const AttribType attribs) const
    {
        return m_attributes == attribs;
    }

    //////////////////////////////////////////////

    Material& Material::addAttributes(const AttribType attribs)
    {
        return setAttributeField(getAttributeField() | attribs);
    }

    //////////////////////////////////////////////

    Material& Material::removeAttributes(const AttribType attribs)
    {
        return setAttributeField(getAttributeField() & ~attribs);
    }

    //////////////////////////////////////////////

    Material& Material::getDefault()
    {
        static WeakReference<Material> defMat;

        if (defMat.expired())
            defMat = static_ref_cast<Material>(ResourceManager::getEmptyResource<Material>("jop_default_material").getReference());

        return *defMat;
    }
}