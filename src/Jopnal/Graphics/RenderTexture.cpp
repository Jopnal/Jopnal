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
    std::atomic<unsigned int> ns_currentBuffer = 0;
}

namespace jop
{
    RenderTexture::RenderTexture()
        : RenderTarget      ("rendertexture"),
          m_texture         (),
          m_depthTexture    (),
          m_frameBuffer     (0),
          m_depthBuffer     (0),
          m_stencilBuffer   (0)          
    {}

    RenderTexture::~RenderTexture()
    {
        destroy();
    }

    //////////////////////////////////////////////

    bool RenderTexture::create(const glm::uvec2& size, const ColorAttachment color, const DepthAttachment depth, const StencilAttachment stencil)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

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

        if (size.x < 1 || size.y < 1 || size.x > Texture::getMaximumSize() || size.y > Texture::getMaximumSize())
        {
            JOP_DEBUG_ERROR("Failed to create RenderTexture, invalid size specified");
            return false;
        }

        destroy();

        auto getDepthColorBytes = [](const ColorAttachment att) -> unsigned int
        {
            switch (att)
            {
                case ColorAttachment::Depth2D16:
                case ColorAttachment::DepthCube16:
                    return 2;

                case ColorAttachment::Depth2D24:
                case ColorAttachment::DepthCube24:
                    return 3;

                case ColorAttachment::Depth2D32:
                case ColorAttachment::DepthCube32:
                    return 4;

                default:
                    return 0;
            }
        };

        // Create texture
        switch (color)
        {
            case ColorAttachment::RGB2D:
            case ColorAttachment::RGBA2D:
            {
                auto tex = std::make_unique<Texture2D>("");
                tex->load(size, color == ColorAttachment::RGB2D ? 3 : 4, true);

                m_texture = std::move(tex);

                break;
            }

            case ColorAttachment::RGBCube:
            case ColorAttachment::RGBACube:
            {
                auto tex = std::make_unique<Cubemap>("");
                tex->load(size, color == ColorAttachment::RGBCube ? 3 : 4, true);

                m_texture = std::move(tex);

                break;
            }

            case ColorAttachment::Depth2D16:
            case ColorAttachment::Depth2D24:
            case ColorAttachment::Depth2D32:
            {
                auto tex = std::make_unique<Texture2DDepth>("");
                tex->load(size, getDepthColorBytes(color));

                m_texture = std::move(tex);

                break;
            }

            case ColorAttachment::DepthCube16:
            case ColorAttachment::DepthCube24:
            case ColorAttachment::DepthCube32:
            {
                auto tex = std::make_unique<CubemapDepth>("");
                tex->load(size, getDepthColorBytes(color));

                m_texture = std::move(tex);
            }
        }

        m_colorAttachment = color;

