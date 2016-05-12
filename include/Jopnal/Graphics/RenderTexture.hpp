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

#ifndef JOP_RENDERTEXTURE_HPP
#define JOP_RENDERTEXTURE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/RenderTarget.hpp>
#include <Jopnal/Graphics/Texture.hpp>
#include <glm/vec2.hpp>
#include <string>
#include <array>
#include <unordered_map>

//////////////////////////////////////////////


namespace jop
{
    class Window;

    class JOP_API RenderTexture final : public RenderTarget
    {
    private:

        JOP_DISALLOW_COPY_MOVE(RenderTexture);

    public:

        /// Color attachment type
        ///
        enum class ColorAttachment
        {
            None,           ///< No color attachment

            RGB2D,          ///< 24 bit 2D texture
            RGBA2D,         ///< 32 bit 2D texture

            RGBCube,        ///< 24 bit cube map texture
            RGBACube,       ///< 32 bit cube map texture
            
            RGB2DFloat16,   ///< 48 bit 2D texture
            RGBA2DFloat16,  ///< 64 bit 2D texture
            RGB2DFloat32,   ///< 96 bit 2D texture
            RGBA2DFloat32,  ///< 128 bit 2D texture

            RGBCubeFloat16,   ///< 48 bit 2D texture
            RGBACubeFloat16,  ///< 64 bit 2D texture
            RGBCubeFloat32,   ///< 96 bit 2D texture
            RGBACubeFloat32,  ///< 128 bit 2D texture

            // Keep depth enums last

            Depth2D16,      ///< 16 bit 2D depth texture
            Depth2D24,      ///< 24 bit 2D depth texture
            Depth2D32,      ///< 32 bit 2D depth texture

            DepthCube16,    ///< 16 bit cube map depth texture
            DepthCube24,    ///< 24 bit cube map depth texture
            DepthCube32     ///< 32 bit cube map depth texture
        };

        /// Color attachment slot
        ///
        enum class ColorAttachmentSlot
        {
            _1 = 2,
            _2,
            _3,
            _4
        };

        /// Depth attachment type
        ///
        /// Render buffer attachments cannot be used with cube map color attachments.
        ///
        enum class DepthAttachment
        {
            None,           ///< No depth attachment

            Renderbuffer16, ///< 16 bit render buffer
            Renderbuffer24, ///< 24 bit render buffer
            Renderbuffer32, ///< 32 bit render buffer

            Texture16,      ///< 16 bit depth texture
            Texture24,      ///< 24 bit depth texture
            Texture32,      ///< 32 bit depth texture

            TextureCube16,  ///< 16 bit depth cube map
            TextureCube24,  ///< 24 bit depth cube map
            TextureCube32,  ///< 32 bit depth cube map
        };
        
        /// Stencil attachment type
        ///
        /// Stencil attachments cannot be used with cube map color attachments.
        ///
        enum class StencilAttachment
        {
            None,   ///< No stencil attachment

            Renderbuffer8,  ///< 8 bit stencil render buffer
            Renderbuffer16, ///< 16 bit stencil render buffer

            Texture8,
            Texture16
        };

        enum class DepthStencilAttachment
        {
            None,

            Renderbuffer24_8,
            Renderbuffer32_8,

            Texture24_8,
            Texture32_8
        };

    public:

        /// \brief Constructor
        ///
        /// Doesn't initialize anything. To create the frame buffer, call create()
        ///
        RenderTexture();

        /// \brief Destructor
        ///
        ~RenderTexture() override;


        bool addColorAttachment(const ColorAttachmentSlot slot, const ColorAttachment attachment, const glm::uvec2& size);

        bool addDepthAttachment(const DepthAttachment attachment, const glm::uvec2& size);

        bool addStencilAttachment(const StencilAttachment attachment, const glm::uvec2& size);

        bool addDepthStencilAttachment(const DepthStencilAttachment attachment, const glm::uvec2& size);

        /// \brief Destroy this frame buffer
        ///
        void destroy(const bool framebuffer, const bool attachments);


        /// \brief Bind this frame buffer for drawing
        ///
        /// \return True if successful
        ///
        bool bind() const override;

        bool bind(const bool read) const;

        /// \brief Unbind the currently bound frame buffer
        ///
        /// Rebinds the window as the frame buffer
        ///
        static void unbind();

        /// \brief Get the frame buffer texture size
        ///
        /// \return glm::vec2 with the size
        ///
        glm::uvec2 getSize() const override;

        /// \brief Check if this frame buffer is valid
        ///
        /// \return True if valid
        ///
        bool isValid() const;

        /// \brief Get the texture
        ///
        /// \return Const pointer to the internal texture, nullptr if no texture exists
        ///
        Texture* getColorTexture(const ColorAttachmentSlot slot);

        const Texture* getColorTexture(const ColorAttachmentSlot slot) const;

        /// \brief Get the depth texture
        ///
        /// \return Const pointer to the internal depth texture, nullptr if no depth texture exists
        ///
        const Texture* getDepthTexture() const;

        const Texture* getStencilTexture() const;

    private:

        bool attach() const;

        void destroy(const bool framebuffer, const bool attachments) const;


        mutable unsigned int m_depthBuffer;                 ///< Handle for the depth buffer
        mutable unsigned int m_stencilBuffer;               ///< Handle for the stencil buffer
        mutable unsigned int m_depthStencilBuffer;
        mutable std::pair<unsigned int, Window*> m_frameBuffer;
        mutable std::array<std::unique_ptr<Texture>, 6> m_colorAttachments;
    };
}

#endif

/// \class RenderTexture
/// \ingroup graphics
///