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
    #include <Jopnal/Core/ResourceManager.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>
    #include <Jopnal/STL.hpp>

#endif

#include <Jopnal/Resources/Resources.hpp>

//////////////////////////////////////////////


namespace
{
    bool isLinear()
    {
    #ifdef JOP_OPENGL_ES

        static bool init = false;
        static bool linear = false;

        if (!init)
        {
            jop::RenderTexture::unbind();

            GLint enc;

            glCheck(glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_BACK, GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING, &enc));

            linear = enc == GL_LINEAR;
            init = true;
        }

        return linear;

    #else

        return true;

    #endif
    }

    void drawQuad(const jop::RectangleMesh& mesh, jop::ShaderProgram& shdr)
    {
        mesh.getVertexBuffer().bind();
        shdr.setAttribute(0, GL_FLOAT, 3, mesh.getVertexSize(), mesh.getVertexOffset(jop::Mesh::Position));
        shdr.setAttribute(1, GL_FLOAT, 2, mesh.getVertexSize(), mesh.getVertexOffset(jop::Mesh::TexCoords));

        mesh.getIndexBuffer().bind();
        glCheck(glDrawElements(GL_TRIANGLES, mesh.getElementAmount(), mesh.getElementEnum(), 0));
    }

    const float ns_defBloomThreshold = 1.f - 0.1f * jop::gl::isGLES();
}

