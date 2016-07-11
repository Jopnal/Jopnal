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
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Graphics/SkyBox.hpp>

    #include <Jopnal/Core/Serializer.hpp>
    #include <Jopnal/Graphics/Camera.hpp>
    #include <Jopnal/Graphics/OpenGL/GlState.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/ShaderProgram.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>
    #include <Jopnal/Graphics/Texture/Cubemap.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    SkyBox& jop__skyBoxFactoryFunc(JOP_COMPONENT_FACTORY_ARGS)
    {
        float size = 2.f;
        
        if (val.HasMember("size") && val["size"].IsDouble())
            size = static_cast<float>(val["size"].GetDouble());

        return obj.createComponent<SkyBox>(scene.getRenderer(), size);
    }
    bool jop__skyBoxLoadFunc(JOP_COMPONENT_LOAD_ARGS)
    {
        // map

        return Serializer::callSingleFunc<Drawable, Serializer::FunctionID::Load>(Serializer::getSerializeID<Drawable>(), comp, val);
    }
    bool jop__skyBoxSaveFunc(JOP_COMPONENT_SAVE_ARGS)
    {
        // map

        return Serializer::callSingleFunc<Drawable, Serializer::FunctionID::Save>(Serializer::getSerializeID<Drawable>(), comp, val, alloc);
    }

    JOP_REGISTER_SERIALIZER(jop, SkyBox, jop__skyBoxFactoryFunc, jop__skyBoxLoadFunc, jop__skyBoxSaveFunc);
}

namespace jop
{
    SkyBox::SkyBox(Object& obj, Renderer& renderer, const float size)
        : Drawable      (obj, renderer, 0),
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

    void SkyBox::draw(const Camera* camera, const LightContainer&, ShaderProgram& shader) const
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
        s.setAttribute(0, GL_FLOAT, 3, stride, msh.getVertexOffset(Mesh::Position));

        mat.sendToShader(s, camera, getAlphaMultiplier());

        GlState::setDepthTest(true, GlState::DepthFunc::LessEqual);

        if (msh.getElementAmount())
        {
            msh.getIndexBuffer().bind();
            glCheck(glDrawElements(GL_TRIANGLES, msh.getElementAmount(), msh.getElementEnum(), (void*)0));
        }
        else
        {
            glCheck(glDrawArrays(GL_TRIANGLES, 0, msh.getVertexAmount()));
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