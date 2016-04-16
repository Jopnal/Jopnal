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

#ifndef JOP_CUBEMAPDEPTH_HPP
#define JOP_CUBEMAPDEPTH_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Texture.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API CubemapDepth final : public Texture
    {
    public:

        /// \brief Constructor
        ///
        /// \param name Name of this resource
        ///
        CubemapDepth(const std::string& name);


        /// \brief Load this cube map
        ///
        /// \param size The size
        /// \param bytes Bytes per pixel
        ///
        /// \return True if loaded successfully
        ///
        bool load(const glm::uvec2& size, const unsigned int bytes);


        /// \brief Get the texture size
        ///
        /// \return The size
        ///
        glm::uvec2 getSize() const override;

    private:

        glm::uvec2 m_size;      ///< Size
        unsigned int m_bytes;   ///< Bytes per pixel
    };
}

#endif