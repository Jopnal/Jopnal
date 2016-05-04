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


namespace jop
{
    RenderTexture::RenderTexture()
        : RenderTarget          ("rendertexture"),
          m_depthBuffer         (0),
          m_stencilBuffer       (0),
          m_frameBuffer         (0, nullptr),
          m_colorAttachments    ()
    {}

    RenderTexture::~RenderTexture()
    {
        destroy(true, true);
    }

    //////////////////////////////////////////////

    bool RenderTexture::addColorAttachment(const ColorAttachmentSlot slot, const ColorAttachment attachment, const glm::uvec2& size)
    {
        using CA = ColorAttachment;

        auto getColorBytes = [](const ColorAttachment att) -> unsigned int
        {
            switch (att)
            {
                case CA::Depth2D16:
                case CA::DepthCube16:
                    return 2;

                case CA::RGB2D:
                case CA::RGBCube:
                case CA::Depth2D24:
                case CA::DepthCube24:
                    return 3;

                case CA::RGBA2D:
                case CA::RGBACube:
                case CA::Depth2D32:
                case CA::DepthCube32:
                    return 4;

                case CA::RGB2DFloat16:
                    return 6;

                case CA::RGB2DFloat32:
                    return 12;

                case CA::RGBA2DFloat16:
                    return 8;

                case CA::RGBA2DFloat32:
                    return 16;

                default:
                    return 0;
            }
        };

        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        auto& texture = m_colorAttachments[(attachment < CA::Depth2D16) * static_cast<int>(slot)];

        switch (attachment)
        {
            case CA::RGB2D:
            case CA::RGBA2D:
            case CA::RGB2DFloat16:
            case CA::RGB2DFloat32:
            case CA::RGBA2DFloat16:
            case CA::RGBA2DFloat32:
            {
                auto tex = std::make_unique<Texture2D>("");
                if (!tex->load(size, getColorBytes(attachment), true))
                    return false;

                texture = std::move(tex);

                break;
            }

            case CA::RGBCube:
            case CA::RGBACube:
            {
                auto tex = std::make_unique<Cubemap>("");
                if (!tex->load(size, getColorBytes(attachment), true))
                    return false;

                texture = std::move(tex);

                break;
            }

            case CA::Depth2D16:
            case CA::Depth2D24:
            case CA::Depth2D32:
            {
                auto tex = std::make_unique<Texture2DDepth>("");
                if (!tex->load(size, getColorBytes(attachment)))
                    return false;

                texture = std::move(tex);

                break;
            }

            case CA::DepthCube16:
            case CA::DepthCube24:
            case CA::DepthCube32:
            {
                auto tex = std::make_unique<CubemapDepth>("");
                if (!tex->load(size, getColorBytes(attachment)))
                    return false;

                texture = std::move(tex);
            }
        }

        destroy(true, false);

        return true;
    }

    //////////////////////////////////////////////

