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

#ifndef JOP_RENDERTARGET_HPP
#define JOP_RENDERTARGET_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/SubSystem.hpp>
#include <Jopnal/Graphics/Color.hpp>
#include <Jopnal/MathInclude.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API RenderTarget : public Subsystem
    {
    private:

        JOP_DISALLOW_COPY_MOVE(RenderTarget);

    public:

        enum
        {
            ColorBit    = 1,
            DepthBit    = 1 << 1,
            StencilBit  = 1 << 2,

            AllBit      = ColorBit | DepthBit | StencilBit
        };

    public:
    
        RenderTarget(const std::string& ID);

        virtual ~RenderTarget() = 0;


        void clear(const unsigned int bits = AllBit);

        virtual bool bind() const = 0;

        virtual glm::uvec2 getSize() const = 0;



        /// \brief Set the clear color
        ///
        /// \param color The new color
        ///
        RenderTarget& setClearColor(const Color color);

        Color getClearColor() const;

        RenderTarget& setClearDepth(const float depth);

        float getClearDepth() const;

        RenderTarget& setClearStencil(const int stencil);

        int getClearStencil() const;

    private:

        Color m_clearColor;
        float m_clearDepth;
        int m_clearStencil;
        bool m_clearAttribsChanged;
    };
}

#endif

/// \class RenderTarget
/// \ingroup graphics
///
/// #TODO Detailed description