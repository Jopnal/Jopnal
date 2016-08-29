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
    #include <Jopnal/Graphics/Material.hpp>
    #include <Jopnal/Graphics/ShaderProgram.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>
    #include <Jopnal/Graphics/OpenGL/GlState.hpp>
    #include <Jopnal/Graphics/Texture/Texture2D.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    SkySphere::SkySphere(Object& obj, Renderer& renderer, const float radius)
        : Drawable      (obj, renderer, RenderPass::Pass::BeforePost),
          m_mesh        (""),
          m_material    ("", false)
    {
        m_material.setAttributes(Material::Attribute::DiffuseMap);
        m_mesh.load(radius, 20, true);

        setModel(Model(m_mesh, m_material));
        setFlags(Reflected);
        addAttributes(Attribute::__SkySphere);
    }

    SkySphere::SkySphere(const SkySphere& other, Object& newObj)
        : Drawable      (other, newObj),
          m_mesh        (other.m_mesh, ""),
          m_material    (other.m_material, "")
    {}

    //////////////////////////////////////////////

    void SkySphere::draw(const ProjectionInfo& proj, const LightContainer&) const
    {
        if (!getModel().isValid())
            return;

        auto& shdr = getShader();
        auto& mat = *getModel().getMaterial();
        auto& msh = *getModel().getMesh();

        // Uniforms
        {
            shdr.setUniform("u_PMatrix", proj.projectionMatrix);
            shdr.setUniform("u_VMatrix", proj.viewMatrix);

            mat.sendToShader(shdr, &proj.cameraPosition);

            glCheck(glVertexAttrib4fv(Mesh::VertexIndex::Color, &getColor().colors[0]));
        }

        GlState::setDepthTest(true, GlState::DepthFunc::LessEqual);
        GlState::setFaceCull(true, GlState::FaceCull::Front);

        msh.draw(mat.getAttributes());

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