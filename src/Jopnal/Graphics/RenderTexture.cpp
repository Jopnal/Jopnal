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
    #include <Jopnal/Core/Android/ActivityState.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/Texture/Texture2D.hpp>
    #include <Jopnal/Graphics/Texture/Cubemap.hpp>
    #include <Jopnal/Utility/Assert.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>
    #include <Jopnal/Window/Window.hpp>
    #include <vector>

#endif

//////////////////////////////////////////////


namespace jop
{
    namespace detail
    {
        bool checkFrameBufferStatus()
        {
            const GLenum status = glCheck(glCheckFramebufferStatus(GL_FRAMEBUFFER));

            if (status != GL_FRAMEBUFFER_COMPLETE)
            {
                const char* errorS = "Unknown error";

                switch (status)
                {
                #if !defined(JOP_OPENGL_ES) || defined(GL_ES_VERSION_3_0)

                    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                        errorS = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
                    case GL_FRAMEBUFFER_UNDEFINED:
                        errorS = "GL_FRAMEBUFFER_UNDEFINED";
                        break;

                #elif !defined(GL_ES_VERSION_3_0)

                    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
                        errorS = "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS";

                #endif

                #ifndef JOP_OPENGL_ES

                    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                        errorS = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
                        break;
                    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                        errorS = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
                        break;
                    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                        errorS = "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
                        break;

                #endif

                    case GL_FRAMEBUFFER_UNSUPPORTED:
                        errorS = "GL_FRAMEBUFFER_UNSUPPORTED";
                        break;
                    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                        errorS = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHEMENT";
                        break;
                    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                        errorS = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHEMENT";
                        break;
                }

                JOP_DEBUG_ERROR("Failed to create render texture: " << errorS);

                return false;
            }

            return true;
        }
    }

    //////////////////////////////////////////////

    RenderTexture::RenderTexture()
        : RenderTarget          (),
          m_frameBuffer         (0),
          m_attachments         (),
          m_size                (0)
    {}

    RenderTexture::~RenderTexture()
    {
        destroy(true, true);
    }

    //////////////////////////////////////////////

    bool RenderTexture::addTextureAttachment(const Slot slot, const Texture::Format format)
    {
        auto& att = m_attachments[static_cast<int>(slot)];
        auto& tex = att.second;

        glCheck(glDeleteRenderbuffers(1, &att.first));
        att.first = 0;
        tex.reset();

        if (format == Texture::Format::None)
            return true;

        if (m_size == glm::uvec2(0))
        {
            JOP_DEBUG_ERROR("Failed to add render texture attachment, size is 0");
            return false;
        }

        using F = Texture::Format;

    #ifdef JOP_OPENGL_ES

        static const bool hdrSupport = JOP_CHECK_GL_EXTENSION(GL_EXT_color_buffer_half_float);

        if (format == F::RGB_F_16 && !hdrSupport && gl::getVersionMajor() >= 3 && gl::getVersionMinor() >= 2)
        {
            JOP_DEBUG_WARNING("HDR RGB frame buffer texture is not supported, attempting to create a HDR RGBA texture...");
            return addTextureAttachment(slot, F::RGBA_F_16);
        }

        #if JOP_MIN_OPENGL_ES_VERSION < 320

            if (format == F::RGB_F_16 || format == F::RGBA_F_16)
            {
                if (!(format == F::RGBA_F_16 && gl::getVersionMajor() >= 3 && gl::getVersionMinor() >= 2) && !hdrSupport)
                {
                    JOP_DEBUG_WARNING("HDR frame buffer textures are not supported, attempting to create respective LDR texture...");
                    return addTextureAttachment(slot, format == F::RGB_F_16 ? F::RGB_UB_8 : F::RGBA_UB_8);
                }
            }

            if (format == F::Stencil_UB_8 && !JOP_CHECK_GL_EXTENSION(GL_OES_texture_stencil8))
            {
                JOP_DEBUG_WARNING("Stancil frame buffer texture attachment is not supported, attempting to create respective render buffer...");
                return addRenderbufferAttachment(slot, format);
            }

        #endif

        #if JOP_MIN_OPENGL_ES_VERSION < 300

            if (gl::getVersionMajor() < 3)
            {
                if ((format == F::Depth_US_16 || format == F::Depth_UI_24) && !JOP_CHECK_GL_EXTENSION(OES_depth_texture))
                {
                    JOP_DEBUG_ERROR("Depth frame buffer texture is not supported");
                    return false;
                }

                if (format == F::DepthStencil_UI_24_B_8 && !JOP_CHECK_GL_EXTENSION(OES_packed_depth_stencil))
                {
                    JOP_DEBUG_ERROR("Depth-stencil frame buffer texture is not supported");
                    return false;
                }
            }

        #endif

    #endif

        auto newTex = std::make_unique<Texture2D>("");

        using TF = Texture::Flag;
        
        if (newTex->load(m_size, format, TF::DisallowCompression | TF::DisallowMipmapGeneration | TF::DisallowSRGB))
            tex = std::move(newTex);

        else
        {
            JOP_DEBUG_ERROR("Failed to add render texture attachment, failed to load texture (possibly invalid or unsupported format)");
            return false;
        }

        return true;
    }

