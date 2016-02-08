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
    const GLenum ns_depthEnums[] =
    {
        gl::NEVER,
        gl::LESS,
        gl::EQUAL,
        gl::LEQUAL,
        gl::GREATER,
        gl::NOTEQUAL,
        gl::GEQUAL,
        gl::ALWAYS
    };

    std::pair<bool, jop::GlState::DepthFunc> ns_depth;
    std::pair<bool, jop::GlState::FaceCull> ns_faceCull;
    std::tuple<bool, jop::GlState::BlendFunc, jop::GlState::BlendFunc> ns_blend;
    std::tuple<bool, jop::GlState::DepthFunc, unsigned int> ns_stencil;
    bool ns_seamlessCubemap;
    bool ns_polygonSmooth;
    std::pair<bool, float> ns_line;

    void enableDisable(const bool enable, GLenum enum_)
    {
        // Don't touch these braces, they're just as they should be
        if (enable)
        {
            glCheck(gl::Enable(enum_));
        }
        else
            glCheck(gl::Disable(enum_));
    }
}

namespace jop
{
    void GlState::reset()
    {
        // Depth test is disabled by default
        ns_depth = std::make_pair(false, DepthFunc::Less);

        // Face culling is disabled by default
        ns_faceCull = std::make_pair(false, FaceCull::Back);

        // Blending is disabled by default
        ns_blend = std::make_tuple(false, BlendFunc::One, BlendFunc::Zero);

        // Stencil test is disabled by default
        ns_stencil = std::make_tuple(false, DepthFunc::Always, 0xFFFFFFFF);

        // Seamless cube map is disabled by default
        ns_seamlessCubemap = false;

        // Polygon smoothing is disabled by default
        ns_polygonSmooth = false;

        // Line smoothing is disabled by default and its width is 1.0
        ns_line = std::make_pair(false, 1.f);
    }

    //////////////////////////////////////////////

    void GlState::setDepthTest(const bool enable, const DepthFunc func)
    {
        if (enable != ns_depth.first || func != ns_depth.second)
        {
            enableDisable(enable, gl::DEPTH_TEST);
            glCheck(gl::DepthFunc(ns_depthEnums[static_cast<int>(func)]));

            ns_depth = std::make_pair(enable, func);
        }
    }

    //////////////////////////////////////////////

    void GlState::setFaceCull(const bool enable, const FaceCull cull)
    {
        if (enable != ns_faceCull.first || cull != ns_faceCull.second)
        {
            static const GLenum modes[] =
            {
                gl::FRONT,
                gl::BACK,
                gl::FRONT_AND_BACK
            };

            enableDisable(enable, gl::CULL_FACE);
            glCheck(gl::CullFace(modes[static_cast<int>(cull)]));

            ns_faceCull = std::make_pair(enable, cull);
        }
    }

    //////////////////////////////////////////////

    void GlState::setBlendFunc(const bool enable, const BlendFunc srcFactor, const BlendFunc dstFactor)
    {
        if (enable != std::get<0>(ns_blend) || srcFactor != std::get<1>(ns_blend) || dstFactor != std::get<2>(ns_blend))
        {
            static const GLenum modes[] =
            {
                gl::ZERO,
                gl::ONE,
                gl::SRC_COLOR,
                gl::ONE_MINUS_SRC_COLOR,
                gl::DST_COLOR,
                gl::ONE_MINUS_DST_COLOR,
                gl::SRC_ALPHA,
                gl::ONE_MINUS_SRC_ALPHA,
                gl::DST_ALPHA,
                gl::ONE_MINUS_DST_ALPHA,
                gl::CONSTANT_COLOR,
                gl::ONE_MINUS_CONSTANT_COLOR,
                gl::CONSTANT_ALPHA,
                gl::ONE_MINUS_CONSTANT_ALPHA
            };

            enableDisable(enable, gl::BLEND);
            glCheck(gl::BlendFunc(modes[static_cast<int>(srcFactor)], modes[static_cast<int>(dstFactor)]));

            ns_blend = std::make_tuple(enable, srcFactor, dstFactor);
        }
    }

    //////////////////////////////////////////////

    void GlState::setStencilTest(const bool enable, const DepthFunc func, const unsigned int mask)
    {
        if (enable != std::get<0>(ns_stencil) || func != std::get<1>(ns_stencil) || mask != std::get<2>(ns_stencil))
        {
            enableDisable(enable, gl::STENCIL_TEST);
            glCheck(gl::StencilFunc(ns_depthEnums[static_cast<int>(func)], 0, mask));

            ns_stencil = std::make_tuple(enable, func, mask);
        }
    }

    //////////////////////////////////////////////

    void GlState::setSeamlessCubemap(const bool enable)
    {
        if (enable != ns_seamlessCubemap)
        {
            enableDisable(enable, gl::TEXTURE_CUBE_MAP_SEAMLESS);

            ns_seamlessCubemap = enable;
        }
    }

    //////////////////////////////////////////////

    void GlState::setPolygonSmooth(const bool enable)
    {
        if (enable != ns_polygonSmooth)
        {
            enableDisable(enable, gl::POLYGON_SMOOTH);

            ns_polygonSmooth = enable;
        }
    }

    //////////////////////////////////////////////

    void GlState::setLineSmooth(const bool enable)
    {
        if (enable != ns_line.first)
        {
            enableDisable(enable, gl::LINE_SMOOTH);

            ns_line.first = enable;
        }
    }

    //////////////////////////////////////////////

    void GlState::setLineWidth(const float width)
    {
        if (width != ns_line.second)
        {
            glCheck(gl::LineWidth(width));

            ns_line.second = width;
        }
    }
}