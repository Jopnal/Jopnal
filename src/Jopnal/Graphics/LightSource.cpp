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

///////////////////////////////////////////

// Headers
#include <Jopnal/Precompiled.hpp>

///////////////////////////////////////////


namespace jop
{
    JOP_DERIVED_COMMAND_HANDLER(Component, LightSource)

        JOP_BIND_MEMBER_COMMAND_NORETURN(&LightSource::setType, "setLightType");
        JOP_BIND_MEMBER_COMMAND_NORETURN((LightSource& (LightSource::*)(const LightSource::Intensity, const Color))&LightSource::setIntensity, "setIntensity");

    JOP_END_COMMAND_HANDLER(LightSource)
}

namespace jop
{
    JOP_REGISTER_LOADABLE(jop, LightSource)[](Object& obj, const Scene& scene, const json::Value& val) -> bool
    {
        auto light = obj.createComponent<LightSource>("");

        const char* const typeField = "type";
        if (val.HasMember(typeField) && val[typeField].IsUint())
            light->setType(static_cast<LightSource::Type>(std::min(2u, val[typeField].GetUint())));

        const char* const intensityField = "intensities";
        if (val.HasMember(intensityField) && val[intensityField].IsArray() && val[intensityField].Size() >= 3)
        {
            auto& intArr = val[intensityField];

            if (intArr[0u].IsUint() && intArr[1u].IsUint() && intArr[2u].IsUint())
            {
                light->setIntensity(Color(intArr[0u].GetUint()),
                                    Color(intArr[1u].GetUint()),
                                    Color(intArr[2u].GetUint()));
            }
            else
                JOP_DEBUG_WARNING("Encountered unexpected values while loading LightSource intensity for object with id \"" << obj.getID() << "\"");
        }

        const char* const attField = "attenuation";
        if (val.HasMember(attField) && val[attField].IsArray() && val[attField].Size() >= 4)
        {
            auto& attArr = val[attField];

            if (attArr[0u].IsDouble() && attArr[1u].IsDouble() && attArr[2u].IsDouble() && attArr[3u].IsDouble())
            {
                light->setAttenuation(static_cast<float>(attArr[0u].GetDouble()),
                                      static_cast<float>(attArr[1u].GetDouble()),
                                      static_cast<float>(attArr[2u].GetDouble()),
                                      static_cast<float>(attArr[3u].GetDouble()));
            }
            else
                JOP_DEBUG_WARNING("Encountered unexpected values while loading LightSource attenuation for object with id \"" << obj.getID() << "\"");
        }

        const char* const coField = "cutoff";
        if (val.HasMember(coField) && val[coField].IsArray() && val[coField].Size() >= 2)
        {
            auto& coArr = val[coField];

            if (coArr[0u].IsDouble() && coArr[1u].IsDouble())
            {
                light->setCutoff(static_cast<float>(coArr[0u].GetDouble()),
                                 static_cast<float>(coArr[1u].GetDouble()));
            }
            else
                JOP_DEBUG_WARNING("Encountered unexpected values while loading LightSource cutoff for object with id \"" << obj.getID() << "\"");
        }

        return Drawable::loadStateBase(*light, scene, val);
    }
    JOP_END_LOADABLE_REGISTRATION(LightSource)

    JOP_REGISTER_SAVEABLE(jop, LightSource)[](const Component& comp, json::Value& val, json::Value::AllocatorType& alloc) -> bool
    {
        auto& light = static_cast<const LightSource&>(comp);

        val.AddMember(json::StringRef("type"), static_cast<unsigned int>(light.getType()), alloc);

        val.AddMember(json::StringRef("intensities"), json::kArrayType, alloc)["intensities"]
           .PushBack(light.getIntensity(LightSource::Intensity::Ambient).asInteger(), alloc)
           .PushBack(light.getIntensity(LightSource::Intensity::Diffuse).asInteger(), alloc)
           .PushBack(light.getIntensity(LightSource::Intensity::Specular).asInteger(), alloc);

        val.AddMember(json::StringRef("attenuation"), json::kArrayType, alloc)["attenuation"]
           .PushBack(light.getAttenuation(LightSource::Attenuation::Constant), alloc)
           .PushBack(light.getAttenuation(LightSource::Attenuation::Linear), alloc)
           .PushBack(light.getAttenuation(LightSource::Attenuation::Quadratic), alloc)
           .PushBack(light.getAttenuation(LightSource::Attenuation::Range), alloc);

        val.AddMember(json::StringRef("cutoff"), json::kArrayType, alloc)["cutoff"]
           .PushBack(light.getCutoff().x, alloc)
           .PushBack(light.getCutoff().y, alloc);

        return Drawable::saveStateBase(light, val, alloc);
    }
    JOP_END_SAVEABLE_REGISTRATION(LightSource)
}

