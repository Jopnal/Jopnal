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

	#include <Jopnal/Graphics/Texture/Texture2DDepth.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    Texture2DDepth::Texture2DDepth(const std::string& name)
        : Texture   (name, GL_TEXTURE_2D),
          m_size    (),
          m_bytes   (0)
    {
        getSampler().setFilterMode(TextureSampler::Filter::Bilinear).setRepeatMode(TextureSampler::Repeat::ClampBorder).setBorderColor(Color::White);
    }

    //////////////////////////////////////////////

    bool Texture2DDepth::load(const glm::uvec2& size, const unsigned int bytes)
    {
        if (size.x > getMaximumSize() || size.y > getMaximumSize())
        {
            JOP_DEBUG_ERROR("Couldn't create depth texture, maximum size is " << getMaximumSize());
            return false;
        }

        bind();

        glCheck(glTexImage2D(GL_TEXTURE_2D, 0, getFormatEnum(bytes), size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));

        m_size = size;
        m_bytes = bytes;

        return true;
    }

    //////////////////////////////////////////////

    glm::uvec2 Texture2DDepth::getSize() const
    {
        return m_size;
    }

    //////////////////////////////////////////////

    unsigned int Texture2DDepth::getFormatEnum(const unsigned int bytes)
    {
        switch (bytes)
        {
            case 2:
                return GL_DEPTH_COMPONENT16;
            case 3:
                return GL_DEPTH_COMPONENT24;
            case 4:
                return GL_DEPTH_COMPONENT32;
            default:
                return GL_DEPTH_COMPONENT;
        }
    }
}