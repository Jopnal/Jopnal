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

//headers
#include <Jopnal/Precompiled.hpp>

///////////////////////////////////////////


namespace jop
{
    LightSource::LightSource(Object& object, const std::string& ID)
        : Component     (object, ID),
          m_type        (),
          m_intensities ()
    {
        std::memset(m_intensities.data(), 255, m_intensities.size()* sizeof(Color));
    }

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

    Color LightSource::getIntensity(const Intensity intensity)
    {
        return m_intensities[static_cast<int>(intensity)];
    }
}