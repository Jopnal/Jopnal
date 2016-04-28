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
    SerializeInfo::SerializeInfo()
        : m_package(),
          m_shouldSerialize(true),
          m_light(false)
    {

    }

    //////////////////////////////////////////////

    void SerializeInfo::setSerializePackage(const uint16 package)
    {
        m_package = package;
    }

    uint16 SerializeInfo::getSerializePackage() const
    {
        return m_package;
    }

    //////////////////////////////////////////////

    void SerializeInfo::setShouldSerialize(const bool set)
    {
        m_shouldSerialize = set;
    }

    bool SerializeInfo::shouldSerialize() const
    {
        return m_shouldSerialize;
    }

    //////////////////////////////////////////////

    void SerializeInfo::setLightSerializeable(const bool set)
    {
        m_light = set;
    }

    bool SerializeInfo::isLightSerializeable() const
    {
        return m_light;
    }
}