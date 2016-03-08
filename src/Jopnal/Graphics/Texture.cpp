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
    {
        setSampler(TextureSampler::getDefault());
    }

    Texture::~Texture()
    {
        destroy();
    }

    //////////////////////////////////////////////

    void Texture::destroy()
    {
        if (m_texture)
        {
            glCheck(gl::DeleteTextures(1, &m_texture));
            m_texture = 0;
        }
    }

    //////////////////////////////////////////////

    void Texture::bind(const unsigned int texUnit) const
    {
        if (!m_texture)
            glCheck(gl::GenTextures(1, &m_texture));

        glCheck(gl::ActiveTexture(gl::TEXTURE0 + texUnit));
        glCheck(gl::BindTexture(m_target, m_texture));

        if (!m_sampler.expired())
            m_sampler = static_ref_cast<const TextureSampler>(TextureSampler::getDefault().getReference());

        m_sampler->bind(texUnit);
    }

    //////////////////////////////////////////////

    void Texture::setSampler(const TextureSampler& sampler)
    {
        m_sampler = static_ref_cast<const TextureSampler>(sampler.getReference());
    }

    //////////////////////////////////////////////

    int Texture::getMaximumSize()
    {
        static int size = 0;
        if (!size)
            glCheck(gl::GetIntegerv(gl::MAX_TEXTURE_SIZE, &size));

        return size;
    }

    //////////////////////////////////////////////

    Texture& Texture::getError()
    {
        static WeakReference<Texture2D> errTex;

        if (errTex.expired())
        {
            errTex = static_ref_cast<Texture2D>(ResourceManager::getEmptyResource<Texture2D>("jop_error_texture").getReference());

            JOP_ASSERT_EVAL(errTex->load(IDB_PNG2), "Failed to load error texture!");

            errTex->setPersistent(true);
            errTex->setManaged(true);
        }

        return *errTex;
    }

    //////////////////////////////////////////////

    Texture& Texture::getDefault()
    {
        static WeakReference<Texture2D> defTex;

        if (defTex.expired())
        {
            defTex = static_ref_cast<Texture2D>(ResourceManager::getEmptyResource<Texture2D>("jop_default_texture").getReference());
            
            JOP_ASSERT_EVAL(defTex->load(IDB_PNG1), "Failed to load default texture!");

            defTex->setPersistent(true);
            defTex->setManaged(true);
        }

        return *defTex;
    }

    //////////////////////////////////////////////

    unsigned int Texture::getHandle() const
    {
        return m_texture;
    }
}