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
    Texture2DDepth::Texture2DDepth(const std::string& name)
        : Texture(name, gl::TEXTURE_2D)
    {
        static WeakReference<TextureSampler> sampler;

        if (sampler.expired())
        {
            if (ResourceManager::resourceExists<TextureSampler>("jop_depth_sampler"))
                sampler = static_ref_cast<TextureSampler>(ResourceManager::getExistingResource<TextureSampler>("jop_depth_sampler").getReference());
            else
            {
                sampler = static_ref_cast<TextureSampler>(ResourceManager::getEmptyResource<TextureSampler>("jop_depth_sampler").getReference());
                sampler->setPersistence(0);

                sampler->setFilterMode(TextureSampler::Filter::Bilinear);
                sampler->setRepeatMode(TextureSampler::Repeat::ClampBorder);
                sampler->setBorderColor(Color::White);
            }
        }

        setSampler(*sampler);
    }

    //////////////////////////////////////////////

    bool Texture2DDepth::load(const unsigned int width, const unsigned int height, const unsigned int bytes)
    {
        if (width > getMaximumSize() || height > getMaximumSize())
        {
            JOP_DEBUG_ERROR("Couldn't create depth texture, maximum size is " << getMaximumSize());
            return false;
        }

        bind();

        glCheck(gl::TexImage2D(gl::TEXTURE_2D, 0, getFormatEnum(bytes), width, height, 0, gl::DEPTH_COMPONENT, gl::FLOAT, NULL));

        m_width = width;
        m_height = height;
        m_bytes = bytes;

        return true;
    }

    //////////////////////////////////////////////

    unsigned int Texture2DDepth::getFormatEnum(const unsigned int bytes)
    {
        switch (bytes)
        {
            case 2:
                return gl::DEPTH_COMPONENT16;
            case 3:
                return gl::DEPTH_COMPONENT24;
            case 4:
                return gl::DEPTH_COMPONENT32;
            default:
                return gl::DEPTH_COMPONENT;
        }
    }
}