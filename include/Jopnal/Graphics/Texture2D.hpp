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

#ifndef JOP_TEXTURE2D_HPP
#define JOP_TEXTURE2D_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Texture.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Texture2D final : public Texture
    {
    private:

        friend class Texture;

    public:

        /// \brief Constructor
        ///
        /// \param name Name of the resource
        ///
        Texture2D(const std::string& name);


        /// \brief Method for using file loader to load new resource from file
        ///
        /// \param path The file path
        /// \param srgb Use SRGB color space?
        ///
        /// \return True if loading was successful
        ///
        bool load(const std::string& path, const bool srgb);

        /// \brief Creates flat/empty texture
        ///
        /// \param size The size
        /// \param bytesPerPixel The byte depth
        /// \param srgb Use SRGB color space?
        ///
        /// \return True if loading was successful
        ///
        bool load(const glm::uvec2& size, const unsigned int bytesPerPixel, const bool srgb);

        /// \brief Create a texture from an array of pixels
        ///
        /// The accepted pixel depth values are 1, 3 and 4.
        ///
        /// \param size The size
        /// \param bytesPerPixel The byte depth of the image
        /// \param pixels Pointer to the beginning of the pixel array
        /// \param srgb Use SRGB color space?
        ///
        /// \return True if loading was successful
        ///
        bool load(const glm::uvec2& size, const unsigned int bytesPerPixel, const unsigned char* pixels, const bool srgb);


        /// \brief Set a subset of pixels
        ///
        /// The byte depth must be the same as this texture's!
        ///
        /// \param start The starting point
        /// \param size The size of the area
        /// \param pixels Pointer to the pixels
        ///
        void setPixels(const glm::uvec2& start, const glm::uvec2& size, const unsigned char* pixels);


        /// \brief Get the texture size
        ///
        /// \return The size
        ///
        glm::uvec2 getSize() const override;

        /// \brief Returns image's bytes per pixel value
        ///
        unsigned int getDepth() const;

        /// \brief Get the OpenGL format enum
        ///
        /// \param bytesPerPixel Bytes per pixel
        ///
        /// \return The OpenGL format enum
        ///
        static unsigned int getFormatEnum(const unsigned int bytesPerPixel);

        /// \brief Get the OpenGL internal format enum
        ///
        /// \param formatEnum The external format enum fetched with getFormatEnum()
        /// \param srgb Use SRGB color space?
        ///
        /// \return The OpenGL internal format enum
        ///
        static unsigned int getInternalFormatEnum(const unsigned int formatEnum, const bool srgb);

        /// \brief Check if the pixel depth value is supported
        ///
        /// \param depth The pixel depth in bytes
        ///
        /// \return True if the depth is supported
        ///
        static bool checkDepthValid(const unsigned int depth);


        /// \brief Get the error texture
        ///
        /// \return Reference to the texture
        ///
        static Texture2D& getError();

        /// \brief Get the default texture
        ///
        /// \return Reference to the texture
        ///
        static Texture2D& getDefault();

    private:

        /// For internal use
        ///
        bool load(const int id, const bool srgb);


        glm::uvec2 m_size;              ///< Size
        unsigned int m_bytesPerPixel;   ///< Byte depth of the texture
    };
}

#endif