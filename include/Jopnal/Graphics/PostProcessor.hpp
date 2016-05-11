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

#ifndef JOP_POSTPROCESSOR_HPP
#define JOP_POSTPROCESSOR_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Core/SubSystem.hpp>
#include <Jopnal/Graphics/RectangleMesh.hpp>
#include <array>

//////////////////////////////////////////////


namespace jop
{
    class Shader;
    class Renderer;
    class RenderTexture;

    class JOP_API PostProcessor final : public Subsystem
    {
    private:

        JOP_DISALLOW_COPY_MOVE(PostProcessor);

    public:

        struct Function
        {
            enum : uint32
            {
                ToneMap = 1,
                Bloom   = 1 << 1
            };
        };

    public:

        PostProcessor(const RenderTexture& mainTarget);


        PostProcessor& enableFunctions(const uint32 funcs);

        PostProcessor& disableFunctions(const uint32 funcs);

        bool functionEnabled(const uint32 func) const;


        void draw();

    private:

        std::array<std::string, 4> m_shaderSources;
        jop::WeakReference<Shader> m_shader;
        RectangleMesh m_quad;
        const RenderTexture& m_mainTarget;
        uint32 m_functions;
        bool m_functionsChanged;
    };
}

#endif