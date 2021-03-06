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

    #include <Jopnal/Graphics/PostProcessor.hpp>

    #include <Jopnal/Graphics/Mesh/RectangleMesh.hpp>
    #include <Jopnal/Graphics/Shader.hpp>
    #include <Jopnal/Graphics/ShaderProgram.hpp>
    #include <Jopnal/Core/ResourceManager.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>
    #include <Jopnal/STL.hpp>

#endif

#include <Jopnal/Resources/Resources.hpp>

//////////////////////////////////////////////


namespace
{
    #if !defined(JOP_OPENGL_ES) || defined(GL_ES_VERSION_3_0)
        #define JOP_ENABLE_BLOOM
    #endif

    bool isLinear()
    {
    #if defined(GL_ES_VERSION_3_0)
    
        static bool init = false;
        static bool linear = false;

        if (!init)
        {
            if (jop::gl::getVersionMajor() < 3)
                linear = true;

            else
            {
                jop::RenderTexture::unbind();

                GLint enc;

                glCheck(glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_BACK, GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING, &enc));

                linear = enc == GL_LINEAR;
            }

            init = true;
        }

        return linear;

    #elif defined(GL_ES_VERSION_2_0)

        return true;

    #else
        
        return false;

    #endif
    }

    const float ns_defBloomThreshold = 2.f - jop::gl::es;
}

namespace jop
{
    PostProcessor::PostProcessor(const RenderTarget& mainTarget)
        : Subsystem             (0),
          m_shaderSources       (),
          m_shaders             (),
          m_quad                (""),
          m_mainTarget          (mainTarget),
          m_functions           (),
          m_exposure            ("engine@Graphics|Postprocessor|Tonemapping|fExposure", 1.f),
          m_gamma               ("engine@Graphics|Postprocessor|GammaCorrection|fGamma", 2.2f),
          m_bloomThreshold      ("engine@Graphics|Postprocessor|Bloom|fThreshold", ns_defBloomThreshold),
          m_subBloomThresholdExp("engine@Graphics|Postprocessor|Bloom|fSubThresholdExponent", 4.f),
          m_ditherMatrix        (""),
          m_bloomTextures       ()
    {
        JOP_ASSERT(m_instance == nullptr, "There must only be one jop::PostProcessor instance!");
        m_instance = this;

        m_quad.load(2.f, glm::vec2(0.f, 1.f), glm::vec2(1.f, 0.f));

        static const unsigned char pattern[] = 
        {
            0,  32, 8,  40, 2,  34, 10, 42,
            48, 16, 56, 24, 50, 18, 58, 26,
            12, 44, 4,  36, 14, 46, 6,  38,
            60, 28, 52, 20, 62, 30, 54, 22,
            3,  35, 11, 43, 1,  33, 9,  41,
            51, 19, 59, 27, 49, 17, 57, 25,
            15, 47, 7,  39, 13, 45, 5,  37,
            63, 31, 55, 23, 61, 29, 53, 21
        };

        m_ditherMatrix.load(glm::uvec2(8, 8), Texture::Format::Alpha_UB_8, pattern, Texture::Flag::DisallowSRGB | Texture::Flag::DisallowMipmapGeneration);
        m_ditherMatrix.setFilterMode(TextureSampler::Filter::None).setRepeatMode(TextureSampler::Repeat::Basic);

        struct FunctionEnabler : SettingCallback<bool>
        {
            uint32 func;

            FunctionEnabler(const char* str, const uint32 function, const bool def)
                : SettingCallback   (),
                  func              (function)
            {
                valueChanged(SettingManager::get<bool>(str, def));
                SettingManager::registerCallback(str, *this);
            }

            void valueChanged(const bool& value) override
            {
                (m_instance->m_functions &= ~func) |= func * value;
            }
        };

        // Tone mapping
        {
            static const FunctionEnabler enabler("engine@Graphics|Postprocessor|Tonemapping|bEnabled", Function::ToneMap, false);
        }

        // Bloom
        {
            static const FunctionEnabler enabler("engine@Graphics|Postprocessor|Bloom|bEnabled", Function::Bloom, false);

            if (m_functions & Function::Bloom)
                enableBloom();
        }

        // Gamma correction settings
        {
            static const FunctionEnabler enabler("engine@Graphics|Postprocessor|GammaCorrection|bEnabled", Function::GammaCorrection, isLinear());
        }

        // Dithering settings
        {
            static const FunctionEnabler enabler("engine@Graphics|Postprocessor|Dithering|bEnabled", Function::Dither, false);
        }

        // Shader sources
        {
            m_shaderSources[0].assign(reinterpret_cast<const char*>(jopr::postProcessVert), sizeof(jopr::postProcessVert));
            m_shaderSources[1].assign(reinterpret_cast<const char*>(jopr::postProcessFrag), sizeof(jopr::postProcessFrag));
        }

    #ifdef JOP_ENABLE_BLOOM

        if (gl::getVersionMajor() >= 3)
        {
            auto& blurShader = ResourceManager::getNamed<ShaderProgram>
            (
                "jop_blur_shader",
                "",
                Shader::Type::Vertex, m_shaderSources[0],
                Shader::Type::Fragment, std::string(reinterpret_cast<const char*>(jopr::gaussianBlurShaderFrag), sizeof(jopr::gaussianBlurShaderFrag))
            );
            blurShader.setPersistence(0);

            JOP_ASSERT(&blurShader != &ShaderProgram::getError(), "Failed to compile gaussian blur shader!");

            auto& brightShader = ResourceManager::getNamed<ShaderProgram>
            (
                "jop_bright_filter_shader",
                "",
                Shader::Type::Vertex, m_shaderSources[0],
                Shader::Type::Fragment, std::string(reinterpret_cast<const char*>(jopr::brightFilter), sizeof(jopr::brightFilter))
            );
            brightShader.setPersistence(0);

            JOP_ASSERT(&brightShader != &ShaderProgram::getError(), "Failed to compile bright filter shader!");

            m_blurShader = static_ref_cast<ShaderProgram>(blurShader.getReference());
            m_brightShader = static_ref_cast<ShaderProgram>(brightShader.getReference());
        }

    #endif
    }

