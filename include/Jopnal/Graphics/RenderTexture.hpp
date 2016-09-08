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
#include <Jopnal/Graphics/Texture/Texture.hpp>
#include <Jopnal/Graphics/Texture/Cubemap.hpp>
#include <glm/vec2.hpp>
#include <string>
#include <array>
#include <unordered_map>

//////////////////////////////////////////////


namespace jop
{
    class Window;

    class JOP_API RenderTexture : public RenderTarget
    {
    private:

        JOP_DISALLOW_COPY_MOVE(RenderTexture);

    public:

        /// Color attachment slot
        ///
        enum class Slot
        {
            Depth,          ///< Depth attachment slot
            Stencil,        ///< Stencil attachment slot
            DepthStencil,   ///< Depth-stencil attachment slot
            Color0,         ///< First color attachment slot

            __Count         ///< For internal functionality, do not use
        };

    private:

        typedef std::array<std::tuple<unsigned int, std::unique_ptr<Texture>, bool>, static_cast<int>(Slot::__Count)> AttachmentArray;

    public:

        /// \brief Constructor
        ///
        /// Doesn't initialize the frame buffer.
        ///
        RenderTexture();

        /// \brief Destructor
        ///
        ~RenderTexture() override;


        /// \brief Add a texture attachment
        ///
        /// This will always return false when the size hasn't been set.
        ///
        /// \param slot The attachment slot
        /// \param format The texture format
        /// 
        /// \return True if added successfully
        ///
        /// \see setSize()
        ///
        bool addTextureAttachment(const Slot slot, const Texture::Format format);

        /// \brief Add a cube map attachment
        ///
        /// \param slot The attachment slot
        /// \param format The cube map format
        ///
        /// \return True if added successfully
        ///
        bool addCubemapAttachment(const Slot slot, const Texture::Format format);

        /// \brief Add a render buffer attachment
        ///
        /// This will always return false when the size hasn't been set.
        ///
        /// \param slot The attachment slot
        /// \param format The render buffer format
        /// 
        /// \return True if added successfully
        ///
        /// \see getMaximumRenderbufferSize()
        /// \see setSize()
        ///
        bool addRenderbufferAttachment(const Slot slot, const Texture::Format format);

        /// \brief Destroy this frame buffer
        ///
        /// \param framebuffer Destroy the frame buffer object?
        /// \param attachments Destroy the attachments?
        ///
        void destroy(const bool framebuffer, const bool attachments);

        /// \brief Bind this frame buffer for drawing
        ///
        /// \return True if successful
        ///
        bool bind() const override;

        /// \brief Bind this frame buffer for reading
        ///
        /// \return True if successful
        ///
        bool bindRead() const;

        /// \copydoc bind()
        ///
        bool bindDraw() const;

        /// \brief Unbind the currently bound draw frame buffer
        ///
        /// Rebinds the window as the frame buffer
        ///
        static void unbind();

        /// \brief Bind cube map face
        ///
        /// \note This will also bind the frame buffer object as the draw frame buffer
        ///
        /// \warning If the texture in the slot is not actually a cube map, but a regular
        ///          2D texture, it will be bound instead
        ///
        /// \param slot The attachment slot
        /// \param face The cube map face to bind
        ///
        /// \return True if successful
        ///
        bool bindCubeFace(const Slot slot, const Cubemap::Face face) const;

        /// \brief Set the size for attachments
        ///
        /// Must be called before attempting to add attachments. Has no effect
        /// when already called once before destroy().
        ///
        /// \param size The size to set
        ///
        void setSize(const glm::uvec2& size);

        /// \brief Get the frame buffer texture size
        ///
        /// \return The size in pixels
        ///
        glm::uvec2 getSize() const override;

        /// \brief Check if this frame buffer is valid
        ///
        /// \return True if valid
        ///
        bool isValid() const;

        /// \brief Get a texture attachment
        ///
        /// \param slot The attachment slot
        ///
        /// \return Pointer to the texture. nullptr if none exists in the slot
        ///
        Texture* getTextureAttachment(const Slot slot);

        /// \copydoc getTextureAttachment()
        ///
        const Texture* getTextureAttachment(const Slot slot) const;

        /// \brief Get the maximum render buffer size
        ///
        /// \return The maximum render buffer size
        ///
        static unsigned int getMaximumRenderbufferSize();

    private:

        bool addTextureAttachment(const Slot slot, const Texture::Format format, const bool cube);

        bool attach() const;

        void destroy(const bool framebuffer, const bool attachments) const;


        mutable unsigned int m_frameBuffer;     ///< The OpenGL frame buffer handle
        mutable AttachmentArray m_attachments;  ///< Attachments
        mutable glm::uvec2 m_size;              ///< Size
    };
}

/// \class jop::RenderTexture
/// \ingroup graphics

#endif