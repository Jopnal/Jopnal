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

#ifndef JOP_TEXTURE_HPP
#define JOP_TEXTURE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <string>

//////////////////////////////////////////////


namespace jop
{
    class TextureSampler;

    class Texture : public Resource
    {
    public:

        /// \brief Constructor
        ///
        Texture(const std::string& name);

        /// \brief Destructor
        ///
        ~Texture() override;


        /// \brief Method for using file loader to load new resource from file
        ///
        /// \param path The file path
        ///
        /// \return True if loading was successful
        ///
        bool load(const std::string& path);

        /// \brief Creates flat/empty texture
        ///
        /// \param x The desired width
        /// \param y The desired height
        /// \param bytesPerPixel The byte depth
        ///
        /// \return True if loading was successful
        ///
        bool load(const int x, const int y, const int bytesPerPixel);

        /// \brief Create a texture from an array of pixels
        ///
        /// \param x Width of the texture
        /// \param y Height of the texture
        /// \param bytesPerPixel The byte depth of the image
        /// \param pixels Pointer to the beginning of the pixel array
        ///
        /// \return True if loading was successful
        ///
        bool load(const int x, const int y, const int bytesPerPixel, const unsigned char* pixels);

        /// \brief Destroy this texture, erasing it from video memory
        ///
        void destroy();


        /// \brief Bind this texture
        ///
        /// \param texUnit The texture unit to bind this texture to
        ///
        /// \return True if successful
        ///
        bool bind(const unsigned int texUnit = 0) const;

        /// \brief Unbind the texture bound to the given texture unit
        ///
        /// \param texUnit The texture unit to unbind the texture from
        ///
        static void unbind(const unsigned int texUnit);


        /// \brief Bind a texture sampler
        ///
        /// \param sampler The sampler
        ///
        void setTextureSampler(const TextureSampler& sampler);


        /// \brief Set a subset of pixels
        ///
        /// The byte depth must be the same as this texture's!
        ///
        /// \param x The X starting point
        /// \param y The Y starting point
        /// \param width Width
        /// \param height Height
        /// \param pixels Pointer to the pixels
        ///
        void setPixels(const int x, const int y, const int width, const int height, const unsigned char* pixels);


        /// \brief Returns image's width
        ///
        int getWidth() const;

        /// \brief Returns image's height
        ///
        int getHeight() const;

        /// \brief Returns image's bytes per pixel value
        ///
        int getDepth() const;

        /// \brief Get the OpenGL handle for this texture
        ///
        unsigned int getHandle() const;

        /// \brief Get the maximum supported texture size of this system
        ///
        static int getMaximumSize();

        /// \brief Get the error texture
        ///
        /// \return Reference to the texture
        ///
        static Texture& getError();

        /// \brief Get the default texture
        ///
        /// \return Reference to the texture
        ///
        static Texture& getDefault();

    private:

        /// \brief Load from dll
        ///
        /// This is for internal use only
        ///
        bool load(const int id);

        /// \brief Check if a byte depth is valid
        ///
        bool checkDepthValid(const int depth) const;


        mutable std::weak_ptr<const TextureSampler> m_sampler;  ///< Texture sampler
        int m_width;                                            ///< Width of the texture
        int m_height;                                           ///< Height of the texture
        int m_bytesPerPixel;                                    ///< Byte depth of the texture
        unsigned int m_texture;                                 ///< The OpenGL handle

    };
}

#endif

/// \class Texture
/// \ingroup graphics

/// stores and creates image data