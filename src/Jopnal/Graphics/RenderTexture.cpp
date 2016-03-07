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
        : m_texture ("")
    {}

    RenderTexture::RenderTexture(const glm::ivec2& size, const unsigned int depthBits, const unsigned int stencilBits)
        : m_texture ("")
    {
        create(size, depthBits, stencilBits);
    }

    RenderTexture::~RenderTexture()
    {
        destroy();
    }

    //////////////////////////////////////////////

    void RenderTexture::clear()
    {
        if (bind())
        {
            if (m_colorChanged)
            {
                glm::vec4 col = m_clearColor.asRGBAFloatVector();

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

        clear();
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

    bool RenderTexture::bind() const
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

    unsigned int RenderTexture::getDepthBits() const
    {
        return m_depthBits;
    }

    //////////////////////////////////////////////

    unsigned int RenderTexture::getStencilBits() const
    {
        return m_stencilBits;
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