namespace jop
{
    LightSource::LightSource(Object& object, const std::string& ID)
        : Drawable      (object, ID),
          m_type        (Type::Point),
          m_intensities (),
          m_attenuation (1.f, 1.f, 2.f, 5.f),
          m_cutoff      (10.f, 10.f)
    {
        m_intensities[0] = Color::Black;    // Ambient
        m_intensities[1] = Color::White;    // Diffuse
        m_intensities[2] = Color::White;    // Specular
    }

    //////////////////////////////////////////////

    void LightSource::draw(const Camera&, const LightContainer&)
    {}

    ///////////////////////////////////////////

    LightSource& LightSource::setType(const Type type)
    {
        m_type = type;
        return *this;
    }

    ///////////////////////////////////////////

    LightSource::Type LightSource::getType() const 
    {
        return m_type;
    }

    ///////////////////////////////////////////

    LightSource& LightSource::setIntensity(const Intensity intensity, const Color color)
    {
        m_intensities[static_cast<int>(intensity)] = color;
        return *this;
    }

    ///////////////////////////////////////////

    LightSource& LightSource::setIntensity(const Color ambient, const Color diffuse, const Color specular)
    {
        return setIntensity(Intensity::Ambient, ambient)
              .setIntensity(Intensity::Diffuse, diffuse)
              .setIntensity(Intensity::Specular, specular);
    }

    //////////////////////////////////////////////

    LightSource& LightSource::setIntensity(const Color intensity)
    {
        return setIntensity(Intensity::Ambient, intensity)
              .setIntensity(LightSource::Intensity::Diffuse, intensity)
              .setIntensity(LightSource::Intensity::Specular, intensity);
    }

    ///////////////////////////////////////////

    Color LightSource::getIntensity(const Intensity intensity) const
    {
        return m_intensities[static_cast<int>(intensity)];
    }

    //////////////////////////////////////////////

    LightSource& LightSource::setAttenuation(const Attenuation attenuation, const float value)
    {
        m_attenuation[static_cast<int>(attenuation)] = value;
        return *this;
    }

    //////////////////////////////////////////////

    LightSource& LightSource::setAttenuation(const float constant, const float linear, const float quadratic, const float range)
    {
        m_attenuation = glm::vec4(constant, linear, quadratic, range);
        return *this;
    }

    //////////////////////////////////////////////

    LightSource& LightSource::setAttenuation(const AttenuationPreset preset)
    {
        static const glm::vec4 att[] =
        {
            //         Range    Constant    Linear      Quadratic
            glm::vec4( 7.f,     1.f,        0.7f,       1.8f),
            glm::vec4( 13.f,    1.f,        0.35f,      0.44f),
            glm::vec4( 20.f,    1.f,        0.22f,      0.20f),
            glm::vec4( 32.f,    1.f,        0.14f,      0.07f),
            glm::vec4( 50.f,    1.f,        0.09f,      0.032f),
            glm::vec4( 65.f,    1.f,        0.07f,      0.017f),
            glm::vec4( 100.f,   1.f,        0.045f,     0.0075f),
            glm::vec4( 160.f,   1.f,        0.27f,      0.0028f),
            glm::vec4( 200.f,   1.f,        0.22f,      0.0019f),
            glm::vec4( 325.f,   1.f,        0.14f,      0.0007f),
            glm::vec4( 600.f,   1.f,        0.007f,     0.0002f),
            glm::vec4( 3250.f,  1.f,        0.0014f,    0.000007f)
        };

        auto& v = att[static_cast<int>(preset)];
        return setAttenuation(v[1], v[2], v[3], v[0]);
    }

    //////////////////////////////////////////////

    float LightSource::getAttenuation(const Attenuation attenuation) const
    {
        return m_attenuation[static_cast<int>(attenuation)];
    }

    //////////////////////////////////////////////

    glm::vec3 LightSource::getAttenuationVec() const
    {
        return glm::vec3(m_attenuation);
    }

    //////////////////////////////////////////////

    LightSource& LightSource::setCutoff(const float inner, const float outer)
    {
        m_cutoff.x = inner; m_cutoff.y = outer;
        return *this;
    }

    //////////////////////////////////////////////

    const glm::vec2& LightSource::getCutoff() const
    {
        return m_cutoff;
    }

    //////////////////////////////////////////////

