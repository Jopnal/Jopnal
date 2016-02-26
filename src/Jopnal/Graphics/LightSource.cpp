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
        JOP_BIND_MEMBER_COMMAND_NORETURN((LightSource& (LightSource::*)(const LightSource::Intensity, const Color))&LightSource::setIntensity, "setLightIntensity");

    JOP_END_COMMAND_HANDLER(LightSource)
}

namespace jop
{
    JOP_REGISTER_LOADABLE(jop, LightSource)[](Object& obj, const Scene& scene, const json::Value& val) -> bool
    {
        auto& light = obj.createComponent<LightSource>("");

        const char* const typeField = "type";
        if (val.HasMember(typeField) && val[typeField].IsUint())
            light.setType(static_cast<LightSource::Type>(std::min(2u, val[typeField].GetUint())));

        const char* const intensityField = "intensities";
        if (val.HasMember(intensityField) && val[intensityField].IsArray() && val[intensityField].Size() >= 3)
        {
            auto& intArr = val[intensityField];

            if (intArr[0u].IsUint() && intArr[1u].IsUint() && intArr[2u].IsUint())
            {
                light.setIntensity(Color(intArr[0u].GetUint()),
                                   Color(intArr[1u].GetUint()),
                                   Color(intArr[2u].GetUint()));
            }
            else
                JOP_DEBUG_WARNING("Encountered unexpected values while loading LightSource for object with id \"" << obj.getID() << "\"");
        }

        return Drawable::loadStateBase(light, scene, val);
    }
    JOP_END_LOADABLE_REGISTRATION(LightSource)

    JOP_REGISTER_SAVEABLE(jop, LightSource)[](const Component& comp, json::Value& val, json::Value::AllocatorType& alloc) -> bool
    {
        auto& light = static_cast<const LightSource&>(comp);

        val.AddMember(json::StringRef("type"), static_cast<unsigned int>(light.getType()), alloc);

        auto& intArr = val.AddMember(json::StringRef("intensities"), json::kArrayType, alloc)["intensities"];
        intArr.PushBack(light.getIntensity(LightSource::Intensity::Ambient).asInteger(), alloc)
              .PushBack(light.getIntensity(LightSource::Intensity::Diffuse).asInteger(), alloc)
              .PushBack(light.getIntensity(LightSource::Intensity::Specular).asInteger(), alloc);

        return Drawable::saveStateBase(light, val, alloc);
    }
    JOP_END_SAVEABLE_REGISTRATION(LightSource)
}

namespace jop
{
    LightSource::LightSource(Object& object, const std::string& ID)
        : Drawable      (object, ID),
          m_type        (),
          m_intensities ()
    {
        std::memset(m_intensities.data(), 255, m_intensities.size() * sizeof(Color));
    }

    //////////////////////////////////////////////

    void LightSource::draw(const Camera&)
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

    ///////////////////////////////////////////

    Color LightSource::getIntensity(const Intensity intensity) const
    {
        return m_intensities[static_cast<int>(intensity)];
    }
}