    //////////////////////////////////////////////

#ifndef JOP_OPENGL_ES

    namespace detail
    {
        extern GLenum getInternalFormatEnum(const Texture::Format format, const bool srgb);
    }

#endif

    //////////////////////////////////////////////

    bool RenderTexture::addRenderbufferAttachment(const Slot slot, const Texture::Format format)
    {
        auto& att = m_attachments[static_cast<int>(slot)];
        auto& rendBuf = att.first;

        glCheck(glDeleteRenderbuffers(1, &rendBuf));
        rendBuf = 0;
        att.second.reset();

        if (format == Texture::Format::None)
            return true;

        if (m_size == glm::uvec2(0))
        {
            JOP_DEBUG_ERROR("Failed to add render render buffer attachment, size is 0");
            return false;
        }
        else if (m_size.x > getMaximumRenderbufferSize() || m_size.y > getMaximumRenderbufferSize())
        {
            // Already warned in setSize()
            return addTextureAttachment(slot, format);
        }

        using F = Texture::Format;

    #ifdef JOP_OPENGL_ES

        if ((format == F::RGB_F_16 || format == F::RGBA_F_16) && !JOP_CHECK_GL_EXTENSION(EXT_color_buffer_half_float))
        {
            JOP_DEBUG_WARNING("HDR render buffers are not supported, attempting to create respective LDR render buffer...");
            return addRenderbufferAttachment(slot, format == F::RGB_F_16 ? F::RGB_UB_8 : F::RGBA_UB_8);
        }

    #endif

    #if defined(JOP_OPENGL_ES) && JOP_MIN_OPENGL_ES_VERSION < 300

        if (gl::getVersionMajor() < 300)
        {
            if (slot == Slot::DepthStencil && !JOP_CHECK_GL_EXTENSION(OES_packed_depth_stencil))
            {
                JOP_DEBUG_ERROR("Packed depth-stencil render buffer is not supported");
                return false;
            }
            else if (slot == Slot::Depth && format == F::Depth_UI_24 && !JOP_CHECK_GL_EXTENSION(OES_depth24))
            {
                JOP_DEBUG_WARNING("24 bit depth render buffer attachment is not supported, attempting to create texture attachment...");
                return addTextureAttachment(slot, format);
            }
            else if (slot == Slot::Color0)
            {
                if (format == F::Alpha_UB_8)
                {
                    JOP_DEBUG_ERROR("Alpha render buffer attachment is not supported");
                    return false;
                }
                if (format == F::RGB_UB_8 && !JOP_CHECK_GL_EXTENSION(OES_rgb8_rgba8))
                {
                    JOP_DEBUG_WARNING("RGB render buffer attachment is not supported, attempting to create texture attachment...");
                    return addTextureAttachment(slot, format);
                }
                else if (format == F::RGBA_UB_8 && !JOP_CHECK_GL_EXTENSION(OES_rgb8_rgba8) && !JOP_CHECK_GL_EXTENSION(ARM_rgba8))
                {
                    JOP_DEBUG_WARNING("RGBA render buffer attachment is not supported, attempting to create texture attachment...");
                    return addTextureAttachment(slot, format);
                }
            }
        }

    #endif

        glCheck(glDeleteRenderbuffers(1, &rendBuf));
        glCheck(glGenRenderbuffers(1, &rendBuf));

        if (!rendBuf)
        {
            JOP_DEBUG_ERROR("Failed to add render buffer attachment, failed to generate render buffer");
            return false;
        }


    #ifndef JOP_OPENGL_ES

        const GLenum intFormat = detail::getInternalFormatEnum(format, false);

    #else

        const GLenum intFormat = [](const Texture::Format format)
        {
            switch (format)
            {
            #ifdef GL_ES_VERSION_3_0

                case F::Alpha_UB_8:
                    return GL_R8;

            #endif

                case F::RGB_UB_8:
                    return GL_RGB8_OES;

                case F::RGBA_UB_8:
                    return GL_RGBA8_OES;

                case F::Depth_US_16:
                    return GL_DEPTH_COMPONENT16;

                case F::Depth_UI_24:
                    return GL_DEPTH_COMPONENT24_OES;

                case F::Stencil_UB_8:
                    return GL_STENCIL_INDEX8;

                case F::DepthStencil_UI_24_B_8:
                    return GL_DEPTH24_STENCIL8_OES;
            }

            return 0;

        }(format);

    #endif

        if (intFormat)
        {
            glCheck(glBindRenderbuffer(GL_RENDERBUFFER, rendBuf));
            glCheck(glRenderbufferStorage(GL_RENDERBUFFER, intFormat, m_size.x, m_size.y));
            glCheck(glBindRenderbuffer(GL_RENDERBUFFER, 0));
        }
        else
        {
            JOP_DEBUG_ERROR("Failed to create render buffer storage, unsupported format");
            glCheck(glDeleteRenderbuffers(1, &rendBuf));
            return false;
        }

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
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (!framebuffer && !attachments)
            return;

        unbind();

        if (framebuffer)
        {
            glCheck(glDeleteFramebuffers(1, &m_frameBuffer));
            m_frameBuffer = 0;
        }

        if (attachments)
        {
            for (auto& i : m_attachments)
            {
                glCheck(glDeleteRenderbuffers(1, &i.first));
                i.second.reset();
            }

            m_size = glm::uvec2(0);
        }
    }

