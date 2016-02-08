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

#ifndef JOP_GLSTATE_HPP
#define JOP_GLSTATE_HPP

// Headers
#include <Jopnal/Header.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API GlState
    {
    private:

        friend class Window;

        static void reset();

    public:

        enum class DepthFunc
        {
            Never,
            Less,
            Equal,
            LessEqual,
            Greater,
            NotEqual,
            GreaterEqual,
            Always
        };

        enum class FaceCull
        {
            Front,
            Back,
            Both
        };

        enum class BlendFunc
        {
            Zero,
            One,
            SrcColor,
            OneMinusSrcColor,
            DstColor,
            OneMinusDstColor,
            SrcAlpha,
            OneMinusSrcAlpha,
            DstAlpha,
            OneMinusDstAlpha,
            ConstantColor,
            OneMinusConstantColor,
            ConstantAlpha,
            OneMinusConstantAlpha
        };

    public:
    
        static void setDepthTest(const bool enable, const DepthFunc func = DepthFunc::Less);

        static void setFaceCull(const bool enable, const FaceCull cull = FaceCull::Back);

        static void setBlendFunc(const bool enable, const BlendFunc srcFactor = BlendFunc::SrcAlpha, const BlendFunc dstFactor = BlendFunc::OneMinusSrcAlpha);

        static void setStencilTest(const bool enable, const DepthFunc func = DepthFunc::Always, const unsigned int mask = 0xFFFFFFFF);

        static void setSeamlessCubemap(const bool enable);

        static void setPolygonSmooth(const bool enable);

        static void setLineSmooth(const bool enable);

        static void setLineWidth(const unsigned int width);
        
    };
}

#endif

/// \class GlStateManager
/// \ingroup graphics
///
/// #TODO Detailed description
