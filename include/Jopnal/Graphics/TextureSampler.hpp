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

#ifndef JOP_TEXTURESAMPLER_HPP
#define JOP_TEXTURESAMPLER_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/Resource.hpp>
#include <Jopnal/Graphics/Color.hpp>
#include <memory>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API TextureSampler : public Resource
    {
    private:

        JOP_DISALLOW_COPY(TextureSampler);

    public:

        /// The filtering mode
        ///
        enum Filter
        {
            None,
            Bilinear,
            Trilinear,
            Anisotropic
        };

        /// The repeat mode
        ///
        enum Repeat
        {
            Basic,
            Mirrored,
            ClampEdge,
            ClampBorder
        };

    public:

        /// \brief Constructor
        /// 
        /// This will create a sampler object but not modify its values
        ///
        TextureSampler(const std::string& name);

        /// \brief Move constructor
        ///
        TextureSampler(TextureSampler&& other);

        /// \brief Move assignment operator
        ///
        TextureSampler& operator =(TextureSampler&& other);

        /// \brief Destructor
        ///
        ~TextureSampler();

        
        /// \brief (Re)load this sampler
        ///
        /// \param filterMode The filtering mode
        /// \param repeatMode The repeating mode
        /// \param param Anisotropic filtering level
        ///
        /// \return True if successful
        ///
        bool load(const Filter filterMode, const Repeat repeatMode, const float param = 1.f);

        /// \brief Bind this sampler
        ///
        /// \param textureUnit The texture unit to bind this sample into
        ///
        void bind(const unsigned int textureUnit) const;

        /// \brief Delete and recreate this sampler, clearing all the settings
        ///
        void reset();

        /// \brief Set the filtering mode
        ///
        /// \param mode The filtering mode
        /// \param param Possible anisotropic filtering level
        ///
        void setFilterMode(const Filter mode, const float param = 1.f);

        /// \brief Set the repeating mode
        ///
        /// \param repeat The repeating mode
        ///
        void setRepeatMode(const Repeat repeat);

        /// \brief Set the border color
        ///
        /// \param color The border color
        ///
        void setBorderColor(const Color& color);

        /// \brief Get the OpenGL handle
        ///
        /// \return The OpenGL handle
        ///
        unsigned int getHandle() const;

        /// \brief Get the filtering mode
        ///
        /// \return The filtering mode
        ///
        Filter getFilteringMode() const;

        /// \brief Get the repeating mode
        ///
        /// \return The repeating mode
        ///
        Repeat getRepeatMode() const;

        /// \brief Get the anisotropy level
        ///
        /// \return The anisotropy level
        ///
        float getAnisotropyLevel() const;

        /// \brief Get the border color
        ///
        /// \return The border color
        ///
        Color getBorderColor() const;

        /// \brief Get the maximum anisotropy level supported by the system
        ///
        /// \return The maximum anisotropy level. Zero if not supported
        ///
        static float getMaxAnisotropy();

        /// \brief Get the default sampler
        ///
        /// \return Reference to the sampler
        ///
        static TextureSampler& getDefault();

    private:

        Filter m_filter;        ///< The filtering mode
        Repeat m_repeat;        ///< The repeating mode
        float m_anisotropic;    ///< The anisotropic level
        Color m_borderColor;    ///< The border color
        unsigned int m_sampler; ///< The OpenGL sampler handle

    };
}

#endif

/// \class TextureSampler
/// \ingroup graphics