    PostProcessor::~PostProcessor()
    {}

    //////////////////////////////////////////////

    void PostProcessor::draw()
    {
        if (m_shaders.find(m_functions) == m_shaders.end())
        {
            std::string pp;
            getPreprocessorStr(m_functions, pp);

            auto& shader = ResourceManager::getNamed<ShaderProgram>("jop_pp_shader_" + std::to_string(m_functions), pp, Shader::Type::Vertex, m_shaderSources[0], Shader::Type::Fragment, m_shaderSources[1]);

            JOP_ASSERT(&shader != &ShaderProgram::getError(), "Failed to compile post process shader!");

            shader.setPersistence(1);
            m_shaders[m_functions] = static_ref_cast<ShaderProgram>(shader.getReference());
        }

        auto& shdr = *m_shaders[m_functions];

        if (m_functions > 0)
        {
            if ((m_functions & Function::ToneMap) != 0)
                shdr.setUniform("u_Exposure", m_exposure);

            if ((m_functions & Function::Bloom) != 0)
            {
                if (m_bloomTextures.empty())
                    enableBloom();

                makeBloom();

            #ifdef JOP_ENABLE_BLOOM

                for (unsigned int i = 0; i < m_bloomTextures.size(); ++i)
                    shdr.setUniform("u_Bloom[" + std::to_string(i) + "]", *m_bloomTextures[i][1].getTextureAttachment(RenderTexture::Slot::Color0), 3 + i);

            #endif
            }

            if ((m_functions & Function::Dither) != 0)
                shdr.setUniform("u_DitherMatrix", m_ditherMatrix, 2);

            if ((m_functions & Function::GammaCorrection) != 0)
                shdr.setUniform("u_Gamma", m_gamma);
        }

        RenderTexture::unbind();
        
        shdr.setUniform("u_Scene", *static_cast<const RenderTexture&>(m_mainTarget).getTextureAttachment(RenderTexture::Slot::Color0), 1);
        m_quad.draw();
    }

    //////////////////////////////////////////////

