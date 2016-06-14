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

#ifndef JOP_PRECOMPILED_HEADER

	#include <Jopnal/Graphics/Texture/Texture.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    /*JOP_REGISTER_LOADABLE(jop, Texture)[](const void*, const json::Value& val)
    {
        if (!val.HasMember("name") || !val["name"].IsString())
        {
            JOP_DEBUG_ERROR("Couldn't load Texture, no name found");
            return false;
        }

        ResourceManager::getResource<Texture>(val["name"].GetString())
            .setPersistent(val.HasMember("persistent") && val["persistent"].IsBool() ? val["persistent"].GetBool() : false);

        return true;
    }
    JOP_END_LOADABLE_REGISTRATION(Texture)

    JOP_REGISTER_SAVEABLE(jop, Texture)[](const void* texture, json::Value& val, json::Value::AllocatorType& alloc)
    {
        val.AddMember(json::StringRef("name"), json::StringRef(static_cast<const Texture*>(texture)->getName().c_str()), alloc);

        return true;
    }
    JOP_END_SAVEABLE_REGISTRATION(Texture)*/
}

namespace jop
{
    Texture::Texture(const std::string& name, const unsigned int glTarget)
        : Resource          (name),
          m_sampler         (),
          m_texture         (0),
          m_target          (glTarget)
    {}

    Texture::~Texture()
    {
        destroy();
    }

    //////////////////////////////////////////////

    void Texture::destroy()
    {
        if (m_texture)
        {
            glCheck(glDeleteTextures(1, &m_texture));
            m_texture = 0;
        }
    }

    //////////////////////////////////////////////

    void Texture::bind(const unsigned int texUnit) const
    {
        if (!m_texture)
            glCheck(glGenTextures(1, &m_texture));

        glCheck(glActiveTexture(GL_TEXTURE0 + texUnit));
        glCheck(glBindTexture(m_target, m_texture));

        m_sampler.bind(texUnit);
    }

    //////////////////////////////////////////////

    TextureSampler& Texture::getSampler()
    {
        return m_sampler;
    }

    //////////////////////////////////////////////

    const TextureSampler& Texture::getSampler() const
    {
        return m_sampler;
    }

    //////////////////////////////////////////////

    bool Texture::isValid() const
    {
        return m_texture != 0;
    }

    //////////////////////////////////////////////

    unsigned int Texture::getHandle() const
    {
        return m_texture;
    }

    //////////////////////////////////////////////

    unsigned int Texture::getMaximumSize()
    {
        static unsigned int size = 0;
        if (!size)
            glCheck(glGetIntegerv(GL_MAX_TEXTURE_SIZE, reinterpret_cast<GLint*>(&size)));

        return size;
    }

    //////////////////////////////////////////////

    unsigned int Texture::getMaxTextureUnits()
    {
        static unsigned int maxUnits = 0;
        if (!maxUnits)
            glCheck(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, reinterpret_cast<GLint*>(&maxUnits)));

        return maxUnits;
    }

    //////////////////////////////////////////////

    void Texture::setPixelStore(const unsigned int depth)
    {
        GLint param = 4;
        switch (depth)
        {
            case 1:
                param = 1;
                break;

            case 2:
                param = 2;
                break;

            case 8:
                param = 8;
        }

        glCheck(glPixelStorei(GL_UNPACK_ALIGNMENT, param));
    }
}