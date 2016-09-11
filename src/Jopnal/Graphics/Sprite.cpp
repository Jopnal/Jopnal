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

    #include <Jopnal/Graphics/Sprite.hpp>

    #include <Jopnal/Core/Object.hpp>
    #include <Jopnal/Graphics/Texture/Texture2D.hpp>
    #include <Jopnal/Graphics/ShaderAssembler.hpp>
    #include <Jopnal/Graphics/ShaderProgram.hpp>
    #include <Jopnal/Graphics/OpenGL/OpenGL.hpp>
    #include <Jopnal/Graphics/OpenGL/GlCheck.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    Sprite::Sprite(Object& object, Renderer& renderer, const RenderPass::Pass pass, const uint32 weight, const bool cull)
        : Drawable      (object, renderer, pass, weight, cull),
          m_texture     (),
          m_mesh        ("")
    {
        setTexture(Texture2D::getDefault(), true);
        setOverrideShader(ShaderAssembler::getShader(1 << static_cast<uint64>(Material::Map::Diffuse0), 0));
    }

    Sprite::Sprite(const Sprite& other, Object& newObj)
        : Drawable      (other, newObj),
          m_texture     (),
          m_mesh        ("")
    {
        setTexture(Texture2D::getDefault(), true);
    }

    //////////////////////////////////////////////

    void Sprite::draw(const ProjectionInfo& proj, const LightContainer&) const
    {
        // Uniforms
        {
            auto& tex = getTexture();

            auto& shdr = getShader();
            auto& modelMat = getObject()->getTransform().getMatrix();

            shdr.setUniform("u_PVMMatrix", proj.projectionMatrix * proj.viewMatrix * modelMat);
            shdr.setUniform("u_DiffuseMap", tex, static_cast<unsigned int>(Material::Map::Diffuse0));

            glCheck(glVertexAttrib4fv(Mesh::VertexIndex::Color, &getColor().colors[0]));
        }

        m_mesh.draw();
    }

    //////////////////////////////////////////////

    Sprite& Sprite::setTexture(const Texture2D& texture, const bool updateSize)
    {
        m_texture = static_ref_cast<const Texture2D>(texture.getReference());

        if (updateSize)
            return setSize(texture.getSize());

        return *this;
    }

    //////////////////////////////////////////////

    const Texture2D& Sprite::getTexture() const
    {
        if (m_texture.expired())
            m_texture = static_ref_cast<const Texture2D>(Texture2D::getDefault().getReference());

        return *m_texture;
    }

    //////////////////////////////////////////////

    Sprite& Sprite::setSize(const glm::vec2& size)
    {
        m_mesh.load(size);
        return *this;
    }

    //////////////////////////////////////////////

    const glm::vec2& Sprite::getSize() const
    {
        return m_mesh.getSize();
    }
}