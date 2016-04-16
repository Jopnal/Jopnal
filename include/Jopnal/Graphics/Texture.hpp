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
#include <Jopnal/Graphics/TextureSampler.hpp>
#include <Jopnal/MathInclude.hpp>
#include <string>

//////////////////////////////////////////////


namespace jop
{
    class TextureSampler;

    class JOP_API Texture : public Resource
    {
    private:

        JOP_DISALLOW_COPY_MOVE(Texture);

    public:

        /// \brief Constructor
        ///
        Texture(const std::string& name, const unsigned int glTarget);

        /// \brief Destructor
        ///
        virtual ~Texture() override = 0;


        /// \brief Destroy this texture, erasing it from video memory
        ///
        void destroy();


        /// \brief Bind this texture
        ///
        /// \param texUnit The texture unit to bind this texture to
        ///
        /// \return True if successful
        ///
        void bind(const unsigned int texUnit = 1) const;


        /// \brief Get the sampler
        ///
        /// \return Reference to the internal sampler
        ///
        TextureSampler& getSampler();

        /// \copydoc getSampler()
        ///
        const TextureSampler& getSampler() const;


        /// \brief Check if this texture is valid
        ///
        /// \return True if a valid texture handle exists
        ///
        bool isValid() const;


        /// \brief Get the texture size
        ///
        /// \return The size
        ///
        virtual glm::uvec2 getSize() const = 0;


        /// \brief Get the OpenGL handle for this texture
        ///
        unsigned int getHandle() const;

        /// \brief Get the maximum supported texture size of this system
        ///
        static unsigned int getMaximumSize();

        /// \brief Get the maximum texture unit value
        ///
        /// \return The maximum texture unit value. Sampler cannot be bound to
        ///         texture units which have a greater number than this
        ///
        static unsigned int getMaxTextureUnits();

        /// \brief Set the pixel store alignment
        ///
        /// This set the OpenGL UNPACK_ALIGNMENT parameter appropriately.
        ///
        /// \param depth The pixel byte depth
        ///
        static void setPixelStore(const unsigned int depth);

    private:

        TextureSampler m_sampler;       ///< Texture sampler
        mutable unsigned int m_texture; ///< The OpenGL handle
        const unsigned int m_target;    ///< The OpenGL texture target
    };
}

#endif

/// \class Texture
/// \ingroup graphics

/// stores and creates image data