    void PostProcessor::getPreprocessorStr(const uint32 funcs, std::string& str) const
    {
        if ((funcs & Function::ToneMap) != 0)
            str += "#define JPP_TONEMAP\n";

        if ((funcs & Function::Bloom) != 0)
        {
            str += "#define JPP_BLOOM\n#define JPP_BLOOM_TEXTURES ";
            str += std::to_string(m_bloomTextures.size());
            str += "\n";
        }

        if ((funcs & Function::Dither) != 0)
            str += "#define JPP_DITHER\n";

        if ((funcs & Function::GammaCorrection) != 0)
            str += "#define JPP_GAMMACORRECTION\n";
    }

    //////////////////////////////////////////////

    void PostProcessor::makeBloom()
    {
    #ifdef JOP_ENABLE_BLOOM

    #ifdef JOP_OPENGL_ES
        if (jop::gl::getVersionMajor() < 3)
            return;
    #endif

        const auto slot = RenderTexture::Slot::Color0;

        // Brightness pass
        m_bloomTextures[0][0].bind();
        m_bloomTextures[0][0].clear(RenderTarget::ColorBit);

        m_brightShader->setUniform("u_Texture", *static_cast<const RenderTexture&>(m_mainTarget).getTextureAttachment(slot), 1);
        m_brightShader->setUniform("u_Threshold", m_bloomThreshold);
        m_brightShader->setUniform("u_SubExponent", m_subBloomThresholdExp);
        m_quad.draw();

        // Blur
        for (auto itr = m_bloomTextures.begin(); itr != m_bloomTextures.end(); ++itr)
        {
            m_blurShader->setUniform("u_Buffer", *(*itr)[0].getTextureAttachment(slot), 1);
            bool horizontal = true;

            static const DynamicSetting<unsigned int> kernelSize("engine@Graphics|Postprocessor|Bloom|uKernelSize", 5);

            for (unsigned int j = 0; j < kernelSize.value * 2; ++j)
            {
                (*itr)[horizontal].bind();
                m_blurShader->setUniform("u_Horizontal", horizontal);
                m_blurShader->setUniform("u_Buffer", *(*itr)[!horizontal].getTextureAttachment(slot), 1);

                m_quad.draw();
                horizontal = !horizontal;
            }

            if (itr != (m_bloomTextures.end() - 1))
            {
                auto& src = (*itr)[1];
                auto& dst = (*++itr)[0];

                dst.bind();
                src.bindRead();

                glCheck(glBlitFramebuffer(0, 0, src.getSize().x, src.getSize().y,
                                          0, 0, dst.getSize().x, dst.getSize().y,
                                          GL_COLOR_BUFFER_BIT, GL_LINEAR));
            }
        }

    #endif
    }

    //////////////////////////////////////////////

    void PostProcessor::enableBloom()
    {
    #ifdef JOP_ENABLE_BLOOM

    #ifdef JOP_OPENGL_ES

        if (gl::getVersionMajor() < 3)
            return;

    #endif

        using CAS = RenderTexture::Slot;
        using CA = Texture::Format;
        using TSR = TextureSampler::Repeat;
        using TSF = TextureSampler::Filter;

        for (unsigned int i = 0; i < m_bloomTextures.size(); ++i)
        {
            for (auto& j : m_bloomTextures[i])
            {
                glm::uvec2 size(glm::uvec2((16 << m_bloomTextures.size()) >> i));
                size.y = static_cast<glm::uvec2::value_type>(size.y * (m_mainTarget.getSize().y / static_cast<float>(m_mainTarget.getSize().x)));

                static const bool hdr = SettingManager::get<bool>("engine@Graphics|Postprocessor|Bloom|bHDR", !gl::es

                #ifdef JOP_OPENGL_ES                 
                    || JOP_CHECK_GL_EXTENSION(GL_EXT_color_buffer_half_float)
                #endif
                );

                j.setSize(size);
                j.addTextureAttachment(CAS::Color0, hdr ? CA::RGB_F_16 : CA::RGB_UB_8);
                j.getTextureAttachment(CAS::Color0)->setRepeatMode(TSR::ClampEdge).setFilterMode(TSF::Bilinear);
            }
        }

    #endif
    }

    //////////////////////////////////////////////

    PostProcessor* PostProcessor::m_instance = nullptr;
}