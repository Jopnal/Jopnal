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

#ifndef JOP_COMPRESSEDTEXTURE2D_HPP
#define JOP_COMPRESSEDTEXTURE2D_HPP

// Headers
#include <Jopnal/Header.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API CompressedTexture2D final : public Texture
    {
    private:

        friend class Texture;

    public:

        /// \brief Constructor
        ///
        /// \param name Name
        ///
        CompressedTexture2D(const std::string& name);

        /// \brief Load from file
        ///
        /// Creates CompressedImage object and calls its load from file method
        ///
        bool load(const std::string& path);

        /// \brief Load from compressed image
        ///
        /// \param image Compressed image
        ///
        /// Gets necessary data from image and calls load from memory method
        ///
        bool load(const CompressedImage& image);

        /// \brief Get the texture size
        ///
        /// \return The size
        ///
        glm::uvec2 getSize() const override;

        ///// \brief Get the error texture
        /////
        ///// \return Reference to the texture
        /////
        //static CompressedTexture2D& getError();

        ///// \brief Get the default texture
        /////
        ///// \return Reference to the texture
        /////
        //static CompressedTexture2D& getDefault();

    private:

        glm::uvec2 m_size; ///< Size of the compressed texture
    };
}

#endif