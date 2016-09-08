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

#ifndef JOP_CUBEMAP_HPP
#define JOP_CUBEMAP_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Texture/Texture.hpp>
#include <Jopnal/Graphics/Image.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Cubemap final : public Texture
    {
    public:

        /// Cube map faces
        ///
        enum class Face
        {
            Right,
            Left,
            Top,
            Bottom,
            Back,
            Front,

            First = Right
        };

    public:

        /// \brief Constructor
        ///
        /// \param name Name of this resource
        ///
        Cubemap(const std::string& name);


        /// \brief Load a cube map from files
        ///
        /// \param right Right side
        /// \param left Left side
        /// \param top Top side
        /// \param bottom Bottom side
        /// \param back Back side
        /// \param front Front side
        /// \param flags Texture flags
        ///
        /// \return True if successful
        ///
        bool load(const std::string& right, const std::string& left,
                  const std::string& top, const std::string& bottom,
                  const std::string& back, const std::string& front,
                  const uint32 flags = 0);

        /// \brief Load an empty cube map
        ///
        /// \param size The size of a single face
        /// \param format The texture format
        /// \param flags Texture flags
        ///
        /// \return True if successful
        ///
        bool load(const glm::uvec2& size, const Format format, const uint32 flags = 0);

        /// \brief Load from .dds file
        ///
        /// Creates Image object and calls its load from file method
        /// \note Only use this with compressed .dds images that contain a full cube map (6 faces)
        ///
        /// \param path Path to file
        /// \param flags Texture flags
        ///
        /// \return True if successful
        ///
        bool load(const std::string& path, const uint32 flags = 0);

        /// \brief Load cube map from compressed image containing a cube map (DXT1/3/5)
        ///
        /// Gets 6 faces from the compressed image and goes through them and their mipmaps.
        /// \note Only works with compressed images that contain a full cube map (6 faces).
        ///
        /// \param image Image containing a cube map
        /// \param flags Texture flags
        ///
        /// \return True if successful
        ///
        bool load(const Image& image, const uint32 flags = 0);

        /// \copydoc Texture::getSize()
        ///
        glm::uvec2 getSize() const override;

        /// \copydoc Texture::getPixelDepth()
        ///
        unsigned int getPixelDepth() const override;

        /// \brief Get the maximum supported cube map size on this system
        ///
        /// \return The maximum cube map size
        ///
        static unsigned int getMaximumSize();

        /// \brief Get the error cube map
        ///
        /// \return Reference to the cube map
        ///
        static Cubemap& getError();

        /// \brief Get the default cube map
        ///
        /// \return Reference to the cube map
        ///
        static Cubemap& getDefault();

    private:

        glm::uvec2 m_size; ///< Size
    };
}

/// \class jop::Cubemap
/// \ingroup graphics

#endif