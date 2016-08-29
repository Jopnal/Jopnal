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

        /// Texture flags
        ///
        struct Flag
        {
            enum : uint32
            {
                DisallowSRGB,               ///< Disallow the use of the sRGB color space
                DisallowMipmapGeneration,   ///< Disallow automatic mipmap generation
                DisallowCompression         ///< Disallow automatic compression
            };
        };

        /// Texture format
        ///
        enum class Format
        {
            None,       ///< For internal functionality, do not use (except with RenderTexture)

            Alpha_UB_8, ///< 8 bit alpha texture
            RGB_UB_8,   ///< 24 bit RGB texture
            RGBA_UB_8,  ///< 32 bit RGBA texture

            /// 48 bit floating point RGB texture
            ///
            /// \warning On GLES 2.0, supported only with the extension GL_OES_texture_half_float
            ///
            RGB_F_16,

            /// 64 bit floating point RGBA texture
            ///
            /// \warning On GLES 2.0, supported only with the extension GL_OES_texture_half_float
            ///
            RGBA_F_16,

            /// 16 bit depth texture
            ///
            /// \warning On GLES 2.0, supported only with the extension OES_depth_texture
            ///
            Depth_US_16,

            /// 24 bit depth texture
            ///
            /// \warning On GLES 2.0, supported only with the extension OES_depth_texture
            ///
            Depth_UI_24,

            /// 8 bit stencil texture
            ///
            /// \warning Not supported on GLES 3.0 or lesser. On GLES 3.1 supported with the extension OES_texture_stencil8
            ///
            Stencil_UB_8,

            /// 24 bit depth, 8 bit stencil
            ///
            /// \warning On GLES 2.0, supported only with the extension OES_packed_depth_stencil
            ///
            DepthStencil_UI_24_B_8
        };

    protected:

        /// Helper class for managing texture formats
        ///
        class FormatBundle
        {
            JOP_DISALLOW_COPY_MOVE(FormatBundle);

        public:

            /// \brief Initialize this bundle
            ///
            /// \param format The texture format
            /// \param srgb Use the sRGB color space?
            ///
            FormatBundle(const Format format, const bool srgb);

            /// \brief Check if this format bundle is valid
            ///
            /// The format can usually only be invalid if the specific format
            /// is not supported by OpenGL.
            ///
            /// \return True if valid
            ///
            bool check() const;

            const uint32 intFormat; ///< Internal format
            const uint32 format;    ///< Format
            const uint32 type;      ///< Type
        };

    public:

        /// \brief Constructor
        ///
        /// \param name Name of the resource
        /// \param glTarget The OpenGL texture target, e.g. GL_TEXTURE_2D
        ///
        Texture(const std::string& name, const unsigned int glTarget);

        /// \brief Virtual destructor
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

        /// \brief Unbind a current texture
        ///
        /// \param texUnit The texture unit to unbind a texture from
        ///
        void unbind(const unsigned int texUnit = 0) const;

        /// \brief Check if this texture is valid
        ///
        /// \return True if a valid texture handle exists
        ///
        bool isValid() const;

        /// \brief Set a texture sampler
        ///
        /// After this call, this texture will use the sampling parameters defined
        /// by the sampler object until it is removed.
        ///
        /// \param sampler The texture sampler to set
        ///
        /// \see removeSampler()
        ///
        void setSampler(const TextureSampler& sampler);

        /// \brief Remove the bound texture sampler
        ///
        /// After this call, the texture-specific sampling parameters will be used.
        ///
        /// \see setSampler()
        ///
        void removeSampler();

        /// \brief Get the currently bound sampler
        ///
        /// \return Pointer to the sampler. nullptr if none is bound
        ///
        const TextureSampler* getSampler() const;

        /// \brief Get the texture size
        ///
        /// \return The texture size
        ///
        virtual glm::uvec2 getSize() const = 0;

        /// \brief Get the texture pixel depth
        ///
        /// \return The texture pixel depth in bytes
        ///
        virtual unsigned int getPixelDepth() const = 0;

        /// \copydoc TextureSampler::setFilterMode()
        ///
        Texture& setFilterMode(const TextureSampler::Filter mode, const float param = 1.f);

        /// \copydoc TextureSampler::setRepeatMode()
        ///
        Texture& setRepeatMode(const TextureSampler::Repeat repeat);

        /// \copydoc TextureSampler::setBorderColor()
        ///
        Texture& setBorderColor(const Color& color);

        /// \copydoc TextureSampler::getFilterMode()
        ///
        TextureSampler::Filter getFilterMode() const;

        /// \copydoc TextureSampler::getRepeatMode()
        ///
        TextureSampler::Repeat getRepeatMode() const;

        /// \copydoc TextureSampler::getAnisotropyLevel()
        ///
        float getAnisotropyLevel() const;

        /// \copydoc TextureSampler::getBorderColor()
        ///
        const Color& getBorderColor() const;

        /// \brief Get the OpenGL handle for this texture
        ///
        /// \return The OpenGL handle
        ///
        unsigned int getHandle() const;

        /// \brief Get the maximum texture units
        ///
        /// \return The maximum texture units. Sampler cannot be bound to
        ///         texture units which have equal or greater number than this
        ///
        static unsigned int getMaxTextureUnits();

        /// \brief Set the pixel store alignment
        ///
        /// This set the OpenGL UNPACK_ALIGNMENT parameter appropriately.
        ///
        /// \param format The texture format
        ///
        static void setUnpackAlignment(const Format format);

        /// \brief Check is the sRGB color space is allowed
        ///
        /// \return True if allowed
        ///
        static bool allowSRGB();

        /// \brief Check is automatic mipmap generation is allowed
        ///
        /// \return True if allowed
        ///
        static bool allowGenMipmaps(const glm::uvec2& size, const bool srgb);

    protected:

        /// \brief Get the texture format from pixel depth
        ///
        /// \param depth The pixel depth value in bytes
        ///
        /// \return The format
        ///
        static Format getFormatFromDepth(const uint32 depth);

        /// \brief Get the texture pixel depth from format
        ///
        /// \param format The format
        ///
        /// \return The pixel depth in bytes
        ///
        static unsigned int getDepthFromFormat(const Format format);

        /// \brief Set the swizzle mask for 8 bit textures
        ///
        /// By default Texture2D and Cubemap swizzle the R component to the A component.
        /// This way the alpha component can be referenced via the a component in shaders,
        /// maintaining the same functionality across GLES 2.0 and 3.0.
        ///
        /// \param format The format. If this is not Alpha_UB_8, nothing happens
        ///
        void setAlphaSwizzle(const Format format);

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

/// \class jop::Texture
/// \ingroup graphics

#endif