namespace jop
{
    PostProcessor::PostProcessor(const MainRenderTarget& mainTarget)
        : Subsystem             (0),
          m_shaderSources       (),
          m_shaders             (),
          m_quad                (static_ref_cast<RectangleMesh>(ResourceManager::getNamed<RectangleMesh>("__jop_fs_quad", 2.f).getReference())),
          m_mainTarget          (mainTarget),
          m_functions           (),
          m_exposure            (1.f),
          m_gamma               (2.2f),
          m_bloomThreshold      (ns_defBloomThreshold),
          m_subBloomThresholdExp(4.f),
          m_ditherMatrix        (""),
          m_bloomTextures       ()
    {
        JOP_ASSERT(m_instance == nullptr, "There must only be one jop::PostProcessor instance!");
        m_instance = this;

        m_quad->load(2.f, glm::vec2(0.f, 1.f), glm::vec2(1.f, 0.f));

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

        m_ditherMatrix.load(glm::uvec2(8, 8), 1, pattern, false, false);
        m_ditherMatrix.getSampler().setFilterMode(TextureSampler::Filter::None).setRepeatMode(TextureSampler::Repeat::Basic);

        // Bloom
        enableBloom();

        // Tone mapping settings
        {
            static const struct EnabledCallback : SettingCallback<bool>
            {
                const char* str;

                EnabledCallback()
                    : str("engine@Graphics|Postprocessor|Tonemapping|bEnabled")
                {
                    valueChanged(SettingManager::get<bool>(str, !gl::isGLES()));
                    SettingManager::registerCallback(str, *this);
                }
                void valueChanged(const bool& value) override {value ? enableFunctions(Function::ToneMap) : disableFunctions(Function::ToneMap);}
            } enabled;

            static const struct ExposureCallback : SettingCallback<float>
            {
                const char* str;

                ExposureCallback()
                    : str("engine@Graphics|Postprocessor|Tonemapping|fExposure")
                {
                    valueChanged(SettingManager::get<float>(str, 1.f));
                    SettingManager::registerCallback(str, *this);
                }
                void valueChanged(const float& value) override {setExposure(value);}
            } exposure;
        }

        // Bloom settings
        {
            static const struct EnabledCallback : SettingCallback<bool>
            {
                const char* str;

                EnabledCallback()
                    : str("engine@Graphics|Postprocessor|Bloom|bEnabled")
                {
                    valueChanged(SettingManager::get<bool>(str, !gl::isGLES()));
                    SettingManager::registerCallback(str, *this);
                }
                void valueChanged(const bool& value) override {value ? enableFunctions(Function::Bloom) : disableFunctions(Function::Bloom);}
            } enabled;

            static const struct ThresholdCallback : SettingCallback<float>
            {
                const char* str;

                ThresholdCallback()
                    : str("engine@Graphics|Postprocessor|Bloom|fThreshold")
                {
                    valueChanged(SettingManager::get<float>(str, ns_defBloomThreshold));
                    SettingManager::registerCallback(str, *this);
                }
                void valueChanged(const float& value) override { setBloomThreshold(value); }
            } threshold;

            static const struct ExponentCallback : SettingCallback<float>
            {
                const char* str;

                ExponentCallback()
                    : str("engine@Graphics|Postprocessor|Bloom|fSubThresholdExponent")
                {
                    valueChanged(SettingManager::get<float>(str, 4.f));
                    SettingManager::registerCallback(str, *this);
                }
                void valueChanged(const float& value) override { setBloomSubThresholdExponent(value); }
            } exponent;

            if (functionEnabled(Function::Bloom))
                enableBloom();
        }

        // Gamma correction settings
        {
            static const struct EnabledCallback : SettingCallback<bool>
            {
                const char* str;

                EnabledCallback()
                    : str("engine@Graphics|Postprocessor|GammaCorrection|bEnabled")
                {
                    valueChanged(SettingManager::get<bool>(str, isLinear()));
                    SettingManager::registerCallback(str, *this);
                }
                void valueChanged(const bool& value) override { value ? enableFunctions(Function::GammaCorrection) : disableFunctions(Function::GammaCorrection); }
            } enabled;

            static const struct GammaCallback : SettingCallback<float>
            {
                const char* str;

                GammaCallback()
                    : str("engine@Graphics|Postprocessor|GammaCorrection|fGamma")
                {
                    valueChanged(SettingManager::get<float>(str, 2.2f));
                    SettingManager::registerCallback(str, *this);
                }
                void valueChanged(const float& value) override { setGamma(value); }
            } exposure;
        }

        // Dithering settings
        {
            static const struct EnabledCallback : SettingCallback<bool>
            {
                const char* str;

                EnabledCallback()
                    : str("engine@Graphics|Postprocessor|Dithering|bEnabled")
                {
                    valueChanged(SettingManager::get<bool>(str, !gl::isGLES()));
                    SettingManager::registerCallback(str, *this);
                }
                void valueChanged(const bool& value) override { value ? enableFunctions(Function::Dither) : disableFunctions(Function::Dither); }
            } enabled;
        }

        // Shader sources
        {
            m_shaderSources[0].assign(reinterpret_cast<const char*>(jopr::postProcessVert), sizeof(jopr::postProcessVert));
            m_shaderSources[1].assign(reinterpret_cast<const char*>(jopr::postProcessFrag), sizeof(jopr::postProcessFrag));
        }

        auto& blurShader = ResourceManager::getNamed<ShaderProgram>
        (
            "jop_blur_shader",
            "", 
            Shader::Type::Vertex, m_shaderSources[0],
            Shader::Type::Fragment, std::string(reinterpret_cast<const char*>(jopr::gaussianBlurShaderFrag), sizeof(jopr::gaussianBlurShaderFrag))
        );

        JOP_ASSERT(&blurShader != &ShaderProgram::getError(), "Failed to compile gaussian blur shader!");

        auto& brightShader = ResourceManager::getNamed<ShaderProgram>
        (
            "jop_bright_filter_shader",
            "",
            Shader::Type::Vertex, m_shaderSources[0],
            Shader::Type::Fragment, std::string(reinterpret_cast<const char*>(jopr::brightFilter), sizeof(jopr::brightFilter))
        );

        JOP_ASSERT(&brightShader != &ShaderProgram::getError(), "Failed to compile bright filter shader!");

        m_blurShader = static_ref_cast<ShaderProgram>(blurShader.getReference());
        m_brightShader = static_ref_cast<ShaderProgram>(brightShader.getReference());
    }

    PostProcessor::~PostProcessor()
    {}

    //////////////////////////////////////////////

    void PostProcessor::enableFunctions(const uint32 funcs)
    {
        if (m_instance)
            m_instance->m_functions |= funcs;
    }

    //////////////////////////////////////////////

    void PostProcessor::disableFunctions(const uint32 funcs)
    {
        if (m_instance)
            m_instance->m_functions &= ~funcs;
    }

    //////////////////////////////////////////////

    void PostProcessor::setExposure(const float exposure)
    {
        if (m_instance)
            m_instance->m_exposure = exposure;
    }

    //////////////////////////////////////////////

    float PostProcessor::getExposure()
    {
        if (m_instance)
            return m_instance->m_exposure;

        return 1.f;
    }

    //////////////////////////////////////////////

    void PostProcessor::setGamma(const float gamma)
    {
        if (m_instance)
            m_instance->m_gamma = gamma;
    }

    //////////////////////////////////////////////

    float PostProcessor::getGamma()
    {
        if (m_instance)
            return m_instance->m_gamma;

        return 0.f;
    }

    //////////////////////////////////////////////

