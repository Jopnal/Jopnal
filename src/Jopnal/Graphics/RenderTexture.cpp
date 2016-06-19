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
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

	#include <Jopnal/Graphics/RenderTexture.hpp>

    #include <Jopnal/Core/DebugHandler.hpp>
    #include <Jopnal/Core/Engine.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/Texture/Texture2D.hpp>
    #include <Jopnal/Graphics/Texture/Texture2DDepth.hpp>
    #include <Jopnal/Graphics/Texture/Cubemap.hpp>
    #include <Jopnal/Graphics/Texture/CubemapDepth.hpp>
    #include <Jopnal/Utility/Assert.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>
    #include <Jopnal/Window/Window.hpp>
    #include <vector>

#ifdef JOP_OPENGL_ES

    #define GL_DEPTH_COMPONENT32 GL_DEPTH_COMPONENT
    #define GL_STENCIL_INDEX16 GL_STENCIL_INDEX8

    #define glFramebufferTexture(target, attachment, texture, level) glFramebufferTexture2D((target), (attachment), GL_TEXTURE_2D, (texture), (level))

#endif

#endif

//////////////////////////////////////////////


namespace jop
{
    RenderTexture::RenderTexture()
        : RenderTarget          (0),
          m_depthBuffer         (0),
          m_stencilBuffer       (0),
          m_depthStencilBuffer  (0),
          m_frameBuffer         (0, nullptr),
          m_colorAttachments    ()
    {}

    RenderTexture::RenderTexture(const uint32 ID)
        : RenderTarget          (ID),
          m_depthBuffer         (0),
          m_stencilBuffer       (0),
          m_depthStencilBuffer  (0),
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
                case CA::RGBCubeFloat16:
                    return 6;

                case CA::RGB2DFloat32:
                case CA::RGBCubeFloat32:
                    return 12;

                case CA::RGBA2DFloat16:
                case CA::RGBACubeFloat16:
                    return 8;

                case CA::RGBA2DFloat32:
                case CA::RGBACubeFloat32:
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
                if (!tex->load(size, getColorBytes(attachment), true, false))
                    return false;

                texture = std::move(tex);

