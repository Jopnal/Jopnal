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

	#include <Jopnal/Graphics/RenderTarget.hpp>

    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    RenderTarget::RenderTarget(const uint32 ID)
        : Subsystem             (ID),
          m_mutex               (),
          m_clearColor          (Color::Black),
          m_clearDepth          (1.f),
          m_clearStencil        (0)
    {}

    RenderTarget::~RenderTarget()
    {}

    //////////////////////////////////////////////

    void RenderTarget::postUpdate(const float)
    {
        clear(AllBit);
    }

    //////////////////////////////////////////////

    void RenderTarget::clear(const unsigned int bits)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        if (bind())
        {
            glCheck(glClearColor(m_clearColor.colors.r, m_clearColor.colors.g, m_clearColor.colors.b, m_clearColor.alpha));

        #ifndef JOP_OPENGL_ES
            glCheck(glClearDepth(static_cast<GLdouble>(m_clearDepth.load())));
        #else
            glCheck(glClearDepthf(m_clearDepth.load()));
        #endif
            glCheck(glClearStencil(m_clearStencil.load()));

            glCheck(glClear
            (
                ((bits & ColorBit) != 0)   * GL_COLOR_BUFFER_BIT |
                ((bits & DepthBit) != 0)   * GL_DEPTH_BUFFER_BIT |
                ((bits & StencilBit) != 0) * GL_STENCIL_BUFFER_BIT
            ));
        }
    }

    //////////////////////////////////////////////

    RenderTarget& RenderTarget::setClearColor(const Color& color)
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);

        m_clearColor = color;
        return *this;
    }

    //////////////////////////////////////////////

    Color RenderTarget::getClearColor() const
    {
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
        return m_clearColor;
    }

    //////////////////////////////////////////////

    RenderTarget& RenderTarget::setClearDepth(const float depth)
    {
        m_clearDepth.store(depth);
        return *this;
    }

    //////////////////////////////////////////////

    float RenderTarget::getClearDepth() const
    {
        return m_clearDepth.load();
    }

    //////////////////////////////////////////////

    RenderTarget& RenderTarget::setClearStencil(const int stencil)
    {
        m_clearStencil.store(stencil);
        return *this;
    }

    //////////////////////////////////////////////

    int RenderTarget::getClearStencil() const
    {
        return m_clearStencil.load();
    }
}