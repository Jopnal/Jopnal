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
#include <Jopnal/Graphics/Texture.hpp>
#include <glm/vec2.hpp>
#include <string>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API RenderTexture final
    {
    private:

        JOP_DISALLOW_COPY_MOVE(RenderTexture);

    public:

        /// \brief Constructor
        ///
        /// Doesn't initialize anything. To create the frame buffer, call create()
        ///
        RenderTexture();

        /// \brief Constructor for initialization
        ///
        /// \param size Size of the frame buffer texture
        /// \param depthBits Depth buffer precision in bits. Zero for no depth buffer
        /// \param stencilBits Stencil buffer precision in bits. Zero for no stencil buffer
        ///
        RenderTexture(const glm::ivec2& size, const unsigned int depthBits, const unsigned int stencilBits);

        /// \brief Destructor
        ///
        ~RenderTexture();


        /// \brief Clear the frame buffer
        ///
        /// This will clear the color, depth and stencil buffers.
        ///
        void clear();

        void clearDepth();

        /// \brief Initialize the frame buffer
        ///
        /// \param size Size of the frame buffer texture
        /// \param depthBits Depth buffer precision in bits. Zero for no depth buffer
        /// \param stencilBits Stencil buffer precision in bits. Zero for no stencil buffer
        ///
        /// \return True if successful
        ///
        bool create(const glm::ivec2& size, const unsigned int depthBits, const unsigned int stencilBits);

        bool createDepth(const glm::ivec2& size, const bool cube);

        /// \brief Destroy this frame buffer
        ///
        void destroy();

        /// \brief Bind this frame buffer for drawing on
        ///
        /// \return True if successful
        ///
        bool bind() const;

        /// \brief Unbind the currently bound frame buffer
        ///
        /// Rebinds the window as the frame buffer
        ///
        static void unbind();

        /// \brief Get the frame buffer texture size
        ///
        /// \return glm::vec2 with the size
        ///
        const glm::ivec2& getSize() const;

        /// \brief Check if this frame buffer is valid
        ///
        /// \return True if valid
        ///
        bool isValid() const;

        /// \brief Set the color buffer clear color
        ///
        /// \param color The new color
        ///
        void setClearColor(const Color& color);

        /// \brief Get the color buffer clear color
        ///
        /// \return Reference to the color
        ///
        const Color& getClearColor() const;

        /// \brief Get the texture
        ///
        /// \return Const reference to the internal texture
        ///
        const Texture* getTexture() const;

        /// \brief Get the depth bits
        ///
        /// \return The depth bits
        ///
        unsigned int getDepthBits() const;

        /// \brief Get the stencil bits
        ///
        /// \return The stencil bits
        ///
        unsigned int getStencilBits() const;

        /// \brief Sets absolute Viewport for the frame buffer
        ///
        /// \param x The upper left x coordinate
        /// \param y The upper left y coordinate
        /// \param width Width of the view port in pixels
        /// \param height Height of the view port in pixels
        ///
        void setViewport(const int x, const int y, const unsigned int width, const unsigned int height);

        /// \brief Sets relative Viewport for the frame buffer
        ///
        /// \param x The relative upper left x coordinate
        /// \param y The relative upper left y coordinate
        /// \param width Relative width of the view port in pixels
        /// \param height Relative height of the view port in pixels
        ///
        void setViewportRelative(const float x, const float y, const float width, const float height);


    private:

        std::unique_ptr<Texture> m_texture;              ///< The attached texture
        glm::ivec2 m_size;
        Color m_clearColor;             ///< The color to use when clearing the buffer
        unsigned int m_frameBuffer;     ///< Handle for the frame buffer
        unsigned int m_depthBuffer;     ///< Handle for the depth buffer
        unsigned int m_stencilBuffer;   ///< Handle for the stencil buffer
        unsigned int m_depthBits;       ///< The depth bits
        unsigned int m_stencilBits;     ///< The stencil bits
        bool m_colorChanged;            ///< Has the clear color been changed?

    };
}

#endif

/// \class RenderTexture
/// \ingroup graphics
///