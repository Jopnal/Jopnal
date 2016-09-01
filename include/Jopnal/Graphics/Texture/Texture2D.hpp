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


        /// \brief Load from file
        ///
        /// \param path The file path
        /// \param flags Texture flags
        ///
        /// \return True if successful
        ///
        bool load(const std::string& path, const uint32 flags = 0);

        /// \brief Load from memory
        ///
        /// \param ptr Pointer to data (pixels)
        /// \param size Size of the data to be loaded in bytes
        /// \param flags Texture flags
        ///
        /// \return True if successful
        ///
        bool load(const void* ptr, const uint32 size, const uint32 flags = 0);

        /// \brief Creates flat/empty texture
        ///
        /// \param size The texture dimensions
        /// \param format The texture format
        /// \param flags Texture flags
        ///
        /// \return True if successful
        ///
        bool load(const glm::uvec2& size, const Format format, const uint32 flags = 0);

        /// \brief Create a texture from an array of pixels
        ///
        /// \param size The texture dimensions
        /// \param format The texture format
        /// \param pixels Pointer to the beginning of the pixel array. Data must match the format
        /// \param flags Texture flags
        ///
        /// \return True if successful
        ///
        bool load(const glm::uvec2& size, const Format format, const void* pixels, const uint32 flags = 0);

        /// \brief Load from a compressed image
        ///
        /// \param image Image to load from
        /// \param flags Texture flags
        ///
        bool load(const Image& image, const uint32 flags = 0);

        /// \brief Set a subset of pixels
        ///
        /// \param start The starting coordinates in pixels
        /// \param size The size of the area in pixels
        /// \param pixels Pointer to the pixels. The data must match the texture format
        ///
        void setPixels(const glm::uvec2& start, const glm::uvec2& size, const void* pixels);

        /// \brief Set a subset of pixels from an image
        ///
        /// The image must not be compressed.
        ///
        /// \param start The starting coordinates in pixels
        /// \param image The image to set the pixels from
        ///
        void setPixels(const glm::uvec2& start, const Image& image);

        /// \copydoc Texture::getSize()
        ///
        glm::uvec2 getSize() const override;

        /// \copydoc Texture::getPixelDepth()
        ///
        unsigned int getPixelDepth() const override;

        /// \brief Copy the texture into an image
        ///
        /// \return A new image with the texture pixels
        ///
        Image getImage() const;

        /// \brief Get the maximum supported 2D texture size on this system
        ///
        /// \return The maximum texture size
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

        glm::uvec2 m_size; ///< Size
    };
}

/// \class jop::Texture2D
/// \ingroup graphics

#endif