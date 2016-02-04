// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//////////////////////////////////////////////

#ifndef JOP_RENDERTEXTURE_HPP
#define JOP_RENDERTEXTURE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Texture.hpp>
#include <Jopnal/Core/SubSystem.hpp>
#include <glm/vec2.hpp>
#include <string>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API RenderTexture final : public Subsystem
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
        RenderTexture(const glm::vec2& size, const unsigned int depthBits, const unsigned int stencilBits);

        /// \brief Destructor
        ///
        ~RenderTexture() override;


        /// \brief Overridden postUpdate. Clears the frame buffer
        ///
        /// \param dt Delta time
        ///
        void postUpdate(const double dt) override;

        /// \brief Initialize the frame buffer
        ///
        /// \param size Size of the frame buffer texture
        /// \param depthBits Depth buffer precision in bits. Zero for no depth buffer
        /// \param stencilBits Stencil buffer precision in bits. Zero for no stencil buffer
        ///
        /// \return True if successful
        ///
        bool create(const glm::vec2& size, const unsigned int depthBits, const unsigned int stencilBits);

        /// \brief Destroy this frame buffer
        ///
        void destroy();

        /// \brief Bind this frame buffer for drawing on
        ///
        /// \return True if successful
        ///
        bool bind();

        /// \brief Unbind the currently bound frame buffer
        ///
        /// Rebinds the window as the frame buffer
        ///
        static void unbind();

        /// \brief Get the frame buffer texture size
        ///
        glm::vec2 getSize() const;

        /// \param Check if this frame buffer is valid
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
        const Texture& getTexture() const;

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

        Texture m_texture;              ///< The attached texture
        Color m_clearColor;             ///< The color to use when clearing the buffer
        unsigned int m_frameBuffer;     ///< Handle for the frame buffer
        unsigned int m_depthBuffer;     ///< Handle for the depth buffer
        unsigned int m_stencilBuffer;   ///< Handle for the stencil buffer
        bool m_colorChanged;            ///< Has the clear color been changed?

    };
}

#endif

/// \class RenderTexture
/// \ingroup graphics
///