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

        Texture2D(const std::string& name);


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
        /// The accepted pixel depth values are 1, 3 and 4.
        ///
        /// \param x Width of the texture
        /// \param y Height of the texture
        /// \param bytesPerPixel The byte depth of the image
        /// \param pixels Pointer to the beginning of the pixel array
        ///
        /// \return True if loading was successful
        ///
        bool load(const int x, const int y, const int bytesPerPixel, const unsigned char* pixels);


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

        static unsigned int getFormatEnum(const int bytesPerPixel);

        static unsigned int getInternalFormatEnum(const int formatEnum);

        static bool checkDepthValid(const int depth);

    private:

        bool load(const int id);


        int m_width;            ///< Width of the texture
        int m_height;           ///< Height of the texture
        int m_bytesPerPixel;    ///< Byte depth of the texture

    };
}

#endif