    void PostProcessor::setBloomThreshold(const float threshold)
    {
        if (m_instance)
            m_instance->m_bloomThreshold = threshold;
    }

    //////////////////////////////////////////////

    float PostProcessor::getBloomThreshold()
    {
        if (m_instance)
            return m_instance->m_bloomThreshold;

        return 0.f;
    }

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

                for (unsigned int i = 0; i < m_bloomTextures.size(); ++i)
                    shdr.setUniform("u_Bloom[" + std::to_string(i) + "]", *m_bloomTextures[i][1].getColorTexture(RenderTexture::ColorAttachmentSlot::_1), 3 + i);
            }

            if ((m_functions & Function::Dither) != 0)
                shdr.setUniform("u_DitherMatrix", m_ditherMatrix, 2);

            if ((m_functions & Function::GammaCorrection) != 0)
                shdr.setUniform("u_Gamma", m_gamma);
        }

        RenderTexture::unbind();
        
        shdr.setUniform("u_Scene", *m_mainTarget.getColorTexture(RenderTexture::ColorAttachmentSlot::_1), 1);
        drawQuad(m_quad, shdr);
    }

    //////////////////////////////////////////////

    bool PostProcessor::functionEnabled(const uint32 func)
    {
        return m_instance != nullptr && (m_instance->m_functions & func) != 0;
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
        const auto slot = RenderTexture::ColorAttachmentSlot::_1;

        // Brightness pass
        m_bloomTextures[0][0].bind();
        m_bloomTextures[0][0].clear(RenderTarget::ColorBit);

        m_brightShader->setUniform("u_Texture", *m_mainTarget.getColorTexture(slot), 1);
        m_brightShader->setUniform("u_Threshold", m_bloomThreshold);
        m_brightShader->setUniform("u_SubExponent", m_subBloomThresholdExp);
        drawQuad(m_quad, m_brightShader);

        // Blur
        for (auto itr = m_bloomTextures.begin(); itr != m_bloomTextures.end(); ++itr)
        {
            m_blurShader->setUniform("u_Buffer", *(*itr)[0].getColorTexture(slot), 1);
            bool horizontal = true;

            static DynamicSetting<unsigned int> kernelSize("engine@Graphics|Postprocessor|Bloom|uKernelSize", 5);

            for (unsigned int j = 0; j < kernelSize.value * 2; ++j)
            {
                (*itr)[horizontal].bind();
                m_blurShader->setUniform("u_Horizontal", horizontal);
                m_blurShader->setUniform("u_Buffer", *(*itr)[!horizontal].getColorTexture(slot), 1);

                drawQuad(m_quad, m_blurShader);
                horizontal = !horizontal;
            }

            if (itr != (m_bloomTextures.end() - 1))
            {
                auto& src = (*itr)[1];
                auto& dst = (*++itr)[0];

                dst.bind();
                src.bind(true);

                glCheck(glBlitFramebuffer(0, 0, src.getSize().x, src.getSize().y,
                                          0, 0, dst.getSize().x, dst.getSize().y,
                                          GL_COLOR_BUFFER_BIT, GL_LINEAR));
            }
        }
    }

    //////////////////////////////////////////////

    void PostProcessor::enableBloom()
    {
        using CAS = RenderTexture::ColorAttachmentSlot;
        using CA = RenderTexture::ColorAttachment;
        using TSR = TextureSampler::Repeat;
        using TSF = TextureSampler::Filter;

        for (unsigned int i = 0; i < m_bloomTextures.size(); ++i)
        {
            for (auto& j : m_bloomTextures[i])
            {
                glm::uvec2 size(glm::uvec2((16 << m_bloomTextures.size()) >> i));
                size.y = static_cast<glm::uvec2::value_type>(size.y * (m_mainTarget.getSize().y / static_cast<float>(m_mainTarget.getSize().x)));

                j.addColorAttachment(CAS::_1, CA::RGB2D, size);
                j.getColorTexture(CAS::_1)->getSampler().setRepeatMode(TSR::ClampEdge).setFilterMode(TSF::Bilinear);
            }
        }
    }

    //////////////////////////////////////////////

    void PostProcessor::setBloomSubThresholdExponent(const float exponent)
    {
        if (m_instance)
            m_instance->m_subBloomThresholdExp = exponent;
    }

    //////////////////////////////////////////////

    float PostProcessor::getBloomSubThresholdExponent()
    {
        if (m_instance)
            return m_instance->m_subBloomThresholdExp;

        return 0.f;
    }

    //////////////////////////////////////////////

    PostProcessor* PostProcessor::m_instance = nullptr;
}