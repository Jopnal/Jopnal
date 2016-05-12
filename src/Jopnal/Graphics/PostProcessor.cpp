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
    PostProcessor::PostProcessor(const RenderTexture& mainTarget)
        : Subsystem             ("postprocessor"),
          m_shaderSources       (),
          m_shaders             (),
          m_quad                (""),
          m_mainTarget          (mainTarget),
          m_functions           (Function::Default),
          m_exposure            (1.f)
    {
        JOP_ASSERT(m_instance == nullptr, "There must only be one jop::PostProcessor instance!");
        m_instance = this;

        m_quad.load(2.f);

        // Shader sources
        {
            std::vector<uint8> vertBuf, fragBuf;
            JOP_ASSERT_EVAL(FileLoader::readResource(JOP_RES_POSTPROCESS_SHADER_VERT, vertBuf), "Failed to read post process vertex shader source!");
            JOP_ASSERT_EVAL(FileLoader::readResource(JOP_RES_POSTPROCESS_SHADER_FRAG, fragBuf), "Failed to read post process fragment shader source!");

            m_shaderSources[0].assign(reinterpret_cast<const char*>(vertBuf.data()), vertBuf.size());
            m_shaderSources[1].assign(reinterpret_cast<const char*>(fragBuf.data()), fragBuf.size());
        }

        // Gaussian blur buffers
        for (auto& i : m_pingPong)
        {
            i.addColorAttachment(RenderTexture::ColorAttachmentSlot::_1, RenderTexture::ColorAttachment::RGB2DFloat16, mainTarget.getSize());
            i.getColorTexture(RenderTexture::ColorAttachmentSlot::_1)->getSampler().setFilterMode(TextureSampler::Filter::Bilinear);
        }

        std::vector<uint8> blurFragBuf;
        JOP_ASSERT_EVAL(FileLoader::readResource(JOP_RES_GAUSSIANBLUR_SHADER_FRAG, blurFragBuf), "Failed to read gaussian blur fragment shader source!");

        auto& blurShader = ResourceManager::getNamedResource<Shader>("jop_blur_shader", m_shaderSources[0], "", std::string(reinterpret_cast<const char*>(blurFragBuf.data()), blurFragBuf.size()));
        JOP_ASSERT(&blurShader != &Shader::getError(), "Failed to compile gaussian blur shader!");

        m_blurShader = static_ref_cast<Shader>(blurShader.getReference());
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

            auto& shader = ResourceManager::getNamedResource<Shader>("jop_pp_shader_" + std::to_string(m_functions), m_shaderSources[0], "", m_shaderSources[1], pp);

            JOP_ASSERT(&shader != &Shader::getError(), "Failed to compile post process shader!");

            shader.setPersistence(1);
            m_shaders[m_functions] = static_ref_cast<Shader>(shader.getReference());
        }

        auto& shdr = *m_shaders[m_functions];

        if (m_functions > 0)
        {
            if ((m_functions & Function::ToneMap) != 0)
                shdr.setUniform("u_Exposure", m_exposure);

            if ((m_functions & Function::Bloom) != 0 && m_mainTarget.getColorTexture(RenderTexture::ColorAttachmentSlot::_2))
            {
                applyBlur(*m_mainTarget.getColorTexture(RenderTexture::ColorAttachmentSlot::_2));
                shdr.setUniform("u_Bloom", *m_pingPong[1].getColorTexture(RenderTexture::ColorAttachmentSlot::_1), 2);
            }
        }

        RenderTexture::unbind();
        
        m_quad.getVertexBuffer().bind();
        shdr.setAttribute(0, gl::FLOAT, 3, m_quad.getVertexSize(), false, m_quad.getVertexOffset(Mesh::Position));
        shdr.setAttribute(1, gl::FLOAT, 2, m_quad.getVertexSize(), false, m_quad.getVertexOffset(Mesh::TexCoords));

        shdr.setUniform("u_Scene", *m_mainTarget.getColorTexture(RenderTexture::ColorAttachmentSlot::_1), 1);

        m_quad.getIndexBuffer().bind();
        glCheck(gl::DrawElements(gl::TRIANGLES, m_quad.getElementAmount(), m_quad.getElementEnum(), 0));
    }

    //////////////////////////////////////////////

    bool PostProcessor::functionEnabled(const uint32 func)
    {
        return m_instance != nullptr && (m_instance->m_functions & func) != 0;
    }

    //////////////////////////////////////////////

    void PostProcessor::getPreprocessorStr(const uint32 funcs, std::string& str) const
    {
        str += "#version 330 core\n";

        if ((funcs & Function::ToneMap) != 0)
            str += "#define JPP_TONEMAP\n";

        if ((funcs & Function::Bloom) != 0)
            str += "#define JPP_BLOOM\n";
    }

    //////////////////////////////////////////////

    void PostProcessor::applyBlur(const Texture& texture)
    {
        /*GLboolean horizontal = true, first_iteration = true;
        GLuint amount = 10;
        shaderBlur.Use();
        for (GLuint i = 0; i < amount; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
            glUniform1i(glGetUniformLocation(shaderBlur.Program, "horizontal"), horizontal);
            glBindTexture(
                GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongBuffers[!horizontal]
                );
            RenderQuad();
            horizontal = !horizontal;
            if (first_iteration)
                first_iteration = false;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

        bool horizontal = true;

        m_blurShader->bind();
        m_blurShader->setUniform("u_Buffer", texture, 1);

        m_quad.getVertexBuffer().bind();
        m_blurShader->setAttribute(0, gl::FLOAT, 3, m_quad.getVertexSize(), false, m_quad.getVertexOffset(Mesh::Position));
        m_blurShader->setAttribute(1, gl::FLOAT, 2, m_quad.getVertexSize(), false, m_quad.getVertexOffset(Mesh::TexCoords));

        m_quad.getIndexBuffer().bind();

        for (uint32 i = 0; i < 15; ++i)
        {
            m_pingPong[horizontal].bind();

            m_blurShader->setUniform("u_Horizontal", horizontal);

            glCheck(gl::DrawElements(gl::TRIANGLES, m_quad.getElementAmount(), m_quad.getElementEnum(), 0));


            horizontal = !horizontal;
            m_blurShader->setUniform("u_Buffer", *m_pingPong[!horizontal].getColorTexture(RenderTexture::ColorAttachmentSlot::_1), 1);
        }
    }

    //////////////////////////////////////////////

    PostProcessor* PostProcessor::m_instance = nullptr;
}