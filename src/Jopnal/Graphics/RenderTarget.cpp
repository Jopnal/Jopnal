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
    RenderTarget::RenderTarget(const std::string& ID)
        : Subsystem             (ID),
          m_clearColor          (Color::Black),
          m_clearDepth          (1.f),
          m_clearStencil        (0),
          m_clearAttribsChanged (true)
    {}

    RenderTarget::~RenderTarget()
    {}

    //////////////////////////////////////////////

    void RenderTarget::clear(const unsigned int bits)
    {
        if (bind())
        {
            if (m_clearAttribsChanged)
            {
                auto vec = m_clearColor.asRGBAFloatVector();

                glCheck(gl::ClearColor(vec.r, vec.g, vec.b, vec.a));
                glCheck(gl::ClearDepth(static_cast<GLdouble>(m_clearDepth)));
                glCheck(gl::ClearStencil(m_clearStencil));
            }

            glCheck(gl::Clear
            (
                ((bits & ColorBit) != 0)   * gl::COLOR_BUFFER_BIT |
                ((bits & DepthBit) != 0)   * gl::DEPTH_BUFFER_BIT |
                ((bits & StencilBit) != 0) * gl::STENCIL_BUFFER_BIT
            ));
        }
    }

    //////////////////////////////////////////////

    RenderTarget& RenderTarget::setClearColor(const Color color)
    {
        m_clearColor = color;
        m_clearAttribsChanged = true;

        return *this;
    }

    //////////////////////////////////////////////

    Color RenderTarget::getClearColor() const
    {
        return m_clearColor;
    }

    //////////////////////////////////////////////

    RenderTarget& RenderTarget::setClearDepth(const float depth)
    {
        m_clearDepth = depth;
        m_clearAttribsChanged = true;

        return *this;
    }

    //////////////////////////////////////////////

    float RenderTarget::getClearDepth() const
    {
        return m_clearDepth;
    }

    //////////////////////////////////////////////

    RenderTarget& RenderTarget::setClearStencil(const int stencil)
    {
        m_clearStencil = stencil;
        m_clearAttribsChanged = true;

        return *this;
    }

    //////////////////////////////////////////////

    int RenderTarget::getClearStencil() const
    {
        return m_clearStencil;
    }
}