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

#ifndef JOP_ANIMATEDSPRITE_HPP
#define JOP_ANIMATEDSPRITE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/GenericDrawable.hpp>
#include <Jopnal/Graphics/Material.hpp>
#include <Jopnal/Graphics/AnimationAtlas.hpp>
#include <Jopnal/Graphics/Mesh/RectangleMesh.hpp>

//////////////////////////////////////////////


namespace jop
{

    class JOP_API AnimatedSprite : public GenericDrawable
    {
    public:

        enum class Status
        {
            Playing,
            Paused,
            Stopped
        };

    public:

        AnimatedSprite(Object& object, Renderer& renderer);

        ~AnimatedSprite();

        /// \brief 
        ///
        void update(const float deltaTime) override;

        /// \brief
        ///
        void stop();

        /// \brief
        ///
        void play();

        /// \brief
        ///
        void pause();

        /// \brief Set animation range
        ///
        /// \param startIndex todo
        /// \param endIndex todo
        ///
        AnimatedSprite& setAnimationRange(const uint32 startIndex, const uint32 endIndex);

        /// \brief Set frame time
        ///
        /// \param seconds todo
        ///
        AnimatedSprite& setFrameTime(const float seconds);

        /// \brief Set animation atlas
        ///
        /// \param atlas todo
        ///
        AnimatedSprite& setAtlas(const AnimationAtlas& atlas);

        /// \brief Get status (Playing / Paused / Stopped)
        ///
        Status getStatus() const;

    private:

        WeakReference<const AnimationAtlas> m_atlas;
        std::unique_ptr<RectangleMesh> m_mesh; ///< 
        Material m_material; ///<
        std::pair<uint32, uint32> m_animationRange; ///< Animation range (Start - End)
        float m_frameTime;
        float m_timer;
        Status m_status;
        int m_currentFrame;
    };

}

#endif