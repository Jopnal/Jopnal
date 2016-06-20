//Jopnal Engine C++ Library
//Copyright(c) 2016 Team Jopnal
//
//This software is provided 'as-is', without any express or implied
//warranty.In no event will the authors be held liable for any damages
//arising from the use of this software.
//
//Permission is granted to anyone to use this software for any purpose,
//including commercial applications, and to alter it and redistribute it
//freely, subject to the following restrictions :
//
//1. The origin of this software must not be misrepresented; you must not
//claim that you wrote the original software.If you use this software
//in a product, an acknowledgement in the product documentation would be
//appreciated but is not required.
//2. Altered source versions must be plainly marked as such, and must not be
//misrepresented as being the original software.
//3. This notice may not be removed or altered from any source distribution.

//////////////////////////////////////////////

#ifndef JOP_IMAGE_HPP
#define JOP_IMAGE_HPP

// Headers
#include <Jopnal/Header.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Image
    {
    public:

        /// \brief Default constructor
        ///
        Image();

        /// \brief Create image and fill it with color
        ///
        void create(uint32 width, uint32 height, const Color& color);

        /// \brief Create image from an array of pixels
        ///
        void create(uint32 width, uint32 height, const unsigned char* pixels);

        /// \brief Method for using file loader to load new resource from file
        ///
        /// \param path The file path
        /// \param srgb Use SRGB color space?
        ///
        /// \return True if loading was successful
        ///
        bool load(const std::string& path, const bool srgb);

        /// \brief Load the image from memory
        ///
        bool load(const void* data, uint32 size);

        /// \brief Return the size of the image
        ///
        glm::vec2 getSize() const;

        /// \brief Copy pixels from another image into this one
        ///
        /// \param sourceImg Source image to copy
        /// \param x X coordinate of the destination position
        /// \param y Y coordinate of the destination position
        /// \param sourceRect Sub-rectangle of the source image to copy
        /// \param applyAlpha Should the copy take into account the source transparency
        ///
        void copy(const Image& sourceImg, uint32 x, uint32 y, const Rect sourceRect, bool applyAlhpa);

        /// \brief Change the color of a pixel
        ///
        /// \param x X coordinate of pixel to change
        /// \param y Y coordinate of pixel to change
        /// \param color New color of the pixel
        ///
        void setPixel(uint32 x, uint32 y, const Color& color);

        /// \brief Get the color of a pixel
        ///
        /// \param x X coordinate of the target pixel
        /// \param y Y coordinate of the target pixel
        ///
        const Color getPixelColor(uint32 x, uint32 y) const;

        /// \brief Flip the image horizontally
        ///
        void flipHorizontally();

        /// \brief Flip the image vertically
        ///
        void flipVertically();

    private:
        glm::vec2           m_size;
        std::vector<uint32> m_pixels;
    };
}


#endif