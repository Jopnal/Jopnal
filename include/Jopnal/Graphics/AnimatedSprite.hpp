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
#include <Jopnal/Graphics/Drawable.hpp>
#include <Jopnal/Graphics/Material.hpp>

//////////////////////////////////////////////


namespace jop
{
    class AnimationAtlas;
    class RectangleMesh;

    class JOP_API AnimatedSprite : public Drawable
    {
    public:

        enum class Status
        {
            Playing,
            Paused,
            Stopped
        };

    public:

        /// \brief Constructor
        ///
        /// \param object Reference to the object which this component is getting bound to
        /// \param renderer Reference to the renderer
        ///
        AnimatedSprite(Object& object, Renderer& renderer);

        /// \brief Destructor
        ///
        ~AnimatedSprite();

        /// \brief Update sprite animation
        ///
        /// Cycles through animation range with given frame time
        ///
        void update(const float deltaTime) override;

        /// \brief Stop animating
        ///
        void stop();

        /// \brief Play animation
        ///
        void play();

        /// \brief Pause animation
        ///
        void pause();

        /// \brief Set animation range
        ///
        /// \param startIndex The first frame in the range
        /// \param endIndex The last frame in the range
        ///
        AnimatedSprite& setAnimationRange(const uint32 startIndex, const uint32 endIndex);

        /// \brief Set frame time
        ///
        /// \param seconds Time taken for each frame (1 / 60 = 60FPS)
        ///
        AnimatedSprite& setFrameTime(const float seconds);

        /// \brief Set animation atlas
        ///
        /// \param atlas Reference to the animation atlas holding the frames
        ///
        AnimatedSprite& setAtlas(const AnimationAtlas& atlas);

        /// \brief Get status (Playing / Paused / Stopped)
        ///
        Status getStatus() const;

    private:

        WeakReference<const AnimationAtlas> m_atlas;    ///< Reference to the animation atlas
        std::unique_ptr<RectangleMesh> m_mesh;          ///< Mesh to be drawn on
        Material m_material;                            ///< Material to be drawn with
        std::pair<uint32, uint32> m_animationRange;     ///< Animation range (Start - End)
        float m_frameTime;                              ///< Time taken for each frame
        float m_timer; 
        Status m_status;                                ///< Animation status
        unsigned int m_currentFrame;
    };
}

#endif