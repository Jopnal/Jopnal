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

        void unbind() const;


        /// \brief Bind a texture sampler
        ///
        /// \param sampler The sampler
        ///
        void setSampler(const TextureSampler& sampler);


        bool isValid() const;


        /// \brief Get the OpenGL handle for this texture
        ///
        unsigned int getHandle() const;

        /// \brief Get the maximum supported texture size of this system
        ///
        static unsigned int getMaximumSize();

        static unsigned int getMaxTextureUnits();


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

        mutable WeakReference<const TextureSampler> m_sampler;  ///< Texture sampler
        mutable unsigned int m_texture;                         ///< The OpenGL handle
        const unsigned int m_target;

    };
}

#endif

/// \class Texture
/// \ingroup graphics

/// stores and creates image data