    bool RenderTexture::addDepthAttachment(const DepthAttachment attachment, const glm::uvec2& size)
    {
        using DA = DepthAttachment;

        auto getDepthEnum = [](const DepthAttachment depth) -> GLenum
        {
            switch (depth)
            {
                case DA::Renderbuffer16:
                case DA::Texture16:
                case DA::TextureCube16:
                    return gl::DEPTH_COMPONENT16;

                case DA::Renderbuffer24:
                case DA::Texture24:
                case DA::TextureCube24:
                    return gl::DEPTH_COMPONENT24;

                case DA::Renderbuffer32:
                case DA::Texture32:
                case DA::TextureCube32:
                    return gl::DEPTH_COMPONENT32;

                default:
                    return gl::DEPTH_COMPONENT;
            }
        };

        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (m_depthBuffer)
        {
            glCheck(gl::DeleteRenderbuffers(1, &m_depthBuffer));
            m_depthBuffer = 0;
        }

        if (attachment != DA::None)
        {
            if (attachment < DA::Texture16)
            {
                glCheck(gl::GenRenderbuffers(1, &m_depthBuffer));

                if (!m_depthBuffer)
                {
                    JOP_DEBUG_ERROR("Failed to create depth buffer");
                    return false;
                }

                glCheck(gl::BindRenderbuffer(gl::RENDERBUFFER, m_depthBuffer));
                glCheck(gl::RenderbufferStorage(gl::RENDERBUFFER, getDepthEnum(attachment), size.x, size.y));
            }
            else
            {
                unsigned int bytes = 0;
                switch (attachment)
                {
                    case DA::Texture16:
                    case DA::TextureCube16:
                        bytes = 2;
                        break;

                    case DA::Texture24:
                    case DA::TextureCube24:
                        bytes = 3;
                        break;

                    case DA::Texture32:
                    case DA::TextureCube32:
                        bytes = 4;
                }

                auto& texture = m_colorAttachments[0];

                if (attachment >= DepthAttachment::TextureCube16)
                {
                    auto tex = std::make_unique<CubemapDepth>("");
                    if (!tex->load(size, bytes))
                        return false;

                    texture = std::move(tex);
                }
                else
                {
                    auto tex = std::make_unique<Texture2DDepth>("");
                    if (!tex->load(size, bytes))
                        return false;

                    texture = std::move(tex);
                }
            }
        }

        destroy(true, false);

        return true;
    }

    //////////////////////////////////////////////

    bool RenderTexture::addStencilAttachment(const StencilAttachment attachment, const glm::uvec2& size)
    {
        auto getStencilEnum = [](const StencilAttachment stencil) -> GLenum
        {
            switch (stencil)
            {
                case StencilAttachment::Int8:   return gl::STENCIL_INDEX8;
                case StencilAttachment::Int16:  return gl::STENCIL_INDEX16;
                default:                        return gl::STENCIL_INDEX;
            }
        };

        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (m_stencilBuffer)
        {
            glCheck(gl::DeleteRenderbuffers(1, &m_stencilBuffer));
            m_stencilBuffer = 0;
        }

        if (attachment != StencilAttachment::None)
        {
            glCheck(gl::GenRenderbuffers(1, &m_stencilBuffer));

            if (!m_stencilBuffer)
            {
                JOP_DEBUG_ERROR("Failed to create stencil buffer");
                return false;
            }

            glCheck(gl::BindRenderbuffer(gl::RENDERBUFFER, m_stencilBuffer));
            glCheck(gl::RenderbufferStorage(gl::RENDERBUFFER, getStencilEnum(attachment), size.x, size.y));
        }

        destroy(true, false);

        return true;
    }

    //////////////////////////////////////////////

    bool RenderTexture::addDepthStencilAttachment(const DepthAttachment depth, const StencilAttachment stencil, const glm::uvec2& size)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (m_depthBuffer)
        {
            glCheck(gl::DeleteRenderbuffers(1, &m_depthBuffer));
            m_depthBuffer = 0;
        }

        if (depth != DepthAttachment::None && stencil != StencilAttachment::None)
        {
            glCheck(gl::GenRenderbuffers(1, &m_depthBuffer));

            if (!m_depthBuffer)
            {
                JOP_DEBUG_ERROR("Failed to create depth-stencil buffer");
                return false;
            }

            glCheck(gl::BindRenderbuffer(gl::RENDERBUFFER, m_depthBuffer));
            glCheck(gl::RenderbufferStorage(gl::RENDERBUFFER, depth == DepthAttachment::Renderbuffer24 ? gl::DEPTH24_STENCIL8 : gl::DEPTH32F_STENCIL8, size.x, size.y));
        }

        destroy(true, false);

