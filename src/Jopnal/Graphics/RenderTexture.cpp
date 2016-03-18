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

    int ns_lastZeroViewport[] =
    {
        0, 0, 0, 0
    };
}

namespace jop
{
    RenderTexture::RenderTexture()
        : m_texture(),
          m_depthTexture(),
          m_size(),
          m_clearColor(),
          m_frameBuffer(0),
          m_depthBuffer(0),
          m_stencilBuffer(0)          
    {}

    RenderTexture::~RenderTexture()
    {
        destroy();
    }

    //////////////////////////////////////////////

    void RenderTexture::clear()
    {
        if (bind())
        {
            glm::vec4 col = m_clearColor.asRGBAFloatVector();

            glCheck(gl::ClearColor(col.r, col.g, col.b, col.a));
            glCheck(gl::ClearDepth(1.0));
            glCheck(gl::ClearStencil(0));

            glCheck(gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT | gl::STENCIL_BUFFER_BIT));
        }
    }

    //////////////////////////////////////////////

    void RenderTexture::clearDepth()
    {
        if (bind())
        {
            glCheck(gl::ClearDepth(1.0));

            glCheck(gl::Clear(gl::DEPTH_BUFFER_BIT));
        }
    }

    //////////////////////////////////////////////

    bool RenderTexture::create(const ColorAttachment color, const glm::uvec2& size, const DepthAttachment depth, const StencilAttachment stencil)
    {
        auto getDepthEnum = [](const DepthAttachment dpth) -> GLenum
        {
            switch (dpth)
            {
                case DepthAttachment::Renderbuffer16:
                case DepthAttachment::Texture16:
                    return gl::DEPTH_COMPONENT16;

                case DepthAttachment::Renderbuffer24:
                case DepthAttachment::Texture24:
                    return gl::DEPTH_COMPONENT24;

                case DepthAttachment::Renderbuffer32:
                case DepthAttachment::Texture32:
                    return gl::DEPTH_COMPONENT32;

                default:
                    return gl::DEPTH_COMPONENT;
            }
        };
        auto getStencilEnum = [](const StencilAttachment stncl) -> GLenum
        {
            switch (stncl)
            {
                case StencilAttachment::Int8:
                    return gl::STENCIL_INDEX8;

                case StencilAttachment::Int16:
                    return gl::STENCIL_INDEX16;

                default:
                    return gl::STENCIL_INDEX;
            }
        };

        if (size.x < 1 || size.y < 1)
        {
            JOP_DEBUG_ERROR("Failed to create RenderTexture, invalid size specified");
            return false;
        }

        destroy();

        m_size = size;

        // Create texture
        switch (color)
        {
            case ColorAttachment::RGB2D:
            case ColorAttachment::RGBA2D:
            {
                auto tex = std::make_unique<Texture2D>("");
                tex->load(size.x, size.y, color == ColorAttachment::RGB2D ? 3 : 4);

                m_texture = std::move(tex);

                break;
            }

            case ColorAttachment::RGBCube:
            case ColorAttachment::RGBACube:
            {
                auto tex = std::make_unique<Cubemap>("");
                tex->load(size.x, size.y, color == ColorAttachment::RGBCube ? 3 : 4);

                m_texture = std::move(tex);

                break;
            }

            case ColorAttachment::Depth2D:
            {
                auto tex = std::make_unique<Texture2DDepth>("");
                tex->load(size.x, size.y, 0);

                m_texture = std::move(tex);

                break;
            }

            case ColorAttachment::DepthCube:
            {
                auto tex = std::make_unique<CubemapDepth>("");
                tex->load(size.x, size.y, 0);

                m_texture = std::move(tex);

                break;
            }
        }

        glCheck(gl::GenFramebuffers(1, &m_frameBuffer));

        if (!m_frameBuffer)
        {
            JOP_DEBUG_ERROR("Failed to create RenderTexture. Couldn't generate frame buffer");
            destroy();
            return false;
        }

        bind();

        GLenum colorAttEnum = gl::COLOR_ATTACHMENT0;

        if (color == ColorAttachment::Depth2D || color == ColorAttachment::DepthCube)
        {
            colorAttEnum = gl::DEPTH_ATTACHMENT;
            glCheck(gl::DrawBuffer(gl::NONE));
            glCheck(gl::ReadBuffer(gl::NONE));
        }

        glCheck(gl::FramebufferTexture(gl::FRAMEBUFFER, colorAttEnum, m_texture->getHandle(), 0));

        if (depth != DepthAttachment::None)
        {
            if (depth < DepthAttachment::Texture16)
            {
                glCheck(gl::GenRenderbuffers(1, &m_depthBuffer));

                if (!m_depthBuffer)
                {
                    JOP_DEBUG_ERROR("Failed to create RenderTexture, Couldn't create depth buffer");
                    destroy();
                    return false;
                }

                glCheck(gl::BindRenderbuffer(gl::RENDERBUFFER, m_depthBuffer));
                glCheck(gl::RenderbufferStorage(gl::RENDERBUFFER, getDepthEnum(depth), size.x, size.y));
                glCheck(gl::FramebufferRenderbuffer(gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT, gl::RENDERBUFFER, m_depthBuffer));
            }
            else
            {
                int bytes;
                switch (depth)
                {
                    case DepthAttachment::Texture16:
                        bytes = 2;
                        break;
                    case DepthAttachment::Texture24:
                        bytes = 3;
                        break;
                    case DepthAttachment::Texture32:
                        bytes = 4;
                        break;

                    default:
                        bytes = 0;
                }

                if (color == ColorAttachment::RGBCube || color == ColorAttachment::RGBACube)
                {
                    auto tex = std::make_unique<CubemapDepth>("");
                    tex->load(size.x, size.y, bytes);

                    m_depthTexture = std::move(tex);
                }
                else
                {
                    auto tex = std::make_unique<Texture2DDepth>("");
                    tex->load(size.x, size.y, bytes);

                    m_depthTexture = std::move(tex);
                }

                glCheck(gl::FramebufferTexture(gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT, m_depthTexture->getHandle(), 0));
            }
        }

        if (stencil != StencilAttachment::None)
        {
            glCheck(gl::GenRenderbuffers(1, &m_stencilBuffer));

            if (!m_stencilBuffer)
            {
                JOP_DEBUG_ERROR("Failed to create RenderTexture, Couldn't create stencil buffer");
                destroy();
                return false;
            }

            glCheck(gl::BindRenderbuffer(gl::RENDERBUFFER, m_stencilBuffer));
            glCheck(gl::RenderbufferStorage(gl::RENDERBUFFER, getStencilEnum(stencil), size.x, size.y));
            glCheck(gl::FramebufferRenderbuffer(gl::FRAMEBUFFER, gl::STENCIL_ATTACHMENT, gl::RENDERBUFFER, m_stencilBuffer));
        }

        auto status = glCheck(gl::CheckFramebufferStatus(gl::FRAMEBUFFER));
        if (status != gl::FRAMEBUFFER_COMPLETE)
        {
            JOP_DEBUG_ERROR("Failed to create RenderTexture. Failed to complete frame buffer");
            destroy();
            return false;
        }

        unbind();

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

        m_texture.reset();
        m_depthTexture.reset();
        m_size = glm::uvec2();
    }

    //////////////////////////////////////////////

    bool RenderTexture::bind() const
    {
        if (m_frameBuffer && m_frameBuffer != ns_currentBuffer)
        {
            if (m_frameBuffer != ns_currentBuffer)
            {
                if (!ns_currentBuffer)
                    glCheck(gl::GetIntegerv(gl::VIEWPORT, ns_lastZeroViewport));

                glCheck(gl::Viewport(0, 0, m_size.x, m_size.y));
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
            gl::Viewport(ns_lastZeroViewport[0], ns_lastZeroViewport[1], ns_lastZeroViewport[2], ns_lastZeroViewport[3]);
            glCheck(gl::BindFramebuffer(gl::FRAMEBUFFER, 0));
            ns_currentBuffer = 0;
        }
    }

    //////////////////////////////////////////////

    const glm::uvec2& RenderTexture::getSize() const
    {
        return m_size;
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
    }

    //////////////////////////////////////////////

    const Color& RenderTexture::getClearColor() const
    {
        return m_clearColor;
    }

    //////////////////////////////////////////////

    const Texture* RenderTexture::getTexture() const
    {
        return m_texture.get();
    }

    //////////////////////////////////////////////

    const Texture* RenderTexture::getDepthTexture() const
    {
        return m_depthTexture.get();
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
            const int textureX = getSize().x;
            const int textureY = getSize().y;

            glCheck(gl::Viewport(static_cast<int>(x * textureX), static_cast<int>(y * textureY),
                                 static_cast<unsigned int>(width * textureX), static_cast<unsigned int>(height * textureY)));
        }
    }
}