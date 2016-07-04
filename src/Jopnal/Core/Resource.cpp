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

    #include <Jopnal/Core/Resource.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    Resource::Resource(const std::string& name)
        : SafeReferenceable<Resource>   (this),
          m_name                        (name),
          m_persistence                 (USHRT_MAX)
    {}

    Resource::Resource(const Resource& other, const std::string& newName)
        : SafeReferenceable<Resource>   (this),
          m_name                        (newName),
          m_persistence                 (other.m_persistence)
    {}

    Resource::~Resource()
    {}

    //////////////////////////////////////////////

    const std::string& Resource::getName() const
    {
        return m_name;
    }

    //////////////////////////////////////////////

    void Resource::setPersistence(const unsigned short persistence)
    {
        m_persistence = persistence;
    }

    //////////////////////////////////////////////

    unsigned short Resource::getPersistence() const
    {
        return m_persistence;
    }
}