        return true;
    }


    //////////////////////////////////////////////

    void RenderTexture::destroy(const bool framebuffer, const bool attachments)
    {
        static_cast<const RenderTexture*>(this)->destroy(framebuffer, attachments);
    }

    //////////////////////////////////////////////

    void RenderTexture::destroy(const bool framebuffer, const bool attachments) const
    {
        if (!framebuffer && !attachments)
            return;

        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        unbind();

        if (framebuffer && m_frameBuffer.first)
        {
            JOP_ASSERT(m_frameBuffer.second == Window::getCurrentContextWindow(), "Tried to destroy frame buffer within different context!");

            glCheck(gl::DeleteFramebuffers(1, &m_frameBuffer.first));
            m_frameBuffer.first = 0;
            m_frameBuffer.second = nullptr;
        }

        if (attachments)
        {
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

            for (auto& i : m_colorAttachments)
                i.reset();
        }
    }

    //////////////////////////////////////////////

    bool RenderTexture::bind() const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (attach() && isValid())
        {
            glCheck(gl::BindFramebuffer(gl::FRAMEBUFFER, m_frameBuffer.first));
            glCheck(gl::Viewport(0, 0, getSize().x, getSize().y));
        }

        return isValid();
    }

    //////////////////////////////////////////////

    void RenderTexture::unbind()
    {
        glCheck(gl::BindFramebuffer(gl::FRAMEBUFFER, 0));
    }

    //////////////////////////////////////////////

    glm::uvec2 RenderTexture::getSize() const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        for (auto& i : m_colorAttachments)
        {
            if (i)
                return i->getSize();
        }

        return glm::uvec2();
    }

    //////////////////////////////////////////////

    bool RenderTexture::isValid() const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        return m_frameBuffer.first != 0 || m_colorAttachments[0] || m_colorAttachments[1];
    }

    //////////////////////////////////////////////

    const Texture* RenderTexture::getColorTexture(const ColorAttachmentSlot slot) const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        return m_colorAttachments[static_cast<int>(slot)].get();
    }

    //////////////////////////////////////////////

    const Texture* RenderTexture::getDepthTexture() const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        return m_colorAttachments[0].get();
    }

    //////////////////////////////////////////////

    bool RenderTexture::attach() const
    {
        JOP_ASSERT(m_frameBuffer.second == Window::getCurrentContextWindow() || m_frameBuffer.second == nullptr, "Tried to compile a frame buffer created in another context! You must destroy the frame buffer in its context of creation before using it in another. RenderTexture id: " + getID());

        if (!m_frameBuffer.second && !m_frameBuffer.first)
        {
            if (!isValid())
                return true;

            glCheck(gl::GenFramebuffers(1, &m_frameBuffer.first));

            if (!m_frameBuffer.first)
            {
                JOP_DEBUG_ERROR("Failed to attach RenderTexture. Couldn't generate frame buffer");
                return false;
            }

            glCheck(gl::BindFramebuffer(gl::FRAMEBUFFER, m_frameBuffer.first));

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
            
            bool hasColor = false;
            for (std::size_t i = 1; i < m_colorAttachments.size(); ++i)
            {
                if (m_colorAttachments[i])
                {
                    hasColor = true;
                    break;
                }
            }

            if (m_colorAttachments[0] && !m_depthBuffer)
            {
                glCheck(gl::FramebufferTexture(gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT, m_colorAttachments[0]->getHandle(), 0));

                if (!hasColor)
                {
                    glCheck(gl::DrawBuffer(gl::NONE));
                    glCheck(gl::ReadBuffer(gl::NONE));
                }
            }
            
            if (hasColor)
            {
                std::vector<GLenum> colorAtt;
                for (std::size_t i = 1; i < m_colorAttachments.size(); ++i)
                {
                    if (!m_colorAttachments[i])
                        continue;

                    glCheck(gl::FramebufferTexture(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0 + i - 1, m_colorAttachments[i]->getHandle(), 0));
                    colorAtt.push_back(gl::COLOR_ATTACHMENT0 + i - 1);
                }

                if (!colorAtt.empty())
                {
                    glCheck(gl::DrawBuffers(colorAtt.size(), colorAtt.data()));
                }
            }

            auto status = glCheck(gl::CheckFramebufferStatus(gl::FRAMEBUFFER));
            if (status != gl::FRAMEBUFFER_COMPLETE)
            {
                JOP_DEBUG_ERROR("Failed to create RenderTexture. Failed to complete frame buffer");
                destroy(true, false);
                return false;
            }

            m_frameBuffer.second = Window::getCurrentContextWindow();
        }

        return true;
    }
}