    //////////////////////////////////////////////

    bool RenderTexture::bind() const
    {
        return bindDraw();
    }

    //////////////////////////////////////////////

    namespace detail
    {
        void bindFBOBase(const GLuint fb, const glm::uvec2& size, const GLenum binding)
        {
        #ifdef JOP_OS_ANDROID

            if (!detail::ActivityState::get()->fullFocus)
                return;

        #endif

            glCheck(glBindFramebuffer(binding, fb));

            glCheck(glViewport(0, 0, size.x, size.y));
            glCheck(glScissor (0, 0, size.x, size.y));
        }
    }

    bool RenderTexture::bindRead() const
    {
    #ifdef JOP_OPENGL_ES
        
        #ifndef GL_ES_VERSION_3_0
            return false;

        #else

            if (gl::getVersionMajor() < 3)
                return false;

        #endif

    #else

        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (attach() && isValid())
            detail::bindFBOBase(m_frameBuffer, m_size, GL_READ_FRAMEBUFFER);

        return isValid();

    #endif
    }

    bool RenderTexture::bindDraw() const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (attach() && isValid())
            detail::bindFBOBase(m_frameBuffer, m_size, GL_FRAMEBUFFER);

        return isValid();
    }

    //////////////////////////////////////////////

    void RenderTexture::unbind()
    {
        detail::bindFBOBase(0, Engine::getMainWindow().getSize(), GL_FRAMEBUFFER);
    }

    //////////////////////////////////////////////

    void RenderTexture::setSize(const glm::uvec2& size)
    {
        if (!m_size.x && !m_size.y)
        {
            if (!size.x || !size.y)
            {
                JOP_DEBUG_ERROR("Both frame buffer dimensions must be greater than 0");
                return;
            }

            else if (size.x > getMaximumRenderbufferSize() || size.y > getMaximumRenderbufferSize())
                JOP_DEBUG_WARNING("Maximum render buffer size is " << getMaximumRenderbufferSize() << ", render texture will use texture attachments only");

            m_size = size;
        }
    }

    //////////////////////////////////////////////

    glm::uvec2 RenderTexture::getSize() const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        return m_size;
    }

    //////////////////////////////////////////////

    bool RenderTexture::isValid() const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        for (auto& i : m_attachments)
        {
            if (i.first || i.second)
                return true;
        }

        return false;
    }

    //////////////////////////////////////////////

    Texture* RenderTexture::getTextureAttachment(const Slot slot)
    {
        return m_attachments[static_cast<int>(slot)].second.get();
    }

    //////////////////////////////////////////////

    const Texture* RenderTexture::getTextureAttachment(const Slot slot) const
    {
        return m_attachments[static_cast<int>(slot)].second.get();
    }

    //////////////////////////////////////////////

    unsigned int RenderTexture::getMaximumRenderbufferSize()
    {
        static unsigned int size = 0;

        if (!size)
        {
            glCheck(glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, reinterpret_cast<GLint*>(&size)));
        }

        return size;
    }

    //////////////////////////////////////////////

    bool RenderTexture::attach() const
    {
        if (!m_frameBuffer)
        {
            if (!isValid())
                return true;

            glCheck(glGenFramebuffers(1, &m_frameBuffer));

            if (!m_frameBuffer)
            {
                JOP_DEBUG_ERROR("Failed to attach RenderTexture. Couldn't generate frame buffer");
                return false;
            }

            const GLenum attachmentPoint[] =
            {
                GL_DEPTH_ATTACHMENT,
                GL_STENCIL_ATTACHMENT,

            #if !defined(JOP_OPENGL_ES) || defined(GL_ES_VERSION_3_0)
                GL_DEPTH_STENCIL_ATTACHMENT,
            #else
                0,
            #endif

                GL_COLOR_ATTACHMENT0
            };

            glCheck(glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer));

            for (std::size_t i = 0; i < m_attachments.size(); ++i)
            {
                auto att = &m_attachments[i];

            #if defined(JOP_OPENGL_ES) && JOP_MIN_OPENGL_ES_VERSION < 300

                if (gl::getVersionMajor() < 3)
                {
                    const auto s = static_cast<Slot>(i);

                    if (s == Slot::DepthStencil)
                        continue;

                    if (s == Slot::Depth || s == Slot::Stencil)
                        att = &m_attachments[static_cast<int>(Slot::DepthStencil)];
                }

            #endif

                if (att->first)
                {
                    glCheck(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentPoint[i], GL_RENDERBUFFER, att->first));
                }
                else if (att->second && att->second->isValid())
                {
                    glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentPoint[i], GL_TEXTURE_2D, att->second->getHandle(), 0));
                }
            }

        #if !defined(JOP_OPENGL_ES) || defined(GL_ES_VERSION_3_0)

            if (gl::getVersionMajor() >= 3)
            {
                // Enable/disable writing to specific color buffers
                std::vector<GLenum> colorAttachments;

                for (std::size_t i = static_cast<int>(Slot::Color0); i < m_attachments.size(); ++i)
                    colorAttachments.push_back((m_attachments[i].first || m_attachments[i].second) ? attachmentPoint[i] : GL_NONE);

                glCheck(glDrawBuffers(colorAttachments.size(), colorAttachments.data()));
            }

        #endif

            return detail::checkFrameBufferStatus();
        }

        return true;
    }
}