                break;
            }

            case CA::RGBCube:
            case CA::RGBACube:
            case CA::RGBCubeFloat16:
            case CA::RGBCubeFloat32:
            case CA::RGBACubeFloat16:
            case CA::RGBACubeFloat32:
            {
                auto tex = std::make_unique<Cubemap>("");
                if (!tex->load(size, getColorBytes(attachment), true, false))
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
                    return GL_DEPTH_COMPONENT16;

                case DA::Renderbuffer24:
                case DA::Texture24:
                case DA::TextureCube24:
                    return GL_DEPTH_COMPONENT24;

                case DA::Renderbuffer32:
                case DA::Texture32:
                case DA::TextureCube32:
                    return GL_DEPTH_COMPONENT32;

                default:
                    return GL_DEPTH_COMPONENT;
            }
        };

        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (m_depthBuffer)
        {
            glCheck(glDeleteRenderbuffers(1, &m_depthBuffer));
            m_depthBuffer = 0;
        }

        if (attachment != DA::None)
        {
            if (attachment < DA::Texture16)
            {
                glCheck(glGenRenderbuffers(1, &m_depthBuffer));

                if (!m_depthBuffer)
                {
                    JOP_DEBUG_ERROR("Failed to create depth buffer");
                    return false;
                }

                glCheck(glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer));
                glCheck(glRenderbufferStorage(GL_RENDERBUFFER, getDepthEnum(attachment), size.x, size.y));
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
                case StencilAttachment::Renderbuffer8:  return GL_STENCIL_INDEX8;
                case StencilAttachment::Renderbuffer16: return GL_STENCIL_INDEX16;
                default:                                return GL_STENCIL_INDEX;
            }
        };

        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (m_stencilBuffer)
        {
            glCheck(glDeleteRenderbuffers(1, &m_stencilBuffer));
            m_stencilBuffer = 0;
        }

        if (attachment != StencilAttachment::None)
        {
            if (attachment < StencilAttachment::Texture8)
            {
                glCheck(glGenRenderbuffers(1, &m_stencilBuffer));

                if (!m_stencilBuffer)
                {
                    JOP_DEBUG_ERROR("Failed to create stencil buffer");
                    return false;
                }

                glCheck(glBindRenderbuffer(GL_RENDERBUFFER, m_stencilBuffer));
                glCheck(glRenderbufferStorage(GL_RENDERBUFFER, getStencilEnum(attachment), size.x, size.y));
            }
            else
            {

            }
        }

        destroy(true, false);

        return true;
    }

    //////////////////////////////////////////////

    bool RenderTexture::addDepthStencilAttachment(const DepthStencilAttachment attachment, const glm::uvec2& size)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (m_depthStencilBuffer)
        {
            glCheck(glDeleteRenderbuffers(1, &m_depthStencilBuffer));
            m_depthStencilBuffer = 0;
        }

        if (attachment != DepthStencilAttachment::None)
        {
            if (attachment < DepthStencilAttachment::Texture24_8)
            {
                glCheck(glGenRenderbuffers(1, &m_depthStencilBuffer));

                if (!m_depthStencilBuffer)
                {
                    JOP_DEBUG_ERROR("Failed to create depth-stencil buffer");
                    return false;
                }

                glCheck(glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBuffer));
                glCheck(glRenderbufferStorage(GL_RENDERBUFFER, attachment == DepthStencilAttachment::Renderbuffer24_8 ? GL_DEPTH24_STENCIL8 : GL_DEPTH32F_STENCIL8, size.x, size.y));
            }
            else
            {

            }
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

            glCheck(glDeleteFramebuffers(1, &m_frameBuffer.first));
            m_frameBuffer.first = 0;
            m_frameBuffer.second = nullptr;
        }

        if (attachments)
        {
            if (m_depthStencilBuffer)
            {
                glCheck(glDeleteRenderbuffers(1, &m_depthStencilBuffer));
                m_depthStencilBuffer = 0;
            }

            if (m_depthBuffer)
            {
                glCheck(glDeleteRenderbuffers(1, &m_depthBuffer));
                m_depthBuffer = 0;
            }

            if (m_stencilBuffer)
            {
                glCheck(glDeleteRenderbuffers(1, &m_stencilBuffer));
                m_stencilBuffer = 0;
            }

            for (auto& i : m_colorAttachments)
                i.reset();
        }
    }

    //////////////////////////////////////////////

    bool RenderTexture::bind() const
    {
        return bind(false);
    }

    //////////////////////////////////////////////

    bool RenderTexture::bind(const bool read) const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (attach() && isValid())
        {
            glCheck(glBindFramebuffer(read ? GL_READ_FRAMEBUFFER : GL_DRAW_FRAMEBUFFER, m_frameBuffer.first));
            glCheck(glViewport(0, 0, getSize().x, getSize().y));
        }

        return isValid();
    }

    //////////////////////////////////////////////

    void RenderTexture::unbind()
    {
        glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        
        const auto size = Engine::getMainWindow().getSize();
        glCheck(glViewport(0, 0, size.x, size.y));
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

        return m_frameBuffer.first != 0 || m_colorAttachments[0] || m_colorAttachments[2];
    }

    //////////////////////////////////////////////

    Texture* RenderTexture::getColorTexture(const ColorAttachmentSlot slot)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        return m_colorAttachments[static_cast<int>(slot)].get();
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

    const Texture* RenderTexture::getStencilTexture() const
    {
        return m_colorAttachments[1].get();
    }

    //////////////////////////////////////////////

    bool RenderTexture::attach() const
    {
        JOP_ASSERT(m_frameBuffer.second == Window::getCurrentContextWindow() || m_frameBuffer.second == nullptr, "Tried to compile a frame buffer created in another context! You must destroy the frame buffer in its context of creation before using it in another. RenderTexture id: " + getID());

        if (!m_frameBuffer.second && !m_frameBuffer.first)
        {
            if (!isValid())
                return true;

            glCheck(glGenFramebuffers(1, &m_frameBuffer.first));

            if (!m_frameBuffer.first)
            {
                JOP_DEBUG_ERROR("Failed to attach RenderTexture. Couldn't generate frame buffer");
                return false;
            }

            m_frameBuffer.second = Window::getCurrentContextWindow();

            glCheck(glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer.first));

            if (m_depthStencilBuffer)
            {
                glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilBuffer));
            }
            else if (m_depthBuffer)
            {
                glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer));
            }
            else if (m_stencilBuffer)
            {
                glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_stencilBuffer));
            }
            
            bool hasColor = false;
            for (std::size_t i = 2; i < m_colorAttachments.size(); ++i)
            {
                if (m_colorAttachments[i])
                {
                    hasColor = true;
                    break;
                }
            }

            if (m_colorAttachments[0] && !m_depthBuffer)
            {
                glCheck(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_colorAttachments[0]->getHandle(), 0));

                if (!hasColor)
                {
                    glCheck(glDrawBuffers(1, GL_NONE));
                    glCheck(glReadBuffer(GL_NONE));
                }
            }
            
            if (hasColor)
            {
                std::vector<GLenum> colorAtt;
                for (std::size_t i = 2; i < m_colorAttachments.size(); ++i)
                {
                    if (!m_colorAttachments[i])
                        continue;

                    glCheck(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i - 2, m_colorAttachments[i]->getHandle(), 0));
                    colorAtt.push_back(GL_COLOR_ATTACHMENT0 + i - 2);
                }

                if (!colorAtt.empty())
                {
                    glCheck(glDrawBuffers(colorAtt.size(), colorAtt.data()));
                }
            }

            auto status = glCheck(glCheckFramebufferStatus(GL_FRAMEBUFFER));
            if (status != GL_FRAMEBUFFER_COMPLETE)
            {
                JOP_DEBUG_ERROR("Failed to create RenderTexture. Failed to complete frame buffer");
                destroy(true, false);
                return false;
            }
        }

        return true;
    }
}