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
            Depth,
            Stencil,
            DepthStencil,
            Color0,

            __Count ///< For internal functionality, do not use
        };

    private:

        typedef std::array<std::pair<unsigned int, std::unique_ptr<Texture>>, static_cast<int>(Slot::__Count)> AttachmentArray;

    public:

        /// \brief Constructor
        ///
        /// Doesn't initialize the frame buffer.
        ///
        RenderTexture();

        /// \brief Destructor
        ///
        ~RenderTexture() override;


        bool addTextureAttachment(const Slot slot, const Texture::Format format);

        bool addRenderbufferAttachment(const Slot slot, const Texture::Format format);

        /// \brief Destroy this frame buffer
        ///
        void destroy(const bool framebuffer, const bool attachments);

        /// \brief Bind this frame buffer for drawing
        ///
        /// \return True if successful
        ///
        bool bind() const override;

        bool bindRead() const;

        bool bindDraw() const;

        /// \brief Unbind the currently bound frame buffer
        ///
        /// Rebinds the window as the frame buffer
        ///
        static void unbind();

        void setSize(const glm::uvec2& size);

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

        Texture* getTextureAttachment(const Slot slot);

        const Texture* getTextureAttachment(const Slot slot) const;

        static unsigned int getMaximumRenderbufferSize();

    private:

        bool attach() const;

        void destroy(const bool framebuffer, const bool attachments) const;


        mutable unsigned int m_frameBuffer;
        mutable AttachmentArray m_attachments;
        mutable glm::uvec2 m_size;
    };
}

#endif

/// \class RenderTexture
/// \ingroup graphics
///