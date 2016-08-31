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

    #include <Jopnal/Graphics/AnimatedSprite.hpp>

    #include <Jopnal/Graphics/AnimationAtlas.hpp>
    #include <Jopnal/Graphics/Mesh/RectangleMesh.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    AnimatedSprite::AnimatedSprite(Object& object, Renderer& renderer, const RenderPass::Pass pass)
        : Drawable          (object, renderer, pass),
          m_atlas           (),
          m_mesh            (""),
          m_material        ("", true),
          m_animationRange  (),
          m_frameTime       (0.f),
          m_timer           (0.f),
          m_status          (Status::Stopped),
          m_currentFrame    (0),
          m_repeats         (0)
    {
        setModel(Model(m_mesh, m_material));
    }

    AnimatedSprite::AnimatedSprite(Object& object, RenderPass& pass)
        : Drawable          (object, pass),
          m_atlas           (),
          m_mesh            (""),
          m_material        ("", true),
          m_animationRange  (),
          m_frameTime       (0.f),
          m_timer           (0.f),
          m_status          (Status::Stopped),
          m_currentFrame    (0),
          m_repeats         (0)
    {
        setModel(Model(m_mesh, m_material));
    }

    AnimatedSprite::~AnimatedSprite()
    {}

    //////////////////////////////////////////////

    void AnimatedSprite::update(const float deltaTime)
    {
        m_timer += deltaTime * (m_status == Status::Playing) * (m_repeats != 0);

        if (m_timer >= m_frameTime)
        {  
            if (++m_currentFrame > m_animationRange.second)
            {
                m_currentFrame = m_animationRange.first;
                m_repeats = std::max(-1, m_repeats - 1);
            }

            const auto coords = m_atlas->getCoordinates(m_currentFrame);
            m_mesh.load(m_atlas->getFrameSize().x, coords.first, coords.second);

            m_timer -= m_frameTime;
        } 
    }

    //////////////////////////////////////////////

    void AnimatedSprite::stop()
    {
        m_status = Status::Stopped;
        m_currentFrame = m_animationRange.first;
        m_repeats = 0;
    }

    //////////////////////////////////////////////

    void AnimatedSprite::play(const unsigned int repeats)
    {
        m_repeats = (repeats == 0 ? -1 : repeats);
        m_status = Status::Playing;
    }

    //////////////////////////////////////////////

    void AnimatedSprite::pause()
    {
        m_status = Status::Paused;
    }

    //////////////////////////////////////////////

    AnimatedSprite& AnimatedSprite::setAnimationRange(const uint32 startIndex, const uint32 endIndex)
    {
        m_animationRange = std::make_pair(startIndex, endIndex);
        m_currentFrame = startIndex;
        return *this;
    }

    //////////////////////////////////////////////

    AnimatedSprite& AnimatedSprite::setFrameTime(const float seconds)
    {
        m_frameTime = seconds;
        return *this;
    }

    //////////////////////////////////////////////

    AnimatedSprite& AnimatedSprite::setAtlas(const AnimationAtlas& atlas)
    {
        m_atlas = static_ref_cast<const AnimationAtlas>(atlas.getReference());
        m_material.setMap(Material::Map::Diffuse, atlas.getTexture());

        const auto coords = m_atlas->getCoordinates(0);
        m_mesh.load(m_atlas->getFrameSize().x, coords.first, coords.second);

        m_currentFrame = 0;

        return *this;
    }

    //////////////////////////////////////////////

    AnimatedSprite::Status AnimatedSprite::getStatus() const
    {
        return m_status;
    }

    //////////////////////////////////////////////

    int AnimatedSprite::getRemainingRepeats() const
    {
        return m_repeats;
    }
}