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
    SkySphere::SkySphere(Object& obj, Renderer& renderer, const float radius)
        : Drawable(obj, renderer, "skysphere"),
          m_mesh(""),
          m_material("", Material::Attribute::__SkySphere | Material::Attribute::DiffuseMap, false)
    {
        m_mesh.load(radius, 20, 20, true);

        setModel(Model(m_mesh, m_material));

        setCastShadows(false);
        setReceiveLights(false);
        setReceiveShadows(false);
    }

    SkySphere::SkySphere(const SkySphere& other, Object& newObj)
        : Drawable      (other, newObj),
          m_mesh        (other.m_mesh, ""),
          m_material    (other.m_material, "")
    {}

    //////////////////////////////////////////////

    void SkySphere::draw(const Camera* camera, const LightContainer&, Shader& shader) const
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
        s.setAttribute(1, gl::FLOAT, 2, stride, false, msh.getVertexOffset(Mesh::TexCoords));

        mat.sendToShader(s, camera);

        GlState::setDepthTest(true, GlState::DepthFunc::LessEqual);
        GlState::setFaceCull(true, GlState::FaceCull::Front);

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
        GlState::setFaceCull(true);
    }

    //////////////////////////////////////////////

    void SkySphere::setMap(const Texture2D& map)
    {
        m_material.setMap(Material::Map::Diffuse, map);
    }

    //////////////////////////////////////////////

    const Texture* SkySphere::getMap() const
    {
        return m_material.getMap(Material::Map::Diffuse);
    }
}