        if ((depth == DepthAttachment::Renderbuffer24 || depth == DepthAttachment::Renderbuffer32) && stencil == StencilAttachment::Int8 &&
            color < ColorAttachment::Depth2D16 && color != ColorAttachment::RGBCube && color != ColorAttachment::RGBACube)
        {
            glCheck(gl::GenRenderbuffers(1, &m_depthBuffer));

            if (!m_depthBuffer)
            {
                JOP_DEBUG_ERROR("Failed to create RenderTexture, Couldn't create depth-stencil buffer");
                destroy();
                return false;
            }

            glCheck(gl::BindRenderbuffer(gl::RENDERBUFFER, m_depthBuffer));
            glCheck(gl::RenderbufferStorage(gl::RENDERBUFFER, depth == DepthAttachment::Renderbuffer24 ? gl::DEPTH24_STENCIL8 : gl::DEPTH32F_STENCIL8, size.x, size.y));
        }
        else
        {
            if (depth != DepthAttachment::None)
            {
                if (depth < DepthAttachment::Texture16 && color < ColorAttachment::Depth2D16 &&
                    color != ColorAttachment::RGBCube && color != ColorAttachment::RGBACube)
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
                }
                else
                {
                    unsigned int bytes;
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
                        tex->load(size, bytes);

                        m_depthTexture = std::move(tex);
                    }
                    else
                    {
                        auto tex = std::make_unique<Texture2DDepth>("");
                        tex->load(size, bytes);

                        m_depthTexture = std::move(tex);
                    }
                }
            }

            if (stencil != StencilAttachment::None && color < ColorAttachment::Depth2D16 &&
                color != ColorAttachment::RGBCube && color != ColorAttachment::RGBACube)
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
            }
        }

        return true;
    }

    //////////////////////////////////////////////

    void RenderTexture::destroy()
    {
        static_cast<const RenderTexture*>(this)->destroy();
    }

    //////////////////////////////////////////////

    void RenderTexture::destroy() const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

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
    }

    //////////////////////////////////////////////

    bool RenderTexture::bind() const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        // Have to complete the frame buffer here, since create() may be called from another thread
        // and the FBO cannot be shared between contexts
        if (!m_frameBuffer && (m_texture || m_depthTexture))
        {
            glCheck(gl::GenFramebuffers(1, &m_frameBuffer));

            if (!m_frameBuffer)
            {
                JOP_DEBUG_ERROR("Failed to create RenderTexture. Couldn't generate frame buffer");
                destroy();
                return false;
            }

            glCheck(gl::BindFramebuffer(gl::FRAMEBUFFER, m_frameBuffer));

            if (m_depthBuffer && m_stencilBuffer)
            {
                glCheck(gl::FramebufferRenderbuffer(gl::FRAMEBUFFER, gl::DEPTH_STENCIL_ATTACHMENT, gl::RENDERBUFFER, m_depthBuffer));
            }
            else if (m_depthBuffer)
            {
                glCheck(gl::FramebufferRenderbuffer(gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT, gl::RENDERBUFFER, m_depthBuffer));
            }
            else if (m_stencilBuffer)
            {
                glCheck(gl::FramebufferRenderbuffer(gl::FRAMEBUFFER, gl::STENCIL_ATTACHMENT, gl::RENDERBUFFER, m_stencilBuffer));
            }

            if (m_depthTexture)
            {
                glCheck(gl::FramebufferTexture(gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT, m_depthTexture->getHandle(), 0));
            }

            GLenum colorAttEnum = gl::COLOR_ATTACHMENT0;
            if (m_colorAttachment >= ColorAttachment::Depth2D16)
            {
                colorAttEnum = gl::DEPTH_ATTACHMENT;
                glCheck(gl::DrawBuffer(gl::NONE));
                glCheck(gl::ReadBuffer(gl::NONE));
            }

            glCheck(gl::FramebufferTexture(gl::FRAMEBUFFER, colorAttEnum, m_texture->getHandle(), 0));

            auto status = glCheck(gl::CheckFramebufferStatus(gl::FRAMEBUFFER));
            if (status != gl::FRAMEBUFFER_COMPLETE)
            {
                JOP_DEBUG_ERROR("Failed to create RenderTexture. Failed to complete frame buffer");
                destroy();
                return false;
            }
        }

        if (isValid())
        {
            if (m_frameBuffer != ns_currentBuffer)
            {
                glCheck(gl::BindFramebuffer(gl::FRAMEBUFFER, m_frameBuffer));
                ns_currentBuffer = m_frameBuffer;
            }

            glCheck(gl::Viewport(0, 0, getSize().x, getSize().y));
        }

        return isValid();
    }

    //////////////////////////////////////////////

    void RenderTexture::unbind()
    {
        if (ns_currentBuffer)
        {
            glCheck(gl::BindFramebuffer(gl::FRAMEBUFFER, 0));
            ns_currentBuffer = 0;
        }
    }

    //////////////////////////////////////////////

    glm::uvec2 RenderTexture::getSize() const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (m_texture)
            return m_texture->getSize();

        return glm::uvec2();
    }

    //////////////////////////////////////////////

    bool RenderTexture::isValid() const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        return m_frameBuffer != 0 || m_texture.operator bool() || m_depthTexture.operator bool();
    }

    //////////////////////////////////////////////

    const Texture* RenderTexture::getTexture() const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        return m_texture.get();
    }

    //////////////////////////////////////////////

    const Texture* RenderTexture::getDepthTexture() const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        return m_depthTexture.get();
    }

    //////////////////////////////////////////////

    unsigned int RenderTexture::getFramebufferHandle() const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        return m_frameBuffer;
    }
}