    unsigned int LightSource::getMaximumLights(const Type type)
    {
        static const unsigned int maxLights[] =
        {
            SettingManager::getUint("uMaxPointLights", 8),
            SettingManager::getUint("uMaxDirectionalLights", 2),
            SettingManager::getUint("uMaxSpotLights", 2)
        };

        return maxLights[static_cast<int>(type)];
    }


    //////////////////////////////////////////////


    LightContainer::LightContainer()
        : m_container()
    {
        clear();
    }

    //////////////////////////////////////////////

    bool LightContainer::empty() const
    {
        for (auto& i : m_container)
        {
            if (!i.empty())
                return false;
        }

        return true;
    }

    //////////////////////////////////////////////

    void LightContainer::clear()
    {
        for (auto& i : m_container)
            i.clear();

        (*this)[LightSource::Type::Point].reserve(LightSource::getMaximumLights(LightSource::Type::Point));
        (*this)[LightSource::Type::Directional].reserve(LightSource::getMaximumLights(LightSource::Type::Directional));
        (*this)[LightSource::Type::Spot].reserve(LightSource::getMaximumLights(LightSource::Type::Spot));
    }

    //////////////////////////////////////////////

    void LightContainer::sendToShader(Shader& shader) const
    {
        // Point lights
        {
            auto& points = (*this)[LightSource::Type::Point];
            shader.setUniform("u_NumPointLights", points.size());

            for (std::size_t i = 0; i < points.size(); ++i)
            {
                auto& li = *points[i];
                const std::string indexed = "u_PointLights[" + std::to_string(i) + "].";

                // Position
                shader.setUniform(indexed + "position", li.getObject()->extractPosition());

                // Intensity
                shader.setUniform(indexed + "ambient", li.getIntensity(LightSource::Intensity::Ambient).asRGBFloatVector());
                shader.setUniform(indexed + "diffuse", li.getIntensity(LightSource::Intensity::Diffuse).asRGBFloatVector());
                shader.setUniform(indexed + "specular", li.getIntensity(LightSource::Intensity::Specular).asRGBFloatVector());

                // Attenuation
                shader.setUniform(indexed + "attenuation", li.getAttenuationVec());
            }
        }

        // Directional lights
        {
            auto& dirs = (*this)[LightSource::Type::Directional];
            shader.setUniform("u_NumDirectionalLights", dirs.size());

            for (std::size_t i = 0; i < dirs.size(); ++i)
            {
                auto& li = *dirs[i];
                const std::string indexed = "u_DirectionalLights[" + std::to_string(i) + "].";

                // Direction
                shader.setUniform(indexed + "direction", li.getObject()->getFront());
                
                // Intensity
                shader.setUniform(indexed + "ambient", li.getIntensity(LightSource::Intensity::Ambient).asRGBFloatVector());
                shader.setUniform(indexed + "diffuse", li.getIntensity(LightSource::Intensity::Diffuse).asRGBFloatVector());
                shader.setUniform(indexed + "specular", li.getIntensity(LightSource::Intensity::Specular).asRGBFloatVector());
            }
        }

        // Spot lights
        {
            auto& spots = (*this)[LightSource::Type::Spot];
            shader.setUniform("u_NumSpotLights", spots.size());

            for (std::size_t i = 0; i < spots.size(); ++i)
            {
                auto& li = *spots[i];
                const std::string indexed = "u_SpotLights[" + std::to_string(i) + "].";

                // Position
                shader.setUniform(indexed + "position", li.getObject()->extractPosition());

                // Direction
                shader.setUniform(indexed + "direction", li.getObject()->getFront());

                // Intensity
                shader.setUniform(indexed + "ambient", li.getIntensity(LightSource::Intensity::Ambient).asRGBFloatVector());
                shader.setUniform(indexed + "diffuse", li.getIntensity(LightSource::Intensity::Diffuse).asRGBFloatVector());
                shader.setUniform(indexed + "specular", li.getIntensity(LightSource::Intensity::Specular).asRGBFloatVector());

                // Attenuation
                shader.setUniform(indexed + "attenuation", li.getAttenuationVec());

                // Cutoff
                shader.setUniform(indexed + "cutoff", glm::vec2(std::cos(li.getCutoff().x), std::cos(li.getCutoff().y)));
            }
        }
    }

    //////////////////////////////////////////////

    LightContainer::ContainerType& LightContainer::operator[](const LightSource::Type type)
    {
        return m_container[static_cast<int>(type)];
    }

    //////////////////////////////////////////////

    const LightContainer::ContainerType& LightContainer::operator[](const LightSource::Type type) const
    {
        return m_container[static_cast<int>(type)];
    }
}