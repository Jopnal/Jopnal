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
    Any::Concept::~Concept()
    {}

    //////////////////////////////////////////////

    Any::Any(const Any& other)
        : m_data()
    {
        *this = other;
    }

    Any::Any(std::nullptr_t)
        : m_data()
    {}

    Any& Any::operator =(const Any& other)
    {
        if (other.isValid())
            m_data = std::unique_ptr<Concept>(other.m_data->clone());

        return *this;
    }

    //////////////////////////////////////////////

    void Any::clear()
    {
        m_data.reset();
    }

    //////////////////////////////////////////////

    bool Any::isValid() const
    {
        return m_data.operator bool();
    }

    //////////////////////////////////////////////

    Any::operator bool() const
    {
        return isValid();
    }

    //////////////////////////////////////////////

    const std::type_info& Any::getType() const
    {
        return isValid() ? m_data->getType() : typeid(void);
    }

    //////////////////////////////////////////////

    bool Any::operator ==(const std::type_info& type) const
    {
        return getType() == type;
    }
}