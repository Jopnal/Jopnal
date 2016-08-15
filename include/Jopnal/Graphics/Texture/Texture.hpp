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
#include <Jopnal/Core/Resource.hpp>
#include <Jopnal/Graphics/Texture/TextureSampler.hpp>
#include <glm/vec2.hpp>
#include <string>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Texture : public Resource
    {
    private:

        JOP_DISALLOW_COPY_MOVE(Texture);

    public:

        struct Flag
        {
            enum : uint32
            {
                DisallowSRGB,
                DisallowMipmapGeneration,
                DisallowCompression
            };
        };

        enum class Format
        {
            None, ///< For internal functionality, do not use

            Alpha_UB_8,
            RGB_UB_8,
            RGBA_UB_8,

            RGB_F_16,
            RGBA_F_16,

            Depth_US_16,
            Depth_UI_24,

            Stencil_UB_8,

            DepthStencil_UI_24_B_8
        };

    protected:

        class FormatBundle
        {
            JOP_DISALLOW_COPY_MOVE(FormatBundle);

        public:

            FormatBundle(const Format format, const bool srgb);

            bool check() const;

            const uint32 intFormat;
            const uint32 format;
            const uint32 type;
        };

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
        bool bind(const unsigned int texUnit = 0) const;

        void unbind(const unsigned int texUnit = 0) const;


        /// \brief Check if this texture is valid
        ///
        /// \return True if a valid texture handle exists
        ///
        bool isValid() const;


        void setSampler(const TextureSampler& sampler);

        const TextureSampler* getSampler() const;


        /// \brief Get the texture size
        ///
        /// \return The size
        ///
        virtual glm::uvec2 getSize() const = 0;

        virtual unsigned int getPixelDepth() const = 0;


        Texture& setFilterMode(const TextureSampler::Filter mode, const float param = 1.f);

        /// \brief Set the repeating mode
        ///
        /// \param repeat The repeating mode
        ///
        /// \return Reference to self
        ///
        Texture& setRepeatMode(const TextureSampler::Repeat repeat);

        /// \brief Set the border color
        ///
        /// \param color The border color
        ///
        /// \return Reference to self
        ///
        Texture& setBorderColor(const Color& color);

        /// \brief Get the filtering mode
        ///
        /// \return The filtering mode
        ///
        TextureSampler::Filter getFilteringMode() const;

        /// \brief Get the repeating mode
        ///
        /// \return The repeating mode
        ///
        TextureSampler::Repeat getRepeatMode() const;

        /// \brief Get the anisotropy level
        ///
        /// \return The anisotropy level
        ///
        float getAnisotropyLevel() const;

        /// \brief Get the border color
        ///
        /// \return The border color
        ///
        const Color& getBorderColor() const;


        /// \brief Get the OpenGL handle for this texture
        ///
        unsigned int getHandle() const;

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
        static void setUnpackAlignment(const Format format);

        static bool allowSRGB();

        static bool allowGenMipmaps(const glm::uvec2& size, const bool srgb);

    protected:

        static Format getFormatFromDepth(const uint32 depth);

        static unsigned int getDepthFromFormat(const Format format);

    private:

        void updateSampling() const;

        WeakReference<const TextureSampler> m_sampler;  ///< Texture sampler
        mutable unsigned int m_texture;                 ///< The OpenGL handle
        const unsigned int m_target;                    ///< The OpenGL texture target
        TextureSampler::Filter m_filter;                ///< The filtering mode
        TextureSampler::Repeat m_repeat;                ///< The repeating mode
        float m_anisotropic;                            ///< The anisotropic level
        Color m_borderColor;                            ///< The border color

    protected:

        Format m_format;    ///< Must be filled by derived classes during load()
    };
}

#endif

/// \class jop::Texture
/// \ingroup graphics