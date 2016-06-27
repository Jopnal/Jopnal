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

#ifndef JOP_COMPRESSEDIMAGE_HPP
#define JOP_COMPRESSEDIMAGE_HPP

// Headers
#include <Jopnal/Header.hpp>

//////////////////////////////////////////////


namespace jop
{

    class JOP_API CompressedImage : public Resource
    {
    public:

        enum class Format
        {
            DXT1RGBA,
            DXT3RGBA,
            DXT5RGBA,
        };

        /// \brief Constructor
        ///
        CompressedImage(const std::string& name);

        /// \brief Load from file
        ///
        /// \param path Filepath to .dds compressed image
        ///
        bool load(const std::string& path);

        /// \brief For internal use
        ///
        bool load(const int id);

        /// \brief Returns compressed data
        ///
        const uint8* getPixels() const;

        /// brief Get size of the compressed image
        ///
        glm::uvec2 getSize() const;

        /// Get internal compressed image format
        ///
        /// (DXT1 / DXT3 / DXT5)
        ///
        Format getFormat() const;

        /// Get amount of mip map levels in image
        ///
        unsigned int getMipMapCount() const;

        /// \brief Does image contain a cube map?
        ///
        bool isCubemap() const;

    private:

        std::vector<uint8>  m_pixels;       ///< Compressed pixel data
        glm::uvec2          m_size;         ///< Size of the compressed image
        Format              m_format;       ///< Compressed image format
        unsigned int        m_mipMapLevels; ///< Count of mipmap levels
        bool                m_isCubemap;

    };
}

#endif