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

#include <Jopnal/Graphics/AnimationAtlas.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    AnimationAtlas::AnimationAtlas(const std::string& name)
        : TextureAtlas(name),
          m_frameSize(0)
    {}

    AnimationAtlas::~AnimationAtlas()
    {}

    //////////////////////////////////////////////

    bool AnimationAtlas::load(const std::string& path, const glm::uvec2& frames)
    {
        const auto& texture = ResourceManager::get<jop::Texture2D>(path, true, true, false);
        return load(path, frames, glm::uvec2(0), glm::uvec2(0));        
    }

    //////////////////////////////////////////////

    bool AnimationAtlas::load(const std::string& path, const glm::uvec2& frames, const glm::uvec2& rectMin, const glm::uvec2& rectMax)
    {
        TextureAtlas::load(path, TextureAtlas::LoadMode::TextureOnly);

        glm::vec2 size(0, 0);

        if (rectMin.x > 0 && rectMin.y > 0 && rectMax.x > 0 && rectMax.y > 0)
            size = rectMax - rectMin;
        else
            size = getTexture().getSize();

        m_frameSize.x = size.x / frames.x;
        m_frameSize.y = size.y / frames.y;

        for (size_t row = 0; row < frames.y; ++row)
        {
            for (size_t col = 0; col < frames.x; ++col)
            {
                defineTexture(glm::vec2(rectMin.x + (m_frameSize.x * col), rectMin.y + (m_frameSize.y * row)), glm::vec2(rectMin.x + m_frameSize.x + (m_frameSize.x * col), rectMin.y + m_frameSize.y + (m_frameSize.y * row)));
            }
        }

        return true;


    }


    const glm::vec2& AnimationAtlas::getFrameSize() const
    {
        return m_frameSize;
    }



}