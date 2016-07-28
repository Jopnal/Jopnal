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

#ifndef JOP_ANIMATIONATLAS_HPP
#define JOP_ANIMATIONATLAS_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Texture/Texture2D.hpp>
#include <Jopnal/Graphics/Texture/TextureAtlas.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API AnimationAtlas : public TextureAtlas
    {
    public:

        /// \brief Constructor
        ///
        /// \param name Name for the resource
        ///
        AnimationAtlas(const std::string& name);

        /// \brief Destructor
        ///
        ~AnimationAtlas();

        /// \brief Load animation atlas from texture
        ///
        /// Creates an animation atlas from texture. Uses texture atlas to load the texture
        /// and automatically calculates the coordinates for each frame.
        ///
        /// \param path Path to the file which contains texture for animation atlas
        /// \param frames How many frames are there in the texture (X, Y)
        ///
        bool load(const std::string& path, const glm::uvec2& frames);

        /// \brief Load animation atlas from part of texture
        ///
        /// Creates an animation atlas from part of texture. Uses texture atlas to load the texture
        /// and automatically calculates the coordinates for each frame.
        ///
        /// \param path Path to the file which contains texture for animation atlas
        /// \param frames How many frames are there in the texture (X, Y)
        /// \param rectMin Minimum coordinates for the rectangle bounding the frames
        /// \param rectMax Maximum coordinates for the rectangle bounding the frames
        ///
        bool load(const std::string& path, const glm::uvec2& frames, const glm::uvec2& rectMin, const glm::uvec2& rectMax);

        /// \brief Get size of a single frame
        ///
        const glm::vec2& getFrameSize() const;

    private:

        glm::vec2 m_frameSize; ///< Size of frame
    };
}

#endif