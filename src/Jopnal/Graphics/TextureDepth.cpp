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
    TextureDepth::TextureDepth(const std::string& name)
        : Texture(name, gl::TEXTURE_2D)
    {
        static WeakReference<TextureSampler> sampler;

        if (sampler.expired())
        {
            sampler = static_ref_cast<TextureSampler>(ResourceManager::getEmptyResource<TextureSampler>("jop_depth_sampler").getReference());

            sampler->setFilterMode(TextureSampler::Filter::Bilinear);
            sampler->setRepeatMode(TextureSampler::Repeat::ClampBorder);
            sampler->setBorderColor(Color::White);

            setSampler(*sampler);
        }
    }

    //////////////////////////////////////////////

    bool TextureDepth::load(const int width, const int height)
    {
        if (width > getMaximumSize() || height > getMaximumSize())
        {

            return false;
        }
        else if (width <= 0 || height <= 0)
        {

            return false;
        }

        bind();

        gl::TexImage2D(gl::TEXTURE_2D, 0, gl::DEPTH_COMPONENT, width, height, 0, gl::DEPTH_COMPONENT, gl::FLOAT, NULL);

        m_width = width;
        m_height = height;

        return true;
    }
}