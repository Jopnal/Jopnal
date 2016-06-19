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

	#include <Jopnal/Graphics/OpenGL/GlState.hpp>

    #include <Jopnal/Core/DebugHandler.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>
    #include <tuple>
    #include <vector>

#endif

//////////////////////////////////////////////


namespace
{
    const GLenum ns_depthEnums[] =
    {
        GL_NEVER,
        GL_LESS,
        GL_EQUAL,
        GL_LEQUAL,
        GL_GREATER,
        GL_NOTEQUAL,
        GL_GEQUAL,
        GL_ALWAYS
    };

    std::pair<bool, jop::GlState::DepthFunc> ns_depth;
    std::pair<bool, jop::GlState::FaceCull> ns_faceCull;
    std::tuple<bool, jop::GlState::BlendFunc, jop::GlState::BlendFunc> ns_blend;
    std::tuple<bool, jop::GlState::DepthFunc, unsigned int> ns_stencil;
    bool ns_seamlessCubemap;
    bool ns_polygonSmooth;
    std::pair<bool, float> ns_line;
    jop::GlState::PolygonMode ns_polygonMode;
    std::vector<bool> ns_vertexAttribs;
    bool ns_depthMask;
    bool ns_framebufSrgb;

    void enableDisable(const bool enable, GLenum enum_)
    {
        if (enable)
        {
            glCheck(glEnable(enum_));
        }
        else
        {
            glCheck(glDisable(enum_));
        }
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

        // Polygon mode is Fill by default
        ns_polygonMode = PolygonMode::Fill;

        // All vertex attributes are disabled by default
        ns_vertexAttribs.clear();

        // Depth mask is enabled by default
        ns_depthMask = true;

        // Frame buffer srgb is disabled by default
        ns_framebufSrgb = false;

        JOP_DEBUG_INFO("OpenGL state reset");
    }

    //////////////////////////////////////////////

    void GlState::setDepthTest(const bool enable, const DepthFunc func)
    {
        if (enable != ns_depth.first || func != ns_depth.second)
        {
            enableDisable(enable, GL_DEPTH_TEST);
            glCheck(glDepthFunc(ns_depthEnums[static_cast<int>(func)]));

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
                GL_FRONT,
                GL_BACK,
                GL_FRONT_AND_BACK
            };

            enableDisable(enable, GL_CULL_FACE);
            glCheck(glCullFace(modes[static_cast<int>(cull)]));

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
                GL_ZERO,
                GL_ONE,
                GL_SRC_COLOR,
                GL_ONE_MINUS_SRC_COLOR,
                GL_DST_COLOR,
                GL_ONE_MINUS_DST_COLOR,
                GL_SRC_ALPHA,
                GL_ONE_MINUS_SRC_ALPHA,
                GL_DST_ALPHA,
                GL_ONE_MINUS_DST_ALPHA,
                GL_CONSTANT_COLOR,
                GL_ONE_MINUS_CONSTANT_COLOR,
                GL_CONSTANT_ALPHA,
                GL_ONE_MINUS_CONSTANT_ALPHA
            };

            enableDisable(enable, GL_BLEND);
            glCheck(glBlendFunc(modes[static_cast<int>(srcFactor)], modes[static_cast<int>(dstFactor)]));

            ns_blend = std::make_tuple(enable, srcFactor, dstFactor);
        }
    }

    //////////////////////////////////////////////

    void GlState::setStencilTest(const bool enable, const DepthFunc func, const unsigned int mask)
    {
        if (enable != std::get<0>(ns_stencil) || func != std::get<1>(ns_stencil) || mask != std::get<2>(ns_stencil))
        {
            enableDisable(enable, GL_STENCIL_TEST);
            glCheck(glStencilFunc(ns_depthEnums[static_cast<int>(func)], 0, mask));

            ns_stencil = std::make_tuple(enable, func, mask);
        }
    }

    //////////////////////////////////////////////

    void GlState::setSeamlessCubemap(const bool enable)
    {
    #ifndef JOP_OPENGL_ES

        if (enable != ns_seamlessCubemap)
        {
            enableDisable(enable, GL_TEXTURE_CUBE_MAP_SEAMLESS);

            ns_seamlessCubemap = enable;
        }

    #else

        enable;

    #endif
    }

    //////////////////////////////////////////////

    void GlState::setPolygonSmooth(const bool enable)
    {
    #ifndef JOP_OPENGL_ES

        if (enable != ns_polygonSmooth)
        {
            enableDisable(enable, GL_POLYGON_SMOOTH);

            ns_polygonSmooth = enable;
        }

    #else

        enable;

    #endif
    }

    //////////////////////////////////////////////

    void GlState::setLineSmooth(const bool enable)
    {
    #ifndef JOP_OPENGL_ES

        if (enable != ns_line.first)
        {
            enableDisable(enable, GL_LINE_SMOOTH);

            ns_line.first = enable;
        }

    #else

        enable;

    #endif
    }

    //////////////////////////////////////////////

    void GlState::setLineWidth(const float width)
    {
        if (width != ns_line.second)
        {
            glCheck(glLineWidth(width));

            ns_line.second = width;
        }
    }

    //////////////////////////////////////////////

    void GlState::setPolygonMode(const PolygonMode mode)
    {
    #ifndef JOP_OPENGL_ES

        if (mode != ns_polygonMode)
        {
            static const GLenum polyModes[] =
            {
                GL_FILL,
                GL_LINE,
                GL_POINT
            };

            glCheck(glPolygonMode(GL_FRONT_AND_BACK, polyModes[static_cast<int>(mode)]));

            ns_polygonMode = mode;
        }

    #else

        mode;

    #endif
    }

    //////////////////////////////////////////////

    void GlState::setVertexAttribute(const bool enable, const unsigned int index)
    {
        static int attribLocs = 0;

        if (ns_vertexAttribs.empty())
        {
            glCheck(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attribLocs));
            ns_vertexAttribs.resize(attribLocs, false);
        }

        const unsigned int clamp = std::min(attribLocs - 1u, index);
        if (ns_vertexAttribs[clamp] != enable)
        {
            if (enable)
            {
                glCheck(glEnableVertexAttribArray(clamp));
            }
            else
            {
                glCheck(glDisableVertexAttribArray(clamp));
            }

            ns_vertexAttribs[clamp] = enable;
        }
    }

    //////////////////////////////////////////////

    void GlState::setDepthWrite(const bool enable)
    {
        if (ns_depthMask != enable)
        {
            glCheck(glDepthMask(enable));

            ns_depthMask = enable;
        }
    }

    //////////////////////////////////////////////

    void GlState::setFramebufferSrgb(const bool enable)
    {
    #ifndef JOP_OPENGL_ES

        if (ns_framebufSrgb != enable)
        {
            enableDisable(enable, GL_FRAMEBUFFER_SRGB);

            ns_framebufSrgb = enable;
        }

    #else

        enable;

    #endif
    }

    //////////////////////////////////////////////

    void GlState::flush()
    {
        glCheck(glFlush());
    }
}