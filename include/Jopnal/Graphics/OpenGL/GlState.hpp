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
    namespace detail
    {
        class WindowImpl;
    }

    class JOP_API GlState
    {
    private:

        friend class detail::WindowImpl;

        /// Reset the cached values to defaults
        ///
        static void reset();

    public:

        /// Depth functions
        ///
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

        /// Face culling modes
        ///
        enum class FaceCull
        {
            Front,
            Back,
            Both
        };

        /// Blend functions
        ///
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

        /// Polygon mode
        ///
        enum class PolygonMode
        {
            Fill,
            Line,
            Point
        };

    public:
    
        /// \brief Set the depth test
        ///
        /// \param enable Enable depth test
        /// \param func The depth function
        ///
        static void setDepthTest(const bool enable, const DepthFunc func = DepthFunc::Less);

        /// \brief Set the face culling mode
        ///
        /// \param enable Enable face culling
        /// \param cull The face culling mode
        ///
        static void setFaceCull(const bool enable, const FaceCull cull = FaceCull::Back);

        /// \brief Set the blend function
        ///
        /// \param enable Enable blending
        /// \param srcFactor The source factor
        /// \param dstFactor The destination factor
        ///
        static void setBlendFunc(const bool enable, const BlendFunc srcFactor = BlendFunc::SrcAlpha, const BlendFunc dstFactor = BlendFunc::OneMinusSrcAlpha);

        /// \brief Set the stencil test
        ///
        /// \param enable Enable stencil test
        /// \param func The stencil function
        /// \param mask The mask
        ///
        static void setStencilTest(const bool enable, const DepthFunc func = DepthFunc::Always, const unsigned int mask = 0xFFFFFFFF);

        /// \brief Enable/disable seamless cube mapping
        ///
        /// \param enable Enable?
        ///
        static void setSeamlessCubemap(const bool enable);

        /// \brief Enable/disable polygon smoothing
        ///
        /// \param enable Enable?
        ///
        static void setPolygonSmooth(const bool enable);

        /// \brief Enable/disable line smoothing
        ///
        /// \param enable Enable?
        ///
        static void setLineSmooth(const bool enable);

        /// \brief Set the line width
        ///
        /// \param width The new width
        ///
        static void setLineWidth(const float width);

        /// \brief Set the polygon rasterization mode
        ///
        /// \param mode The mode
        ///
        static void setPolygonMode(const PolygonMode mode);

        /// \brief Enable/disable vertex attribute
        ///
        /// \param enable Enable the attribute?
        /// \param index The attribute index
        ///
        static void setVertexAttribute(const bool enable, const unsigned int index);

        /// \brief Enable/disable depth buffer writing
        ///
        /// \param enable Enable depth writes?
        ///
        static void setDepthWrite(const bool enable);

        /// \brief Call glFlush
        ///
        static void flush();
    };
}

/// \class jop::GlStateManager
/// \ingroup graphics

#endif