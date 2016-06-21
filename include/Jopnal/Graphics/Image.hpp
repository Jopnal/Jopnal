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

        /// \brief Method for using file loader to load new resource from file
        ///
        /// \param path The file path
        /// \param srgb Use SRGB color space?
        ///
        /// \return True if loading was successful
        ///
        bool load(const std::string& path);

        /// \brief Load the image from memory
        ///
        /// \param size The size
        /// \param bytesPerPixel The byte depth of the image
        /// \param pixels Pointer to the beginning of the pixel array
        /// \param srgb Use SRGB color space?
        ///
        /// \return True if loading was successful
        ///
        bool load(const glm::uvec2& size, const uint32 bytesPerpixel, const unsigned char* pixels);

        /// \brief Load from resource
        ///
        /// \param id Resource id
        /// \param srgb Use SRGB color space?
        ///
        bool load(const int id, const bool srgb);

        /// \brief Return the size of the image
        ///
        glm::uvec2 getSize() const;

        /// \brief Get image depth (bytes per pixel)
        ///
        uint32 getDepth() const;

        /// \brief Get pixels in image
        ///
        const uint8* getPixels() const;

        /// \brief Check if the pixel depth value is supported
        ///
        /// \param depth The pixel depth in bytes
        ///
        /// \return True if the depth is supported
        ///
         static bool checkDepthValid(const uint32 depth);

    private:
        glm::uvec2           m_size;
        std::vector<uint8>  m_pixels;
        uint32              m_bytesPerPixel;
    };
}


#endif