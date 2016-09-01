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
#include <Jopnal/Core/SettingManager.hpp>
#include <Jopnal/Graphics/RenderTexture.hpp>
#include <Jopnal/Graphics/Texture/Texture2D.hpp>
#include <Jopnal/Graphics/Mesh/RectangleMesh.hpp>
#include <array>
#include <unordered_map>

//////////////////////////////////////////////


namespace jop
{
    class ShaderProgram;
    class Renderer;
    class RenderTarget;

    class JOP_API PostProcessor final : public Subsystem
    {
    private:

        JOP_DISALLOW_COPY_MOVE(PostProcessor);

    public:

        /// Post-process functions
        ///
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

        /// \brief Constructor
        ///
        /// \param mainTarget The main render target
        ///
        PostProcessor(const RenderTarget& mainTarget);

        /// \brief Destructor
        ///
        ~PostProcessor();


        /// \brief Draw
        ///
        /// This will draw a full-screen quad using the post-process settings.
        ///
        void draw() override;

    private:

        void getPreprocessorStr(const uint32 funcs, std::string& str) const;

        void makeBloom();

        void enableBloom();


        static PostProcessor* m_instance;                                   ///< The single instance

        std::array<std::string, 2> m_shaderSources;                         ///< Shader sources
        std::unordered_map<uint32, WeakReference<ShaderProgram>> m_shaders; ///< Shader map
        WeakReference<ShaderProgram> m_blurShader;                          ///< Gaussian blur shader
        RectangleMesh m_quad;                                               ///< Full screen quad
        const RenderTarget& m_mainTarget;                                   ///< Reference to the main render target
        uint32 m_functions;                                                 ///< Enabled functions
        DynamicSetting<float> m_exposure;                                   ///< Exposure
        DynamicSetting<float> m_gamma;                                      ///< Gamma value
        DynamicSetting<float> m_bloomThreshold;                             ///< Full bloom threshold
        DynamicSetting<float> m_subBloomThresholdExp;                       ///< Sub-threshold bloom exponent
        Texture2D m_ditherMatrix;                                           ///< Dithering matrix texture
        std::array<std::array<RenderTexture, 2>, 5> m_bloomTextures;        ///< Bloom frame buffers
        WeakReference<ShaderProgram> m_brightShader;                        ///< Bright filter shader
    };
}

/// \class jop::PostProcessor
/// \ingroup graphics

#endif