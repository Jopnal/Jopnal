// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//////////////////////////////////////////////

// Headers
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace jop
{
    TextureSampler::TextureSampler()
        : m_sampler(0)
    {
        reset();
    }

    TextureSampler::~TextureSampler()
    {
        if (m_sampler)
            glCheck(gl::DeleteSamplers(1, &m_sampler));
    }

    //////////////////////////////////////////////

    void TextureSampler::bind(const unsigned int textureUnit) const
    {
        if (m_sampler)
            glCheck(gl::BindSampler(textureUnit, m_sampler));
    }

    //////////////////////////////////////////////

    void TextureSampler::reset()
    {
        if (m_sampler)
            glCheck(gl::DeleteSamplers(1, &m_sampler));

        glCheck(gl::GenSamplers(1, &m_sampler));
    }

    //////////////////////////////////////////////

    void TextureSampler::setFilterMode(const Filter mode, const float param)
    {
        if (m_sampler)
        {
            switch (mode)
            {
                case Filter::None:
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_MIN_FILTER, gl::NEAREST));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_MAG_FILTER, gl::NEAREST));
                    break;
                case Filter::Bilinear:
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_MIN_FILTER, gl::LINEAR));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_MAG_FILTER, gl::LINEAR));
                    break;
                case Filter::Trilinear:
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_MIN_FILTER, gl::LINEAR_MIPMAP_LINEAR));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_MAG_FILTER, gl::LINEAR));
                    break;
                case Filter::Anisotropic:
                    if (getMaxAnisotropy() > 0.f)
                    {
                        glCheck(gl::SamplerParameterf(m_sampler, gl::TEXTURE_MAX_ANISOTROPY_EXT, std::min(param, getMaxAnisotropy())));
                    }
                    else
                        // Should never happen but just to be sure.
                        JOP_DEBUG_WARNING("Anisotropic filtering is not supported on this system");
                    break;
                default:
                    JOP_DEBUG_WARNING("Incorrect sampler filtering mode specified");
                    break;
            }
        }
    }

    //////////////////////////////////////////////

    void TextureSampler::setRepeatMode(const Repeat repeat)
    {
        if (m_sampler)
        {
            switch (repeat)
            {
                case Repeat::Basic:
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_S, gl::REPEAT));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_T, gl::REPEAT));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_R, gl::REPEAT));
                    break;
                case Repeat::Mirrored:
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_S, gl::MIRRORED_REPEAT));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_T, gl::MIRRORED_REPEAT));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_R, gl::MIRRORED_REPEAT));
                    break;
                case Repeat::ClampEdge:
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_EDGE));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_EDGE));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_R, gl::CLAMP_TO_EDGE));
                    break;
                case Repeat::ClampBorder:
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_S, gl::CLAMP_TO_BORDER));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_T, gl::CLAMP_TO_BORDER));
                    glCheck(gl::SamplerParameteri(m_sampler, gl::TEXTURE_WRAP_R, gl::CLAMP_TO_BORDER));
                    break;
                default:
                    JOP_DEBUG_WARNING("Invalid sampler repeat mode specified");
                    break;
            }
        }
    }

    //////////////////////////////////////////////

    void TextureSampler::setBorderColor(const Color& color)
    {
        if (m_sampler)
        {
            const glm::vec4 col = color.asFloatVector();
            glCheck(gl::SamplerParameterfv(m_sampler, gl::TEXTURE_BORDER_COLOR, &col[0]));
        }
    }

    //////////////////////////////////////////////

    unsigned int TextureSampler::getHandle() const
    {
        return m_sampler;
    }

    //////////////////////////////////////////////

    float TextureSampler::getMaxAnisotropy()
    {
        float level = 0.f;

        if (gl::exts::var_EXT_texture_filter_anisotropic)
            glCheck(gl::GetFloatv(gl::MAX_TEXTURE_MAX_ANISOTROPY_EXT, &level));

        return level;
    }

    //////////////////////////////////////////////

    std::shared_ptr<const TextureSampler> TextureSampler::getDefaultSampler()
    {
        static std::weak_ptr<const TextureSampler> sampler;

        if (sampler.expired())
        {
            auto newSampler = std::make_shared<TextureSampler>();

            newSampler->setFilterMode(static_cast<Filter>(SettingManager::getUint("uDefaultTextureFilterMode", 1)),
                                                          SettingManager::getFloat("fDefaultTextureAnisotropyLevel", 1.f));

            newSampler->setRepeatMode(static_cast<Repeat>(SettingManager::getUint("uDefaultTextureRepeatMode", 2)));

            newSampler->setBorderColor(Color(SettingManager::getString("sDefaultTextureBorderColor", "FFFFFFFF")));

            return (sampler = newSampler).lock();
        }

        return sampler.lock();
    }
}