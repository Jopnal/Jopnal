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

// Headers
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace
{
    unsigned int ns_currentBuffer = 0;
}

namespace jop
{
    RenderTexture::RenderTexture()
        : Subsystem ("Render Texture"),
          m_texture ("Render Texture Texture")
    {}

    RenderTexture::RenderTexture(const glm::ivec2& size, const unsigned int depthBits, const unsigned int stencilBits)
        : Subsystem ("Render Texture"),
          m_texture ("Render Texture Texture")
    {
        create(size, depthBits, stencilBits);
    }

    RenderTexture::~RenderTexture()
    {
        destroy();
    }

    //////////////////////////////////////////////

    void RenderTexture::postUpdate(const float)
    {
        if (bind())
        {
            if (m_colorChanged)
            {
                glm::vec4 col = m_clearColor.asFloatVector();

                glCheck(gl::ClearColor(col.r, col.g, col.b, col.a));
                glCheck(gl::ClearDepth(1.0));
                glCheck(gl::ClearStencil(0));

                m_colorChanged = false;
            }

            glCheck(gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT | gl::STENCIL_BUFFER_BIT));
        }
    }

    //////////////////////////////////////////////

    bool RenderTexture::create(const glm::ivec2& size, const unsigned int depthBits, const unsigned int stencilBits)
    {
        auto getDepthEnum = [](const unsigned int bits) -> GLenum
        {
            if (bits % 8 == 0 && bits <= 32 && bits >= 16)
            {
                switch (bits)
                {
                    case 16:
                        return gl::DEPTH_COMPONENT16;
                    case 24:
                        return gl::DEPTH_COMPONENT24;
                    case 32:
                        return gl::DEPTH_COMPONENT32;
                }
            }

            JOP_DEBUG_ERROR("Invalid RenderTexture depth bit value (" << bits << ") specified. Defaulting to 24");

            return gl::DEPTH_COMPONENT24;
        };
        auto getStencilEnum = [](const unsigned int bits) -> GLenum
        {
            if (bits % 8 == 0 && bits <= 16 && bits >= 8)
            {
                switch (bits)
                {
                    case 8:
                        return gl::STENCIL_INDEX8;
                    case 16:
                        return gl::STENCIL_INDEX16;
                }
            }

            JOP_DEBUG_ERROR("Invalid RenderTexture stencil bit value (" << bits << ") specified. Defaulting to 8");

            return gl::STENCIL_INDEX8;
        };

        destroy();

        if (!m_texture.load(size.x, size.y, 4))
        {
            JOP_DEBUG_ERROR("Failed to create RenderTexture. Couldn't create texture");
            return false;
        }

        glCheck(gl::GenFramebuffers(1, &m_frameBuffer));

        if (!m_frameBuffer)
        {
            JOP_DEBUG_ERROR("Failed to create RenderTexture. Couldn't generate frame buffer");
            return false;
        }

        bind();

        if (depthBits)
        {
            glCheck(gl::GenRenderbuffers(1, &m_depthBuffer));

            if (!m_depthBuffer)
            {
                JOP_DEBUG_ERROR("Failed to create RenderTexture, Couldn't create depth buffer");
                destroy();
                return false;
            }

            glCheck(gl::BindRenderbuffer(gl::RENDERBUFFER, m_depthBuffer));
            glCheck(gl::RenderbufferStorage(gl::RENDERBUFFER, getDepthEnum(depthBits), m_texture.getWidth(), m_texture.getHeight()));
            glCheck(gl::FramebufferRenderbuffer(gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT, gl::RENDERBUFFER, m_depthBuffer));
        }

        if (stencilBits)
        {
            glCheck(gl::GenRenderbuffers(1, &m_stencilBuffer));

            if (!m_stencilBuffer)
            {
                JOP_DEBUG_ERROR("Failed to create RenderTexture, Couldn't create stencil buffer");
                destroy();
                return false;
            }

            glCheck(gl::BindRenderbuffer(gl::RENDERBUFFER, m_stencilBuffer));
            glCheck(gl::RenderbufferStorage(gl::RENDERBUFFER, getStencilEnum(stencilBits), m_texture.getWidth(), m_texture.getHeight()));
            glCheck(gl::FramebufferRenderbuffer(gl::FRAMEBUFFER, gl::STENCIL_ATTACHMENT, gl::RENDERBUFFER, m_stencilBuffer));
        }

        glCheck(gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_2D, m_texture.getHandle(), 0));

        auto status = glCheck(gl::CheckFramebufferStatus(gl::FRAMEBUFFER));
        if (status != gl::FRAMEBUFFER_COMPLETE)
        {
            JOP_DEBUG_ERROR("Failed to create RenderTexture. Failed to create frame buffer");
            destroy();
            return false;
        }

        postUpdate(0.0);
        glCheck(gl::Viewport(0, 0, m_texture.getWidth(), m_texture.getHeight()));

        return true;
    }

    //////////////////////////////////////////////

    void RenderTexture::destroy()
    {
        unbind();

        if (m_frameBuffer)
        {
            glCheck(gl::DeleteFramebuffers(1, &m_frameBuffer));
            m_frameBuffer = 0;
        }

        if (m_depthBuffer)
        {
            glCheck(gl::DeleteRenderbuffers(1, &m_depthBuffer));
            m_depthBuffer = 0;
        }

        if (m_stencilBuffer)
        {
            glCheck(gl::DeleteRenderbuffers(1, &m_stencilBuffer));
            m_stencilBuffer = 0;
        }

        m_texture.destroy();
    }

    //////////////////////////////////////////////

    bool RenderTexture::bind()
    {
        if (m_frameBuffer && m_frameBuffer != ns_currentBuffer)
        {
            if (m_frameBuffer != ns_currentBuffer)
            {
                glCheck(gl::BindFramebuffer(gl::FRAMEBUFFER, m_frameBuffer));
                ns_currentBuffer = m_frameBuffer;
            }
        }

        return isValid();
    }

    //////////////////////////////////////////////

    void RenderTexture::unbind()
    {
        if (ns_currentBuffer != 0)
        {
            glCheck(gl::BindFramebuffer(gl::FRAMEBUFFER, 0));
            ns_currentBuffer = 0;
        }
    }

    //////////////////////////////////////////////

    glm::vec2 RenderTexture::getSize() const
    {
        return glm::vec2(m_texture.getWidth(), m_texture.getHeight());
    }

    //////////////////////////////////////////////

    bool RenderTexture::isValid() const
    {
        return m_frameBuffer != 0;
    }

    //////////////////////////////////////////////

    void RenderTexture::setClearColor(const Color& color)
    {
        m_clearColor = color;
        m_colorChanged = true;
    }

    //////////////////////////////////////////////

    const Color& RenderTexture::getClearColor() const
    {
        return m_clearColor;
    }

    //////////////////////////////////////////////

    const Texture& RenderTexture::getTexture() const
    {
        return m_texture;
    }

    //////////////////////////////////////////////

    void RenderTexture::setViewport(const int x, const int y, const unsigned int width, const unsigned int height)
    {
        if (isValid())
            glCheck(gl::Viewport(x, y, width, height));
    }

    //////////////////////////////////////////////

    void RenderTexture::setViewportRelative(const float x, const float y, const float width, const float height)
    {
        if (isValid())
        {
            float textureX = getSize().x;
            float textureY = getSize().y;

            glCheck(gl::Viewport(static_cast<int>(x * textureX), static_cast<int>(y * textureY),
                                 static_cast<unsigned int>(width * textureX), static_cast<unsigned int>(height * textureY)));
        }
    }
}