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
    JOP_REGISTER_LOADABLE(jop, TextureSampler)[](const void*, const json::Value& val)
    {
        if (!val.HasMember("name") || !val["name"].IsString())
        {
            JOP_DEBUG_ERROR("Couldn't load TextureSampler, no name found");
            return false;
        }

        TextureSampler::Filter filt = val.HasMember("filter") && val["filter"].IsUint() ? static_cast<TextureSampler::Filter>(std::min(3u, val["filter"].GetUint())) : TextureSampler::Filter::None;
        TextureSampler::Repeat rep = val.HasMember("repeat") && val["repeat"].IsUint() ? static_cast<TextureSampler::Repeat>(std::min(3u, val["repeat"].GetUint())) : TextureSampler::Repeat::Basic;
        float anis = val.HasMember("anis") && val["anis"].IsDouble() ? static_cast<float>(val["anis"].GetDouble()) : 1.f;
        Color bcol(val.HasMember("bcolor") && val["bcolor"].IsUint() ? val["bcolor"].GetUint() : Color::White);

        auto& samp = ResourceManager::getNamedResource<TextureSampler>(val["name"].GetString(), filt, rep, anis);
        //samp.setPersistent(val.HasMember("persistent") && val["persistent"].IsBool() ? val["persistent"].GetBool() : false);
        samp.setBorderColor(bcol);

        return true;
    }
    JOP_END_LOADABLE_REGISTRATION(TextureSampler)

    JOP_REGISTER_SAVEABLE(jop, TextureSampler)[](const void* sampler, json::Value& val, json::Value::AllocatorType& alloc)
    {
        const TextureSampler& ref = *static_cast<const TextureSampler*>(sampler);

        val.AddMember(json::StringRef("name"), json::StringRef(ref.getName().c_str()), alloc);
        //val.AddMember(json::StringRef("persistent"), ref.isPersistent(), alloc);
        val.AddMember(json::StringRef("filter"), static_cast<unsigned int>(ref.getFilteringMode()), alloc);
        val.AddMember(json::StringRef("repeat"), static_cast<unsigned int>(ref.getRepeatMode()), alloc);
        val.AddMember(json::StringRef("anis"), ref.getAnisotropyLevel(), alloc);
        val.AddMember(json::StringRef("bcolor"), ref.getBorderColor().asInteger(), alloc);

        return true;
    }
    JOP_END_SAVEABLE_REGISTRATION(TextureSampler)
}

namespace jop
{
    TextureSampler::TextureSampler(const std::string& name)
        : Resource      (name),
          m_sampler     (0),
          m_filter      (Filter::None),
          m_repeat      (Repeat::Basic),
          m_anisotropic (1.f),
          m_borderColor ()
    {
        reset();
    }

    TextureSampler::~TextureSampler()
    {
        if (m_sampler)
            glCheck(gl::DeleteSamplers(1, &m_sampler));
    }

    //////////////////////////////////////////////

    bool TextureSampler::load(const Filter filterMode, const Repeat repeatMode, const float param)
    {
        setFilterMode(filterMode, param);
        setRepeatMode(repeatMode);

        return true;
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
            }

            m_filter = mode;
            m_anisotropic = param;
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
            }

            m_repeat = repeat;
        }
    }

    //////////////////////////////////////////////

    void TextureSampler::setBorderColor(const Color& color)
    {
        if (m_sampler)
        {
            const glm::vec4 col = color.asRGBAFloatVector();
            glCheck(gl::SamplerParameterfv(m_sampler, gl::TEXTURE_BORDER_COLOR, &col[0]));

            m_borderColor = color;
        }
    }

    //////////////////////////////////////////////

    unsigned int TextureSampler::getHandle() const
    {
        return m_sampler;
    }

    //////////////////////////////////////////////

    TextureSampler::Filter TextureSampler::getFilteringMode() const
    {
        return m_filter;
    }

    //////////////////////////////////////////////

    TextureSampler::Repeat TextureSampler::getRepeatMode() const
    {
        return m_repeat;
    }

    //////////////////////////////////////////////

    float TextureSampler::getAnisotropyLevel() const
    {
        return m_anisotropic;
    }

    //////////////////////////////////////////////

    Color TextureSampler::getBorderColor() const
    {
        return m_borderColor;
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

    TextureSampler& TextureSampler::getDefault()
    {
        static WeakReference<TextureSampler> defSampler;

        if (defSampler.expired())
        {
            defSampler = static_ref_cast<TextureSampler>(ResourceManager::getEmptyResource<TextureSampler>("jop_default_sampler").getReference());
            
            JOP_ASSERT_EVAL(defSampler->load
            (
                static_cast<Filter>(SettingManager::getUint("uDefaultTextureFilterMode", 0)),
                static_cast<Repeat>(SettingManager::getUint("uDefaultTextureRepeatMode", 0)),
                SettingManager::getFloat("fDefaultTextureAnisotropyLevel", 1.f)

            ), "Couldn't create default sampler!");

            defSampler->setPersistence(0);
        }

        return *defSampler;
    }
}