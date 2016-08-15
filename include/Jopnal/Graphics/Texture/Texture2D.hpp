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
#include <Jopnal/Graphics/Texture/Texture.hpp>
#include <Jopnal/Graphics/Image.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Texture2D : public Texture
    {
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
        bool load(const std::string& path, const uint32 flags = 0);

        /// \brief Load from memory
        ///
        /// \param ptr Pointer to data (pixels)
        /// \param size Size of the data to be loaded
        /// \param srgb Use SRGB color space?
        ///
        bool load(const void* ptr, const uint32 size, const uint32 flags = 0);

        /// \brief Creates flat/empty texture
        ///
        /// \param size The size
        /// \param bytesPerPixel The byte depth
        /// \param srgb Use SRGB color space?
        ///
        /// \return True if loading was successful
        ///
        bool load(const glm::uvec2& size, const Format format, const uint32 flags = 0);

        /// \brief Create a texture from an array of pixels
        ///
        /// \param size The size
        /// \param bytesPerPixel The byte depth of the image
        /// \param pixels Pointer to the beginning of the pixel array
        /// \param srgb Use SRGB color space?
        ///
        /// \return True if loading was successful
        ///
        bool load(const glm::uvec2& size, const Format format, const void* pixels, const uint32 flags = 0);

        /// \brief Load texture from compressed image
        ///
        /// \param image Image to load into the texture
        /// \param srgb Use SRGB color space?
        ///
        bool load(const Image& image, const uint32 flags = 0);

        /// \brief Set a subset of pixels
        ///
        /// The byte depth must be the same as this texture's!
        ///
        /// \param start The starting point
        /// \param size The size of the area
        /// \param pixels Pointer to the pixels
        ///
        void setPixels(const glm::uvec2& start, const glm::uvec2& size, const void* pixels);

        void setPixels(const glm::uvec2& start, const Image& image);

        /// \brief Get the texture size
        ///
        /// \return The size
        ///
        glm::uvec2 getSize() const override;

        /// \brief Returns textures bytes per pixel value
        ///
        unsigned int getPixelDepth() const override;

        /// \brief Copies the texture into an image and returns it
        ///
        Image getImage() const;

        /// \brief Get the maximum supported texture size of this system
        ///
        static unsigned int getMaximumSize();

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

        glm::uvec2  m_size; ///< Size
    };
}

#endif