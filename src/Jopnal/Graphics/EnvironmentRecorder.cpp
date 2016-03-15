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
    EnvironmentRecorder::EnvironmentRecorder(Object& obj, Renderer& renderer)
        : Component (obj, "environmentrecorder"),
          m_fbo     (),
          m_rendererRef(renderer)
    {}

    EnvironmentRecorder::EnvironmentRecorder(const EnvironmentRecorder& other, Object& newObj)
        : Component (other, newObj),
          m_fbo     (),
          m_rendererRef(other.m_rendererRef)
    {
        m_rendererRef.bind(*this);
    }

    EnvironmentRecorder::~EnvironmentRecorder()
    {
        m_rendererRef.unbind(*this);
    }

    //////////////////////////////////////////////

    void EnvironmentRecorder::record(const std::set<const Drawable*>& drawables, const std::set<const LightSource*>& /*lights*/, const uint32 /*mask*/)
    {
        static const unsigned int mapResolution = SettingManager::getUint("uEnvironmentMapSize", 256);


        for (auto& i : drawables)
        {
            auto shdr = &ShaderManager::getShader(i->getModel().getMaterial()->getAttributeField() | Material::Attribute::RecordEnv);

            if (shdr == &Shader::getDefault())
            {
                JOP_DEBUG_ERROR("Couldn't compile environment record shader. Trying to draw the rest...");
                continue;
            }


        }
    }

    //////////////////////////////////////////////

    void EnvironmentRecorder::setRenderMask(const uint32 mask)
    {
        m_mask = mask;
    }

    //////////////////////////////////////////////

    uint32 EnvironmentRecorder::getRenderMask() const
    {
        return m_mask;
    }
}