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

#endif

#include <Jopnal/Resources/Resources.hpp>

//////////////////////////////////////////////


namespace jop
{
    PostProcessor::PostProcessor(const RenderTexture& mainTarget)
        : Subsystem             (0),
          m_shaderSources       (),
          m_shaders             (),
          m_quad                (static_ref_cast<RectangleMesh>(ResourceManager::getNamedResource<RectangleMesh>("__jop_fs_quad", 2.f).getReference())),
          m_mainTarget          (mainTarget),
          m_functions           (Function::GammaCorrection),
          m_exposure            (1.f),
          m_gamma               (2.2f),
          m_ditherMatrix        ("")
    {
        JOP_ASSERT(m_instance == nullptr, "There must only be one jop::PostProcessor instance!");
        m_instance = this;

        m_quad->load(2.f);

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

        // Tone mapping settings
        {
            static const struct EnabledCallback : SettingCallback<bool>
            {
                const char* str;

                EnabledCallback()
                    : str("engine@Graphics|Postprocessor|Tonemapping|bEnabled")
                {
                    enableFunctions(SettingManager::get<bool>(str, true) * Function::ToneMap);
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
                    setExposure(SettingManager::get<float>(str, 1.f));
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
                    enableFunctions(SettingManager::get<bool>(str, true) * Function::Bloom);
                    SettingManager::registerCallback(str, *this);
                }
                void valueChanged(const bool& value) override {value ? enableFunctions(Function::Bloom) : disableFunctions(Function::Bloom);}
            } enabled;
        }

        // Gamma correction settings
        {

        }

        // Shader sources
        {
            m_shaderSources[0].assign(reinterpret_cast<const char*>(jopr::postProcessVert), sizeof(jopr::postProcessVert));
            m_shaderSources[1].assign(reinterpret_cast<const char*>(jopr::postProcessFrag), sizeof(jopr::postProcessFrag));
        }

        auto& blurShader = ResourceManager::getNamedResource<ShaderProgram>
        (
            "jop_blur_shader",
            "", 
            Shader::Type::Vertex, m_shaderSources[0],
            Shader::Type::Fragment, std::string(reinterpret_cast<const char*>(jopr::gaussianBlurShaderFrag), sizeof(jopr::gaussianBlurShaderFrag))
        );

        JOP_ASSERT(&blurShader != &ShaderProgram::getError(), "Failed to compile gaussian blur shader!");

        m_blurShader = static_ref_cast<ShaderProgram>(blurShader.getReference());
        
    }

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

    void PostProcessor::draw()
    {
        if (m_shaders.find(m_functions) == m_shaders.end())
        {
            std::string pp;
            getPreprocessorStr(m_functions, pp);

            auto& shader = ResourceManager::getNamedResource<ShaderProgram>("jop_pp_shader_" + std::to_string(m_functions), pp, Shader::Type::Vertex, m_shaderSources[0], Shader::Type::Fragment, m_shaderSources[1]);

            JOP_ASSERT(&shader != &ShaderProgram::getError(), "Failed to compile post process shader!");

            shader.setPersistence(1);
            m_shaders[m_functions] = static_ref_cast<ShaderProgram>(shader.getReference());
        }

        auto& shdr = *m_shaders[m_functions];

        if (m_functions > 0)
        {
            if ((m_functions & Function::ToneMap) != 0)
                shdr.setUniform("u_Exposure", m_exposure);

            if ((m_functions & Function::Dither) != 0)
                shdr.setUniform("u_DitherMatrix", m_ditherMatrix, 3);

            if ((m_functions & Function::GammaCorrection) != 0)
                shdr.setUniform("u_Gamma", m_gamma);
        }

        RenderTexture::unbind();
        
        m_quad->getVertexBuffer().bind();
        shdr.setAttribute(0, GL_FLOAT, 3, m_quad->getVertexSize(), m_quad->getVertexOffset(Mesh::Position));
        shdr.setAttribute(1, GL_FLOAT, 2, m_quad->getVertexSize(), m_quad->getVertexOffset(Mesh::TexCoords));

        shdr.setUniform("u_Scene", *m_mainTarget.getColorTexture(RenderTexture::ColorAttachmentSlot::_1), 1);

        m_quad->getIndexBuffer().bind();
        glCheck(glDrawElements(GL_TRIANGLES, m_quad->getElementAmount(), m_quad->getElementEnum(), 0));
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

        //if ((funcs & Function::Bloom) != 0)
        //    str += "#define JPP_BLOOM\n";

        if ((funcs & Function::Dither) != 0)
            str += "#define JPP_DITHER\n";

        if ((funcs & Function::GammaCorrection) != 0)
            str += "#define JPP_GAMMACORRECTION\n";
    }

    //////////////////////////////////////////////

    PostProcessor* PostProcessor::m_instance = nullptr;
}