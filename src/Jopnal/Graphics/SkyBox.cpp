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
//    in a product, an acknowledgment in the product documentation would be
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
    JOP_REGISTER_LOADABLE(jop, SkyBox)[](Object& obj, const Scene& scene, const json::Value& val)
    {
        const float size = val.HasMember("size") && val["size"].IsDouble() ? static_cast<float>(val["size"].GetDouble()) : 2.f;

        auto& box = obj.createComponent<SkyBox>(scene.getRenderer(), size);

        

        return Drawable::loadStateBase(box, scene, val);
    }
    JOP_END_LOADABLE_REGISTRATION(SkyBox)

    JOP_REGISTER_SAVEABLE(jop, SkyBox)[](const Component& comp, json::Value& val, json::Value::AllocatorType& alloc)
    {
        auto& box = static_cast<const SkyBox&>(comp);

        return Drawable::saveStateBase(box, val , alloc);
    }
    JOP_END_SAVEABLE_REGISTRATION(SkyBox)
}

namespace jop
{
    SkyBox::SkyBox(Object& obj, Renderer& renderer, const float size)
        : Drawable      (obj, renderer, "skybox"),
          m_mesh        (""),
          m_material    ("", Material::Attribute::__SkyBox | Material::Attribute::EnvironmentMap, false)
    {
        m_mesh.load(size, true);

        setModel(Model(m_mesh, m_material));

        setCastShadows(false);
        setReceiveLights(false);
        setReceiveShadows(false);
    }

    SkyBox::SkyBox(const SkyBox& other, Object& newObj)
        : Drawable      (other, newObj),
          m_mesh        (other.m_mesh, ""),
          m_material    (other.m_material, "")
    {}

    //////////////////////////////////////////////

    void SkyBox::draw(const Camera* camera, const LightContainer&, Shader& shader) const
    {
        if (!getModel().isValid())
            return;

        auto& s = shader;
        auto& mat = *getModel().getMaterial();
        auto& msh = *getModel().getMesh();

        if (camera)
        {
            s.setUniform("u_PMatrix", camera->getProjectionMatrix());
            s.setUniform("u_VMatrix", camera->getViewMatrix());
        }

        msh.getVertexBuffer().bind();
        const auto stride = msh.getVertexSize();
        s.setAttribute(0, gl::FLOAT, 3, stride, false, msh.getVertexOffset(Mesh::Position));

        mat.sendToShader(s, camera);

        GlState::setDepthTest(true, GlState::DepthFunc::LessEqual);

        if (msh.getElementAmount())
        {
            msh.getIndexBuffer().bind();
            glCheck(gl::DrawElements(gl::TRIANGLES, msh.getElementAmount(), msh.getElementEnum(), (void*)0));
        }
        else
        {
            glCheck(gl::DrawArrays(gl::TRIANGLES, 0, msh.getVertexAmount()));
        }

        GlState::setDepthTest(true);
    }

    //////////////////////////////////////////////

    void SkyBox::setMap(const Cubemap& map)
    {
        m_material.setMap(Material::Map::Environment, map);
    }

    //////////////////////////////////////////////

    const Texture* SkyBox::getMap() const
    {
        return m_material.getMap(Material::Map::Environment);
    }
}