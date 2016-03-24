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
//    in a product, an acknowledgment in the product documentation would be
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
    JOP_REGISTER_LOADABLE(jop, SphereMesh)[](const void*, const json::Value& val)
    {
        if (!val.HasMember("name") || !val["name"].IsString())
        {
            JOP_DEBUG_ERROR("Couldn't load SphereMesh, no name found");
            return false;
        }

        float radius = 1.f;
        unsigned int rings = 20;
        unsigned int sectors = 20;
        bool norm = true;

        // Radius
        if (val.HasMember("radius") && val["radius"].IsDouble())
            radius = static_cast<float>(val["radius"].GetDouble());

        // Rings
        if (val.HasMember("rings") && val["rings"].IsUint())
            rings = val["rings"].GetUint();

        // Sectors
        if (val.HasMember("sectors") && val["sectors"].IsUint())
            sectors = val["sectors"].GetUint();

        // Normalized?
        if (val.HasMember("normtc") && val["normtc"].IsBool())
            norm = val["normtc"].GetBool();

        ResourceManager::getNamedResource<SphereMesh>(val["name"].GetString(), radius, rings, sectors, norm)
;//            .setPersistent(val.HasMember("persistent") && val["persistent"].IsBool() ? val["persistent"].GetBool() : false);

        return true;
    }
    JOP_END_LOADABLE_REGISTRATION(SphereMesh)

    JOP_REGISTER_SAVEABLE(jop, SphereMesh)[](const void* sphere, json::Value& val, json::Value::AllocatorType& alloc)
    {
        const SphereMesh& ref = *static_cast<const SphereMesh*>(sphere);

        val.AddMember(json::StringRef("name"), json::StringRef(ref.getName().c_str()), alloc)
           .AddMember(json::StringRef("radius"), ref.getRadius(), alloc)
           .AddMember(json::StringRef("rings"), ref.getRings(), alloc)
           .AddMember(json::StringRef("sectors"), ref.getSectors(), alloc)
           .AddMember(json::StringRef("normtc"), ref.normalizedTexCoords(), alloc)
;//           .AddMember(json::StringRef("persistent"), ref.isPersistent(), alloc);

        return true;
    }
    JOP_END_SAVEABLE_REGISTRATION(SphereMesh)
}

namespace jop
{    
    SphereMesh::SphereMesh(const std::string& name)
        : Mesh(name)
    {}

    SphereMesh::SphereMesh(const std::string& name, const float radius, const unsigned int rings, const unsigned int sectors, const bool normalizedTexCoords)
        : Mesh(name)
    {
        load(radius, rings, sectors, normalizedTexCoords);
    }

    //////////////////////////////////////////////

    bool SphereMesh::load(const float radius, const unsigned int rings, const unsigned int sectors, const bool normalizedTexCoords)
    {
        m_radius = radius;
        m_rings = rings;
        m_sectors = sectors;
        m_normTexCoords = normalizedTexCoords;

        const float R = 1.0f / static_cast<float>(rings - 1);
        const float S = 1.0f / static_cast<float>(sectors - 1);

        const unsigned int texCoordMultS = normalizedTexCoords ? 1u : sectors;
        const unsigned int texCoordMultR = normalizedTexCoords ? 1u : rings;

        std::vector<Vertex> vertexArray(rings * sectors);
        auto itr = vertexArray.begin();

        for (std::size_t s = 0; s < sectors; ++s)
        {
            for (std::size_t r = 0; r < rings; ++r)
            {
                static const float pi1 = glm::pi<float>();
                static const float pi2 = glm::half_pi<float>();

                const float y = sin(-pi2 + pi1 * r * R);
                const float x = cos(2 * pi1 * s * S) * sin(pi1 * r * R);
                const float z = sin(2 * pi1 * s * S) * sin(pi1 * r * R);

                itr->position.x = x * radius;
                itr->position.y = y * radius;
                itr->position.z = z * radius;

                itr->texCoords.x = texCoordMultS * s * -S;
                itr->texCoords.y = texCoordMultR * r * R;

                itr->normalVector.x = x;
                itr->normalVector.y = y;
                itr->normalVector.z = z;

                ++itr;
            }
        }

        std::vector<unsigned int> indices(rings * sectors * 6);
        std::vector<unsigned int>::iterator i = indices.begin();
        for (std::size_t r = 0; r < rings - 1; ++r)
        {
            for (std::size_t s = 0; s < sectors - 1; ++s)
            {
                // First triangle
                *i++ = r * sectors + s;             // 0, 0          
                *i++ = r * sectors + (s + 1);       // 1, 0
                *i++ = (r + 1) * sectors + (s + 1); // 1, 1

                // Second triangle
                *i++ = r * sectors + s;             // 0, 0    
                *i++ = (r + 1) * sectors + (s + 1); // 1, 1
                *i++ = (r + 1) * sectors + s;       // 0, 1
            }
        }

        return Mesh::load(vertexArray, indices);
    }

    //////////////////////////////////////////////

    float SphereMesh::getRadius() const
    {
        return m_radius;
    }

    //////////////////////////////////////////////

    unsigned int SphereMesh::getRings() const
    {
        return m_rings;
    }

    //////////////////////////////////////////////

    unsigned int SphereMesh::getSectors() const
    {
        return m_sectors;
    }

    //////////////////////////////////////////////

    bool SphereMesh::normalizedTexCoords() const
    {
        return m_normTexCoords;
    }
}