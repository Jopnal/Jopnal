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
#include <Jopnal/Graphics/RenderTexture.hpp>
#include <Jopnal/Graphics/Texture/Texture2D.hpp>
#include <array>
#include <unordered_map>

//////////////////////////////////////////////


namespace jop
{
    class ShaderProgram;
    class Renderer;
    class RectangleMesh;
    class RenderTarget;

    class JOP_API PostProcessor final : public Subsystem
    {
    private:

        JOP_DISALLOW_COPY_MOVE(PostProcessor);

    public:

        struct Function
        {
            enum : uint32
            {
                ToneMap         = 1,
                Bloom           = 1 << 1,
                Dither          = 1 << 2,
                GammaCorrection = 1 << 3
            };
        };

    public:

        PostProcessor(const RenderTarget& mainTarget);

        ~PostProcessor();


        static void enableFunctions(const uint32 funcs);

        static void disableFunctions(const uint32 funcs);

        static bool functionEnabled(const uint32 func);

        static void setExposure(const float exposure);

        static float getExposure();

        static void setGamma(const float gamma);

        static float getGamma();

        static void setBloomThreshold(const float threshold);

        static float getBloomThreshold();

        static void setBloomSubThresholdExponent(const float exponent);

        static float getBloomSubThresholdExponent();


        void draw() override;

    private:

        void getPreprocessorStr(const uint32 funcs, std::string& str) const;

        void makeBloom();

        void enableBloom();


        static PostProcessor* m_instance;

        std::array<std::string, 2> m_shaderSources;
        std::unordered_map<uint32, WeakReference<ShaderProgram>> m_shaders;
        WeakReference<ShaderProgram> m_blurShader;
        WeakReference<RectangleMesh> m_quad;
        const RenderTarget& m_mainTarget;
        uint32 m_functions;
        float m_exposure;
        float m_gamma;
        float m_bloomThreshold;
        float m_subBloomThresholdExp;
        Texture2D m_ditherMatrix;
        std::array<std::array<RenderTexture, 2>, 5> m_bloomTextures;
        WeakReference<ShaderProgram> m_brightShader;
    };
}

#endif