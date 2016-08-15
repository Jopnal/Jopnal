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

#ifndef JOP_IMAGE_HPP
#define JOP_IMAGE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <glm/vec2.hpp>
#include <string>
#include <vector>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Image
    {
    public:

        /// Compressed image formats
        ///
        enum class Format
        {
            DXT1RGB,
            DXT1RGBA,
            DXT3RGBA,
            DXT5RGBA,
        }; 

    public:

        /// \brief Default constructor
        ///
        Image();


        /// \brief Method for using file loader to load new resource from file
        ///
        /// \param path The file path
        ///
        /// \return True if loading was successful
        ///
        bool load(const std::string& path, const bool allowCompression = true);

        /// \brief Load the image from memory
        ///
        /// \param ptr Pointer to data
        /// \param size Size of the data
        ///
        bool load(const void* ptr, const uint32 size);

        /// \brief Load the image from memory
        ///
        /// \param size The size
        /// \param bytesPerPixel The byte depth of the image
        /// \param pixels Pointer to the beginning of the pixel array
        ///
        /// \return True if loading was successful
        ///
        bool load(const glm::uvec2& size, const uint32 bytesPerpixel, const unsigned char* pixels);

        /// \brief Return the dimensions of the image
        ///
        glm::uvec2 getSize() const;

        unsigned int getPixelSize() const;

        /// \brief Get image depth (bytes per pixel)
        ///
        uint32 getDepth() const;

        /// \brief Get pixels in image
        ///
        const uint8* getPixels() const;

        /// Get internal compressed image format
        ///
        /// (DXT1 / DXT3 / DXT5)
        ///
        Format getFormat() const;

        /// Get amount of mipmap levels in compressed image
        ///
        unsigned int getMipMapCount() const;

        /// \brief Does image contain a cube map?
        ///
        bool isCubemap() const;

        /// \brief Is image compressed (DDS)
        ///
        bool isCompressed() const;

        /// \brief Flip image vertically
        ///
        void flipVertically();

        /// brief Flip image horizontally
        ///
        void flipHorizontally();

    private:

        /// \brief Compress uncompressed image
        ///
        /// Compresses image to DXT1 format if RGB color space
        /// or DXT5 format if RGBA color space
        ///
        bool compress(const bool allowCompression);

        std::vector<uint8>  m_pixels;           ///< Pixels in image
        uint32              m_bytesPerPixel;    ///< Image depth
        glm::uvec2          m_size;             ///< Size of the image
        Format              m_format;           ///< Compressed image format (DDS)
        unsigned int        m_mipMapLevels;     ///< Count of mipmap levels (DDS)
        bool                m_isCubemap;        ///< true if compressed image contains cubemap (DDS)
        bool                m_isCompressed;     ///< Is image compressed?
    };
}

#endif