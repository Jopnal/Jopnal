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
}

namespace jop
{
    void GlState::reset()
    {
        // Depth test is disabled by default
        glCheck(gl::Disable(gl::DEPTH_TEST)); glCheck(gl::DepthFunc(gl::LESS));

        // Face culling is disabled by default
        glCheck(gl::Disable(gl::CULL_FACE)); glCheck(gl::CullFace(gl::BACK));

        // Blending is disabled by default
        glCheck(gl::Disable(gl::BLEND)); glCheck(gl::BlendFunc(gl::ONE, gl::ZERO));

        // Stencil test is disabled by default
        glCheck(gl::Disable(gl::STENCIL_TEST)); glCheck(gl::StencilFunc(gl::ALWAYS, 0, 0xFFFFFFFF));

        // Seamless cubemap is disabled by default
    }

    //////////////////////////////////////////////

    void GlState::setDepthTest(const bool enable, const DepthFunc func)
    {
        if (enable != ns_depth.first || func != ns_depth.second)
        {
            // Don't touch these braces, they're just as they should be
            if (enable)
            {
                glCheck(gl::Enable(gl::DEPTH_TEST));
            }
            else
                glCheck(gl::Disable(gl::DEPTH_TEST));

            glCheck(gl::DepthFunc(ns_depthEnums[static_cast<int>(func)]));

            ns_depth.first = enable; ns_depth.second = func;
        }
    }

    //////////////////////////////////////////////

    void GlState::setFaceCull(const bool enable, const FaceCull cull)
    {

    }

    //////////////////////////////////////////////

    void GlState::setBlendFunc(const bool enable, const BlendFunc srcFactor, const BlendFunc dstFactor)
    {

    }

    //////////////////////////////////////////////

    void GlState::setStencilTest(const bool enable, const DepthFunc func, const unsigned int mask)
    {

    }

    //////////////////////////////////////////////

    void GlState::setSeamlessCubemap(const bool enable)
    {

    }

    //////////////////////////////////////////////

    void GlState::setPolygonSmooth(const bool enable)
    {

    }

    //////////////////////////////////////////////

    void GlState::setLineSmooth(const bool enable)
    {

    }

    //////////////////////////////////////////////

    void GlState::setLineWidth(const unsigned int width)
    {

    }

}