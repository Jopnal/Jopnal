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

	#include <Jopnal/Graphics/SkySphere.hpp>

    #include <Jopnal/Graphics/Camera.hpp>
    #include <Jopnal/Graphics/Shader.hpp>
    #include <Jopnal/Graphics/OpenGL.hpp>
    #include <Jopnal/Graphics/GlCheck.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    SkySphere::SkySphere(Object& obj, Renderer& renderer, const float radius)
        : Drawable      (obj, renderer, 0),
          m_mesh        (""),
          m_material    ("", Material::Attribute::__SkySphere | Material::Attribute::DiffuseMap, false)
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
        s.setAttribute(0, GL_FLOAT, 3, stride, false, msh.getVertexOffset(Mesh::Position));
        s.setAttribute(1, GL_FLOAT, 2, stride, false, msh.getVertexOffset(Mesh::TexCoords));

        mat.sendToShader(s, camera, getAlphaMultiplier());

        GlState::setDepthTest(true, GlState::DepthFunc::LessEqual);
        GlState::setFaceCull(true, GlState::FaceCull::Front);

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
        GlState::setFaceCull(true);
    }

    //////////////////////////////////////////////

    SkySphere& SkySphere::setMap(const Texture2D& map)
    {
        m_material.setMap(Material::Map::Diffuse, map);

        return *this;
    }

    //////////////////////////////////////////////

    const Texture* SkySphere::getMap() const
    {
        return m_material.getMap(Material::Map::Diffuse);
    }
}