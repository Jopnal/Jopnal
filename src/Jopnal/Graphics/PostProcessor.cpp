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
    PostProcessor::PostProcessor(Renderer& renderer, const RenderTexture& mainTarget)
        : m_shaderSources       (),
          m_shader              (),
          m_quad                (""),
          m_rendererRef         (renderer),
          m_mainTarget          (mainTarget),
          m_functions           (0),
          m_functionsChanged    (false)
    {
        m_quad.load(2.f);
    }

    //////////////////////////////////////////////

    PostProcessor& PostProcessor::enableFunctions(const uint32 funcs)
    {
        m_functionsChanged = (m_functions & funcs) != funcs;
        m_functions |= funcs;

        return *this;
    }

    //////////////////////////////////////////////

    PostProcessor& PostProcessor::disableFunctions(const uint32 funcs)
    {
        m_functionsChanged = (m_functions & funcs) > 0;
        m_functions &= ~funcs;

        return *this;
    }

    //////////////////////////////////////////////

    void PostProcessor::draw()
    {
        if (m_functionsChanged)
        {

            m_functionsChanged = false;
        }

        RenderTexture::unbind();
        
        // There should always be a valid shader
        m_shader->bind();

        
    }
}